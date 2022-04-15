/*
 * Object.cpp
 *
 *  Created on: Apr 18, 2017
 *      Author: Carl
 */

#include <iostream>
#include <SDL2/SDL_image.h>
#include "Object.h"

// Initialize
void Object::init(Object object[]) {
	count = 0;
	for (int i=0; i < max; i++) {
		object[i].x 	= 0;
		object[i].y 	= 0;
		object[i].w 	= 32;
		object[i].h 	= 32;
		object[i].id	= -1;
		object[i].timer	= 0;
		object[i].timeAlive	= 300;
		object[i].alive = false;
	}
}

// Update
void Object::update(Object object[]) {
	for (int i=0; i < max; i++) {
		if (object[i].alive) {

			// Slash attack object (despawns after 1 frame)
			if (object[i].id == 0) {

				// death timer
				object[i].timer += 1;

				// After x frames
				if (object[i].timer > object[i].timeAlive) {

					// Remove object
					object[i].alive = false;
					count--;
				}
			}

			// Down-stab attack object (follows the player's sword position, it will despawn when it collides with a slime or tilec block)
			if (object[i].id == 1) {
				object[i].x = *object[i].xFollow;
				object[i].y = *object[i].yFollow;
			}

			// Slime attack object (despawns after x frame)
			if (object[i].id == 2) {

				// death timer
				object[i].timer += 1;

				// After x frames
				if (object[i].timer > object[i].timeAlive) {

					// Remove object
					object[i].alive = false;
					count--;
				}
			}
		}
	}
}

// Render
void Object::render(SDL_Renderer *gRenderer, Object object[], int camx, int camy) {
	for (int i=0; i < max; i++) {
		if (object[i].alive) {

			// Render Player attack objects
			if (object[i].id == 0 || object[i].id == 1) {
				SDL_Rect tempR = { object[i].x-camx, object[i].y-camy, object[i].w, object[i].h };
				SDL_SetRenderDrawColor(gRenderer, 251, 141, 255, 255);
				SDL_RenderDrawRect(gRenderer, &tempR);
			}

			// Render Slime attack objects
			else if (object[i].id == 2) {
				SDL_Rect tempR = { object[i].x-camx, object[i].y-camy, object[i].w, object[i].h };
				SDL_SetRenderDrawColor(gRenderer, 20, 200, 20, 255);
				SDL_RenderDrawRect(gRenderer, &tempR);
			}
		}
	}
}

// Spawn
void Object::spawn(Object object[], float x, float y, int w, int h, int id, int CreatorIndex) {
	for (int i=0; i < max; i++) {
		if (!object[i].alive) {
			// Player attack
			if (id == 0) {
				object[i].timeAlive	= 1;
			}

			// Player attack
			else if (id == 1) {
				object[i].timeAlive	= 0;
			}

			// Slime attack
			else if (id == 2) {
				object[i].timeAlive	= 1;
			}
			object[i].x 	= x;
			object[i].y 	= y;
			object[i].w 	= w;
			object[i].h 	= h;
			object[i].id	= id;
			object[i].CreatorIndex	= CreatorIndex;
			object[i].timer	= 0;
			object[i].alive = true;
			count++;
			break;
		}
	}
}
void Object::spawnFollow(Object object[], float x, float y,
		int w, int h, int id,
		float *xFollow, float *yFollow) {
	for (int i=0; i < max; i++) {
		if (!object[i].alive) {
			// Player attack
			if (id == 0) {
				object[i].timeAlive	= 1;
			}

			// Player attack
			else if (id == 1) {
				object[i].timeAlive	= 0;
			}

			// Slime attack
			else if (id == 2) {
				object[i].timeAlive	= 300;
			}
			object[i].x 	= x;
			object[i].y 	= y;
			object[i].xFollow 	= xFollow;
			object[i].yFollow 	= yFollow;
			object[i].w 	= w;
			object[i].h 	= h;
			object[i].id	= id;
			object[i].timer	= 0;
			object[i].alive = true;
			count++;
			break;
		}
	}
}

