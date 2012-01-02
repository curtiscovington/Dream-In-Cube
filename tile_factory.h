#ifndef TILEFACTORY
#define TILEFACTORY

#include "tile.h"

/*
*	Provides quick creation of tiles.
*/

class TileFactory {
private:
public:
	static const char EMPTY = ' ';
	static const char WALL = '0';
	static const char STAIR = '^';
	static const char BOX = '#';
	static const char ICE = '+';

	Tile makeTile(char);
	Tile makeWallTile();
	Tile makeEmptyTile();
	Tile makeStairTile();
	Tile makeBoxTile();
	Tile makeIceTile();
};

#endif