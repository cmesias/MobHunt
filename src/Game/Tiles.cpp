/*
 * Tile.cpp
 *
 *  Created on: Jan 16, 2017
 *      Author: Carl
 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mouse.h>

#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>

#include "Tiles.h"

void Tile::load(SDL_Renderer *gRenderer, LTexture *gJar) {
	gTiles.loadFromFile(gRenderer, "resource/gfx/tileset.png");

	this->gJar = gJar;

	LoadFonts();
}

void Tile::free() {
	gTiles.free();
	//gTileBreak.free();
	FreeFonts();
}


void Tile::initTile(Tile tile[]) {
	layer 		= 0;
	id 			= 0;
	tileCount 	= 0;
	multiW 		= 1;
	multiH 		= 1;
	hideOtherLayers 	= false;
	for (int i = 0; i < max; i++) {
		tile[i].w = tilew;
		tile[i].h = tileh;
		tile[i].id = -1;
		tile[i].alpha = 255;
		tile[i].layer = -1;
		tile[i].animTimer = 0;
		tile[i].hoverAmount = 9;
		tile[i].hoverDir = 1;
		tile[i].mouse = false;
		tile[i].mouseBox = false;
		tile[i].screen = false;
		tile[i].player = false;
		tile[i].side = "right";
		tile[i].collisionTile = false;
		tile[i].PlayerBehindTile = false;
		tile[i].promptSelf = false;
		tile[i].alive = false;

		// Jars
		{
			tile[i].startJarBreaking = false;
			tile[i].jarIndex = 0;
			tile[i].animTimer = 0;
		}

		// Jars & Barrels
		{
			tile[i].destructible = false;
		}

	}
}

void Tile::placeTile(Tile tile[], float x, float y,
					int id, int layer,
					bool collisionTile,
					SDL_Rect clip) {
	for (int i = 0; i < max; i++) {
		if (!tile[i].alive){
			tile[i].x 		= x;
			tile[i].y 		= y;
			tile[i].w = tilew;
			tile[i].h = tileh;
			tile[i].id 		= id;
			tile[i].alpha 	= 255;
			tile[i].clip 	= clip;
			tile[i].layer 	= layer;
			tile[i].collisionTile 	= collisionTile;
			tile[i].PlayerBehindTile = false;
			tile[i].animTimer = 0;
			tile[i].mouse 	= false;
			tile[i].mouseBox = false;
			tile[i].screen 	= false;
			tile[i].player = false;
			tile[i].side = "right";
			tile[i].alive 	= true;

			// Set default parameters
			setStatsBasedOnType(tile, i);

			tileCount++;
			break;
		}
	}
}
void Tile::copyTile(Tile tile[]) {
	for (int i = 0; i < max; i++) {
		if (tile[i].alive){
			if (tile[i].mouse){
				if (tile[i].layer == layer) {
					id = tile[i].id;
					collisionTile = tile[i].collisionTile;
				}
			}
		}
	}
}
void Tile::changeTileCollision(Tile tile[]) {
	for (int i = 0; i < max; i++) {
		if (tile[i].alive){
			if (tile[i].mouse){
				if (tile[i].layer == layer) {
					tile[i].collisionTile = (!tile[i].collisionTile);
				}
			}
		}
	}
}
void Tile::changeTileLayer(Tile tile[], int value) {
	for (int i = 0; i < max; i++) {
		if (tile[i].alive){
			if (tile[i].mouse){
				if (tile[i].layer == layer) {
					tile[i].layer += value;
				}
			}
		}
	}
}
void Tile::removeTile(Tile tile[], int click) {
	for (int i = 0; i < max; i++) {
		if (tile[i].alive){
			// Left click
			if (click == 0) {
				if (tile[i].mouseBox){
					if (tile[i].layer == layer) {
						tile[i].alive = false;
						tileCount--;
					}
				}
			// Right click
			}else{
				if (tile[i].mouse){
					if (tile[i].layer == layer) {
						tile[i].alive = false;
						tileCount--;
					}
				}
			}
		}
	}
}

void Tile::RemoveAll(Tile tile[]) {
	for (int i = 0; i < max; i++) {
		if (tile[i].alive){
			tile[i].alive = false;
		}
	}
	tileCount = 0;
}

/* Place Tile */
void Tile::spawnTile(Tile tile[], int newMx, int newMy, int camx, int camy, SDL_Rect rTiles[]) {
	removeTile(tile, 0);
	for (int j = 0; j < multiW; j++) {
		for (int h = 0; h < multiH; h++) {
			int x = int(newMx + j *(rTiles[id].w*2)+camx);
			int y = int(newMy + h *(rTiles[id].h*2)+camy);
			placeTile(tile, x, y, id, layer, collisionTile, rTiles[id]);
		}
	}
}

