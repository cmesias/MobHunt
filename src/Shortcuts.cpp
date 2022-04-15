/*
 * Shortcuts.cpp
 *
 *  Created on: Apr 6, 2022
 *      Author: healt
 */

#include "Shortcuts.h"



void Shortcuts::DrawRect(SDL_Renderer *gRenderer, float x, float y, float w, float h, SDL_Color color) {
	SDL_Rect tempRect = {x, y, w, h};
	SDL_SetRenderDrawColor(gRenderer, color.r, color.g, color.b, 255);
	SDL_RenderDrawRect(gRenderer, &tempRect);
}

void Shortcuts::DrawFillRect(SDL_Renderer *gRenderer, float x, float y, float w, float h, SDL_Color color) {
	SDL_Rect tempRect = {x, y, w, h};
	SDL_SetRenderDrawColor(gRenderer, color.r, color.g, color.b, 255);
	SDL_RenderFillRect(gRenderer, &tempRect);
}
