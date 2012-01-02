#include <Windows.h>
#include "stdio.h"
#include "stdlib.h"
#include <string>

#include "cube.h"
#include "map.h"
#include "view_frame.h"
#include "curses.h"
#include "tile_factory.h"
#include "colors.h"



#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3


void draw(WINDOW*,ViewFrame,int,int);
void draw(Cube);
bool canMove(Map);
bool canMove(Map,int,int);
Tile* tileAtCenter(Map);
bool collisionLogic(WINDOW*,WINDOW*[],ViewFrame*,Map,Cube*,int,int,int,bool*);
void gameLoop();
void fillWin(WINDOW*, Map*);
void drawMiniMap(WINDOW*[], ViewFrame);
void init();
void mainMenu();
void helpMenu();
void drawMainMenu(WINDOW*,int);


int main() {
	//view.getCube()->loadCube(4);
	
	//printw("TEST HELLO WORLD");
	
	//getch();
	
	init();
	mainMenu();
	endwin();
	
	return 0;
}

/*
*	Runs through the game logic, ending when there are no more levels
*/
void gameLoop() {
	ViewFrame view;
	bool gameOver = false;
	int windowX, windowY, windowWidth, windowHeight;

	if (!view.getCube()->loadNextCube()) {
		printf("Can not find levels!\n\n");
		system("pause");
		exit(-1);
	}

	

	windowWidth = 10 + 9;
	windowHeight = 10 + 9;
	windowX = (COLS - windowWidth) /2;
	windowY = (LINES - windowHeight) /2 + 3;
	int gameWinX = windowX;
	int gameWinY = windowY;
	int gameWinWidth = 10;
	int gameWinHeight = 10;

	//Set up the window that the map will be rendered in. Add in 2 to both the width and height to accomodate the border
	WINDOW* gameWin = newwin(gameWinWidth+2,gameWinHeight+2,gameWinY,gameWinX);
	wborder(gameWin,0,0,0,0,0,0,0,0);

	keypad(gameWin,true);

	//This will hold the minimaps
	int infoWinWidth = 9;
	int infoWinHeight = 9;
	int infoWinX = gameWinX + gameWinWidth + 2;
	int infoWinY = gameWinY + 1;

	WINDOW* infoWin = newwin(infoWinWidth+2,infoWinHeight+2,infoWinY,infoWinX);
	wborder(infoWin,0,0,0,0,0,0,0,0);
	refresh();
	wrefresh(infoWin);
	

	int subWinOffset = 1;
	
	//Create the windows for the minimap
	WINDOW* miniMapWindows[5];
		
	miniMapWindows[0] = subwin(infoWin,3,3,subWinOffset+infoWinY,subWinOffset+3+infoWinX);		//Top
	miniMapWindows[1] = subwin(infoWin,3,3,subWinOffset+infoWinY+3,subWinOffset+3+infoWinX);	//Front
	miniMapWindows[2] = subwin(infoWin,3,3,subWinOffset+infoWinY+3,subWinOffset+0+infoWinX);	//Left
	miniMapWindows[3] = subwin(infoWin,3,3,subWinOffset+infoWinY+3,subWinOffset+6+infoWinX);	//Right
	miniMapWindows[4] = subwin(infoWin,3,3,subWinOffset+infoWinY+6,subWinOffset+3+infoWinX);	//Bottom
	
	
	int x = 0, y = 0;
	while (!gameOver) {
		
		x = view.getXOffset();
		y = view.getYOffset();
		
		
		//Draw the cube and the minimap based on the current offsets
		draw(gameWin, view,x,y);
		drawMiniMap(miniMapWindows,view);
		
		
		char input;

		input = wgetch(gameWin);
		
		int newX = x, newY = y;
		int direction = -1;		//Store the direction the player is moving in

		switch (input) {
			case 'W':
			case 'w':
			case 3:
				newY++;
				direction = UP;
				break;
			case 'S':
			case 's':
			case 2:
				newY--;
				direction = DOWN;
				break;
			case 'A':
			case 'a':
			case 4:
				newX--;
				direction = LEFT;
				break;
			case 'D':
			case 'd':
			case 5:
				newX++;
				direction = RIGHT;
				break;
			case 27:		//ESC
				gameOver = true;
				break;
			case 'r':
			case 'R':
				//Reload current level
				newX =0;
				newY =0;
				view.getCube()->reloadCube();
				break;
			default:
				
				break;

		}
		
		if (collisionLogic(gameWin,miniMapWindows,&view,view.getView(newX,newY),view.getCube(),newX,newY,direction,&gameOver)) {
			if (canMove(view.getView(newX,newY))) {
				view.setYOffset(newY);
				view.setXOffset(newX);
			}
		} else {
			newX = 0;
			newY = 0;
		}
		
		
		
	}

	werase(gameWin);
	wrefresh(gameWin);
	delwin(gameWin);
	
	for (int i = 0; i < 5; i++) {
		werase(miniMapWindows[i]);
		wrefresh(miniMapWindows[i]);
		delwin(miniMapWindows[i]);
	}

	werase(infoWin);
	wrefresh(infoWin);
	delwin(infoWin);
	
}

