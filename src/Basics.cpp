/*
 * Basics.cpp
 *
 *  Created on: May 15, 2017
 *      Author: Carl
 */

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "Basics.h"

void Basics::Init() {

}

void Basics::Load(SDL_Renderer *gRenderer) {

}

void Basics::Free() {

}
/*
void Basics::Show(LWindow &gWindow, SDL_Renderer *gRenderer, Basics::Result &result) {

	// Upon entry
	quit = false;
	leftClick = false;
	shift = false;

	// Load resources
	Load(gRenderer);

	// While loop
	while (!quit) {

		// Get mouse position
		SDL_GetMouseState(&mx, &my);

		// Handle Events
		while (SDL_PollEvent(&event)) {

			// Handle window events
			gWindow.handleEvent(gRenderer, event);

			// Back Game
			if (event.type == SDL_QUIT) {
				result = Exit;
				quit = true;
				Free();
				return;
			}else{
				// Key Pressed
				if (event.type == SDL_KEYDOWN && event.key.repeat == 0) {
					switch (event.key.keysym.sym) {
					case SDLK_LSHIFT:
						shift = true;
						break;
					case SDLK_RSHIFT:
						shift = true;
						break;
					}
				}
				// Key Released
				else if (event.type == SDL_KEYUP && event.key.repeat == 0) {
					switch (event.key.keysym.sym) {
					case SDLK_LSHIFT:
						shift = false;
						break;
					case SDLK_RSHIFT:
						shift = false;
						break;
					}
				}

				// Mouse Pressed
				mousePressed(event);

				// Mouse Released
				result = mouseReleased(event);

				// Customize Character results
				switch (result)  {
					case Back:				// Back (to Main Menu)
						quit = true;
						break;
					case Nothing:			// Nothing

						break;
					case StartGame:			// Start Game
						quit = true;
						break;
				}
			}
		}

		// Update Variables


		// Clear render screen
		SDL_SetRenderDrawColor(gRenderer, 80, 80, 0, 255);
		SDL_RenderClear(gRenderer);

			SDL_Rect tempr = {128, 128, 128, 128};
			SDL_SetRenderDrawColor(gRenderer, 255, 50, 50, 255);
			SDL_RenderDrawRect(gRenderer, &tempr);

		// Update screen
		SDL_RenderPresent(gRenderer);
	}
	// Free everything
	Free();
}*/
