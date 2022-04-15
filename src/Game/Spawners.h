/*
 * Spawners.h
 *
 *  Created on: Dec 18, 2016
 *      Author: Carl
 */

#ifndef LOCAL_SPAWNERS_H_
#define LOCAL_SPAWNERS_H_

#include <SDL2/SDL_ttf.h>

#include "../Helper.h"
#include "../LTexture.h"
#include "TextNFont.h"

class Spawner: public Helper, public TextNFont {
public:
	int count = 0;
	LTexture gSpawner;
	LTexture gText;

public:
	// Spawner variables
	float spawnedW, spawnedH;
	float x,y,w,h;
	int spawnTimer;
	int spawned;
	int maxSpawn;
	bool alive;
	bool moused;
	int type;
	int frame;
	int tick;
	SDL_Rect rSpawner[5];
	float reduction;

public:	// functions
	// Load
	void load(SDL_Renderer* gRenderer);

	// Free
	void free();

	// Initialize Spawner
	void init(Spawner spawner[]);

	// Spawn a Spawner
	void spawn(Spawner spawner[], float x, float y, float w, float h);

	// Remove Spawner
	void remove(Spawner spawner[]);

	// Update Spawner
	void update(Spawner spawner[], int targetX, int targetY, int mx, int my, int camx, int camy);

	// Render Spawner
	void render(Spawner spawner[], int camx, int camy, SDL_Renderer* gRenderer);
};

#endif /* LOCAL_SPAWNERS_H_ */