bool Tile::updateTile(Tile tile[], LWindow &gWindow,
					  float targetX, float targetY, float targetW, float targetH,
					  int newMx, int newMy,
					  int mex, int mey, int camx, int camy,
					  SDL_Rect rTiles[]) {

	bool didWeRemoveAJarTile = false;

	//std::cout << "newMx: " << newMx << std::endl;
	//std::cout << "newMy: " << newMy << std::endl;
	int tileW = (getPixelW(rTiles)*2)*multiW;
	int tileH = (getPixelH(rTiles)*2)*multiH;
	for (int i = 0; i < max; i++)
	{
		if (tile[i].alive)
		{
			// Tile destruction if its a destructible
			// destruction code is inside PlayGame.cpp

			// If Tile is not a Floor Tile
			if (tile[i].layer == 1 || tile[i].layer == 2) {
				// Check if player is in front of Tile or not
				if (targetY+targetH < tile[i].y+tile[i].h){
					tile[i].PlayerBehindTile = true;
				} else {
					tile[i].PlayerBehindTile = false;
				}
			}

			//If the tile is in the screen
			if (tile[i].x + tile[i].w > camx-64 && tile[i].x < camx + screenWidth+64
			 && tile[i].y + tile[i].h > camy-64 && tile[i].y < camy + screenHeight+64) {
				tile[i].screen = true;
			} else {
				tile[i].screen = false;
			}

			//If the mouse+size is on the tile
			if (newMx+tileW-4 > tile[i].x && newMx+2 < tile[i].x + tile[i].w &&
					newMy+tileH-4 > tile[i].y && newMy+2 < tile[i].y + tile[i].h) {
				tile[i].mouseBox = true;
			} else {
				tile[i].mouseBox = false;
			}

			//If the mouse is on the tile
			if (mex > tile[i].x && mex < tile[i].x + tile[i].w &&
				mey > tile[i].y && mey < tile[i].y + tile[i].h) {
				tile[i].mouse = true;
			} else {
				tile[i].mouse = false;
			}

			// Reset layer if goes higher
			if (tile[i].layer > 6) {
				tile[i].layer = 0;
			}

			// Animations

			// Jar breaking
			if (tile[i].id == 1) {
				if (tile[i].startJarBreaking) {

					tile[i].animTimer += tile[i].animTimerSpe;

					if (tile[i].animTimer > 60) {
						tile[i].animTimer = 0;

						// Next jar breaking fram
						tile[i].jarIndex++;

						// Last frame of jar being destroyed
						if (tile[i].jarIndex > 5) {

							didWeRemoveAJarTile = true;

							// Remove tile
							tile[i].startJarBreaking = false;
							tile[i].alive = false;
							this->tileCount--;
						}
					}
				}
			}

			// If on first light Tile frame
			/*if (tile[i].id >= 328 && tile[i].id < 335) {
				// Start animation timer
				tile[i].animTimer += this->animTimerSpe;

				// If animation timer goes over 60 frames
				if (tile[i].animTimer > 60) {

					// Reset animation timer again
					tile[i].animTimer = 0;

					// Change tile into next light Tile id/frame
					tile[i].id++;
				}
			}

			// If on last light Tile frame, loop around
			else if (tile[i].id == 335) {
				// Start animation timer
				tile[i].animTimer += this->animTimerSpe;

				// If animation timer goes over 60 frames
				if (tile[i].animTimer > 60) {

					// Reset animation timer again
					tile[i].animTimer = 0;

					// Change tile into next light Tile id/frame
					tile[i].id = 328;
				}
			}*/
		}
	}

	return didWeRemoveAJarTile;
}


