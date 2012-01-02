#ifndef MAP
#define MAP

#include "tile.h"

class Map {
private:
	Tile tiles[10][10];
	int color;
	int rotation;
	int width;
	int height;
public:
	Map();

	Map(char*);
	Map(char*, int, int, int);
	Tile getTile(int, int);
	void setTile(Tile, int, int);
	int getWidth();
	void setWidth(int);
	int getHeight();
	void setHeight(int);
	int getColor();
	void setColor(int);
	int getRotation();
	void setRotation(int);
	bool loadMap(char*);

	void rotateClockwise();
	void rotateCounterClockwise();
	void xAxisFlip();
	void yAxisFlip();

	void copyTiles(Tile[][10]);
};


#endif