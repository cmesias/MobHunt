/*
 * Tilec.cpp
 *
 *  Created on: Jul 21, 2017
 *      Author: Carl
 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mouse.h>

#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>

#include "Tilec.h"

///////////////////////////////////////////////////////////////////////////
//------------------------------ Collision ------------------------------//
bool Tilec::checkCollisionRect( SDL_Rect a, SDL_Rect b )
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
void Tilec::checkCollisionX(Tilec tilec[], float &x, float y, int w, int h, std::string &jumpstate, float &vX, int &jumps) {
	SDL_Rect rectA;
	rectA.x = x;
	rectA.y = y;
	rectA.w = w;
	rectA.h = h;

	bool moveBack;
	moveBack = false;

	for (int i = 0; i < max; i++) {
		if (tilec[i].alive){
			if (tilec[i].type == 0) {
				if (x + w <= tilec[i].x) {
					tilec[i].side = "left";
				}
				if (x >= tilec[i].x + tilec[i].w) {
					tilec[i].side = "right";
				}
				SDL_Rect rectB;
				rectB.x = tilec[i].x;
				rectB.y = tilec[i].y;
				rectB.w = tilec[i].w;
				rectB.h = tilec[i].h;
				if  ( checkCollisionRect( rectA, rectB )) {
					moveBack = true;
					/*if (tilec[i].side == "left") {
						x = tilec[i].x - w;
					}
					if (tilec[i].side == "right") {
						x = tilec[i].x + tilec[i].w;
					}*/
				}
			}
		}
	}
	if (moveBack){
		x -= vX;
	}

	/*for (int i = 0; i < max; i++) {
		if (tilec[i].alive) {
			if (tilec[i].id == 9) {
				if (tilec[i].collide) {
					if (y + h <= tilec[i].y) {
						if (x + w < tilec[i].x || x > tilec[i].x + tilec[i].w) {
							jumpstate = "falling";
						}
					}
				}

				//Collision Check - Spiked on top - touching body
				if (x + w > tilec[i].x && x < tilec[i].x + tilec[i].w &&
					y + h > tilec[i].y && y < tilec[i].y + tilec[i].h) {
					tilec[i].collide = true;
				} else {
					tilec[i].collide = false;
				}

				if (tilec[i].collide) {
					if (tilec[i].side == "left") {
						x = tilec[i].x - w;
						if (vX > 0) {
							vX = 0;
						}
					}
					if (tilec[i].side == "right") {
						x = tilec[i].x + tilec[i].w;
						if (vX < 0) {
							vX = 0;
						}
					}
					if (tilec[i].side == "top") {
						vY = 0.0;
						y = tilec[i].y - h;
				        jumpstate = "ground";
						jumps = 1;
					}
					if (tilec[i].side == "down") {
						y = tilec[i].y + tilec[i].h;
						vY = 1;
						jumpstate = "falling";
					}
				}
				if (x + w <= tilec[i].x) {
					tilec[i].side = "left";
				}
				if (y + h <= tilec[i].y) {
					tilec[i].side = "top";
				}
				if (x >= tilec[i].x + tilec[i].w) {
					tilec[i].side = "right";
				}
				if (y >= tilec[i].y + tilec[i].h) {
					tilec[i].side = "down";
				}
			} // end checking tile-9
		}
	}*/

}

void Tilec::checkCollisionY(Tilec tilec[], float x, float &y, int w, int h, std::string &jumpstate, float &vY, int &jumps, bool &jump) {

	// Temporary rectangle
	SDL_Rect rectA;
	rectA.x = x;
	rectA.y = y;
	rectA.w = w;
	rectA.h = h;

	// If we should move back
	bool moveBack;

	// Set move back default to false
	moveBack = false;

	// Which side the player is in relation to the tilec
	std::string side = "";

	// Loop all tilecs
	for (int i = 0; i < max; i++) {
		if (tilec[i].alive){
			if (tilec[i].type == 0) {

				// If player is on top
				if (y + h <= tilec[i].y) {
					tilec[i].side = "top";
				}

				// If player is below
				if (y >= tilec[i].y + tilec[i].h) {
					tilec[i].side = "down";
				}

				// Temporary rectangle
				SDL_Rect rectB;
				rectB.x = tilec[i].x;
				rectB.y = tilec[i].y;
				rectB.w = tilec[i].w;
				rectB.h = tilec[i].h;

				// If there is collision
				if  ( checkCollisionRect( rectA, rectB )) {

					// Set moveback to true
					moveBack = true;

					// If player is on top
					if (tilec[i].side == "top") {
						y = tilec[i].y - h - 1;
						jumps = 1;
				        jumpstate = "ground";
						side = "top";
					}

					// If player is below
					if (tilec[i].side == "down") {
						jump = false;
						side = "down";
					}
					// Move player back
					/*y = tilec[i].y - h - 1;
			        jumpstate = "ground";
					jumps = 1;
					if (tilec[i].side == "down") {
						jump = false;
					}*/

					/*double overLapX = x - tilec[i].x;
					double overLapY = y - tilec[i].y;
					std::cout << "overLapX: " << overLapX << std::endl;
					std::cout << "overLapY: " << overLapY << std::endl;*/
				}


			}
		}
	}
	if (moveBack){
		if (side == "top") {
			vY = 0;
		}
		if (side == "down") {
			vY = 2;
		}
	}
}

