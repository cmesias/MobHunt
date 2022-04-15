/*
 * Credits.h
 *
 *  Created on: Aug 22, 2017
 *      Author: Carl
 */

#ifndef CREDITS_H_
#define CREDITS_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include "LWindow.h"
#include "LTexture.h"
#include "Helper.h"
#include "Game/TextNFont.h"
#include "Particless.h"

#include <fstream>
#include <sstream>
#include <vector>
#include <math.h>


class Credits : public Helper, public TextNFont {
public:
	bool quit;
	enum CreditsResult { StartGame, Nothing, Back, Exit };
	void Show(LWindow &gWindow, SDL_Renderer *gRenderer, Credits::CreditsResult &result);
private:
	void free();
	LTexture gCursor;
	LTexture gText;
	LTexture gBG;
	LTexture gBG2;
	LTexture gPoof;
	LTexture gTexture;
	LTexture gTargetTexture;
	LTexture gDialogueBox;
	SDL_Rect rPoof[7];

	float indicatorTimer;
	int indicatorFrame;
	std::string indicator;

	float timer = 0;
	const unsigned int maxPoofs = 10;

	struct Poof {
		float x, y;
		float timer;
		int frame;
		Poof(float newX, float newY) {
			x = newX;
			y = newY;
			timer = 0;
			frame = 0;
		}
	};

	std::vector<Poof> poofs;

	bool holdUp;
	bool holdDown;
	float holdTimer;
};

#endif /* ENGINE_CREDITS_H_ */