/*
*	Draws the current view of the cube on the screen
*	Places the player avatar '@' in the middle
*/
void draw(WINDOW* win, ViewFrame view, int offsetX, int offsetY) {
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			if(i == 4 && j == 4) {
				//draw the player icon in the center
				int color;
				if (tileAtCenter(view.getView(offsetX,offsetY))->getModel() == TileFactory::ICE) {
					color = TileFactory::ICE;
				} else {
					color = COLOR_WHITE;
				}
				wattron(win,COLOR_PAIR(color));
				waddch(win,'@');
				wattroff(win,COLOR_PAIR(color));

				
			} else {
				int tileColor = view.getView(offsetX,offsetY).getTile(j,i).getColor();
				char tileModel = view.getView(offsetX,offsetY).getTile(j,i).getModel();
				wattron(win,COLOR_PAIR(tileColor));
				mvwaddch(win,i+1,j+1,tileModel);
				wattroff(win,COLOR_PAIR(tileColor));
			}
		}
	}
	
	refresh();
			refresh();
			wrefresh(win);
			Sleep(50);
}

/*
*	Checks to see if the x and y offset has a colision on a map
*/
bool canMove(Map map) {
	bool result = true;
	//check to see if the center of the map (Where the player will always be) is avialable to
	//stand on
	
	if (map.getTile(4,4).isSolid()) {
		result = false;
	}
	return result;
}

bool canMove(Map map, int x, int y) {
	bool result = true;
	//check to see if the center of the map (Where the player will always be) is avialable to
	//stand on
	
	if (map.getTile(x,y).isSolid()) {
		result = false;
	}
	
	if (x < 0 || x > 9 || y < 0 || y > 9) {
		result = false;
	}

	return result;
}

/*
*	Returns whatever tile is at the center of the map that is passed in.
*	Useful for quick access to the center tile of the current view.
*/
Tile* tileAtCenter(Map map) {
	Tile* tile;
	//return whatever is in the center of the given Map

	
		tile = &map.getTile(4,4);
	
	return tile;
}

