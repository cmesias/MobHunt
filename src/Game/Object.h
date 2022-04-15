/*
 * Object.h
 *
 *  Created on: Apr 18, 2017
 *      Author: Carl
 */

#ifndef OBJECT_H_
#define OBJECT_H_

#include <SDL2/SDL.h>

class Object  {

public:	// constant variables, separate from instances
	int count;				// Player count
	const int max = 10;		// Max count

public:	// variables
	float x;
	float y;
	float *xFollow;
	float *yFollow;
	int w;
	int h;

	/* 0: Player attack
	 * 1: Player attack
	 * 2: Slime attack
	 */
	int id;
	int timer;
	float timeAlive;
	bool alive;

	/*
	 * For example: if slime[3] spawned an attack object, then the object will save its index (i = 3)
	 * Example #2: if slime[68] spawned an attack object, then the object will save its index (i = 68)
	 * This is used for example when the slime spawns an attack object, and we are checking collision between
	 * the attack object spawned by the enemy, and the hit-box of the Player.
	 *
	 * Example code: if the attack-object (spawned by theenemy), hits the Player's hit box, then we have a reference
	 * to which exact slime hit the Player when the attack-object saved the Slime's index.
	 */
	int CreatorIndex;

public:	// Initial functions
	void init(Object object[]);
	void update(Object object[]);
	void render(SDL_Renderer *gRenderer, Object object[], int camx, int camy);

public:	// editor functions
	void spawn(Object object[], float x, float y, int w, int h, int id, int CreatorIndex = -1);
	void spawnFollow(Object object[], float x, float y,
			int w, int h, int id,
			float *xFollow, float *yFollow);

};

#endif /* OBJECT_H_ */
