/*
* Bar.cpp
*
*  Created on: Apr 28, 2017
*      Author: Carl
*/

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "Bar.h"



// Get value from a bar relative to mouse
int Bar::getValueFromBar(int mx, int barX, int widthOnScreen, int maxValue) {
	int newMouseX = mx - barX;
	int value = maxValue*newMouseX/widthOnScreen;
	return value;
}

// initialize
void Bar::init(std::string newName, int newWidth, int newHeight) {
	load();
	name = newName;
	x = 0;
	y = 0;
	width = newWidth;
	height = newHeight;
	maxColor = 255;
	for (int i=0; i<3; i++) {
		color[i] = 255;
		bar[i].h = height;
		bar[i].x = 0;
		bar[i].y = 0;
	}
}
// update
void Bar::update(bool leftClick) {
	int mx, my;
	SDL_GetMouseState(&mx, &my);
	for (int i=0; i<3; i++) {

		// Set bar position
		bar[i].h = height;
		bar[i].x = x;
		bar[i].y = y+i*bar[i].h+5;

		// Mouse collision check
		int wedth = ((width*maxColor)/maxColor);
		if (mx+1 >= bar[i].x && mx <= bar[i].x+wedth && my+1 >= bar[i].y && my <= bar[i].y+bar[i].h) {
			if (leftClick) {
				int value = getValueFromBar(mx, bar[i].x, width, maxColor);
				if (value < 0)
					value = 0;
				if (value > 255)
					value = 255;
				if (i==0) {
					color[i] = value;
				}else if (i==1) {
					color[i] = value;
				}else if (i==2) {
					color[i] = value;
				}
			}
		}
	}
}

// render RGB channel buttons for something (eyes, body, etc.)
void Bar::render(SDL_Renderer *gRenderer) {
	for (int i=0; i<3; i++) {
		SDL_Color tempColor = {100,0,0};
		SDL_Color tempColorBG = {255,0,0};
		if (i==0) {
			tempColor.r 	= 100;
			tempColorBG.r 	= 190;
			tempColor.g 	= 0;
			tempColorBG.g 	= 0;
			tempColor.b 	= 0;
			tempColorBG.b 	= 0;
		}else if (i==1) {
			tempColor.r 	= 0;
			tempColorBG.r 	= 0;
			tempColor.g 	= 100;
			tempColorBG.g 	= 190;
			tempColor.b 	= 0;
			tempColorBG.b 	= 0;
		}else if (i==2) {
			tempColor.r 	= 0;
			tempColorBG.r 	= 0;
			tempColor.g 	= 0;
			tempColorBG.g 	= 0;
			tempColor.b 	= 100;
			tempColorBG.b 	= 190;
		}
		// Background health
		bar[i].w = ((width*maxColor)/maxColor);
		SDL_SetRenderDrawColor(gRenderer, tempColor.r, tempColor.g, tempColor.b, 255);
		SDL_RenderFillRect(gRenderer, &bar[i]);
		// Current health
		bar[i].w = ((width*color[i])/maxColor);
		SDL_SetRenderDrawColor(gRenderer, tempColorBG.r, tempColorBG.g, tempColorBG.b, 255);
		SDL_RenderFillRect(gRenderer, &bar[i]);
		// Border
		bar[i].w = ((width*maxColor)/maxColor);
		SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
		SDL_RenderDrawRect(gRenderer, &bar[i]);
	}

	// Render text, RGB bar name
	for (int i=0; i<3; i++) {
		std::stringstream tempss;
		tempss << color[i];
		gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {255,255,255}, gFont12);
		gText.setAlpha(255);
		gText.render(gRenderer, bar[i].x+bar[i].w/2-gText.getWidth()/2, bar[i].y, gText.getWidth(), gText.getHeight());
	}

	// Render text, RGB bar name
	std::stringstream tempss;
	tempss << name;
	gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {255,255,255}, gFont20);
	gText.setAlpha(255);
	gText.render(gRenderer, x, y-21, gText.getWidth(), gText.getHeight());
}



void Bar::load() {
	LoadFonts();
}


void Bar::free() {
	FreeFonts();
}
