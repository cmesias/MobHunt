/*
 * gTextAndFonts.cpp
 *
 *  Created on: Mar 8, 2022
 *      Author: healt
 */



#include "TextNFont.h"

void TextNFont::LoadFonts() {

	// Load fonts
	gFont =   TTF_OpenFont("resource/fonts/PressStart2P.ttf", 18);
	gFont12 = TTF_OpenFont("resource/fonts/PressStart2P.ttf", 12);
	gFont20 = TTF_OpenFont("resource/fonts/PressStart2P.ttf", 20);
}

void TextNFont::FreeFonts() {

	// Free textures
	gText.free();

	// Free fonts
	TTF_CloseFont(gFont);
	TTF_CloseFont(gFont12);
	TTF_CloseFont(gFont20);

	gFont = NULL;
	gFont12 = NULL;
	gFont20 = NULL;
}

void TextNFont::RenderText(SDL_Renderer *gRenderer, float x, float y, std::string textField, float alpha, SDL_Color color)
{
	gText.loadFromRenderedText(gRenderer, textField, color, gFont12);
	gText.setAlpha(alpha);
	gText.render(gRenderer, x, y, gText.getWidth(), gText.getHeight());
}

void TextNFont::RenderText(SDL_Renderer *gRenderer, float x, float y, float valueTextField, float alpha, SDL_Color color, std::string alignment)
{
	std::stringstream tempss;
	tempss << valueTextField;
	gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), color, gFont12);
	gText.setAlpha(alpha);

	float xPos = x;
	float yPos = y;
	if (alignment == "right") {
		xPos = x;
		yPos = y;
	}
	if (alignment == "left") {
		xPos = x - gText.getWidth();
		yPos = y;
	}
	if (alignment == "top") {
		xPos = x;
		yPos = y - gText.getHeight();
	}
	if (alignment == "bottom") {
		xPos = x;
		yPos = y + gText.getHeight();
	}


	gText.render(gRenderer, xPos, yPos, gText.getWidth(), gText.getHeight());
}

void TextNFont::RenderText(SDL_Renderer *gRenderer, float x, float y, std::string textField, float alpha, SDL_Color color, std::string alignment)
{
	gText.loadFromRenderedText(gRenderer, textField.c_str(), color, gFont12);
	gText.setAlpha(alpha);

	float xPos = x;
	float yPos = y;
	if (alignment == "right") {
		xPos = x;
		yPos = y;
	}
	if (alignment == "left") {
		xPos = x - gText.getWidth();
		yPos = y;
	}
	if (alignment == "top") {
		xPos = x;
		yPos = y - gText.getHeight();
	}
	if (alignment == "bottom") {
		xPos = x;
		yPos = y + gText.getHeight();
	}


	gText.render(gRenderer, xPos, yPos, gText.getWidth(), gText.getHeight());
}