void Tilec::Init(Tilec tilec[]) {
	layer 			= 0;
	count = 0;
	multiW = 1;
	multiH = 1;
	tilew = 64;
	tileh = 64;
	id = 0;
	LevelToLoad = 0;
	for (int i = 0; i < max; i++) {
		tilec[i].x 				= 0;
		tilec[i].y 				= 0;
		tilec[i].w 				= 0;
		tilec[i].h 				= 0;
		tilec[i].mouse 			= false;
		tilec[i].mouseBox 		= false;
		tilec[i].promptSelf 	= false;
		tilec[i].collision 		= false;
		tilec[i].alive 			= false;
		tilec[i].getInput 		= false;
		tilec[i].type 			= 0;
		tilec[i].layer 			= 0;
		tilec[i].LevelToLoad 	= 0;
		tilec[i].side 			= "right";
	}
}

void Tilec::Load() {
	LoadFonts();
}

void Tilec::Free() {
	FreeFonts();
}

void Tilec::Update(Tilec tilec[], Map &map, int newMx, int newMy, int mex, int mey, int camx, int camy) {
	int tileW = tilew*multiW;
	int tileH = tileh*multiH;
	for (int i = 0; i < max; i++)
	{
		if (tilec[i].alive)
		{
			// Mouse on Tilec
			if (newMx+tileW-4 > tilec[i].x && newMx+2 < tilec[i].x + tilec[i].w &&
				newMy+tileH-4 > tilec[i].y && newMy+2 < tilec[i].y + tilec[i].h) {
				tilec[i].mouseBox = true;
			} else {
				tilec[i].mouseBox = false;
			}

			// Mouse on Tilec
			if (mex > tilec[i].x && mex < tilec[i].x + tilec[i].w &&
				mey > tilec[i].y && mey < tilec[i].y + tilec[i].h) {
				tilec[i].mouse = true;
			} else {
				tilec[i].mouse = false;
			}
		}
	}
}

void Tilec::RemoveTile(Tilec tilec[], int click) {
	for (int i = 0; i < max; i++) {
		if (tilec[i].alive){
			// Left click
			if (click == 0) {
				if (tilec[i].mouseBox){
					//if (tilec[i].layer == layer) {
					//}
					tilec[i].alive = false;
					count--;
				}
			// Right click
			}else{
				if (tilec[i].mouse){
					//if (tilec[i].layer == layer) {
					//}
					tilec[i].alive = false;
					count--;
				}
			}
		}
	}
}

void Tilec::RemoveAll(Tilec tilec[]) {
	count = 0;
	multiW = 1;
	multiH = 1;
	tilew = 64;
	tileh = 64;
	id = 0;
	LevelToLoad = 0;
	for (int i = 0; i < max; i++) {
		tilec[i].alive = false;
	}
}

void Tilec::Copy(Tilec tilec[]) {
	for (int i = 0; i < max; i++) {
		if (tilec[i].alive){
			if (tilec[i].mouse){
				if (tilec[i].layer == layer) {
					tilew = tilec[i].w;
					tileh = tilec[i].h;
					id = tilec[i].type;
					LevelToLoad = tilec[i].LevelToLoad;
				}
			}
		}
	}
}

void Tilec::ChangePos(Tilec tilec[]) {
	for (int i = 0; i < max; i++) {
		if (tilec[i].alive){
			if (tilec[i].mouse){
				tilec[i].newPos++;

				// Loop around
				if (tilec[i].newPos > 3) {
					tilec[i].newPos = 0;
				}
			}
		}
	}
}

