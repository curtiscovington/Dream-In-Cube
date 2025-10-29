export const TILE = Object.freeze({
  EMPTY: ' ',
  WALL: '0',
  STAIRS: '^',
  BOX: '#',
  ICE: '+',
});

const TILE_INFO = {
  [TILE.EMPTY]: { solid: false, css: 'empty', label: 'Empty' },
  [TILE.WALL]: { solid: true, css: 'wall', label: 'Wall' },
  [TILE.STAIRS]: { solid: true, css: 'stairs', label: 'Stairs' },
  [TILE.BOX]: { solid: true, css: 'box', label: 'Pushable box' },
  [TILE.ICE]: { solid: false, css: 'ice', label: 'Ice' },
};

export function tileInfo(model) {
  return TILE_INFO[model] ?? TILE_INFO[TILE.EMPTY];
}

export function isSolid(model) {
  return tileInfo(model).solid;
}

export function cssClassForTile(model, isPlayer = false) {
  const base = tileInfo(model).css;
  return isPlayer ? `${base} player`.trim() : base;
}

export function displaySymbol(model) {
  if (model === TILE.EMPTY) {
    return '';
  }
  return model;
}
