/*
 * Settings.h
 *
 *  Created on: Apr 6, 2022
 *      Author: healt
 */

#ifndef SETTINGS_H_
#define SETTINGS_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include "Helper.h"
#include "LTexture.h"
#include "LWindow.h"

#include "Game/TextNFont.h"
#include "Game/Button.h"
#include "Shortcuts.h"


class Settings: public Shortcuts {

public:
	Helper helper;
	TextNFont textNFont;

	enum SettingsResult { Back, Exit, Nothing, StartGame, ShowingMenu };
	SettingsResult settingsResult = Nothing;

	void mousePressed(SDL_Event event);
	Settings::SettingsResult mouseReleased(LWindow &gWindow, SDL_Event event);

public:	// Exclusive for this game
	// Players.cpp
	Mix_Music *sAmbientMusic 		= NULL;
	Mix_Chunk *sCast 				= NULL;
	Mix_Chunk *sSlash 				= NULL;
	Mix_Chunk *sDownStab			= NULL;
	Mix_Chunk *sParry				= NULL;
	Mix_Chunk *sParrySuccess		= NULL;
	Mix_Chunk *sStep				= NULL;
	Mix_Chunk *sDash				= NULL;
	Mix_Chunk *sSlashTilec			= NULL;
	Mix_Chunk *sDownStabHitTilec	= NULL;
	Mix_Chunk *sSlashHitBoss		= NULL;
	Mix_Chunk *sCastHitBoss			= NULL;

	Mix_Chunk *sPotBreak			= NULL;
	Mix_Chunk *sValidation			= NULL;

	Mix_Chunk *sGrunts[5];

	void LoadAudio();
	void FreeAudio();

private:

	SDL_Event e;

public:	// Menu

	LTexture gCursor;
	int mx, my, mex, mey;
	bool leftClick;
	bool pauseLoop;

	/* 0: Audio Settings
	 */
	int settingsIndex = 0;

	// We'll use this to store all the buttons
	/*
	 * 	no Index
	 * 		0: Show Audio button
	 * 		1: Show Video button
	 * 		2: back button
	 * 		3: menu button
	 * 		4: exit button
	 *
	 * 	0 Index
	 * 		5: Master Volume
	 * 		6: Music Volume
	 * 	    7: SFX Volume
	 *
	 * 	1 Index
	 * 		8: Resolution
	 * 		9: Anti Aliasing
	 * 		10: VSYNC
	 * 		11: Fullscreen
	 *
	 * 	2 Index
	 * 		12: Apply
	 * 		13: Keep
	 * 		14: Revert
	 */
	std::vector<Button*> vButtons;
	unsigned int buttonsSize;
	int btnSpacingH = 80;
	int btnTextXOffset = -10;
	int btnTextYOffset = -40;
	int applyButtonYOffset = 40;
	int keepXOffset = 50;
	int revertXOffset = 40;
	bool promptToKeep = false;
	int promptTimer = 0;

	Button audioBtn;		unsigned int staticMax = 5;
	Button videoBtn;
	Button backBtn;
	Button menuBtn;
	Button exitBtn;

	Button masterVolBtn;	unsigned int startIndexVol = 5;
	Button musicVolBtn;		unsigned int stopIndexVol = 7;
	Button sfxVolBtn;		int valueOffset = 290;

	Button resolutionBtn;	unsigned int startIndexVid = 8;
	Button antiAliasBtn;	unsigned int stopIndexVid = 11;
	Button vsyncBtn;
	Button fullscrenBtn;

	Button applyBtn;
	Button keepBtn;
	Button revertBtn;

	// Save/Load CFG

	// Default directory for audio and video config files
	const std::string defDir = "data/cfg/";

	// Load audio/video menu
	void start(LWindow &gWindow, SDL_Renderer *gRenderer);

	void SaveAudioCFG();

	void LoadAudioCFG();

	void SaveVideoCFG();

	void LoadVideoCFG();

	// TODO [ ] - finish asdding sliders for each button for the user to change the values

	/*
	 *	These values can be changed
	 *  0 masterVolume;
	 *  1 musicVolume;
	 *  2 sfxVolume;
	 *  3 RESOLUTION;
	 *  4 ANTI_ALIAS;
	 *  5 VSYNC;
	 *  6 FULLSCREEN;
	 */
	int option[7];

	int getValue(int findIndex) {
		return option[findIndex];
	}

	int getMasterVolume() {
		return option[0];
	}

	int getMsicVolume() {
		return option[1];
	}

	int getSfxVolume() {
		return option[2];
	}

	int getResolutionValue() {
		return option[3];
	}

	int getAntiAliasValue() {
		return option[4];
	}

	int getVsyncValue() {
		return option[5];
	}

	int getFullscreenValue() {
		return option[6];
	}

	void setVolumes(int masterVolume, int musicVolume, int sfxVolume) {
		option[0] = masterVolume;
		option[1] = musicVolume;
		option[2] = sfxVolume;
	}

	void setVideo(int resolution, int antiAlias, int vsync, int fullscreen) {
		option[3] = resolution;
		option[4] = antiAlias;
		option[5] = vsync;
		option[6] = fullscreen;
	}

public:
	void ApplyAudioCfgToSFX();

public:
	void ApplyAudioCfgToSFX(Settings &settings);

};

#endif /* SETTINGS_H_ */