bool Tile::checkCollisionRect( SDL_Rect a, SDL_Rect b )
{
    //The sides of the rectangles
    int leftA,   leftB;
    int rightA,  rightB;
    int topA, 	 topB;
    int bottomA, bottomB;

    //Calculate the sides of rect A
    leftA 	= a.x;
    rightA 	= a.x + a.w;
    topA 	= a.y;
    bottomA = a.y + a.h;

    //Calculate the sides of rect B
    leftB 	= b.x;
    rightB 	= b.x + b.w;
    topB 	= b.y;
    bottomB = b.y + b.h;

    //If any of the sides from A are outside of B
    if( bottomA < topB )
    {
        return false;
    }

    if( topA > bottomB )
    {
        return false;
    }

    if( rightA < leftB )
    {
        return false;
    }

    if( leftA > rightB )
    {
        return false;
    }

    //If none of the sides from A are outside B
    return true;
}

// Tile collision check, player x position
void Tile::checkCollisionXY(Tile tile[],
		float &x, float &y,
		int w, int h,
		float &vX, float &vY) {

	{
		/*
		 *
		 *
		 *
		 * TODO [ ] - do ccheck for collision if player presses "E" key on chest, open chest and throw out sword
		 *
		 *
		 *
		 *
		 *
		 *
		 */

		// Update collision with Tiles
		//tl.checkCollision(tile, x, y, w, h, y, vY);

		// Update collision with Tiles
		// Player Velocity X Axis
		x += vX;

		// Get rect of Player
		SDL_Rect rectA;
		rectA.x = x;
		rectA.y = y;
		rectA.w = w;
		rectA.h = h;

		// Set moveback to false
		bool moveBack;
		moveBack = false;

		// For loop Tiles
		for (int i = 0; i < max; i++) {
			if (tile[i].alive){

				// Check only collision-enabled Tiles
				if (tile[i].collisionTile) {
					if (!tile[i].startJarBreaking) {

						// Only do collision check to nearby tiles
						if (getDistance(rectA.x+rectA.w/2,
										rectA.y+rectA.h/2,
										tile[i].rectB.x+tile[i].rectB.w/2,
										tile[i].rectB.y+tile[i].rectB.h/2) < 75) {

							// If Player colliding with Tile
							if  ( checkCollisionRect( rectA, tile[i].rectB )) {
								/*// If Player has more than 0 keys, then unlock door, otherwise continue collision check
								if (keys > 0 && useKey) {
									useKey = false;
									if (tile[i].id == 65 || tile[i].id == 69 || tile[i].id == 73 || tile[i].id == 77 ||
										tile[i].id == 67 || tile[i].id == 71 || tile[i].id == 75 || tile[i].id == 79) {
										keys--;
										Mix_PlayChannel(-1, sKeyPickup, 0);
										tile[i].alive = false;
										tl.tileCount--;
									}
								}*/

								// Continue handling collision
								moveBack = true;
							}
						}
					}
				}
			}
		}
		if (moveBack){
			x -= vX;
		}

		// Player Velocity Y Axis
		y += vY;
		rectA.x = x;
		rectA.y = y;
		rectA.w = w;
		rectA.h = h;

		// Set moveBack always to false
		moveBack = false;

		// For loop Tiles
		for (int i = 0; i < max; i++) {
			if (tile[i].alive){
				if (tile[i].collisionTile) {
					if (!tile[i].startJarBreaking) {

						// Only do collision check to nearby tiles
						if (getDistance(rectA.x+rectA.w/2,
										rectA.y+rectA.h/2,
										tile[i].rectB.x+tile[i].rectB.w/2,
										tile[i].rectB.y+tile[i].rectB.h/2) < 75) {

							if  ( checkCollisionRect( rectA, tile[i].rectB )) {
								// If Player has more than 0 keys, then unlock door, otherwise continue collision check
								/*if (keys > 0 && useKey) {
									useKey = false;
									if (tile[i].id == 65 || tile[i].id == 69 || tile[i].id == 73 || tile[i].id == 77 ||
										tile[i].id == 67 || tile[i].id == 71 || tile[i].id == 75 || tile[i].id == 79) {
										keys--;
										Mix_PlayChannel(-1, sKeyPickup, 0);
										tile[i].alive = false;
										tl.tileCount--;
									}
								}*/
								// Continue handling collision
								moveBack = true;
							}
						}
					}
				}
			}
		}
		if (moveBack){
			y -= vY;
		}
	}
}

