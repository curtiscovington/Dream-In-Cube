import Cube from './cube.js';
import ViewFrame from './viewFrame.js';
import { TILE, isSolid, tileInfo, displaySymbol } from './tile.js';

const SIZE = 10;
const CENTER = 4;

const Direction = Object.freeze({
  UP: 'UP',
  DOWN: 'DOWN',
  LEFT: 'LEFT',
  RIGHT: 'RIGHT',
});

function tileAtCenter(map) {
  return map.getTile(CENTER, CENTER);
}

function canMove(map) {
  return !isSolid(tileAtCenter(map));
}

export default class Game {
  constructor() {
    this.dom = {
      grid: document.querySelector('#game-grid'),
      start: document.querySelector('#start-game'),
      restart: document.querySelector('#restart-game'),
      help: document.querySelector('#show-help'),
      message: document.querySelector('#status-message'),
      levelInfo: document.querySelector('#level-info'),
      helpDialog: document.querySelector('#help-dialog'),
      miniCells: Array.from(document.querySelectorAll('.mini-cell')),
      touchControls: document.querySelector('.touch-controls'),
      touchButtons: Array.from(
        document.querySelectorAll('.touch-controls button[data-direction]'),
      ),
    };

    this.levels = [];
    this.cube = null;
    this.view = null;
    this.isActive = false;
    this.isBusy = false;
    this.cells = [];
    this.touchButtonTimers = new WeakMap();
  }

  async init() {
    try {
      await this.loadLevels();
      this.setupGrid();
      this.attachEventListeners();
      this.configureTouchControlAvailability();
      this.setStatus('Ready when you are. Press Start to enter the cube.');
    } catch (error) {
      console.error(error);
      this.setStatus('Failed to load levels. Check the console for details.');
      this.dom.start.disabled = true;
    }
  }

  async loadLevels() {
    const response = await fetch(new URL('./levels.json', import.meta.url));
    if (!response.ok) {
      throw new Error(`Failed to fetch levels.json (${response.status})`);
    }
    const payload = await response.json();
    this.levels = (payload.levels ?? []).slice().sort((a, b) => a.id - b.id);
    if (this.levels.length === 0) {
      throw new Error('No levels found in levels.json');
    }

    this.cube = new Cube(this.levels);
    this.view = new ViewFrame(this.cube);
  }

  attachEventListeners() {
    this.dom.start.addEventListener('click', () => this.startGame());
    this.dom.restart.addEventListener('click', () => this.restartGame());
    this.dom.help.addEventListener('click', () => this.toggleHelp(true));
    if (this.dom.helpDialog) {
      this.dom.helpDialog.addEventListener('cancel', (event) => {
        event.preventDefault();
        this.toggleHelp(false);
      });
    }

    window.addEventListener('keydown', (event) => this.handleKeyDown(event));

    if (this.dom.touchButtons.length) {
      this.dom.touchButtons.forEach((button) => {
        const direction = this.parseDirection(button.dataset.direction);
        if (!direction) {
          return;
        }

        button.addEventListener('pointerdown', (event) => {
          event.preventDefault();
          button.dataset.pointerActive = 'true';
          this.startTouchButtonPress(button);
          this.handleDirection(direction);
        });

        const clearButtonState = () => {
          this.clearTouchButtonPress(button);
          delete button.dataset.pointerActive;
        };

        button.addEventListener('pointerup', clearButtonState);
        button.addEventListener('pointerleave', clearButtonState);
        button.addEventListener('pointercancel', clearButtonState);

        button.addEventListener('click', (event) => {
          event.preventDefault();
          if (button.dataset.pointerActive === 'true') {
            return;
          }
          this.flashTouchButton(button);
          this.handleDirection(direction);
        });
      });
    }
  }

