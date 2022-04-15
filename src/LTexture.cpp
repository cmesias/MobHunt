/*
 * LTexture.cpp
 *
 *  Created on: Apr 18, 2017
 *      Author: healt
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "LTexture.h"




LTexture::LTexture() {
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture() {
	//Deallocate
	free();
}

bool LTexture::loadFromFile(SDL_Renderer *gRenderer, std::string path, bool itsAFont, bool save) {
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());

	/*if (save) {
		std::stringstream testt;
		testt << IMG_Load(path.c_str());
		std::cout << "testt: " << testt.str().c_str() << std::endl;

		std::ofstream fileS2;
		fileS2.open("testSTR.png");
		fileS2 << testt.str().c_str();
		fileS2.close();
	}*/

	if (loadedSurface == NULL) {
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(),
		IMG_GetError());
	} else {
		//Color key image
		SDL_SetColorKey(loadedSurface, SDL_TRUE,
				SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL) {
			printf("Unable to create texture from %s! SDL Error: %s\n",
					path.c_str(), SDL_GetError());
		} else {
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

#ifdef _SDL_TTF_H
bool LTexture::loadFromRenderedText(SDL_Renderer *gRenderer,
									std::string textureText,
									SDL_Color textColor,
									TTF_Font *fontname,
									Uint32 wrapLength) {
	//Get rid of preexisting texture
	free();

	//Render text surface
	/*SDL_Surface* textSurface = TTF_RenderText_Solid(fontname,
			textureText.c_str(), textColor);*/

	/*SDL_Surface* textSurface = TTF_RenderText_Shaded(fontname,
			textureText.c_str(), textColor, {120, 0, 50});*/

	SDL_Surface* textSurface = TTF_RenderText_Blended_Wrapped(fontname,
			textureText.c_str(), textColor, wrapLength);

	if (textSurface != NULL) {
		//Create texture from surface pixels
		mTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
		if (mTexture == NULL) {
			//printf(
			//		"Unable to create texture from rendered text! SDL Error: %s\n",
			//		SDL_GetError());
		} else {
			//Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface(textSurface);
	} else {
		//printf("Unable to render text surface! SDL_ttf Error: %s\n",
		//TTF_GetError());
	}

	//Return success
	return mTexture != NULL;
}
#endif

void LTexture::free() {
	//Free texture if it exists
	if (mTexture != NULL) {
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue) {
	//Modulate texture rgb
	SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void LTexture::setBlendMode(SDL_BlendMode blending) {
	//Set blending function
	SDL_SetTextureBlendMode(mTexture, blending);
}

void LTexture::setAlpha(Uint8 alpha) {
	//Modulate texture alpha
	SDL_SetTextureAlphaMod(mTexture, alpha);
}

void LTexture::render(SDL_Renderer *gRenderer, int x, int y, int w, int h, SDL_Rect* clip, double angle,
				SDL_Point* center, SDL_RendererFlip flip) {


	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, w, h };

	//Set clip rendering dimensions
	if (clip != NULL) {
		renderQuad.w = w;
		renderQuad.h = h;
	}

	//Render to screen
	SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderQuad, angle, center,
			flip);
}

void LTexture::setAsRenderTarget(SDL_Renderer *gRenderer)
{
    //Make self render target
    SDL_SetRenderTarget( gRenderer, mTexture );
}

bool LTexture::createBlank( SDL_Renderer *gRenderer, int width, int height, SDL_TextureAccess access )
{
	//Create uninitialized texture
	mTexture = SDL_CreateTexture( gRenderer, SDL_PIXELFORMAT_RGBA8888, access, width, height );
	if( mTexture == NULL )
	{
		printf( "Unable to create blank texture! SDL Error: %s\n", SDL_GetError() );
	}
	else
	{
		mWidth = width;
		mHeight = height;
	}

	return mTexture != NULL;
}

// Initialize texture (blank one)
void LTexture::init(SDL_Renderer *gRenderer, int w, int h, SDL_Color color, Uint8 alpha) {
	// Create blank texture
	createBlank( gRenderer, w, h, SDL_TEXTUREACCESS_TARGET );
	// Set texture as render target
	setAsRenderTarget(gRenderer);
	// Set transparency
	setAlpha(alpha);
	// Render on texture
	SDL_Rect tempr = {0, 0, w, h};
	SDL_SetRenderDrawColor(gRenderer, color.r, color.g, color.b, color.a);
	SDL_RenderDrawRect(gRenderer, &tempr);
	//Reset render target
	SDL_SetRenderTarget( gRenderer, NULL );
}

int LTexture::getWidth() {
	return mWidth;
}

int LTexture::getHeight() {
	return mHeight;
}