void Tile::renderTile(SDL_Renderer *gRenderer, Tile tile[], int layer_dummy, int camx, int camy, SDL_Rect rJar[]) {
	for (int i = 0; i < max; i++)
	{
		if (tile[i].alive)
		{
			// If the tile is on the screen
			if (tile[i].screen)
			{
				// Tile trasparency on Player collision
				if (tile[i].layer != 0 && tile[i].player)
				{
					//tile[i].alpha = 90;
				}else{
					//tile[i].alpha = 255;
				}

				// Render layer in hand
				if (hideOtherLayers)
				{
					if (layer == tile[i].layer)
					{
						gTiles.setAlpha(tile[i].alpha);
						gTiles.render(gRenderer, tile[i].x - camx, tile[i].y - camy, tile[i].w, tile[i].h, &tile[i].clip);
					}

				// Render all tiles
				}else{
					if (layer_dummy == tile[i].layer) {


						// Handle Jar tile breaking
						if (tile[i].id == 1 && tile[i].startJarBreaking) {
							// Render tile
							gJar->setAlpha(tile[i].alpha);
							gJar->render(gRenderer, tile[i].x - camx, tile[i].y - camy, tile[i].w, tile[i].h, &rJar[tile[i].jarIndex]);
						} else {
							gTiles.setAlpha(tile[i].alpha);
							gTiles.render(gRenderer, tile[i].x - camx, tile[i].y - camy, tile[i].w, tile[i].h, &tile[i].clip);
						}
					}

					// If destructible, render destructible Texture on Tiles
					{
						/*if (tile[i].destructible) {
							int sprite_index = 9;

							if (tile[i].health >= 45 && tile[i].health <= 50) {
								sprite_index = 9;
							} else if (tile[i].health >= 40 && tile[i].health <= 44) {
								sprite_index = 8;
							} else if (tile[i].health >= 35 && tile[i].health <= 39) {
								sprite_index = 7;
							} else if (tile[i].health >= 30 && tile[i].health <= 34) {
								sprite_index = 6;
							} else if (tile[i].health >= 25 && tile[i].health <= 29) {
								sprite_index = 5;
							} else if (tile[i].health >= 20 && tile[i].health <= 24) {
								sprite_index = 4;
							} else if (tile[i].health >= 15 && tile[i].health <= 19) {
								sprite_index = 3;
							} else if (tile[i].health >= 10 && tile[i].health <= 14) {
								sprite_index = 2;
							} else if (tile[i].health >= 5 && tile[i].health <= 9) {
								sprite_index = 1;
							} else if (tile[i].health >= 0 && tile[i].health <= 4) {
								sprite_index = 0;
							}

							gTileBreak.render(gRenderer, tile[i].x - camx, tile[i].y - camy, tile[i].w, tile[i].h, &rTileBreak[sprite_index]);
						}*/
					}
				}
			}
		}
	}
}