  configureTouchControlAvailability() {
    const controls = this.dom.touchControls;
    if (!controls || typeof window.matchMedia !== 'function') {
      return;
    }

    const mediaQuery = window.matchMedia('(pointer: coarse)');
    const applyAvailability = (matches) => {
      controls.classList.toggle('is-available', matches);
    };

    applyAvailability(mediaQuery.matches);

    const listener = (event) => applyAvailability(event.matches);

    if (typeof mediaQuery.addEventListener === 'function') {
      mediaQuery.addEventListener('change', listener);
    } else if (typeof mediaQuery.addListener === 'function') {
      mediaQuery.addListener(listener);
    }
  }

  startTouchButtonPress(button) {
    this.cancelTouchButtonTimer(button);
    button.classList.add('is-pressed');
    button.setAttribute('aria-pressed', 'true');
  }

  clearTouchButtonPress(button) {
    this.cancelTouchButtonTimer(button);
    button.classList.remove('is-pressed');
    button.setAttribute('aria-pressed', 'false');
  }

  flashTouchButton(button) {
    this.startTouchButtonPress(button);
    const timeout = window.setTimeout(() => {
      this.clearTouchButtonPress(button);
    }, 160);
    this.touchButtonTimers.set(button, timeout);
  }

  cancelTouchButtonTimer(button) {
    const timeout = this.touchButtonTimers.get(button);
    if (timeout) {
      window.clearTimeout(timeout);
      this.touchButtonTimers.delete(button);
    }
  }

  setupGrid() {
    if (this.cells.length > 0) {
      return;
    }
    const fragment = document.createDocumentFragment();
    for (let row = 0; row < SIZE; row += 1) {
      for (let col = 0; col < SIZE; col += 1) {
        const cell = document.createElement('div');
        cell.className = 'tile empty';
        cell.dataset.row = String(row);
        cell.dataset.col = String(col);
        cell.setAttribute('role', 'gridcell');
        cell.setAttribute('aria-label', 'Empty tile');
        fragment.appendChild(cell);
        this.cells.push(cell);
      }
    }
    this.dom.grid.appendChild(fragment);
  }

  startGame() {
    if (this.isBusy) {
      return;
    }
    const firstLevel = this.levels[0]?.id ?? 1;
    if (!this.cube.loadCube(firstLevel)) {
      this.setStatus('Unable to load the first cube.');
      return;
    }

    this.view.setXOffset(0);
    this.view.setYOffset(0);
    this.isActive = true;
    this.dom.restart.disabled = false;
    this.setStatus('Exploring cube 1.');
    this.render();
  }

  restartGame() {
    if (!this.isActive) {
      this.startGame();
      return;
    }
    this.startGame();
  }

  toggleHelp(show) {
    if (!this.dom.helpDialog) {
      return;
    }
    if (show) {
      if (!this.dom.helpDialog.open) {
        this.dom.helpDialog.showModal();
      }
    } else {
      this.dom.helpDialog.close();
    }
  }

  handleKeyDown(event) {
    if (!this.isActive || this.isBusy) {
      return;
    }

    // Prevent game interaction while help dialog is open.
    if (this.dom.helpDialog?.open) {
      return;
    }

    let direction = null;
    switch (event.key.toLowerCase()) {
      case 'w':
      case 'arrowup':
        direction = Direction.UP;
        break;
      case 's':
      case 'arrowdown':
        direction = Direction.DOWN;
        break;
      case 'a':
      case 'arrowleft':
        direction = Direction.LEFT;
        break;
      case 'd':
      case 'arrowright':
        direction = Direction.RIGHT;
        break;
      case 'r':
        this.cube.reloadCube();
        this.view.setXOffset(0);
        this.view.setYOffset(0);
        this.render();
        this.setStatus(`Reloaded cube ${this.cube.getLevel()}.`);
        break;
      default:
        return;
    }

    if (direction) {
      event.preventDefault();
      this.handleDirection(direction);
    }
  }

  parseDirection(value) {
    switch ((value ?? '').toLowerCase()) {
      case 'up':
        return Direction.UP;
      case 'down':
        return Direction.DOWN;
      case 'left':
        return Direction.LEFT;
      case 'right':
        return Direction.RIGHT;
      default:
        return null;
    }
  }

  handleDirection(direction) {
    if (!direction || !this.isActive || this.isBusy) {
      return;
    }

    if (this.dom.helpDialog?.open) {
      return;
    }

    this.attemptMove(direction);
  }

