const SIZE = 10;

export default class ViewFrame {
  constructor(cube) {
    this.cube = cube;
    this.xOffset = 0;
    this.yOffset = 0;
  }

  getCube() {
    return this.cube;
  }

  setCube(cube) {
    this.cube = cube;
  }

  getXOffset() {
    return this.xOffset;
  }

  setXOffset(value) {
    this.xOffset = value;
    if (this.xOffset < -4) {
      this.xOffset = 5;
      this.getCube().rotateRight();
    } else if (this.xOffset > 5) {
      this.xOffset = -4;
      this.getCube().rotateLeft();
    }
  }

  getYOffset() {
    return this.yOffset;
  }

  setYOffset(value) {
    this.yOffset = value;
    if (this.yOffset > 4) {
      this.yOffset = -5;
      this.getCube().rotateDown();
    } else if (this.yOffset < -5) {
      this.yOffset = 4;
      this.getCube().rotateUp();
    }
  }

  getView(x, y) {
    const front = this.cube.getFront();
    const map = front.clone();

    if (x < 0 && y > 0) {
      for (let i = SIZE - y, yIter = 0; i < SIZE; i += 1, yIter += 1) {
        let xIter = 0;
        for (let j = SIZE + x; j < SIZE; j += 1, xIter += 1) {
          const tile = this.cube.getBack().getTile(j, i);
          map.setTile(tile, xIter, yIter);
        }
      }

      for (let i = SIZE - y, yIter = 0; i < SIZE; i += 1, yIter += 1) {
        let xIter = -x;
        for (let j = 0; j < SIZE + x; j += 1, xIter += 1) {
          const tile = this.cube.getTop().getTile(j, i);
          map.setTile(tile, xIter, yIter);
        }
      }

      for (let i = 0, yIter = y; i < SIZE - y; i += 1, yIter += 1) {
        let xIter = 0;
        for (let j = SIZE + x; j < SIZE; j += 1, xIter += 1) {
          const tile = this.cube.getLeft().getTile(j, i);
          map.setTile(tile, xIter, yIter);
        }
      }

      for (let i = 0, yIter = y; i < SIZE - y; i += 1, yIter += 1) {
        let xIter = -x;
        for (let j = 0; j < SIZE + x; j += 1, xIter += 1) {
          const tile = front.getTile(j, i);
          map.setTile(tile, xIter, yIter);
        }
      }
    } else if (x > 0 && y > 0) {
      for (let i = SIZE - y, yIter = 0; i < SIZE; i += 1, yIter += 1) {
        let xIter = SIZE - x;
        for (let j = 0; j < x; j += 1, xIter += 1) {
          const tile = this.cube.getBack().getTile(j, i);
          map.setTile(tile, xIter, yIter);
        }
      }

      for (let i = SIZE - y, yIter = 0; i < SIZE; i += 1, yIter += 1) {
        let xIter = 0;
        for (let j = x; j < SIZE; j += 1, xIter += 1) {
          const tile = this.cube.getTop().getTile(j, i);
          map.setTile(tile, xIter, yIter);
        }
      }

      for (let i = 0, yIter = y; i < SIZE; i += 1, yIter += 1) {
        let xIter = SIZE - x;
        for (let j = 0; j < x; j += 1, xIter += 1) {
          if (
            j < SIZE &&
            j >= 0 &&
            xIter < SIZE &&
            xIter >= 0 &&
            yIter < SIZE &&
            yIter >= 0 &&
            i < SIZE &&
            i >= 0
          ) {
            const tile = this.cube.getRight().getTile(j, i);
            map.setTile(tile, xIter, yIter);
          }
        }
      }

      for (let i = 0, yIter = y; i < SIZE - y; i += 1, yIter += 1) {
        let xIter = 0;
        for (let j = x; j < SIZE; j += 1, xIter += 1) {
          const tile = front.getTile(j, i);
          map.setTile(tile, xIter, yIter);
        }
      }
    } else if (x > 0 && y === 0) {
      for (let i = 0, yIter = 0; i < SIZE; i += 1, yIter += 1) {
        let xIter = SIZE - x;
        for (let j = 0; j < x; j += 1, xIter += 1) {
          const tile = this.cube.getRight().getTile(j, i);
          map.setTile(tile, xIter, yIter);
        }
      }

      for (let i = 0, yIter = 0; i < SIZE; i += 1, yIter += 1) {
        let xIter = 0;
        for (let j = x; j < SIZE; j += 1, xIter += 1) {
          const tile = front.getTile(j, i);
          map.setTile(tile, xIter, yIter);
        }
      }
    } else if (x < 0 && y === 0) {
      for (let i = 0, yIter = 0; i < SIZE; i += 1, yIter += 1) {
        let xIter = 0;
        for (let j = SIZE + x; j < SIZE; j += 1, xIter += 1) {
          const tile = this.cube.getLeft().getTile(j, i);
          map.setTile(tile, xIter, yIter);
        }
      }

      for (let i = 0, yIter = 0; i < SIZE; i += 1, yIter += 1) {
        let xIter = -x;
        for (let j = 0; j < SIZE + x; j += 1, xIter += 1) {
          const tile = front.getTile(j, i);
          map.setTile(tile, xIter, yIter);
        }
      }
    } else if (x === 0 && y > 0) {
      for (let i = SIZE - y, yIter = 0; i < SIZE; i += 1, yIter += 1) {
        let xIter = 0;
        for (let j = 0; j < SIZE; j += 1, xIter += 1) {
          const tile = this.cube.getTop().getTile(j, i);
          map.setTile(tile, xIter, yIter);
        }
      }

      for (let i = 0, yIter = y; i < SIZE - y; i += 1, yIter += 1) {
        let xIter = 0;
        for (let j = 0; j < SIZE; j += 1, xIter += 1) {
          const tile = front.getTile(j, i);
          map.setTile(tile, xIter, yIter);
        }
      }
    } else if (x === 0 && y < 0) {
      for (let i = 0, yIter = SIZE + y; i < -y; i += 1, yIter += 1) {
        let xIter = 0;
        for (let j = 0; j < SIZE; j += 1, xIter += 1) {
          const tile = this.cube.getBottom().getTile(j, i);
          map.setTile(tile, xIter, yIter);
        }
      }

      for (let i = -y, yIter = 0; i < SIZE; i += 1, yIter += 1) {
        let xIter = 0;
        for (let j = 0; j < SIZE; j += 1, xIter += 1) {
          const tile = front.getTile(j, i);
          map.setTile(tile, xIter, yIter);
        }
      }
    } else if (x < 0 && y < 0) {
      for (let i = 0, yIter = SIZE + y; i < -y; i += 1, yIter += 1) {
        let xIter = 0;
        for (let j = SIZE + x; j < SIZE; j += 1, xIter += 1) {
          const tile = this.cube.getBack().getTile(j, i);
          map.setTile(tile, xIter, yIter);
        }
      }

      for (let i = 0, yIter = SIZE + y; i < -y; i += 1, yIter += 1) {
        let xIter = -x;
        for (let j = 0; j < SIZE + x; j += 1, xIter += 1) {
          const tile = this.cube.getBottom().getTile(j, i);
          map.setTile(tile, xIter, yIter);
        }
      }

      for (let i = -y, yIter = 0; i < SIZE; i += 1, yIter += 1) {
        let xIter = 0;
        for (let j = SIZE + x; j < SIZE; j += 1, xIter += 1) {
          const tile = this.cube.getLeft().getTile(j, i);
          map.setTile(tile, xIter, yIter);
        }
      }

      for (let i = -y, yIter = 0; i < SIZE; i += 1, yIter += 1) {
        let xIter = -x;
        for (let j = 0; j < SIZE + x; j += 1, xIter += 1) {
          const tile = front.getTile(j, i);
          map.setTile(tile, xIter, yIter);
        }
      }
    } else if (x > 0 && y < 0) {
      for (let i = 0, yIter = SIZE + y; i < -y; i += 1, yIter += 1) {
        let xIter = SIZE - x;
        for (let j = 0; j < x; j += 1, xIter += 1) {
          const tile = this.cube.getBack().getTile(j, i);
          map.setTile(tile, xIter, yIter);
        }
      }

      for (let i = 0, yIter = SIZE + y; i < -y; i += 1, yIter += 1) {
        let xIter = 0;
        for (let j = x; j < SIZE; j += 1, xIter += 1) {
          const tile = this.cube.getBottom().getTile(j, i);
          map.setTile(tile, xIter, yIter);
        }
      }

      for (let i = -y, yIter = 0; i < SIZE; i += 1, yIter += 1) {
        let xIter = SIZE - x;
        for (let j = 0; j < x; j += 1, xIter += 1) {
          const tile = this.cube.getRight().getTile(j, i);
          map.setTile(tile, xIter, yIter);
        }
      }

      for (let i = -y, yIter = 0; i < SIZE; i += 1, yIter += 1) {
        let xIter = 0;
        for (let j = x; j < SIZE; j += 1, xIter += 1) {
          const tile = front.getTile(j, i);
          map.setTile(tile, xIter, yIter);
        }
      }
    }

    return map;
  }
}
