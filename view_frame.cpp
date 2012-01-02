#include "view_frame.h"

#include "map.h"
#include "cube.h"


ViewFrame::ViewFrame() {
	//cube = NULL;
	xOffset = 0;
	yOffset = 0;
}


/*
*	Returns a composite map centered around the given x and y offsets of the current front face and any of the parts of the sides 
*	that may be in view
*/
Map ViewFrame::getView(int x, int y) {
	// (0,0) origin

	/*
	*	Take (j,i) as an (x,y) pair and place it on the current map at (x_itteration, y_itteration)
	*/

	Map map = *cube.getFront();	//Placeholder incase of error
	
	if(x < 0 && y > 0) {	//check to see if the top left corner is the back
		//bottom right of the back is in the top left of the map
		
		for(int i = (10 - y), x_itteration = 0, y_itteration = 0; i < 10; i++, y_itteration++, x_itteration = 0) {
			for (int j = (10 + x); j < 10; j++, x_itteration++) {
				Tile tile =  cube.getBack()->getTile(j,i);
				map.setTile(tile,x_itteration,y_itteration);
			}
		}

		//place the bottom of the top map in the rest of the space
		for(int i = (10 - y), x_itteration = -x, y_itteration = 0; i < 10; i++, y_itteration++, x_itteration = -x) {
			for (int j = 0; j < 10 + x; j++, x_itteration++) {
				Tile tile = cube.getTop()->getTile(j,i);
				
				map.setTile(tile,x_itteration,y_itteration);
			}
		}

		//place the right of the left map on the left side of the map
		for(int i = 0, x_itteration = 0, y_itteration = y; i < 10-y; i++, y_itteration++, x_itteration = 0) {
			for (int j = (10 + x); j < 10; j++, x_itteration++) {
				Tile tile =  cube.getLeft()->getTile(j,i);
			
				map.setTile(tile,x_itteration,y_itteration);
			}
		}

		//fill in the remaining area with the top left corner of the front map
		for(int i = 0, x_itteration = -x, y_itteration = y; i < 10-y; i++, y_itteration++, x_itteration = -x) {
			for (int j = 0; j < (10 + x); j++, x_itteration++) {
				Tile tile =  cube.getFront()->getTile(j,i);
			
				map.setTile(tile,x_itteration,y_itteration);
			}
		}
	} 
	
	else if (x > 0 && y > 0) {	//check to see if the top right corner is the back
		//bottom left of the back is in the top right of the map
		for(int i = (10 - y), x_itteration = (10 - x), y_itteration = 0; i < 10; i++, y_itteration++, x_itteration = (10 - x)) {
			for (int j = 0; j < x; j++, x_itteration++) {
				Tile tile =  cube.getBack()->getTile(j,i);
				map.setTile(tile,x_itteration,y_itteration);
			}
		}

		//place the bottom of the top map in the rest of the space
		for(int i = (10 - y), x_itteration = 0, y_itteration = 0; i < 10; i++, y_itteration++, x_itteration = 0) {
			for (int j = x; j < 10; j++, x_itteration++) {
				Tile tile =  cube.getTop()->getTile(j,i);
				map.setTile(tile,x_itteration,y_itteration);
			}
		}

		//place the left of the right map on the right side of the map
		for(int i = 0, x_itteration = 10-x, y_itteration = y; i < 10; i++, y_itteration++, x_itteration = 10-x) {
			for (int j = 0; j < x; j++, x_itteration++) {
				//temporary fix for the heap coruption in the release version
				
				if (j <10 && j >= 0 && x_itteration <10 && x_itteration>=0 && y_itteration<10&&y_itteration>=0&&i<10&&i>=0){
					Tile tile =  cube.getRight()->getTile(j,i);
				
					map.setTile(tile,x_itteration,y_itteration);
				}
			}
		}

		//fill in the remaining area with the top right corner of the front map
		for(int i = 0, x_itteration = 0, y_itteration = y; i < 10-y; i++, y_itteration++, x_itteration = 0) {
			for (int j = x; j < 10; j++, x_itteration++) {
				Tile tile =  cube.getFront()->getTile(j,i);
			
				map.setTile(tile,x_itteration,y_itteration);
			}
		}
	}
	else if (x > 0 && y == 0) {	//check to see if the right side only is on screen
		
		//place the left of the right map on the right side of the map
		for(int i = 0, x_itteration = 10-x, y_itteration = 0; i < 10; i++, y_itteration++, x_itteration = 10 - x) {
			for (int j = 0; j < x; j++, x_itteration++) {
				Tile tile =  cube.getRight()->getTile(j,i);
				map.setTile(tile,x_itteration,y_itteration);
			}
		}

		//fill in the remaining area with right of the front map
		for(int i = 0, x_itteration = 0, y_itteration = 0; i < 10; i++, y_itteration++, x_itteration = 0) {
			for (int j = x; j < 10; j++, x_itteration++) {
				Tile tile =  cube.getFront()->getTile(j,i);
			
				map.setTile(tile,x_itteration,y_itteration);
			}
		}
	}
	else if (x < 0 && y == 0) {	//check to see if the left side only is on screen
		
		//place the Right of the Left map on the right side of the map
		for(int i = 0, x_itteration = 0, y_itteration = 0; i < 10; i++, y_itteration++, x_itteration = 0) {
			for (int j = 10+x; j < 10; j++, x_itteration++) {
				Tile tile =  cube.getLeft()->getTile(j,i);
				map.setTile(tile,x_itteration,y_itteration);
			}
		}

		//fill in the remaining area with right of the front map
		for(int i = 0, x_itteration = -x, y_itteration = 0; i < 10; i++, y_itteration++, x_itteration = -x) {
			for (int j = 0; j < 10+x; j++, x_itteration++) {
				Tile tile =  cube.getFront()->getTile(j,i);
			
				map.setTile(tile,x_itteration,y_itteration);
			}
		}
	}
	else if (x == 0 && y > 0) {	//check to see if the top side only is on screen
		
		//place the bottom of the top map on the top side of the map
		for(int i = 10 - y, x_itteration = 0, y_itteration = 0; i < 10; i++, y_itteration++, x_itteration = 0) {
			for (int j = 0; j < 10; j++, x_itteration++) {
				Tile tile =  cube.getTop()->getTile(j,i);
				map.setTile(tile,x_itteration,y_itteration);
			}
		}

		//fill in the remaining area with right of the front map
		for(int i = 0, x_itteration = 0, y_itteration = y; i < 10-y; i++, y_itteration++, x_itteration = 0) {
			for (int j = 0; j < 10; j++, x_itteration++) {
				Tile tile =  cube.getFront()->getTile(j,i);
			
				map.setTile(tile,x_itteration,y_itteration);
			}
		}
	}
	else if (x == 0 && y < 0) {	//check to see if the bottom side only is on screen
		
		//place the top of the bottom map on the bottom side of the map
		for(int i = 0, x_itteration = 0, y_itteration = 10+y; i < -y; i++, y_itteration++, x_itteration = 0) {
			for (int j = 0; j < 10; j++, x_itteration++) {
				Tile tile =  cube.getBottom()->getTile(j,i);
				map.setTile(tile,x_itteration,y_itteration);
			}
		}

		//fill in the remaining area with right of the front map
		for(int i = -y, x_itteration = 0, y_itteration = 0; i < 10; i++, y_itteration++, x_itteration = 0) {
			for (int j = 0; j < 10; j++, x_itteration++) {
				Tile tile =  cube.getFront()->getTile(j,i);
			
				map.setTile(tile,x_itteration,y_itteration);
			}
		}
	}

	else if(x < 0 && y < 0) {	//check to see if the bottom left corner is the back
		//top right of the back is in the bottom left of the map
		
		for(int i = 0, x_itteration = 0, y_itteration = 10 + y; i < -y; i++, y_itteration++, x_itteration = 0) {
			for (int j = (10 + x); j < 10; j++, x_itteration++) {
				Tile tile =  cube.getBack()->getTile(j,i);
				map.setTile(tile,x_itteration,y_itteration);
			}
		}

		//place the top of the bottom map in the rest of the space
		for(int i = 0, x_itteration = -x, y_itteration = 10 + y; i < -y; i++, y_itteration++, x_itteration = -x) {
			for (int j = 0; j < 10 + x; j++, x_itteration++) {
				Tile tile = cube.getBottom()->getTile(j,i);
				
				map.setTile(tile,x_itteration,y_itteration);
			}
		}

		//place the bottom right of the left map on the left side of the map
		for(int i = -y, x_itteration = 0, y_itteration = 0; i < 10; i++, y_itteration++, x_itteration = 0) {
			for (int j = (10 + x); j < 10; j++, x_itteration++) {
				Tile tile =  cube.getLeft()->getTile(j,i);
			
				map.setTile(tile,x_itteration,y_itteration);
			}
		}

		//fill in the remaining area with the bottom left corner of the front map
		for(int i = -y, x_itteration = -x, y_itteration = 0; i < 10; i++, y_itteration++, x_itteration = -x) {
			for (int j = 0; j < (10 + x); j++, x_itteration++) {
				Tile tile =  cube.getFront()->getTile(j,i);
			
				map.setTile(tile,x_itteration,y_itteration);
			}
		}
	} 

	else if(x > 0 && y < 0) {	//check to see if the bottom right corner is the back
		//top left of the back is in the bottom right of the map
		
		for(int i = 0, x_itteration = (10 - x), y_itteration = 10 + y; i < -y; i++, y_itteration++, x_itteration = (10 - x)) {
			for (int j = 0; j < x; j++, x_itteration++) {
				Tile tile =  cube.getBack()->getTile(j,i);
				map.setTile(tile,x_itteration,y_itteration);
			}
		}

		//place the top of the bottom map in the rest of the space
		for(int i = 0, x_itteration = 0, y_itteration = 10 + y; i < -y; i++, y_itteration++, x_itteration = 0) {
			for (int j = x; j < 10; j++, x_itteration++) {
				Tile tile = cube.getBottom()->getTile(j,i);
				
				map.setTile(tile,x_itteration,y_itteration);
			}
		}

		//place the bottom left of the right map on the left side of the map
		for(int i = -y, x_itteration = (10 - x), y_itteration = 0; i < 10; i++, y_itteration++, x_itteration = (10 - x)) {
			for (int j = 0; j < x; j++, x_itteration++) {
				Tile tile =  cube.getRight()->getTile(j,i);
			
				map.setTile(tile,x_itteration,y_itteration);
			}
		}

		//fill in the remaining area with the bottom right corner of the front map
		for(int i = -y, x_itteration = 0, y_itteration = 0; i < 10; i++, y_itteration++, x_itteration = 0) {
			for (int j = x; j < 10; j++, x_itteration++) {
				Tile tile =  cube.getFront()->getTile(j,i);
			
				map.setTile(tile,x_itteration,y_itteration);
			}
		}
	}
	
	return map;
}

