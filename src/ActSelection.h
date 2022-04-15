/*
 * ActSelection.h
 *
 *  Created on: May 15, 2017
 *      Author: Carl
 */

#ifndef ACTSELECTION_H_
#define ACTSELECTION_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "LWindow.h"
#include "LTexture.h"
#include "Helper.h"
#include "Game/Button.h"
#include "Game/TextNFont.h"

#include <fstream>
#include <limits>
#include <sstream>
#include <iostream>


class ActSelection: public TextNFont, public Helper {
public:
	enum Result { Back, Nothing, StartGame, Exit };
	void Show(LWindow &gWindow, SDL_Renderer *gRenderer, ActSelection::Result &result, int &LevelToLoad);
	ActSelection::Result mousePressed(SDL_Event event);
	ActSelection::Result mouseReleased(SDL_Event event);

private:	// Textures

	// Mouse cursor
	LTexture gCursor;

	// Level preview textures
	LTexture gLevelPreviews[5];

	// Locked level preview texture
	LTexture gLockedLevel;

private:
	int mx, my, mex, mey;
	bool leftClick;
	bool quit;
	bool shift;
	bool debug;
	SDL_Event event;
	float camx, camy;

	/* Focus:
	 * 0: Keyboard controls
	 * 1: Mouse controls
	 */
	int controls=-1;

private:	// Variables for ActSelection

	const int levelMax = 5;	// Max levels we currently have

	/* We will make 5 levels for this game
	 * Level 1:
	 * Level 2:
	 * Level 3:
	 * Level 4:
	 * Level 5:
	 */
	int levels[5];
	int levelIndex;		// Current hovered level
	int selectedLevel;	// Current selected level
	struct levelBox {
		float x, y;
		float w, h;
		bool hover;
		bool selected;
	};

	levelBox levelbox[5];

	int widthSpacing = screenWidth * 0.15;

public:	// Save these data to a file

	int LevelsCompleted;	// how many levels the player has unlocked



private:	// Other classes

	// Start game button
	Button StartGameBtn;

	// Erase data button
	Button EraseDataBtn;

	// Back button
	Button BackBtn;

public:

	// Initialize
	void Init();

	// Load resources
	void Load(SDL_Renderer *gRenderer);

	// Free resources
	void Free();

	// Load previous levelsCompleted.mp
	void LoadLevelsUnlockedFile();

	// Erase levelsCompleted.mp, and set default
	void EraseFileData();

};

#endif /* ACTSELECTION_H_ */