void Tile::RenderBehindPlayer(SDL_Renderer *gRenderer, Tile tile[], int layerToRender, int camx, int camy, SDL_Rect rTiles[], SDL_Rect rJar[]) {
	for (int i = 0; i < max; i++) {
		if (tile[i].alive) {


			// Render layer in hand
			if (hideOtherLayers)
			{
				if (layer == tile[i].layer)
				{
					gTiles.setAlpha(tile[i].alpha);
					gTiles.render(gRenderer, tile[i].x - camx, tile[i].y - camy, tile[i].w, tile[i].h, &tile[i].clip);
				}
			} else {

				// If player is below Tile
				if (tile[i].PlayerBehindTile)
				{
					// Do nothing
				}
				// If player is above Tile
				if (!tile[i].PlayerBehindTile)
				{
					// Render all Tiles that are layer 1 and above
					if (tile[i].layer == layerToRender) {

						// Handle Jar tile breaking
						if (tile[i].id == 1 && tile[i].startJarBreaking) {
							// Render tile
							gJar->setAlpha(tile[i].alpha);
							gJar->render(gRenderer, tile[i].x - camx, tile[i].y - camy, tile[i].w, tile[i].h, &rJar[tile[i].jarIndex]);
						}

						// Render every other Tile normally
						else {
							// Render tile
							gTiles.setAlpha(tile[i].alpha);
							gTiles.render(gRenderer, tile[i].x - camx, tile[i].y - camy, tile[i].w, tile[i].h, &tile[i].clip);
						}

						/*
						// For loop to loop through all 8 light Tiles
						for (int j=0; j<8; j++) {

							////////////////////////////////////////////////////////////////////////////
							// -----------------------------------------------------------------------//
							// ---------------- This will render ONLY light Tiles: -------------------//
							// ----------------------- id's 296 through 303 --------------------------//
							// ----------------------- id's 328 through 335 --------------------------//
							if (tile[i].id == 328 + j )
							{
								// Render top half of light tile
								gTiles.setAlpha(tile[i].alpha);
								int topLightTileId = tile[i].id-32;
								gTiles.render(gRenderer, tile[i].x - camx, tile[i].y-tile[i].h - camy, tile[i].w, tile[i].h, &rTiles[topLightTileId]);

								// Render bottom half of light tile
								gTiles.setAlpha(tile[i].alpha);
								gTiles.render(gRenderer, tile[i].x - camx, tile[i].y - camy, tile[i].w, tile[i].h, &tile[i].clip);
							}
							// ----------------------- id's 328 through 335 --------------------------//
							// ----------------------- id's 296 through 303 --------------------------//
							// ---------------- This will render ONLY light Tiles: -------------------//
							// -----------------------------------------------------------------------//
							////////////////////////////////////////////////////////////////////////////

							////////////////////////////////////////////////////////////////////////////
							// -----------------------------------------------------------------------//
							// ---------------- This will render all other Tiles ---------------------//
							else {
							}
							// -----------------------------------------------------------------------//
							// ---------------- This will render all other Tiles ---------------------//
							////////////////////////////////////////////////////////////////////////////

						}*/
					}
				}
			}
		}
	}
}

void Tile::RenderOnTopOfPlayer(SDL_Renderer *gRenderer, Tile tile[], int layerToRender, int camx, int camy, SDL_Rect rTiles[], SDL_Rect rJar[]) {
	for (int i = 0; i < max; i++) {
		if (tile[i].alive) {

			// Render layer in hand
			if (hideOtherLayers)
			{
				if (layer == tile[i].layer)
				{
					gTiles.setAlpha(tile[i].alpha);
					gTiles.render(gRenderer, tile[i].x - camx, tile[i].y - camy, tile[i].w, tile[i].h, &tile[i].clip);
				}
			} else {

				// If player is front of Tile
				if (tile[i].PlayerBehindTile)
				{
					// Render all Tiles that are layer 1 and above
					if (layerToRender == tile[i].layer) {

						// Handle Jar tile breaking
						if (tile[i].id == 1 && tile[i].startJarBreaking) {
							// Render tile
							gJar->setAlpha(tile[i].alpha);
							gJar->render(gRenderer, tile[i].x - camx, tile[i].y - camy, tile[i].w, tile[i].h, &rJar[tile[i].jarIndex]);
						}

						// Render every other Tile normally
						else {
							// Render tile
							gTiles.setAlpha(tile[i].alpha);
							gTiles.render(gRenderer, tile[i].x - camx, tile[i].y - camy, tile[i].w, tile[i].h, &tile[i].clip);
						}

						// For loop to loop through all 8 light Tiles
						/*for (int j=0; j<8; j++) {

							////////////////////////////////////////////////////////////////////////////
							// -----------------------------------------------------------------------//
							// ---------------- This will render ONLY light Tiles: -------------------//
							// ----------------------- id's 296 through 303 --------------------------//
							// ----------------------- id's 328 through 335 --------------------------//
							if (tile[i].id == 328 + j )
							{
								// Render top half of light tile
								gTiles.setAlpha(tile[i].alpha);
								int topLightTileId = tile[i].id-32;
								gTiles.render(gRenderer, tile[i].x - camx, tile[i].y-tile[i].h - camy, tile[i].w, tile[i].h, &rTiles[topLightTileId]);

								// Render bottom half of light tile
								gTiles.setAlpha(tile[i].alpha);
								gTiles.render(gRenderer, tile[i].x - camx, tile[i].y - camy, tile[i].w, tile[i].h, &tile[i].clip);
							}
							// ----------------------- id's 328 through 335 --------------------------//
							// ----------------------- id's 296 through 303 --------------------------//
							// ---------------- This will render ONLY light Tiles: -------------------//
							// -----------------------------------------------------------------------//
							////////////////////////////////////////////////////////////////////////////

							////////////////////////////////////////////////////////////////////////////
							// -----------------------------------------------------------------------//
							// ---------------- This will render all other Tiles ---------------------//
							else {

							}
							// -----------------------------------------------------------------------//
							// ---------------- This will render all other Tiles ---------------------//
							////////////////////////////////////////////////////////////////////////////
						}*/
					}
				}


				// If player is above Tile
				if (!tile[i].PlayerBehindTile)
				{
					// Do nothing
				}
			}
		}
	}
}

