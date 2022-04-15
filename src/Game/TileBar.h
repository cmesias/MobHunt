/*
 * TileBar.h
 *
 *  Created on: Jan 16, 2017
 *      Author: Carl
 */

#ifndef LOCAL_TILEBAR_H_
#define LOCAL_TILEBAR_H_

#include "../LWindow.h"
#include "../LTexture.h"

//Tile Class
class TileBar {
public:

	static const int TILES_UNIQUE = 100;	// Total number of Unique Tiles

	const int HowManyTilesHorizontal = 10;
	const int HowManyTilesVertical = 10;

	// Texture clip sizes
    static const int TilesCutSizeW   = 16;
    static const int TilesCutSizeH = 16;

	static const int tileSizeInWorldW = 32;
	static const int tileSizeInWorldH = 32;
	int tileCount;

public:
	SDL_Rect clip;			//Tile clips
	int x;					//Tile x position
	int y;					//Tile y position
	int w;					//Tile width
	int h;					//Tile height
	int angle;				//Tile angle
	int alpha;				//Tile alpha
	int id;					//Tile ID number
	std::stringstream name;	//Tile name
	bool MouseOnBlock;		//Tile has mouse above its
	bool screen;			//Tile in screen
	bool selected;			//Tile selected (multiples)
	bool alive;				//Tile is alive

	// Resources
	LTexture gTileBar;
	LTexture gRect;

	// Indirect
	//const int TILES_UNIQUE = 288;	// Total number of Unique Tiles

	// 57 tiles width
	// 31 tiles width
	//const int TILES_UNIQUE = 900;	// Total number of Unique Tiles

	int MARGINX = 0;
	int MARGINY = 0;
	SDL_Rect rRect[6];				// Colors 64x64, [0: Green], [1: Orange], [2: White], [3: Blue], [4: Purple], [5: Dark Green fill, White border]
	bool touching;					// Player is hovering over Tile Bar

	bool checkCollision(int x, int y, int w, int h, int x2, int y2, int w2, int h2);

	void load(SDL_Renderer *gRenderer);

	void init(TileBar tilebar[]);

	void free();

	void placeTile(TileBar tilebar[], int x, int y, int w, int h,
									int startx, int starty, int endW, int endH,
									std::string collision);

	void placeTileBar(TileBar tilebar[]);

	void update(TileBar tilebar[], LWindow gWindow, int mx, int my, float camx, float camy);

	void selectBlock(TileBar tilebar[], int &tile_selection);

	void selectBlockMultiple(TileBar tilebar[], int &tile_selection, int mx, int my);

	void moveBarSelection(TileBar tilebar[], std::string direction);

	void render(SDL_Renderer *gRenderer, TileBar tilebar[], int tile_selection);
};

#endif /* LOCAL_TILEBAR_H_ */
