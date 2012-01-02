#ifndef CUBE
#define CUBE
#include "map.h"

class Cube {
private:
	Map sides[6];
	int level;
public:
	Cube();
	Cube(int);

	Map* getFront();
	Map* getTop();
	Map* getLeft();
	Map* getRight();
	Map* getBottom();
	Map* getBack();

	int getLevel();

	void rotateUp();
	void rotateDown();
	void rotateLeft();
	void rotateRight();

	Map* getSides();

	bool loadCube(int);
	bool reloadCube();
	bool loadNextCube();
};

#endif