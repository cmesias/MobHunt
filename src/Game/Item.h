/*
 * Item.h
 *
 *  Created on: Apr 18, 2017
 *      Author: Carl
 */

#ifndef ITEM_H_
#define ITEM_H_


#include "TextNFont.h"
#include "../LWindow.h"
#include "../LTexture.h"
#include "../Helper.h"

#include <fstream>
#include <sstream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Item: public Helper, public TextNFont  {

public:	// Parent object Variables
	static const int ITEMS_UNIQUE = 28;
	int multiW = 1;
	int multiH = 1;
	const int max = 100;		// max number of instances
	int count;					// number of instances


	/* Item types
	 *
	 * 0: Wooden Sword
	 * 1: Rusty Sword
	 * 2: Iron Sword
	 *
	 * 11: Tool? 						7 x 17
	 * 12: Hammer 						10 x 21
	 *
	 * 13: Thicc Sword 					10 x 25
	 * 14: Long Rapier 					8 x 27
	 * 15: Golden Long Sword 			10 x 29
	 * 16: Silver Sword 				6 x 27
	 * 17: Silver Thicc Sword 			10 x 24
	 * 18: Silver Thicc Sword w/ Gold 	10 x 24
	 *
	 * 19: Broken Iron sword short 		6 x 9
	 * 20: Broken Iron sword 			6 x 12
	 * 21: Broken Golden sword 			6 x 12
	 * 22: Broken Silver sword 			6 x 12
	 *
	 * 23: Bomb							14 x 13
	 * 24: Heart 						10 x 10
	 * 25: Coin 						10 x 10
	 * 26: Silver key 					5 x 10
	 * 27: Gold key 					5 x 10
	 * 28: Green health					8 x 7
	 */
	int id;			// id of instances

	// These get applied to the players stats
	float damage;
	float slashAtkSpe;

public:	// Object Variables
	float x, y;
	float w, h;
	float x2, y2;
	float vX, vY;
	bool collision;
	bool mouse;
	bool mouseBox;
	bool onScreen;
	bool alive;
	bool promptSelf;

public:	// Resources
	LTexture gSwords;

	/* Item types
	 *
	 * 0: Wooden Sword
	 * 1: Rusty Sword
	 * 2: Iron Sword
	 *
	 * 11: Tool? 						7 x 17
	 * 12: Hammer 						10 x 21
	 *
	 * 13: Thicc Sword 					10 x 25
	 * 14: Long Rapier 					8 x 27
	 * 15: Golden Long Sword 			10 x 29
	 * 16: Silver Sword 				6 x 27
	 * 17: Silver Thicc Sword 			10 x 24
	 * 18: Silver Thicc Sword w/ Gold 	10 x 24
	 *
	 * 19: Broken Iron sword short 		6 x 9
	 * 20: Broken Iron sword 			6 x 12
	 * 21: Broken Golden sword 			6 x 12
	 * 22: Broken Silver sword 			6 x 12
	 *
	 * 23: Bomb							14 x 13
	 * 24: Heart 						10 x 10
	 * 25: Coin 						10 x 10
	 * 26: Silver key 					5 x 10
	 * 27: Gold key 					5 x 10
	 * 28: Green health					8 x 7
	 */
	SDL_Rect rSwords[29];

public:	// Variables
	float hoverAmount;
	float hoverDir;
	const float yOffset = -10;

public:	// Core functions

	// Initialize
	void Init(Item item[]);

	// Load
	void Load(SDL_Renderer *gRenderer);

	// Load
	void Free();

	// Copy
	void Copy(Item item[]);

	// Remove
	void Remove(Item item[], int click);

	// Remove all
	void RemoveAll(Item item[]);

	// Spawn
	void Spawn(Item item[], float x, float y, int id);

	// Spawn and throw in a direction
	void SpawnAndThrowItem(Item item[], float x, float y, int id, float vX, float vY);

	void SpawnAll(Item item[], int newMx, int newMy, int camx, int camy);

	// Update
	void Update(Item item[], int newMx, int newMy, int mex, int mey, int camx, int camy,
			  float targetX, float targetY, float targetW, float targetH);

	// Update
	void UpdateEditor(Item item[], int newMx, int newMy, int mex, int mey, int camx, int camy);

	bool PlayerBehindItem;

	void RenderBehindPlayer(SDL_Renderer *gRenderer, Item item[], int camx, int camy);

	void RenderOnTopOfPlayer(SDL_Renderer *gRenderer, Item item[], int camx, int camy);

	void RenderUI(SDL_Renderer *gRenderer, Item item[], int camx, int camy);

	// Render debug
	void RenderDebug(SDL_Renderer *gRenderer, Item item[], int camx, int camy);

	void RenderHand(SDL_Renderer *gRenderer, Item item[], int newMx, int newMy, int mex, int mey);

	void setStatsBasedOnType(Item tile[], int);

public:	// Getter functions for Editor

	// Get size of sword in Hand
	int getItemSizeW();
	int getItemSizeH();

	// Get size of item from item[i]
	int getW(Item item[], int i);
	int getH(Item item[], int i);

public: // Save functions

	void LoadData(Item item[], std::fstream &fileTileDataL);

	std::string SaveData(Item item[]);
};

#endif /* ITEM_H_ */
