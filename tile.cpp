
#include "tile.h"

#include "string.h"

Tile::Tile() {
	model = ' ';
	solid = false;
}

Tile::Tile(char val) {
	model = val;
}

Tile::Tile(char val, char* description, bool solid, int color) {
	model = val;
	memset(this->description,'\0',26);
	strcpy(this->description, description);
	this->solid = solid;
	this->color = color;
}

void Tile::setModel(char val) {
	model = val;
}

char Tile::getModel() {
	return model;
}

char* Tile::getDescription() {
	return description;
}

bool Tile::isSolid() {
	return solid;
}
int Tile::getColor() {
	return color;
}