/*
 * Bar.h
 *
 *  Created on: Apr 28, 2017
 *      Author: Carl
 */

#ifndef BAR_H_
#define BAR_H_

#include <sstream>

#include <SDL2/SDL_ttf.h>
#include "Game/TextNFont.h"
#include "LTexture.h"

struct Bar: public TextNFont {
public:
	int x;				// x position of bar
	int y;				// y position of bar
	int width;			// bar width
	int height;			// bar height
	int color[3];		// bar color
	int maxColor;
	std::string name;
	SDL_Rect bar[3];

	// Get value from a bar relative to mouse
	int getValueFromBar(int mx, int barX, int widthOnScreen, int maxValue);

	// initialize
	void init(std::string newName, int newWidth, int newHeight);

	void update(bool leftClick);

	// render RGB channel buttons for something (eyes, body, etc.)
	void render(SDL_Renderer *gRenderer);

	void load();
	void free();
};

#endif /* BAR_H_ */
