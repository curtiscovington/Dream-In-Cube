#include "tile_factory.h"

#include "curses.h"

Tile TileFactory::makeTile(char model) {
	Tile tile;

	switch (model) {
	case ' ':
		tile = makeEmptyTile();
		break;
	case '0':
		tile = makeWallTile();
		break;
	case '^':
		tile = makeStairTile();
		break;
	case '#':
		tile = makeBoxTile();
		break;
	case '+':
		tile = makeIceTile();
		break;
	default:
		tile = makeEmptyTile();
		
	}

	return tile;
}

Tile TileFactory::makeWallTile() {
	Tile tile;

	tile = Tile(WALL,"Wall",true,WALL);

	return tile;
}

Tile TileFactory::makeEmptyTile() {
	Tile tile;

	tile = Tile(EMPTY,"",false,COLOR_BLACK);

	return tile;
}

Tile TileFactory::makeStairTile() {
	Tile tile;

	tile = Tile(STAIR,"Stairs",true,STAIR);

	return tile;
}
Tile TileFactory::makeBoxTile() {
	Tile tile;

	tile =  Tile(BOX,"(Push)Box",true,BOX);

	return tile;
}

Tile TileFactory::makeIceTile() {
	Tile tile;

	tile =  Tile(ICE,"(Slide)Ice",false,ICE);

	return tile;
}