void Tile::RenderUI(SDL_Renderer *gRenderer, Tile tile[], int camx, int camy) {
	for (int i = 0; i < max; i++) {
		if (tile[i].alive) {
			if (tile[i].promptSelf) {
				// Hover effect prompt for Tiles with prompts
				tile[i].hoverAmount += 0.1 * tile[i].hoverDir;
				if (tile[i].hoverAmount > 4) {
					tile[i].hoverDir = -1;
				}
				else if (tile[i].hoverAmount < 1) {
					tile[i].hoverDir = 1;
				}

				std::stringstream tempsi;
				tempsi << "E";
				gText.loadFromRenderedText(gRenderer, tempsi.str().c_str(), {255, 255, 255}, gFont20);
				gText.render(gRenderer, tile[i].x+tile[i].w/2-gText.getWidth()/2-camx,
						tile[i].y+tile[i].hoverAmount-gText.getWidth()-camy,
										gText.getWidth(), gText.getHeight());
			}
		}
	}
}

void Tile::renderTileDebug(SDL_Renderer *gRenderer, Tile tile[], int newMx, int newMy, int mex, int mey, int camx, int camy, SDL_Rect rTiles[]){
	for (int i = 0; i < max; i++) {
		if (tile[i].alive){
			if (tile[i].layer == layer) {

				// If its a collision tile, render filled blue square on top left inside the Tile
				if (tile[i].collisionTile) {
					SDL_Rect tempr = {tile[i].x - camx, tile[i].y - camy, 4, 8};
					SDL_SetRenderDrawColor(gRenderer, 0, 0, 255, 255);
					SDL_RenderFillRect(gRenderer, &tempr);
				} else {
					SDL_Rect tempr = {tile[i].x - camx, tile[i].y - camy, 4, 8};
					SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
					SDL_RenderFillRect(gRenderer, &tempr);
				}
			}
			if (tile[i].id == 1 || tile[i].id == 4) {
				std::stringstream tempsi;
				tempsi << tile[i].jarIndex;
				gText.loadFromRenderedText(gRenderer, tempsi.str().c_str(), {255, 255, 255}, gFont12);
				gText.render(gRenderer, tile[i].x-camx, tile[i].y-gText.getHeight()-camy,
										gText.getWidth(), gText.getHeight());
			}
		}
	}
}