/*
*	Figures out what tile is being collided with and how to respond.
*	Returns false if we need to reset newX and newY and ignore movement. 
*/
bool collisionLogic(WINDOW* win, WINDOW* miniMapWindows[], ViewFrame* view, Map map, Cube* cube,int xOffset, int yOffset,int direction, bool* gameOver) {
	//if a box, try to push the box
	//if stairs, try to go to the next level
	
	//convert offset to the x y position on the front map
	int x = xOffset+4;
	int y = -(-4 + yOffset);  //WRONG
	
	Tile tile = *tileAtCenter(map);
	TileFactory tf;

	switch (tile.getModel()) {
	case TileFactory::BOX:
		//move box right
		//check if space is free
		switch (direction) {
			case UP:
			/*	if (!map->getTile(4,3).isSolid()) {
					if (y <= 0) {
						//push to the bottom of the top map
						cube->getTop()->setTile(*tile,x,9 + y);
					} else {
						cube->getFront()->setTile(*tile,x,y-1);
					}
					
					if (y < 0) {
						cube->getTop()->setTile(*tf.makeEmptyTile(),x,(9+1)+y);
					} else {
						cube->getFront()->setTile(*tf.makeEmptyTile(),x,y);
					}
				}*/
				break;
			case DOWN:
				if (!map.getTile(4,5).isSolid()) {
					if (y >= 9) {
						//push to the top of the bottom map
						cube->getBottom()->setTile(tile,x,y-9);
					} else {
						cube->getFront()->setTile(tile,x,y+1);
					}
					if (y > 9) {
						cube->getBottom()->setTile(tf.makeEmptyTile(),x,y - (9 + 1));
					} else {
						cube->getFront()->setTile(tf.makeEmptyTile(),x,y);
					}
				}
				break;
			case LEFT:
				if (!map.getTile(3,4).isSolid()) {
					if ( x <= 0 ) {
						//push to the right of the left map
						cube->getLeft()->setTile(tile,9 + x,y);
					} else {
						cube->getFront()->setTile(tile,x-1,y);
					}

					if (x < 0) {
						cube->getLeft()->setTile(tf.makeEmptyTile(),9,y);
					} else {
						cube->getFront()->setTile(tf.makeEmptyTile(),x,y);
					}
				}
				break;
			case RIGHT:
				if (!map.getTile(5,4).isSolid()) {
					if ( x >= 9 ) {
						//push to the left of the right map
						cube->getRight()->setTile(tile,x - 9,y);

					} else {
						cube->getFront()->setTile(tile,x+1,y);
					}
					
					if (x > 9) {
						cube->getRight()->setTile(tf.makeEmptyTile(),x - (9 + 1),y);
					} else {
						cube->getFront()->setTile(tf.makeEmptyTile(),x,y);
					}
				}
				break;
		}
		
		break;
	case TileFactory::STAIR:
		//try to load next level in the cube
		if (!cube->loadNextCube()) {
			//loadgameover
			*gameOver = true;
		} else {
			view->setXOffset(0);
			view->setYOffset(0);
			return false;
		}

				
		break;
	case TileFactory::ICE:
		int newX = view->getXOffset(), newY = view->getYOffset();
		switch (direction) {
		case UP:
			//newY++;
			do {
				
				newY = view->getYOffset() + 1;
				view->setYOffset(newY);
				draw(win,*view,view->getXOffset(),view->getYOffset());
				drawMiniMap(miniMapWindows,*view);
			}while (!tileAtCenter(view->getView(view->getXOffset(),view->getYOffset()+1))->isSolid() && tileAtCenter(view->getView(view->getXOffset(),view->getYOffset()))->getModel() == TileFactory::ICE);
			
			break;
		case DOWN:
			do {
				
				newY = view->getYOffset() - 1;
				view->setYOffset(newY);
				draw(win,*view,view->getXOffset(),view->getYOffset());
				drawMiniMap(miniMapWindows,*view);
			}while (!tileAtCenter(view->getView(view->getXOffset(),view->getYOffset()-1))->isSolid() && tileAtCenter(view->getView(view->getXOffset(),view->getYOffset()))->getModel() == TileFactory::ICE);
			
			break;
		case LEFT:
			do {
				newX = view->getXOffset() - 1;
				view->setXOffset(newX);
				draw(win,*view,view->getXOffset(),view->getYOffset());
				drawMiniMap(miniMapWindows,*view);
				
			}while (!tileAtCenter(view->getView(view->getXOffset()-1,view->getYOffset()))->isSolid() && tileAtCenter(view->getView(view->getXOffset(),view->getYOffset()))->getModel() == TileFactory::ICE);
			break;
		case RIGHT:
			do {
				newX = view->getXOffset() + 1;
				view->setXOffset(newX);
				draw(win,*view,view->getXOffset(),view->getYOffset());
				drawMiniMap(miniMapWindows,*view);
			}while (!tileAtCenter(view->getView(view->getXOffset()+1,newY))->isSolid() && tileAtCenter(view->getView(view->getXOffset(),view->getYOffset()))->getModel() == TileFactory::ICE);
			break;
		}

		

		return false;
		break;
	}

	return true;
}

