/*
 * Tilec.h
 *
 *  Created on: Jul 21, 2017
 *      Author: Carl
 */

#ifndef TILEC_H_
#define TILEC_H_

#include "Maps.h"
#include "TextNFont.h"

class Tilec : public TextNFont {
public:
	const int max = 1000;
	int count = 0;
	int multiW = 1;
	int multiH = 1;
	int tilew = 64;
	int tileh = 64;
	int id = 0;			// 0: blue collision tile

	/*
	 * -1: This Tilec cannot load other levels
	 *  number > 0: This Tile has the ability to load other Levels
	 */
	int LevelToLoad;		// If -1

	/*
	 * 0: new position will be on Top of screen
	 * 1: new position will be on Right of screen
	 * 2: new position will be on Bottom of screen
	 * 3: new position will be on Left of screen
	 */
	int newPos;
	bool getInput;

public:
	float x,y;
	int w,h;
	int type;
	int layer;
	bool mouse;
	bool mouseBox;
	bool collision;
	bool promptSelf;
	bool alive;
	std::string side;		// collision with player

public:	// Core Functions
	void Init(Tilec tilec[]);

	void Load();

	void Free();

	void Update(Tilec tilec[], Map &map, int newMx, int newMy, int mex, int mey, int camx, int camy);

public: // Editor functions

	void RemoveTile(Tilec tilec[], int click);
	void Copy(Tilec tilec[]);
	void RemoveAll(Tilec tilec[]);
	void ChangePos(Tilec tilec[]);
	void Spawn(Tilec tilec[], float x, float y, int w, int h);
	void SpawnAll(Tilec tilec[], int newMx, int newMy, int camx, int camy);
	void MoveTilecs(Tilec tilec[], std::string direction);

public: // Render functions

	void RenderUI(SDL_Renderer *gRenderer, Tilec tilec[], int camx, int camy);

	void Render(SDL_Renderer *gRenderer, Tilec tilec[], int layer, int camx, int camy);

	void RenderDebug(SDL_Renderer *gRenderer, Tilec tilec[], int camx, int camy);

	void RenderHand(SDL_Renderer *gRenderer, Tilec tilec[], int newMx, int newMy, int mex, int mey);

public: // tile saving functions
	void LoadData(Tilec tilec[], std::fstream &fileTileDataL);	// Load Tile data
	std::string SaveData(Tilec tilec[]);	// Save Tile data

public:	// Tile manipulation

public:	// Collision functions

	bool checkCollisionRect( SDL_Rect a, SDL_Rect b );

	// Tile collision check, player x position
	void checkCollisionX(Tilec tilec[], float &x, float y, int w, int h, std::string &jumpstate, float &vX, int &jumps);

	// Tile collision check, player y position
	void checkCollisionY(Tilec tilec[], float x, float &y, int w, int h, std::string &jumpstate, float &vY, int &jumps, bool &jump);

};

#endif /* TILEC_H_ */
