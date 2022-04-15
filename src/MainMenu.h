/*
 * MainMenu.h
 *
 *  Created on: Apr 20, 2017
 *      Author: Carl
 */

#ifndef MAINMENU_H_
#define MAINMENU_H_

#include <SDL2/SDL_ttf.h>

// Include other headers
#include "Helper.h"

// Native for this header only
#include "LTexture.h"
#include "LWindow.h"
#include "Game/TextNFont.h"
#include "Settings.h"

class MainMenu : public Helper, public TextNFont  {
public:	// Resources
	Settings settings;
	LTexture gMenu;
	LTexture gText;

	// Mouse cursor
	LTexture gCursor;
public:
	enum MenuResult { Nothing, NewGame, HowToPlay, Options, Credits, Exit };
	void Show(LWindow &gWindow, SDL_Renderer *gRenderer, MainMenu::MenuResult &result);
	MainMenu::MenuResult mousePressed(SDL_Event event);
	MainMenu::MenuResult mouseReleased(LWindow gWindow, SDL_Renderer *gRenderer, SDL_Event event);

public:	// Variables

	int mex, mey;

public:

	// Initialize
	void Init();

	// Load resources
	void Load(SDL_Renderer *gRenderer);

	// Free resources
	void Free();

	// Render
	void Render(SDL_Renderer *gRenderer);

private:
	SDL_Joystick *joy=NULL;

	// JoyStick controls
	void updateJoystick(SDL_Renderer *gRenderer, LWindow &gWindow, SDL_Event *e, MainMenu::MenuResult &result);

	bool A;									// XBOX a button
	bool LAnalogTrigger;					// this will let us use the joystick as a trigger rather than an axis
	const int JOYSTICK_DEAD_ZONE = 8000;
	const int LTRIGGER_DEAD_ZONE = 25000;
	const int RTRIGGER_DEAD_ZONE = 25000;
	int joyStickID;
	double LAngle;
	double RAngle;
	/* Main Menu items
	 * 0: New Game
	 * 1: How To Play
	 * 2: Options
	 * 3: Credits
	 * 4: Exit
	 */
	std::string buttonName[5];
	SDL_Rect levelsBox[5];
	int menuIndex;				// Which level the Player is currently on
	int key;
	int mx, my;
	bool leftClick;
	bool confirmKey;
	bool quit;
	bool shift;
	SDL_Event event;
};

#endif /* MAINMENU_H_ */