  attemptMove(direction) {
    const currentX = this.view.getXOffset();
    const currentY = this.view.getYOffset();

    let newX = currentX;
    let newY = currentY;

    switch (direction) {
      case Direction.UP:
        newY += 1;
        break;
      case Direction.DOWN:
        newY -= 1;
        break;
      case Direction.LEFT:
        newX -= 1;
        break;
      case Direction.RIGHT:
        newX += 1;
        break;
      default:
        break;
    }

    const preCollisionView = this.view.getView(newX, newY);
    const shouldContinue = this.resolveCollision(preCollisionView, newX, newY, direction);

    if (!shouldContinue) {
      this.render();
      return;
    }

    const postCollisionView = this.view.getView(newX, newY);
    if (canMove(postCollisionView)) {
      this.view.setXOffset(newX);
      this.view.setYOffset(newY);
      this.render();
    }
  }

  resolveCollision(map, xOffset, yOffset, direction) {
    const tile = tileAtCenter(map);
    const x = xOffset + 4;
    const y = 4 - yOffset;

    switch (tile) {
      case TILE.BOX:
        this.pushBox(map, x, y, direction);
        break;
      case TILE.STAIRS:
        this.advanceLevel();
        return false;
      case TILE.ICE:
        this.slide(direction);
        return false;
      default:
        break;
    }

    return true;
  }

  pushBox(map, x, y, direction) {
    switch (direction) {
      case Direction.UP:
        // Original build intentionally prevented pushing upward.
        break;
      case Direction.DOWN:
        if (!isSolid(map.getTile(CENTER, CENTER + 1))) {
          if (y >= 9) {
            this.cube.getBottom().setTile(TILE.BOX, x, y - 9);
          } else {
            this.cube.getFront().setTile(TILE.BOX, x, y + 1);
          }

          if (y > 9) {
            this.cube.getBottom().setTile(TILE.EMPTY, x, y - 10);
          } else {
            this.cube.getFront().setTile(TILE.EMPTY, x, y);
          }
        }
        break;
      case Direction.LEFT:
        if (!isSolid(map.getTile(CENTER - 1, CENTER))) {
          if (x <= 0) {
            this.cube.getLeft().setTile(TILE.BOX, 9 + x, y);
          } else {
            this.cube.getFront().setTile(TILE.BOX, x - 1, y);
          }

          if (x < 0) {
            this.cube.getLeft().setTile(TILE.EMPTY, 9, y);
          } else {
            this.cube.getFront().setTile(TILE.EMPTY, x, y);
          }
        }
        break;
      case Direction.RIGHT:
        if (!isSolid(map.getTile(CENTER + 1, CENTER))) {
          if (x >= 9) {
            this.cube.getRight().setTile(TILE.BOX, x - 9, y);
          } else {
            this.cube.getFront().setTile(TILE.BOX, x + 1, y);
          }

          if (x > 9) {
            this.cube.getRight().setTile(TILE.EMPTY, x - 10, y);
          } else {
            this.cube.getFront().setTile(TILE.EMPTY, x, y);
          }
        }
        break;
      default:
        break;
    }
  }

  slide(direction) {
    this.isBusy = true;
    let keepSliding = true;

    while (keepSliding) {
      switch (direction) {
        case Direction.UP:
          this.view.setYOffset(this.view.getYOffset() + 1);
          break;
        case Direction.DOWN:
          this.view.setYOffset(this.view.getYOffset() - 1);
          break;
        case Direction.LEFT:
          this.view.setXOffset(this.view.getXOffset() - 1);
          break;
        case Direction.RIGHT:
          this.view.setXOffset(this.view.getXOffset() + 1);
          break;
        default:
          break;
      }

      const currentMap = this.view.getView(this.view.getXOffset(), this.view.getYOffset());

      const nextOffsets = {
        [Direction.UP]: [this.view.getXOffset(), this.view.getYOffset() + 1],
        [Direction.DOWN]: [this.view.getXOffset(), this.view.getYOffset() - 1],
        [Direction.LEFT]: [this.view.getXOffset() - 1, this.view.getYOffset()],
        [Direction.RIGHT]: [this.view.getXOffset() + 1, this.view.getYOffset()],
      }[direction];

      const nextMap = this.view.getView(nextOffsets[0], nextOffsets[1]);

      this.render();

      const stillOnIce = tileAtCenter(currentMap) === TILE.ICE;
      const blockedAhead = isSolid(tileAtCenter(nextMap));

      if (!stillOnIce || blockedAhead) {
        keepSliding = false;
      }
    }

    this.isBusy = false;
  }