/*
*	Fills the window with the color of the map. 
*	Used for the minimaps color.
*/
void fillWin(WINDOW* win, Map* map) {
	
	int color = map->getColor();
	int rotation = map->getRotation();
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			wattron(win,COLOR_PAIR(color));
			mvwaddch(win,i,j,' ');
			wattroff(win,COLOR_PAIR(color));
		}
	}

	int x, y;

	switch (rotation) {
		case 0:
			x = 1;
			y = 0;
			break;
		case 1:
			x = 2;
			y = 1;
			break;
		case 2:
			x = 1;
			y = 2;
			break;
		case 3:
			x = 0;
			y = 1;
			break;
	}
	mvwaddch(win,y,x,'^');
	
}

/*
*	Fills in all the miniMapWindows to be displayed properly and then displays them.
*/
void drawMiniMap(WINDOW* miniMapWindows[], ViewFrame view) {
	fillWin(miniMapWindows[0],view.getCube()->getTop());
	fillWin(miniMapWindows[1],view.getCube()->getFront());
	fillWin(miniMapWindows[2],view.getCube()->getLeft());
	fillWin(miniMapWindows[3],view.getCube()->getRight());
	fillWin(miniMapWindows[4],view.getCube()->getBottom());

	for (int i = 0; i < 5; i++)  {
		wrefresh(miniMapWindows[i]);
	}
	
}

/*
*	Initialized a bunch of pdcurses stuff, as well as the color pairs that will be used.
*/
void init() {
	initscr();
	start_color();
	keypad(stdscr, true);
	noecho();
	curs_set(0);
	raw();

	init_pair(TileFactory::WALL,COLOR_MAGENTA,COLOR_MAGENTA);
	init_pair(TileFactory::ICE,COLOR_WHITE,COLOR_CYAN);
	init_pair(TileFactory::STAIR,COLOR_RED,COLOR_WHITE);
	init_pair(TileFactory::BOX,COLOR_RED,COLOR_GREEN);
	init_pair(COLOR_RED,COLOR_RED,COLOR_BLACK);
	init_pair(Colors::WHITE_ON_BLACK,COLOR_WHITE,COLOR_BLACK);
	init_pair(Colors::RED,COLOR_RED,COLOR_RED);
	init_pair(Colors::BLUE,COLOR_CYAN,COLOR_CYAN);
	init_pair(Colors::GREEN,COLOR_GREEN,COLOR_GREEN);
	init_pair(Colors::WHITE,COLOR_WHITE,COLOR_WHITE);
	init_pair(Colors::YELLOW,COLOR_YELLOW,COLOR_YELLOW);
	init_pair(Colors::PURPLE,COLOR_MAGENTA,COLOR_MAGENTA);
	init_pair(Colors::GREEN_ON_BLACK,COLOR_GREEN,COLOR_BLACK);
	
	
}
/*
*	Creates a menu. This will serve as the lauching place for the game and Help menu.
*/
void mainMenu() {
	int windowX, windowY, windowWidth, windowHeight;
	windowWidth = 15;
	windowHeight = 10;
	windowX = (COLS - windowWidth) /2;
	windowY = (LINES - windowHeight) /2;

	WINDOW* window = newwin(windowHeight, windowWidth, windowY, windowX);

	keypad(window,true);
	int index = 0;
	char ch = 0;
	do {
		
		werase(window);
		switch (ch) {
		case 2:		//Down key
			if (index < 2) {
				index++;
			}
			break;
		case 3:		//Up key
			if (index > 0) {
				index--;
			}
			break;
		case 13:	//Enter Key
			if (index == 0) {
				gameLoop();
			} else if (index == 1) {
				helpMenu();
			} else {
				ch = 'Q';
			}
			break;
		}

		drawMainMenu(window, index);

		if (ch != 'Q') {
			ch = getch();
		}
		
	}while ( ch != 'Q' && ch != 'q');

	werase(window);
	refresh();
	wrefresh(window);
	delwin(window);
}

