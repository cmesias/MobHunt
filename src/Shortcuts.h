/*
 * Shortcuts.h
 *
 *  Created on: Apr 6, 2022
 *      Author: healt
 */

#ifndef SHORTCUTS_H_
#define SHORTCUTS_H_

#include <SDL2/SDL.h>

class Shortcuts {
public:

	void DrawRect(SDL_Renderer *gRenderer, float x, float y, float w, float h, SDL_Color color);

	void DrawFillRect(SDL_Renderer *gRenderer, float x, float y, float w, float h, SDL_Color color);
};

#endif /* SHORTCUTS_H_ */
