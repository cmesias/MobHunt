/*
 * TextNFont.h
 *
 *  Created on: Mar 8, 2022
 *      Author: healt
 */

#ifndef TEXTNFONT_H_
#define TEXTNFONT_H_

#include <sstream>

#include <SDL2/SDL_ttf.h>

#include "../LTexture.h"

class TextNFont {

public:
	// Textures
	LTexture gText;

	// Fonts
	TTF_Font *gFont = NULL;
	TTF_Font *gFont12 = NULL;
	TTF_Font *gFont20 = NULL;

	// Load fonts
	void LoadFonts();

	// Free resources
	void FreeFonts();

	// Render text
	void RenderText(SDL_Renderer *gRenderer, float x, float y,
			std::string textField,
			float alpha,
			SDL_Color color);

	/*
	 * Alignment "right": 		render to right of Text (normal)
	 * Alignment "left": 		render to left of Text
	 * Alignment "bottom": 		render to bottom of Text
	 * Alignment "top": 		render to top of Text
	 */
	void RenderText(SDL_Renderer *gRenderer, float x, float y,
			float valueTextField, float alpha,
			SDL_Color color, std::string alignment="right");

	void RenderText(SDL_Renderer *gRenderer, float x, float y,
			std::string textField, float alpha,
			SDL_Color color, std::string alignment="right");

};



#endif /* TEXTNFONT_H_ */