void Tilec::Spawn(Tilec tilec[], float x, float y, int w, int h) {
	for (int i = 0; i < max; i++) {
		if (!tilec[i].alive) {
			tilec[i].x 				= x;
			tilec[i].y 				= y;
			tilec[i].w 				= w;
			tilec[i].h 				= h;
			tilec[i].type 			= id;
			tilec[i].layer 			= layer;
			tilec[i].LevelToLoad 	= LevelToLoad;;
			tilec[i].newPos 		= newPos;
			tilec[i].side 			= "right";
			tilec[i].mouse 			= false;
			tilec[i].mouseBox 		= false;
			tilec[i].collision 		= false;
			tilec[i].promptSelf 	= false;
			tilec[i].alive 			= true;
			count++;
			break;
		}
	}
}
void Tilec::SpawnAll(Tilec tilec[], int newMx, int newMy, int camx, int camy) {
	RemoveTile(tilec, 0);
	for (int j = 0; j < multiW; j++) {
		for (int h = 0; h < multiH; h++) {
			int x = int(newMx + j * tilew+camx);
			int y = int(newMy + h * tileh+camy);
			Spawn(tilec, x, y, tilew, tileh);
		}
	}
}

void Tilec::MoveTilecs(Tilec tilec[], std::string direction){
	for (int i = 0; i < max; i++) {
		if (tilec[i].alive){
			if (direction == "left") {
				tilec[i].x -= 64;
			}
			else if (direction == "right") {
				tilec[i].x += 64;
			}
			else if (direction == "up") {
				tilec[i].y -= 64;
			}
			else if (direction == "down") {
				tilec[i].y += 64;
			}

		}
	}
}
void Tilec::RenderUI(SDL_Renderer *gRenderer, Tilec tilec[], int camx, int camy) {
	for (int i = 0; i < max; i++) {
		if (tilec[i].alive) {
			if (tilec[i].promptSelf) {
				std::stringstream tempsi;
				tempsi << "E";
				gText.loadFromRenderedText(gRenderer, tempsi.str().c_str(), {255, 255, 255}, gFont20);
				gText.render(gRenderer, tilec[i].x+tilec[i].w/2-gText.getWidth()/2-camx,
										tilec[i].y+tilec[i].h/2-gText.getHeight()/2-camy,
										gText.getWidth(),
										gText.getHeight());
			}
		}
	}
}



void Tilec::Render(SDL_Renderer *gRenderer, Tilec tilec[], int layer, int camx, int camy) {
	for (int i = 0; i < max; i++) {
		if (tilec[i].alive) {
			//if (tilec[i].layer == layer) {
				if (tilec[i].type == 0) {
					SDL_Rect tempRect = {tilec[i].x+2-camx, tilec[i].y+2-camy, tilec[i].w-4, tilec[i].h-4};
					SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
					SDL_RenderDrawRect(gRenderer, &tempRect);
				}
				if (tilec[i].type == 1) {
					SDL_Rect tempRect = {tilec[i].x-camx, tilec[i].y-camy, tilec[i].w, tilec[i].h};
					SDL_SetRenderDrawColor(gRenderer, 255, 255, 0, 255);
					SDL_RenderDrawRect(gRenderer, &tempRect);
				}
				if (tilec[i].type == 2) {
					SDL_Rect tempRect = {tilec[i].x-camx, tilec[i].y-camy, tilec[i].w, tilec[i].h};
					SDL_SetRenderDrawColor(gRenderer, 255, 0, 255, 255);
					SDL_RenderDrawRect(gRenderer, &tempRect);
				}

				// If mouse is on tilec
				if (tilec[i].mouse) {
					SDL_Rect tempr = {tilec[i].x+4 - camx, tilec[i].y+4 - camy, tilec[i].w-8, tilec[i].h-8};
					SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255);
					SDL_RenderDrawRect(gRenderer, &tempr);
				}

				// If mouse w/ hand size on tilec
				if (tilec[i].mouseBox) {
					SDL_Rect tempr = {tilec[i].x+6 - camx, tilec[i].y+6 - camy, tilec[i].w-12, tilec[i].h-12};
					SDL_SetRenderDrawColor(gRenderer, 244, 144, 20, 255);
					SDL_RenderDrawRect(gRenderer, &tempr);
				}

				// If our tilec can load levels
				if (tilec[i].LevelToLoad >= 1) {
					SDL_Rect tempRect = {tilec[i].x+tilec[i].w-8-camx, tilec[i].y+tilec[i].h-8-camy, 8, 8};
					SDL_SetRenderDrawColor(gRenderer, 244, 144, 40, 255);
					SDL_RenderDrawRect(gRenderer, &tempRect);
				} else {

					// This kind of Tilec does nothing
					SDL_Rect tempRect = {tilec[i].x+tilec[i].w-8-camx, tilec[i].y+tilec[i].h-8-camy, 8, 8};
					SDL_SetRenderDrawColor(gRenderer, 0, 255, 255, 255);
					SDL_RenderDrawRect(gRenderer, &tempRect);
				}
			//}
		}
	}
}