/*
*	Creates a temporary window to display a bit of information about the game.
*/
void helpMenu() {
	int windowX, windowY, windowWidth, windowHeight, numberOfTiles, startX, startY;

	numberOfTiles = 4;
	windowWidth = 65;
	windowHeight = 10 + 5;
	windowX = (COLS - windowWidth) /2;
	windowY = (LINES - windowHeight) /2;
	WINDOW* window = newwin(windowHeight, windowWidth, windowY, windowX);
	wborder(window,0,0,0,0,0,0,0,0);

	startX = 1;
	startY = (windowHeight - numberOfTiles) / 2 -3;

	//Display what each tile is
	wattron(window,COLOR_PAIR(TileFactory::WALL));
	mvwaddch(window,startY,startX,TileFactory::WALL);
	wattroff(window, COLOR_PAIR(TileFactory::WALL));
	mvwprintw(window,startY,startX+1," - Soild Wall");
	startY++;
	startY++;
	wattron(window,COLOR_PAIR(TileFactory::BOX));
	mvwaddch(window,startY,startX,TileFactory::BOX);
	wattroff(window, COLOR_PAIR(TileFactory::BOX));
	mvwprintw(window,startY,startX+1," - Pushable Box");
	startY++;
	startY++;
	wattron(window,COLOR_PAIR(TileFactory::ICE));
	mvwaddch(window,startY,startX,TileFactory::ICE);
	wattroff(window, COLOR_PAIR(TileFactory::ICE));
	mvwprintw(window,startY,startX+1," - Slide on Ice");
	startY++;
	startY++;
	wattron(window,COLOR_PAIR(TileFactory::STAIR));
	mvwaddch(window,startY,startX,TileFactory::STAIR);
	wattroff(window, COLOR_PAIR(TileFactory::STAIR));
	mvwprintw(window,startY,startX+1," - Stairs to next Cube");
	startY++;
	startY++;
	
	//Create a subwindow to have the text wrap around the border
	WINDOW* windowText = subwin(window,5,windowWidth -2 ,windowY+startY, windowX+ startX);
	wprintw(windowText,"Navigate each cube! Boxes can not be pushed up, you must change the rotation of a side if you wish to push some boxes! Pay attention to the sides and their current rotations on the minimap. Learn how the cube rotates to use it to your advantage!");
	
	refresh();
	wrefresh(windowText);
	wrefresh(window);
	
	//wait for key press before closing
	getch();


	//Clean up
	werase(window);
	werase(windowText);

	refresh();
	wrefresh(window);
	wrefresh(windowText);

	delwin(window);
	delwin(windowText);
}

/*
*	Draw the main menu with the seleted index highlighted.
*/
void drawMainMenu(WINDOW* window,int index) {
	const int numOfOptions = 3;
	char options[numOfOptions][10];

	int startY = 3;
	int startX = 4;
	
	for (int i = 0; i < 3; i++) {
		memset(options[i],'\0',10);
	}

	sprintf(options[0], "Play!");
	sprintf(options[1], "Help!");
	sprintf(options[2], "Quit!");


	werase(window);
	wborder(window,0,0,0,0,0,0,0,0);
	
		mvwprintw(window,0,0,"Dream in Cube");
		for (int i = 0; i<numOfOptions; i++) {
			if (i == index) {
				wattron(window, COLOR_PAIR(Colors::GREEN_ON_BLACK));
				mvwprintw(window,startY+i,startX, options[i]);
				wattroff(window, COLOR_PAIR(Colors::GREEN_ON_BLACK));
			}else {
			mvwprintw(window,startY+i,startX, options[i]);
			}
			
		}
		refresh();
		wrefresh(window);
}