import Map from './map.js';

const FACE_KEYS = ['front', 'top', 'left', 'right', 'bottom', 'back'];

export default class Cube {
  constructor(levels = []) {
    this.levels = levels;
    this.levelId = 0;
    this.sides = Array.from({ length: FACE_KEYS.length }, () => new Map());
  }

  getFront() {
    return this.sides[0];
  }

  getTop() {
    return this.sides[1];
  }

  getLeft() {
    return this.sides[2];
  }

  getRight() {
    return this.sides[3];
  }

  getBottom() {
    return this.sides[4];
  }

  getBack() {
    return this.sides[5];
  }

  getSides() {
    return this.sides;
  }

  getLevel() {
    return this.levelId;
  }

  rotateUp() {
    this.getBack().xAxisFlip();

    const temp = this.sides[0];
    this.sides[0] = this.sides[4];
    this.sides[4] = this.sides[5];
    this.sides[5] = this.sides[1];
    this.sides[1] = temp;

    this.getRight().rotateClockwise();
    this.getLeft().rotateCounterClockwise();

    this.getBack().xAxisFlip();
  }

  rotateDown() {
    this.getBack().xAxisFlip();

    const temp = this.sides[0];
    this.sides[0] = this.sides[1];
    this.sides[1] = this.sides[5];
    this.sides[5] = this.sides[4];
    this.sides[4] = temp;

    this.getLeft().rotateClockwise();
    this.getRight().rotateCounterClockwise();

    this.getBack().xAxisFlip();
  }

  rotateLeft() {
    this.getBack().yAxisFlip();

    const temp = this.sides[0];
    this.sides[0] = this.sides[3];
    this.sides[3] = this.sides[5];
    this.sides[5] = this.sides[2];
    this.sides[2] = temp;

    this.getTop().rotateClockwise();
    this.getBottom().rotateCounterClockwise();

    this.getBack().yAxisFlip();
  }

  rotateRight() {
    this.getBack().yAxisFlip();

    const temp = this.sides[0];
    this.sides[0] = this.sides[2];
    this.sides[2] = this.sides[5];
    this.sides[5] = this.sides[3];
    this.sides[3] = temp;

    this.getTop().rotateCounterClockwise();
    this.getBottom().rotateClockwise();

    this.getBack().yAxisFlip();
  }

  loadCube(levelId) {
    const levelData = this.levels.find((lvl) => lvl.id === levelId);
    if (!levelData) {
      return false;
    }
    this.levelId = levelId;

    const { faces } = levelData;
    this.sides[0] = Map.fromLines(faces.front, 'front');
    this.sides[1] = Map.fromLines(faces.top, 'top');
    this.sides[2] = Map.fromLines(faces.left, 'left');
    this.sides[3] = Map.fromLines(faces.right, 'right');
    this.sides[4] = Map.fromLines(faces.bottom, 'bottom');
    this.sides[5] = Map.fromLines(faces.back, 'back');

    this.sides.forEach((side) => side.setRotation(0));
    return true;
  }

  reloadCube() {
    return this.loadCube(this.levelId);
  }

  loadNextCube() {
    return this.loadCube(this.levelId + 1);
  }
}
