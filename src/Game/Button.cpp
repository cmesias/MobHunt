/*
 * Button.cpp
 *
 *  Created on: Mar 11, 2022
 *      Author: healt
 */

#include "Button.h"


void Button::Init(std::string textField, std::string resultText, bool sameSizeAsText) {
	this->textField 		= textField;
	this->resultText 		= resultText;
	this->hover 			= false;
	this->pressed 			= false;
	this->sameSizeAsText 	= sameSizeAsText;
}

void Button::Init(std::string textField, std::string resultText, float w, float h) {
	this->textField 		= textField;
	this->resultText 		= resultText;
	this->w 				= w;
	this->h 				= h;
	this->hover 			= false;
	this->pressed 			= false;
	this->sameSizeAsText 	= false;
}

void Button::Load() {
	// Other classes
	LoadFonts();
}

void Button::Free() {
	// Other classes
	FreeFonts();
}

void Button::Update(float mex, float mey, float x, float y, bool leftClick)
{
	// Anchor to this position
	this->x 	= x;
	this->y 	= y;

	// If mouse is hovering on level selection box
	if (checkCollision(mex, mey, 1, 1, this->x-20, this->y-15, this->w+40, this->h+30))
	{
		this->hover = true;
	} else {
		this->hover = false;
	}

	// If clicking on button
	if (this->hover && leftClick) {
		this->pressed = true;
	} else {
		this->pressed = false;
	}
}

std::string Button::CheckMouseReleased(float mex, float mey)
{
	std::string result = "";

	if (checkCollision(mex-25, mey-25, 50, 50, this->x, this->y, this->w, this->h))
	{
		// Set result text
		result = this->resultText;
	}

	return result;
}

void Button::Render(SDL_Renderer *gRenderer)
{
	// Button hovered by mouse
	if (this->hover) {
		SDL_Rect tempr = {this->x-20, this->y-15, this->w+40, this->h+30};
		SDL_SetRenderDrawColor(gRenderer, 180, 180, 180, 255);
		SDL_RenderDrawRect(gRenderer, &tempr);
	}

	// Button pressed by mouse
	if (this->pressed) {
		SDL_Rect tempr = {this->x-20, this->y-15, this->w+40, this->h+30};
		SDL_SetRenderDrawColor(gRenderer, 120, 120, 120, 255);
		SDL_RenderDrawRect(gRenderer, &tempr);
	}

	// Text
	gText.loadFromRenderedText(gRenderer, this->textField, {255,255,255}, gFont12);

	if (this->sameSizeAsText) {
		this->w = gText.getWidth();
		this->h = gText.getHeight();
	}

	// Render bg for text
	//DL_Rect tempr = {this->x-10, this->y-7, this->w+20, this->h+14};
	//SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
	//SDL_RenderFillRect(gRenderer, &tempr);

	// Render border for text
	//tempr = {this->x-10, this->y-7, this->w+20, this->h+14};
	//SDL_SetRenderDrawColor(gRenderer, 210, 144, 80, 255);
	//SDL_RenderDrawRect(gRenderer, &tempr);

	// Render text
	gText.setAlpha(255);
	gText.setColor(255,255,255);
	gText.render(gRenderer, this->x, this->y, gText.getWidth(), gText.getHeight());
}

void Button::Render(SDL_Renderer *gRenderer, float xOffset, float yOffset)
{
	// Button hovered by mouse
	if (this->hover) {
		SDL_Rect tempr = {this->x-20, this->y-10, this->w+40, this->h+20};
		SDL_SetRenderDrawColor(gRenderer, 180, 180, 180, 255);
		SDL_RenderDrawRect(gRenderer, &tempr);
	}

	// Button pressed by mouse
	if (this->pressed) {
		SDL_Rect tempr = {this->x-20, this->y-10, this->w+40, this->h+20};
		SDL_SetRenderDrawColor(gRenderer, 120, 120, 120, 255);
		SDL_RenderDrawRect(gRenderer, &tempr);
	}

	// Text
	gText.loadFromRenderedText(gRenderer, this->textField, {255,255,255}, gFont12);

	// Render text
	gText.setAlpha(255);
	gText.setColor(255,255,255);
	gText.render(gRenderer, this->x+xOffset, this->y+yOffset, gText.getWidth(), gText.getHeight());
}
