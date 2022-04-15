/*
 * Spawners.cpp
 *
 *  Created on: Dec 18, 2016
 *      Author: Carl
 */

#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mouse.h>

#include "Spawners.h"

// Load
void Spawner::load(SDL_Renderer* gRenderer){
	gSpawner.loadFromFile(gRenderer, "resource/gfx/spawner.png");
	setClips(rSpawner[4], 0, 0, 128, 128);
	setClips(rSpawner[3], 128, 0, 128, 128);
	setClips(rSpawner[2], 256, 0, 128, 128);
	setClips(rSpawner[1], 384, 0, 128, 128);
	setClips(rSpawner[0], 512, 0, 128, 128);
	gSpawner.setColor(255, 25, 25);
	gFont = TTF_OpenFont("resource/fonts/Viga-Regular.ttf", 18);

	/*for (int i=0; i<5; i++){
		setClips(rSpawner[i], i*128, 0, 128, 128);
	}*/
	LoadFonts();
}

// Free
void Spawner::free(){
	gFont = NULL;
	gSpawner.free();
	gText.free();
	FreeFonts();
}

// Initialize Spawner
void Spawner::init(Spawner spawner[]) {
	count = 0;
	for (int i = 0; i < 200; i++) {
		spawner[i].x 			= 0;
		spawner[i].y 			= 0;
		spawner[i].w 			= 0;
		spawner[i].h 			= 0;
		spawner[i].frame 		= 0;
		spawner[i].tick 		= 0;
		spawner[i].spawnTimer 	= 0;
		spawner[i].spawned 		= 0;
		spawner[i].maxSpawn 	= 10;
		spawner[i].type 		= -1;
		spawner[i].alive 		= false;
		spawner[i].moused 		= false;
	}
}

// Spawn a spawner
void Spawner::spawn(Spawner spawner[], float x, float y, float w, float h) {
	for (int i = 0; i < 200; i++){
		if (!spawner[i].alive){
			spawner[i].type 		= 0;
			spawner[i].x 			= x;
			spawner[i].y 			= y;
			spawner[i].w 			= w;
			spawner[i].h 			= h;
			spawner[i].spawnedW 	= w;
			spawner[i].spawnedH 	= h;
			spawner[i].reduction 	= w/60;
			spawner[i].frame 		= 0;
			spawner[i].tick 		= 0;
			spawner[i].spawnTimer 	= 0;
			spawner[i].spawned 		= 0;
			spawner[i].maxSpawn 	= 1;
			spawner[i].moused 		= false;
			spawner[i].alive 		= true;
			count++;
			break;
		}
	}
}

// Remove a spawner
void Spawner::remove(Spawner spawner[]) {
	for (int i = 0; i < 200; i++) {
		if (spawner[i].alive) {
			if (spawner[i].moused) {
				spawner[i].alive = false;
				count--;
			}
		}
	}
}

// Update Spawner
void Spawner::update(Spawner spawner[], int targetX, int targetY, int mx, int my, int camx, int camy){
	for (int i = 0; i < 200; i++){
		if (spawner[i].alive)
		{
			// Mouse on Spawner
			if (mx > spawner[i].x && mx < spawner[i].x + spawner[i].w &&
				my > spawner[i].y && my < spawner[i].y + spawner[i].h) {
				spawner[i].moused = true;
			} else {
				spawner[i].moused = false;
			}

			// Spawn asteroids
			if (spawner[i].type == 0)
			{

				spawner[i].x+=spawner[i].reduction/2;
				spawner[i].y+=spawner[i].reduction/2;
				spawner[i].w -= spawner[i].reduction;
				spawner[i].h -= spawner[i].reduction;
				if (spawner[i].w <= 0){
					spawner[i].spawnTimer 	= 0;
					spawner[i].spawned 	   += 1;
		            int randAngle 			= randDouble(0.0, 360.0);
		            //a_dummy.spawnAsteroidAngle(asteroid, spawner[i].x-spawner[i].spawnedW/2,
		            //									 spawner[i].y-spawner[i].spawnedH/2,
		            //									 spawner[i].spawnedW, spawner[i].spawnedH,
		    		//							 	 	 	 randAngle, 0.5);
					spawner[i].alive = false;
					count--;
				}


				// Spawner animation
				/*spawner[i].tick+= 1;
				if (spawner[i].tick>12){
					spawner[i].tick = 0;
					spawner[i].frame += 1;
				}

				// Animation ended, do something
				if (spawner[i].frame>4){
					spawner[i].frame = 0;
					spawner[i].tick = 0;

					// Spawn asteroid
					spawner[i].spawnTimer 	= 0;
					spawner[i].spawned 	   += 1;
		            int randAngle 			= fRandS(0.0, 360.0);
		            a_dummy.spawnAsteroidAngle(asteroid, spawner[i].x, spawner[i].y, spawner[i].w, spawner[i].h,
		    									 randAngle, 0.5);
				}

				// Destroy Spawner after max spawns has been reached
				if (spawner[i].spawned >= spawner[i].maxSpawn){
					spawner[i].alive = false;
					count--;
				}*/

				// Spawn Asteroid after x milliseconds
				/*spawner[i].spawnTimer += 1;
				if (spawner[i].spawnTimer > 60)
				{
					spawner[i].spawnTimer 	= 0;
					spawner[i].spawned 	   += 1;
		            int randAngle 			= fRandS(0.0, 360.0);
		            a_dummy.spawnAsteroidAngle(asteroid, spawner[i].x, spawner[i].y, spawner[i].w, spawner[i].h,
		    									 randAngle, 0.5);
				}

				// Destroy Spawner after max spawns has been reached
				if (spawner[i].spawned >= spawner[i].maxSpawn){
					spawner[i].alive = false;
					count--;
				}*/
			}
		}
	}
}

// Render Spawner
void Spawner::render(Spawner spawner[], int camx, int camy, SDL_Renderer* gRenderer) {
	for (int i = 0; i < 200; i++) {
		if (spawner[i].alive) {

			gSpawner.render(gRenderer, spawner[i].x-camx, spawner[i].y-camy,
					spawner[i].w, spawner[i].h, &rSpawner[0]);


			/*std::stringstream tempsi;
			tempsi.str( std::string() );
			tempsi << spawner[i].reduction;
			gText.loadFromRenderedText(tempsi.str().c_str(), {244, 255, 255}, gFont, gRenderer);
			gText.render(gRenderer, spawner[i].x-camx, spawner[i].y-camy, gText.getWidth(), gText.getHeight());*/

			/*SDL_Rect spawnerRect = { spawner[i].x-camx, spawner[i].y-camy, spawner[i].w, spawner[i].h};
			SDL_SetRenderDrawColor(gRenderer, 244, 144, 20, 255);
			SDL_RenderDrawRect(gRenderer, &spawnerRect);*/
		}
	}
}