void Tile::RenderHand(SDL_Renderer *gRenderer, Tile tile[], int newMx, int newMy, int mex, int mey, int camx, int camy, SDL_Rect rTiles[]){
	// Render Tile info
	for (int i = 0; i < max; i++) {
		if (tile[i].alive){

			// If mouse is hovering on Tile and we have the same layer in Hand
			if (tile[i].layer == layer) {
				if (tile[i].mouse) {
					std::stringstream tempss;
					tempss << tile[i].layer;
					gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {255,255,255}, gFont12);
					gText.setAlpha(255);
					gText.render(gRenderer, tile[i].x+tile[i].w-gText.getWidth() - camx, tile[i].y - camy, gText.getWidth(), gText.getHeight());

					// If its a collision tile, render filled blue square on top left inside the Tile
					/*if (tile[i].collisionTile) {
						SDL_Rect tempr = {tile[i].x - camx, tile[i].y - camy, 4, 8};
						SDL_SetRenderDrawColor(gRenderer, 0, 0, 255, 255);
						SDL_RenderFillRect(gRenderer, &tempr);
					} else {
						SDL_Rect tempr = {tile[i].x - camx, tile[i].y - camy, 4, 8};
						SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
						SDL_RenderFillRect(gRenderer, &tempr);
					}*/

					// black
					SDL_Rect tempr = {tile[i].x - camx, tile[i].y - camy, 32, 32};
					SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
					SDL_RenderDrawRect(gRenderer, &tempr);
				}
			}
		}
	}
	// Render tile in hand
	for (int j = 0; j < multiW; j++) {
		for (int h = 0; h < multiH; h++) {
			gTiles.setAlpha(110);
			//gTiles.render(gRenderer, newMx+j*tilew, newMy+h*tileh, tilew, tileh, &rTiles[id]);
			gTiles.render(gRenderer, newMx+j*(rTiles[id].w*2), newMy+h*(rTiles[id].h*2), rTiles[id].w*2, rTiles[id].h*2, &rTiles[id]);

			// Render mouse coordinates snapped to grid
			SDL_Rect tempr = {newMx, newMy, (rTiles[id].w*2)*multiW, (rTiles[id].h*2)*multiH};
			SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255);
			SDL_RenderDrawRect(gRenderer, &tempr);
		}
	}
}

int Tile::getPixelW(SDL_Rect rTiles[]) {
	return rTiles[id].w;
}

int Tile::getPixelH(SDL_Rect rTiles[]) {
	return rTiles[id].h;
}


void Tile::setStatsBasedOnType(Tile tile[], int i)
{
	// Default parameters
	{
		tile[i].promptSelf = false;

		// Jar variables
		{
			tile[i].destructible = false;
			tile[i].startJarBreaking = false;
			tile[i].jarIndex = 0;
			tile[i].animTimer = 0;
		}
	}

	// Every other Tile
	{
		tile[i].rectB.x = tile[i].x;
		tile[i].rectB.y = tile[i].y;
		tile[i].rectB.w = tile[i].w;
		tile[i].rectB.h = tile[i].h;
	}

	// Specific Tiles
	{
		// Pot
		if (tile[i].id == 1) {
			tile[i].health = 1;
			tile[i].destructible = true;
			tile[i].collisionTile = true;
		}

		// Barrel
		{
			if (tile[i].id == 4) {
				// Set health and destructible
				tile[i].health = 1;
				tile[i].destructible = true;
				tile[i].collisionTile = true;
			}
		}

		// Locked door
		{
			// Top door
			if (tile[i].id == 2 || tile[i].id == 3 || tile[i].id == 12 || tile[i].id == 13)
			{
				// Set size
				tile[i].w = 64;
				tile[i].h = 64;

				// Set collision box around tile
				tile[i].rectB.x = tile[i].x;
				tile[i].rectB.y = tile[i].y+tile[i].h/2;
				tile[i].rectB.w = tile[i].w;
				tile[i].rectB.h = tile[i].h/2;

				// Set clips for texture
				tile[i].clip = {64, 0, 64, 64};
			}

			// Bottom door
			if (tile[i].id == 22 || tile[i].id == 23 || tile[i].id == 32 || tile[i].id == 33)
			{
				// Set size
				tile[i].w = 64;
				tile[i].h = 64;

				// Set collision box around tile
				tile[i].rectB.x = tile[i].x;
				tile[i].rectB.y = tile[i].y+tile[i].h/2;
				tile[i].rectB.w = tile[i].w;
				tile[i].rectB.h = tile[i].h/2;

				// Set clips for texture
				tile[i].clip = {64, 64, 64, 64};
			}

			// Left door
			if (tile[i].id == 40 || tile[i].id == 41 || tile[i].id == 50 || tile[i].id == 51)
			{
				// Set size
				tile[i].w = 64;
				tile[i].h = 64;

				// Set collision box around tile
				tile[i].rectB.x = tile[i].x;
				tile[i].rectB.y = tile[i].y+tile[i].h/2;
				tile[i].rectB.w = tile[i].w;
				tile[i].rectB.h = tile[i].h/2;

				// Set clips for texture
				tile[i].clip = {0, 128, 64, 64};
			}

			// Right door
			if (tile[i].id == 42 || tile[i].id == 43 || tile[i].id == 52 || tile[i].id == 53)
			{
				// Set size
				tile[i].w = 64;
				tile[i].h = 64;

				// Set collision box around tile
				tile[i].rectB.x = tile[i].x;
				tile[i].rectB.y = tile[i].y+tile[i].h/2;
				tile[i].rectB.w = tile[i].w;
				tile[i].rectB.h = tile[i].h/2;

				// Set clips for texture
				tile[i].clip = {64, 128, 64, 64};
			}
		}
	}
}

