/*
 * Maps.cpp
 *
 *  Created on: Dec 20, 2016
 *      Author: Carl
 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <fstream>
using namespace std;

#include "Maps.h"

// Render map
void Map::render(SDL_Renderer* gRenderer, int camx, int camy){
	SDL_Rect tempRect = { this->x-camx, this->y-camy, this->w, this->h };
	SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
	SDL_RenderDrawRect(gRenderer, &tempRect);
}

int Map::getWidth(){
	return this->w;
}

int Map::getHeight(){
	return this->h;
}

void Map::IncreaseWidth(int width){
	this->w += width;
}

void Map::IncreaseHeight(int height){
	this->h += height;
}