/*
*	Returns a pointer to the Cube that the view is working with. 
*	Used to mainly for rotating the cube when the x or y offsets 
*	go out of bounds for the front face.
*/
Cube* ViewFrame::getCube() {
	return &cube;
}

void ViewFrame::setCube(Cube cube) {
	this->cube = cube;
}

int ViewFrame::getXOffset() {
	return xOffset;
}

/*
*	Makes sure the xOffset stay within the range of the front side of the cube.
*	Rotate the cube as needed.
*/
void ViewFrame::setXOffset(int xOffset) {
	this->xOffset = xOffset;

	if (xOffset  < -4) {
			this->xOffset = 5;
			getCube()->rotateRight();
		} 
		else if (xOffset  > 5) {
			this->xOffset = -4;
			getCube()->rotateLeft();
		}
}

int ViewFrame::getYOffset() {
	return yOffset;
}

/*
*	Makes sure the yOffset stay within the range of the front side of the cube.
*	Rotate the cube as needed.
*/
void ViewFrame::setYOffset(int yOffset) {
	this->yOffset = yOffset;

	if(yOffset > 4) {
			this->yOffset = -5;
			getCube()->rotateDown();
		} 
		else if (yOffset < -5) {
			this->yOffset = 4;
			getCube()->rotateUp();
		} 
}
