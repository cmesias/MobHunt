/*
 * GetInput.h
 *
 *  Created on: Apr 6, 2022
 *      Author: healt
 */

#ifndef GETINPUT_H_
#define GETINPUT_H_

#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "Helper.h"
#include "LTexture.h"
#include "LWindow.h"
#include "Game/TextNFont.h"

class GetInput {

public:

	Helper helper;
	TextNFont textNFont;

	enum GetInputResult { Back, Exit, Nothing, StartGame, ShowingMenu };
	GetInputResult getInputResult = Nothing;

public:

	// Return a string the user has entered (hint is used to give the Editor an idea of what to write)
	std::string PromptInput(LWindow &gWindow, SDL_Renderer *gRenderer, bool &mainLoop, std::string hint);
};

#endif /* GETINPUT_H_ */
