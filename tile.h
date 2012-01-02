#ifndef TILE
#define TILE


class Tile {
private:
	char model; //A character to represent the given tile
	char description[26]; //A brief description of the tile
	bool solid;
	int color;
public:
	Tile();
	Tile(char);
	Tile(char,char*,bool,int);
	void setModel(char);
	char getModel();
	char* getDescription();
	bool isSolid();
	int getColor();

};

#endif