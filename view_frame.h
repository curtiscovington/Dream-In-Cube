#ifndef VIEWFRAME
#define VIEWFRAME
#include "cube.h"

/*
*	Responsible for the view of the cube.
*/
class ViewFrame {
private:
	Cube cube;
	int xOffset;
	int yOffset;
public:
	ViewFrame();
	Cube* getCube();
	void setCube(Cube);

	int getXOffset();
	void setXOffset(int);
	int getYOffset();
	void setYOffset(int);

	Map getView(int, int);

};


#endif