void Tile::MoveTiles(Tile tile[], std::string direction){
	for (int i = 0; i < max; i++) {
		if (tile[i].alive){
			if (direction == "left") {
				tile[i].x -= 64;
			}
			else if (direction == "right") {
				tile[i].x += 64;
			}
			else if (direction == "up") {
				tile[i].y -= 64;
			}
			else if (direction == "down") {
				tile[i].y += 64;
			}

		}
	}
}

void Tile::LoadData(Tile tile[], std::fstream &fileToReturn)
{
	//std::cout << "Attempting to load... Tile" << std::endl;
	// Load Tile count data
	fileToReturn >> this->tileCount;
	//std::cout << this->tileCount << std::endl;

	// Load rest of Tile data
	for (int i = 0; i < this->tileCount; i++) {
		if (!tile[i].alive) {
			fileToReturn >>  tile[i].x 				>>
							  tile[i].y 			>>
							  tile[i].w 			>>
							  tile[i].h 			>>
							  tile[i].id 			>>
							  tile[i].collisionTile >>
							  tile[i].layer 		>>
							  tile[i].alpha 		>>
							  tile[i].clip.x 		>>
							  tile[i].clip.y 		>>
							  tile[i].clip.w 		>>
							  tile[i].clip.h 		>>
							  tile[i].mouse 		>>
							  tile[i].screen 		>>
							  tile[i].alive;
		}

		// Set collision rects for specific Tiles
		for (int i = 0; i < this->tileCount; i++) {
			if (tile[i].alive) {

				// Set default parameters
				setStatsBasedOnType(tile, i);
			}
		}

		//break;
	}
}

// Save Tile data
std::string Tile::SaveData(Tile tile[])
{
	/////////////////////////////////////////////////////////////////////////////////
	//-----------------------------------------------------------------------------//
	//----------------------------- Save Everything in One File -------------------//

	// Save all tile data in a string first
	std::stringstream tempsstb;
	for (int i = 0; i < max; i++) {
		if (tile[i].alive) {
			tempsstb << tile[i].x 				<< " "
					 << tile[i].y 				<< " "
					 << tile[i].w  				<< " "
					 << tile[i].h  				<< " "
					 << tile[i].id  			<< " "
					 << tile[i].collisionTile 	<< " "
					 << tile[i].layer 			<< " "
					 << tile[i].alpha  			<< " "
					 << tile[i].clip.x 			<< " "
					 << tile[i].clip.y 			<< " "
					 << tile[i].clip.w 			<< " "
					 << tile[i].clip.h 			<< " "
					 << tile[i].mouse 			<< " "
					 << tile[i].screen 			<< " "
			   	   	 << tile[i].alive 			<< "\n";
		}
	}
	std::string temps;
	temps = tempsstb.str().c_str();

	// Commented out because this takes out the extra line thats empty at the end
	//temps = temps.substr(0, temps.size()-1);

	// Save number of tiles in World
	std::stringstream aVeryLongString;
	aVeryLongString << tileCount << "\n";

	// Save the data of all Tiles
	aVeryLongString << temps.c_str();
	//----------------------------- Save Everything in One File -------------------//
	//-----------------------------------------------------------------------------//
	/////////////////////////////////////////////////////////////////////////////////

	// Return save data for all Tiles
	return aVeryLongString.str().c_str();
}