  advanceLevel() {
    const nextLevelId = this.cube.getLevel() + 1;
    if (!this.cube.loadNextCube()) {
      this.handleWin();
      return;
    }

    this.view.setXOffset(0);
    this.view.setYOffset(0);
    this.setStatus(`Reached cube ${nextLevelId}.`);
    this.render();
  }

  handleWin() {
    this.isActive = false;
    this.setStatus('You escaped all cubes! Congratulations!');
  }

  render() {
    this.renderGrid();
    this.updateLevelInfo();
    this.updateMinimap();
  }

  renderGrid() {
    if (!this.isActive || !this.cells.length) {
      return;
    }
    const map = this.view.getView(this.view.getXOffset(), this.view.getYOffset());

    for (let row = 0; row < SIZE; row += 1) {
      for (let col = 0; col < SIZE; col += 1) {
        const tile = map.getTile(col, row);
        const cell = this.cells[row * SIZE + col];
        const info = tileInfo(tile);

        cell.className = `tile ${info.css}`.trim();
        cell.dataset.tile = tile;
        if (row === CENTER && col === CENTER) {
          cell.className = `${cell.className} player`.trim();
          cell.textContent = '@';
          cell.setAttribute('aria-label', `Player on ${info.label}`);
        } else {
          cell.textContent = displaySymbol(tile);
          cell.setAttribute('aria-label', info.label);
        }
      }
    }
  }

  updateLevelInfo() {
    if (!this.isActive) {
      this.dom.levelInfo.textContent = '';
      return;
    }
    this.dom.levelInfo.textContent = `Cube ${this.cube.getLevel()}`;
  }

  updateMinimap() {
    const faceMeta = {
      front: { label: 'Front', state: 'current' },
      top: { label: 'Top', state: 'adjacent' },
      left: { label: 'Left', state: 'adjacent' },
      right: { label: 'Right', state: 'adjacent' },
      bottom: { label: 'Bottom', state: 'adjacent' },
      back: { label: 'Back', state: 'adjacent' },
    };

    const faces = [
      { key: 'front', map: this.cube.getFront() },
      { key: 'top', map: this.cube.getTop() },
      { key: 'left', map: this.cube.getLeft() },
      { key: 'right', map: this.cube.getRight() },
      { key: 'bottom', map: this.cube.getBottom() },
      { key: 'back', map: this.cube.getBack() },
    ];

    faces.forEach(({ key, map }) => {
      const cell = this.dom.miniCells.find((mini) => mini.dataset.face === key);
      if (!cell) {
        return;
      }
      const meta = faceMeta[key] ?? { label: key, state: 'adjacent' };
      const rotation = map.getRotation() * 90;
      const rotationLabel = `${rotation}°`;

      cell.classList.remove('current', 'adjacent');
      const faceColor = typeof map.getColor === 'function' ? map.getColor() : null;
      if (faceColor) {
        cell.dataset.color = faceColor;
      } else {
        delete cell.dataset.color;
      }
      cell.classList.add(meta.state);
      cell.innerHTML = '';
      cell.title = `${meta.label} face${meta.state === 'current' ? ' (current view)' : ''} • ${rotationLabel}`;
      cell.setAttribute(
        'aria-label',
        `${meta.label} face${meta.state === 'current' ? ' (current view)' : ''}, rotation ${rotationLabel}`,
      );
    });
  }

  setStatus(message) {
    this.dom.message.textContent = message;
  }
}
