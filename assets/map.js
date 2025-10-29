import { TILE } from './tile.js';

const SIZE = 10;

export default class Map {
  constructor(tiles = createEmptyGrid(), color = '', rotation = 0) {
    this.tiles = tiles;
    this.color = color;
    this.rotation = rotation % 4;
    this.width = SIZE;
    this.height = SIZE;
  }

  static fromLines(lines, color = '') {
    const tiles = lines.map((line) => [...line]);
    return new Map(tiles, color, 0);
  }

  clone() {
    const tiles = this.tiles.map((row) => [...row]);
    return new Map(tiles, this.color, this.rotation);
  }

  getTile(x, y) {
    return this.tiles[y][x];
  }

  setTile(value, x, y) {
    this.tiles[y][x] = value;
  }

  getRotation() {
    return this.rotation;
  }

  setRotation(value) {
    let rotation = value;
    if (rotation < 0) {
      rotation = -rotation;
    }
    this.rotation = rotation % 4;
  }

  getColor() {
    return this.color;
  }

  setColor(color) {
    this.color = color;
  }

  rotateClockwise() {
    const temp = createEmptyGrid();
    for (let row = SIZE - 1; row >= 0; row -= 1) {
      for (let col = 0; col < SIZE; col += 1) {
        temp[col][SIZE - 1 - row] = this.tiles[row][col];
      }
    }
    this.tiles = temp;
    this.setRotation(this.rotation + 1);
  }

  rotateCounterClockwise() {
    const temp = createEmptyGrid();
    for (let row = SIZE - 1; row >= 0; row -= 1) {
      for (let col = 0; col < SIZE; col += 1) {
        temp[row][col] = this.tiles[col][SIZE - 1 - row];
      }
    }
    this.tiles = temp;
    this.setRotation(this.rotation + 3);
  }

  xAxisFlip() {
    const temp = createEmptyGrid();
    for (let row = 0; row < SIZE; row += 1) {
      for (let col = 0; col < SIZE; col += 1) {
        temp[row][col] = this.tiles[SIZE - 1 - row][col];
      }
    }
    this.tiles = temp;
    this.setRotation(this.rotation + 2);
  }

  yAxisFlip() {
    const temp = createEmptyGrid();
    for (let row = 0; row < SIZE; row += 1) {
      for (let col = 0; col < SIZE; col += 1) {
        temp[row][col] = this.tiles[row][SIZE - 1 - col];
      }
    }
    this.tiles = temp;
  }
}

function createEmptyGrid() {
  return Array.from({ length: SIZE }, () => Array.from({ length: SIZE }, () => TILE.EMPTY));
}
