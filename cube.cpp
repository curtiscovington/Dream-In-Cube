#include "cube.h"
#include "map.h"
#include <string>
#include "colors.h"

Cube::Cube() {
	level = 0;
}

Cube::Cube(int level) {
	loadCube(level);
}

Map* Cube::getFront() {
	return &sides[0];
}

Map* Cube::getTop() {
	return &sides[1];
}

Map* Cube::getLeft() {
	return &sides[2];
}

Map* Cube::getRight() {
	return &sides[3];
}

Map* Cube::getBottom() {
	return &sides[4];
}

Map* Cube::getBack() {
	return &sides[5];
}

int Cube::getLevel() {
	return level;
}

/*
*	Rotates the cube up by changing the sides, and rotating them as needed.
*/
void Cube::rotateUp() {

	getBack()->xAxisFlip();
	
	Map temp = sides[0];
	sides[0] = sides[4];
	sides[4] = sides[5];
	sides[5] = sides[1];
	sides[1] = temp;
	
	
	getRight()->rotateClockwise();

	getLeft()->rotateCounterClockwise();

	getBack()->xAxisFlip();
	
}

/*
*	Rotates the cube down by changing the sides, and rotating them as needed.
*/
void Cube::rotateDown() {
	getBack()->xAxisFlip();
	
	Map temp = sides[0];
	sides[0] = sides[1];
	sides[1] = sides[5];
	sides[5] = sides[4];
	sides[4] = temp;

	getLeft()->rotateClockwise();
	getRight()->rotateCounterClockwise();

	getBack()->xAxisFlip();
}

/*
*	Rotates the cube left by changing the sides, and rotating them as needed.
*/
void Cube::rotateLeft() {

	//invert y axis of the back
	getBack()->yAxisFlip();

	Map temp = sides[0];
	sides[0] = sides[3];
	sides[3] = sides[5];
	sides[5] = sides[2];
	sides[2] = temp;

	getTop()->rotateClockwise();
	getBottom()->rotateCounterClockwise();

	//invert y axis of the back
	getBack()->yAxisFlip();
}

/*
*	Rotates the cube right by changing the sides, and rotating them as needed.
*/
void Cube::rotateRight() {

	//invert y axis of the back
	getBack()->yAxisFlip();

	Map temp = sides[0];
	sides[0] = sides[2];
	sides[2] = sides[5];
	sides[5] = sides[3];
	sides[3] = temp;

	getTop()->rotateCounterClockwise();
	getBottom()->rotateClockwise();

	//invert y axis of the back
	getBack()->yAxisFlip();
}


Map* Cube::getSides() {
	return sides;
}

/*
*
*/
bool Cube::loadCube(int newLevel){
	bool isSuccess = true;
	char str[36];

	level = newLevel;

	memset(str,'\0',36);
	sprintf(str,"levels\\level_%i\\front.txt", level);
	sides[0].setRotation(0);
	sides[0].setColor(Colors::RED);
	isSuccess = sides[0].loadMap(str);

	memset(str,'\0',36);
	sprintf(str,"levels\\level_%i\\top.txt", level);
	sides[1].setRotation(0);
	sides[1].setColor(Colors::BLUE);
	isSuccess = sides[1].loadMap(str);

	memset(str,'\0',36);
	sprintf(str,"levels\\level_%i\\left.txt", level);
	sides[2].setRotation(0);
	sides[2].setColor(Colors::GREEN);
	isSuccess = sides[2].loadMap(str);

	memset(str,'\0',36);
	sprintf(str,"levels\\level_%i\\right.txt", level);
	sides[3].setRotation(0);
	sides[3].setColor(Colors::YELLOW);
	isSuccess = sides[3].loadMap(str);

	memset(str,'\0',36);
	sprintf(str,"levels\\level_%i\\bottom.txt", level);
	sides[4].setRotation(0);
	sides[4].setColor(Colors::WHITE);
	isSuccess = sides[4].loadMap(str);

	memset(str,'\0',36);
	sprintf(str,"levels\\level_%i\\back.txt", level);
	sides[5].setRotation(0);
	sides[5].setColor(Colors::PURPLE);
	isSuccess = sides[5].loadMap(str);

	return isSuccess;
} 

bool Cube::reloadCube() {
	return loadCube(level);
}

bool Cube::loadNextCube() {
	return loadCube(level+1);
}