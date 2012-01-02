#include <limits>
#include <iostream>
#include <iomanip>
#include <fstream>

#include "tile_factory.h"
#include "map.h"

using namespace std;



Map::Map() {
	this->width = 10;
	this->height = 10;
	this->rotation = 0;
}


Map::Map(char* filePath) {
	this->width = 10;
	this->height = 10;
	this->rotation = 0;
	loadMap(filePath);
}

Map::Map(char* filePath, int color, int width, int height) {
	this->width = width;
	this->height = height;
	this->rotation = 0;
	loadMap(filePath);

	this->color = color;
}

Tile Map::getTile(int x, int y) {
	return tiles[y][x];
}

void Map::setTile(Tile tile, int x, int y) {
	tiles[y][x] = tile;
}

int Map::getWidth() {
	return width;
}

void Map::setWidth(int width) {
	this->width = width;
}

int Map::getHeight() {
	return height;
}

void Map::setHeight(int height) {
	this->height = height;
}

int Map::getColor() {
	return color;
}

void Map::setColor(int color) {
	this->color = color;
}

int Map::getRotation() {
	return rotation;
}

void Map::setRotation(int rotation) {
		if (rotation < 0)
			rotation = -rotation;

		this->rotation = (rotation % 4);

}

bool Map::loadMap(char* filePath) {
	bool isSuccess = true;

	int width = 10;
	int height = 10;
	//Read file and create new tiles 
	ifstream file(filePath);

	if(!file) {
		isSuccess = false;
	} else {

		TileFactory tileFactory;
		for (int i = 0; i < width; i++) {
			for (int j = 0; j < height; j++) {
				char model;
				file.read(&model,1);
				tiles[i][j] = tileFactory.makeTile(model);
			}
			file.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
		}
	
		file.close();
	}
	return isSuccess;
}

void Map::rotateClockwise() {
	Tile temp[10][10];

	setRotation(rotation + 1);
	
	for (int i = 9; i >= 0; i--) {
		for (int j = 0; j < 10; j++) {
			temp[j][9-i] = tiles[i][j];
		}
	}

	copyTiles(temp);
}



void Map::rotateCounterClockwise() {
	Tile temp[10][10];

	setRotation(rotation + 3);

	for (int i = 9; i >= 0; i--) {
		for (int j = 0; j < 10; j++) {
			temp[i][j] = tiles[j][9-i];
		}
	}
	
	copyTiles(temp);
}

void Map::xAxisFlip() {
	Tile temp[10][10];
	setRotation(rotation + 2);
	for (int i = 9; i >= 0; i--) {
		for (int j = 0; j < 10; j++) {
			temp[i][j] = tiles[9-i][j];
		}
	}
	
	copyTiles(temp);
}

void Map::yAxisFlip() {
	Tile temp[10][10];
	//setRotation(rotation + 2);
	for (int i = 0; i < 10; i++) {
		for (int j = 9; j >= 0; j--) {
			temp[i][j] = tiles[i][9-j];
		}
	}

	copyTiles(temp);
}

void Map::copyTiles(Tile temp[][10]) {
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			tiles[i][j] = temp[i][j];
		}
	}
}