void Tilec::RenderDebug(SDL_Renderer *gRenderer, Tilec tilec[], int camx, int camy) {
	for (int i = 0; i < max; i++){
		if (tilec[i].alive){
			//if (tilec[i].LevelToLoad >= 1)

			std::stringstream tempss;
			tempss << tilec[i].LevelToLoad << " - " << tilec[i].newPos;
			gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {244,144,30}, gFont12);
			gText.setAlpha(255);
			gText.render(gRenderer, tilec[i].x-camx,
									tilec[i].y+tilec[i].h-gText.getHeight()-gText.getHeight()-camy,
									gText.getWidth(), gText.getHeight());
		}
	}
}

void Tilec::RenderHand(SDL_Renderer *gRenderer, Tilec tilec[], int newMx, int newMy, int mex, int mey){
	int newWidth = tilew*multiW;
	int newHeight = tileh*multiH;
	for (int j = 0; j < multiW; j++) {
		for (int h = 0; h < multiH; h++) {
			// Render mouse coordinates snapped to grid
			if (id == 0) {
				SDL_Rect tempr = {newMx, newMy, newWidth, newHeight};
				SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
				SDL_RenderDrawRect(gRenderer, &tempr);
			}else if (id == 1) {
				SDL_Rect tempr = {newMx, newMy, newWidth, newHeight};
				SDL_SetRenderDrawColor(gRenderer, 255, 255, 0, 255);
				SDL_RenderDrawRect(gRenderer, &tempr);
			}else if (id == 2) {
				SDL_Rect tempr = {newMx, newMy, newWidth, newHeight};
				SDL_SetRenderDrawColor(gRenderer, 255, 0, 255, 255);
				SDL_RenderDrawRect(gRenderer, &tempr);
			}
		}
	}
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------------------------------//
//-------------------------------------------- Load Tilec Data ------------------------------------------------//
// Load Tile data
void Tilec::LoadData(Tilec tilec[], std::fstream &fileTileDataL)
{
	//std::cout << "Attempting to load... Tilec" << std::endl;
	// Load Tilec count data
	fileTileDataL >> this->count;
	//std::cout << this->count << std::endl;

	// Load rest of Tilec data
	for (int j = 0; j < this->count; j++) {
		if (!tilec[j].alive) {
			fileTileDataL >>  tilec[j].x 			>>
							  tilec[j].y 			>>
							  tilec[j].w 			>>
							  tilec[j].h 			>>
							  tilec[j].type 		>>
							  tilec[j].layer 		>>
							  tilec[j].LevelToLoad 	>>
							  tilec[j].newPos 		>>
							  tilec[j].alive;

			//std::cout << "Tilec j: " << j << ", x: " << tilec[j].x << ", y: " << tilec[j].y << std::endl;
		}
		//break;
	}
}
//-------------------------------------------- Load Tilec Data ------------------------------------------------//
//-------------------------------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------------------------------//
//-------------------------------------------- Save Tilec Data ------------------------------------------------//
std::string Tilec::SaveData(Tilec tilec[])
{
	// Save all tile data in a string first
	std::stringstream tempsstb;
	for (int i = 0; i < max; i++) {
		if (tilec[i].alive) {
			tempsstb << tilec[i].x 				<< " "
					 << tilec[i].y 				<< " "
					 << tilec[i].w  			<< " "
					 << tilec[i].h  			<< " "
					 << tilec[i].type  			<< " "
					 << tilec[i].layer  		<< " "
					 << tilec[i].LevelToLoad  	<< " "
					 << tilec[i].newPos  		<< " "
			   	   	 << tilec[i].alive 			<< "\n";
		}
	}
	std::string temps;
	temps = tempsstb.str().c_str();

	// Commented out because this takes out the extra line thats empty at the end
	//temps = temps.substr(0, temps.size()-1);

	// Save number of tiles in World
	std::stringstream aVeryLongString;
	aVeryLongString << count << "\n";

	// Save the data of all Tiles
	aVeryLongString << temps.c_str();
	//----------------------------- Save Everything in One File -------------------//
	//-----------------------------------------------------------------------------//
	/////////////////////////////////////////////////////////////////////////////////

	return aVeryLongString.str().c_str();
}
//-------------------------------------------- Save Tilec Data ------------------------------------------------//
//-------------------------------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

