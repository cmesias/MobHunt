/*
 * LTexture.h
 *
 *  Created on: Apr 18, 2017
 *      Author: healt
 */

#ifndef LTEXTURE_H_
#define LTEXTURE_H_

/* Putting this here will prevent errors
 * if some other class includes this class */
#include <iostream>

//Texture wrapper class
class LTexture {
public:
	//Initializes variables
	LTexture();

	//Deallocates memory
	~LTexture();

	//Loads image at specified path
	bool loadFromFile(SDL_Renderer *gRenderer, std::string path, bool itsAFont = false, bool save = false);

#ifdef _SDL_TTF_H
	//Creates image from font string
	bool loadFromRenderedText(SDL_Renderer *gRenderer, std::string textureText, SDL_Color textColor,
							TTF_Font *fontname, Uint32 wrapLength = 0xffffffff);
#endif

	//Creates blank texture
	bool createBlank( SDL_Renderer *gRenderer, int width, int height, SDL_TextureAccess = SDL_TEXTUREACCESS_STREAMING );

	//Deallocates texture
	void free();

	//Set color modulation
	void setColor(Uint8 red, Uint8 green, Uint8 blue);

	//Set blending
	void setBlendMode(SDL_BlendMode blending);

	//Set alpha modulation
	void setAlpha(Uint8 alpha);

	//Renders texture at given point
	void render(SDL_Renderer *gRenderer, int x, int y, int w, int h, SDL_Rect* clip = NULL,
			double angle = 0.0, SDL_Point* center = NULL,
			SDL_RendererFlip flip = SDL_FLIP_NONE);

    //Set self as render target
    void setAsRenderTarget(SDL_Renderer *gRenderer);

    // Initialize texture (blank one)
    void init(SDL_Renderer *gRenderer, int w, int h, SDL_Color color, Uint8 alpha);

	//Gets image dimensions
	int getWidth();
	int getHeight();

	SDL_Texture *getTexture() {
		return mTexture;
	}

private:

	//The actual hardware texture
	SDL_Texture* mTexture;

	//Image dimensions
	int mWidth;
	int mHeight;
};

#endif /* LTEXTURE_H_ */
