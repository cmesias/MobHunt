/*
 * Game.h
 *
 *  Created on: Apr 19, 2017
 *      Author: healt
 */

#ifndef GAME_H_
#define GAME_H_

#include "LWindow.h"
#include <cstdio>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include "Options.h"
#include "Helper.h"
#include "Game/TextNFont.h"
#include "Settings.h"

class Game : public Options, public Helper, public TextNFont {
public:	// globals
	LWindow gWindow;
	SDL_Renderer *gRenderer;
	Settings settings;
	int LevelToLoad;

private:
	//Window focus
	bool mMouseFocus;
	bool mKeyboardFocus;
	bool mFullScreen;
	bool mMinimized;
public:
	void Init();
	void Load();
	void Free();
	void Start();
private:
	bool IsExiting();
	void GameLoop();

	// Splash Screen
	void ShowSplashScreen();

	// Menu Screen
	void ShowMenu();

	// Customize Character Screen
	void ShowCustomizeCharacter(LWindow &gWindow, SDL_Renderer *gRenderer);

	// Choose Act X to play (for example Act I)
	void ShowActSelectionScreen(LWindow &gWindow, SDL_Renderer *gRenderer);

	// Actual game play
	void ShowPlayGame();

	// Credits scene
	void ShowCredits();

	enum GameState {Uninitialized, ShowingSplash, ShowingMenu, ShowingCustomizeCharacter, ShowingActSelection, ShowingPlayGame,
					NewGameScene, LoadGameScene, OptionScene, CreditScene, Exiting };
	static GameState _gameState;
};

#endif /* GAME_H_ */
