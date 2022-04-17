/*
 * Settings.cpp
 *
 *  Created on: Apr 6, 2022
 *      Author: healt
 */

#include "Settings.h"

// TODO [x] fix memory leak here


void Settings::SaveAudioCFG()
{
	// open config file
	std::stringstream tempss;

	// set default directory
	tempss << defDir;

	// set filename
	tempss << "audio.cfg";
	std::ofstream fileSettings( tempss.str().c_str() );
	if (fileSettings.is_open()) {
		fileSettings << getMasterVolume() << " " << getMsicVolume() << " " << getSfxVolume();
	}
	fileSettings.close();
}

void Settings::SaveVideoCFG()
{
	// open config file
	std::stringstream tempss;

	// set default directory
	tempss << defDir;

	// set filename
	tempss << "video.cfg";
	std::ofstream fileSettings( tempss.str().c_str() );
	if (fileSettings.is_open()) {
		fileSettings <<  getResolutionValue() << " " << getAntiAliasValue() << " " << getVsyncValue()  << " " << getFullscreenValue();
	}
	fileSettings.close();
}

void Settings::LoadAudioCFG(){
	// open config file
	std::stringstream tempss;

	// set default directory
	tempss << defDir;

	// set filename
	tempss << "audio.cfg";
	std::fstream fileSettings( tempss.str().c_str() );

	int tempMasterVol = -1;
	int tempMusicVol = -2;
	int tempSfxVol = -3;

	// File opened
	if (fileSettings.is_open()) {
		fileSettings >>  tempMasterVol >> tempMusicVol >> tempSfxVol;
		//std::cout << "Loaded audio.cfg" << std::endl;

		// Set audio values from loaded file
		setVolumes(tempMasterVol, tempMusicVol, tempSfxVol);
	}

	// Unable to open file, create new file
	else{
		//std::cout << "Error opening audio.cfg." << std::endl;
		//std::cout << "\tCreating default audio.cfg..." << std::endl;
		std::ofstream newConfigFile("cfg/audio.cfg");
		if (newConfigFile.is_open()) {
			//std::cout << "\t\tSuccessfully created audio.cfg." << std::endl;
			newConfigFile << "100 128 128";
		}

		// Opening and creating default config file failed, set Settings manually
		else{
			//std::cout << "\t\tError creating default audio.cfg." << std::endl;
			//std::cout << "\t\t\tManually setting default audio.cfg settings." << std::endl;
			setVolumes(100, 128, 128);
		}
		newConfigFile.close();
	}
	fileSettings.close();
}

void Settings::LoadVideoCFG(){
	// open config file
	std::stringstream tempss;

	// set default directory
	tempss << defDir;

	// set filename
	tempss << "video.cfg";
	std::fstream fileSettings( tempss.str().c_str() );

	int tempReso = -4;
	int tempAnti = -5;
	int tempVsync = -6;
	int tempFull = -7;

	if (fileSettings.is_open())
	{
		fileSettings >>  tempReso >> tempAnti >> tempVsync >> tempFull;
		//std::cout << "Loaded video.cfg" << std::endl;

		// Set video values from loaded file
		setVideo(tempReso, tempAnti, tempVsync, tempFull);
	}

	else {
		//std::cout << "Error opening video.cfg." << std::endl;
		//std::cout << "\tCreating default video.cfg..." << std::endl;
		std::ofstream newConfigFile("cfg/video.cfg");
		if (newConfigFile.is_open()) {
			//std::cout << "\t\tSuccessfully created video.cfg." << std::endl;
			newConfigFile << "0 0 0 0";
		}
		// Opening and creating default config file failed, set Settings manually
		else{
			//std::cout << "\t\tError creating default video.cfg." << std::endl;
			//std::cout << "\t\t\tManually setting default video.cfg settings." << std::endl;
			setVideo(0, 0, 0, 0);
		}
		newConfigFile.close();
	}
	fileSettings.close();
}

void Settings::LoadAudio()
{
	// Players.cpp
	sAmbientMusic 		= Mix_LoadMUS("sounds/music.mp3");
	sCast 				= Mix_LoadWAV("sounds/bfxr/snd_cast.wav");
	sSlash 				= Mix_LoadWAV("sounds/cmesias/snd_slash.wav");
	sDownStab			= Mix_LoadWAV("sounds/bfxr/snd_downStab.wav");
	sParry				= Mix_LoadWAV("sounds/bfxr/snd_parry.wav");
	sParrySuccess		= Mix_LoadWAV("sounds/bfxr/snd_parrySuccess.wav");
	sStep				= Mix_LoadWAV("sounds/cmesias/snd_step.wav");
	sDash				= Mix_LoadWAV("sounds/cmesias/snd_dash.wav");
	sSlashTilec			= Mix_LoadWAV("sounds/bfxr/snd_slashTilec.wav");
	sDownStabHitTilec	= Mix_LoadWAV("sounds/bfxr/snd_downStabHitTilec.wav");
	sSlashHitBoss		= Mix_LoadWAV("sounds/bfxr/snd_slashHitBoss.wav");
	sCastHitBoss		= Mix_LoadWAV("sounds/bfxr/snd_castHitBoss.wav");
	sPotBreak			= Mix_LoadWAV("sounds/snd_potBreak.wav");
	sValidation			= Mix_LoadWAV("sounds/zelda_pack/snd_validation.wav");
	sGrunts[0]			= Mix_LoadWAV("sounds/cmesias/snd_grunt1.wav");
	sGrunts[1]			= Mix_LoadWAV("sounds/cmesias/snd_grunt2.wav");
	sGrunts[2]			= Mix_LoadWAV("sounds/cmesias/snd_grunt3.wav");
	sGrunts[3]			= Mix_LoadWAV("sounds/cmesias/snd_grunt4.wav");
	sGrunts[4]			= Mix_LoadWAV("sounds/cmesias/snd_grunt5.wav");
}

void Settings::FreeAudio()
{
    // Players.cpp
	Mix_FreeMusic(sAmbientMusic);
	Mix_FreeChunk(sCast);
	Mix_FreeChunk(sSlash);
	Mix_FreeChunk(sDownStab);
	Mix_FreeChunk(sParry);
	Mix_FreeChunk(sParrySuccess);
	Mix_FreeChunk(sStep);
	Mix_FreeChunk(sDash);
	Mix_FreeChunk(sSlashTilec);
	Mix_FreeChunk(sDownStabHitTilec);
	Mix_FreeChunk(sSlashHitBoss);
	Mix_FreeChunk(sCastHitBoss);
	Mix_FreeChunk(sPotBreak);
	Mix_FreeChunk(sValidation);
	sAmbientMusic 		= NULL;
	sCast 				= NULL;
	sSlash 				= NULL;
	sDownStab			= NULL;
	sParry				= NULL;
	sParrySuccess		= NULL;
	sStep				= NULL;
	sDash				= NULL;
	sSlashTilec			= NULL;
	sDownStabHitTilec	= NULL;
	sSlashHitBoss		= NULL;
	sCastHitBoss		= NULL;
	sPotBreak			= NULL;
	sValidation			= NULL;

	for (int i=0; i<5; i++) {
		Mix_FreeChunk(sGrunts[i]);
		sGrunts[i]		= NULL;
	}
}

//Get's input from user and returns it
void Settings::start(LWindow &gWindow, SDL_Renderer *gRenderer)
{
	// Clear everytime we come back
	vButtons.clear();

	// Other classes fonts
	textNFont.LoadFonts();
	pauseLoop = true;
	gCursor.loadFromFile(gRenderer, "resource/gfx/cursor.png");

	// Load audio files (we need to load them here or else we cannot apply the audio CFG to them
	LoadAudio();

	// Load audio settings everytime we come back
	LoadAudioCFG();
	LoadVideoCFG();
	ApplyAudioCfgToSFX();

	// Initialize buttons
	audioBtn.Init("Audio", "Index1", 70, 10);
	videoBtn.Init("Video", "Index2", 70, 10);
	backBtn.Init("Back", "Back", 70, 10);
	menuBtn.Init("Exit to Menu", "Menu", 70*2+50, 10);
	exitBtn.Init("Exit to Desktop", "Exit", 70*2+50, 10);

	masterVolBtn.Init("Main volume", "MasterVolume", 300, 10);
	musicVolBtn.Init("Music volume", "MusicVolume", 300, 10);
	sfxVolBtn.Init("SFX volume", "SFXVolume", 300, 10);

	resolutionBtn.Init("Resolution", "Resolution", 250, 10);
	antiAliasBtn.Init("Anti Aliasing", "AntiAliasing", 250, 10);
	vsyncBtn.Init("Vsync", "Vsync", 250, 10);
	fullscrenBtn.Init("Fullscreen", "Fullscreen", 250, 10);

	applyBtn.Init("Apply", "Apply", 70, 10);
	keepBtn.Init("Keep", "Keep", 70, 10);
	revertBtn.Init("Revert", "Revert", 70, 10);

	// Store buttons in a vector
	vButtons.push_back(&audioBtn);
	vButtons.push_back(&videoBtn);
	vButtons.push_back(&backBtn);
	vButtons.push_back(&menuBtn);
	vButtons.push_back(&exitBtn);

	vButtons.push_back(&masterVolBtn);
	vButtons.push_back(&musicVolBtn);
	vButtons.push_back(&sfxVolBtn);

	vButtons.push_back(&resolutionBtn);
	vButtons.push_back(&antiAliasBtn);
	vButtons.push_back(&vsyncBtn);
	vButtons.push_back(&fullscrenBtn);

	vButtons.push_back(&applyBtn);
	vButtons.push_back(&keepBtn);
	vButtons.push_back(&revertBtn);

	// Load buttons
	for (unsigned int i=0; i<vButtons.size(); i++) {
		vButtons[i]->Load();
	}

	// Used globally
	buttonsSize = vButtons.size();

	while (pauseLoop) {

		// Start FPS cap
		helper.fps.start();

		// Handle events
		while (SDL_PollEvent(&e) != 0) {

			//Handle window events
			gWindow.handleEvent(gRenderer, e);

			if (e.type == SDL_QUIT) {
				settingsResult	= Exit;
				pauseLoop 	= false;
				FreeAudio();
				textNFont.FreeFonts();
				gCursor.free();
			} else {

				if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
					switch (e.key.keysym.sym) {
					case SDLK_ESCAPE:
						//settingsResult	= Back;
						// TODO [ ] this crashers program, but we need to call this to fix memory leak
						pauseLoop = false;
						break;
					case SDLK_DOWN:
						//
						break;
					}
				}

				//If Key Released
				else if (e.type == SDL_KEYUP && e.key.repeat == 0) {
					switch (e.key.keysym.sym) {
					case SDLK_UP:
						//
						break;
					case SDLK_DOWN:
						//
						break;
					}
				}

				// Mouse Pressed
				mousePressed(e);

				// Mouse Released
				settingsResult = mouseReleased(gWindow, e);

				// Results
				switch (settingsResult)  {
					case Back:				// Back (to Main Menu)
						//settingsResult	= Back;
						pauseLoop 	= false;
						//FreeAudio();
						//textNFont.FreeFonts();
						///gCursor.free();
						break;
					case ShowingMenu:
						settingsResult	= ShowingMenu;
						pauseLoop 	= false;
						FreeAudio();
						gCursor.free();
						textNFont.FreeFonts();

						// Load buttons
						for (unsigned int i=0; i<vButtons.size(); i++) {
							vButtons[i]->Free();
						}
						// Clear everytime we come back
						vButtons.clear();
						return;
					case Exit:
						settingsResult	= Exit;
						pauseLoop 	= false;
						FreeAudio();
						gCursor.free();
						textNFont.FreeFonts();

						// Load buttons
						for (unsigned int i=0; i<vButtons.size(); i++) {
							vButtons[i]->Free();
						}
						// Clear everytime we come back
						vButtons.clear();
						return;
				}
			}
		}

		// Get mouse coordinates
		SDL_GetMouseState(&mx, &my);

		// New mouse coordinates, no relation to camera
		mex = (helper.screenWidth*mx)/gWindow.getWidth();
		mey = (helper.screenHeight*my)/gWindow.getHeight();

		// Default positions for buttons that are always in view
		int xPos = helper.screenWidth  * 0.02;
		int yPos = helper.screenHeight * 0.18;

		// Always in view: Audio, Video, Back, Menu and Exit buttons
		for (unsigned int i=0; i<staticMax; i++) {
			int exitYOffet = 0;
			if (i==3) {
				exitYOffet = 2;
			}
			if (i==4) {
				exitYOffet = 2;
			}
			vButtons[i]->Update(mex, mey, xPos, yPos + (i+exitYOffet) * 45,
								leftClick);
		}

		// Always in view: Apply button
		//for (unsigned int i=11; i<=3; i++) {
		//	vButtons[11]->Update(mex, mey, xPos, yPos + i * 45,
		//						leftClick);
		//}

		// Default positions for modifying buttons
		xPos = helper.screenWidth  * 0.25;

		// View Audio buttons
		if (settingsIndex == 0) {

			// Set location of Audio sliders in view
			for (unsigned int i=startIndexVol; i<=stopIndexVol; i++) {
				vButtons[i]->Update(mex, mey, xPos, yPos + (i-startIndexVol) * btnSpacingH, leftClick);
			}

			// Set location of Video sliders OUT of view
			for (unsigned int i=startIndexVid; i<=stopIndexVid; i++) {
				vButtons[i]->Update(mex, mey, -2000, -2000, leftClick);
			}

			// Set location of Apply button
			int anchorIndex = stopIndexVol;
			vButtons[12]->Update(mex, mey, xPos, vButtons[anchorIndex]->y + vButtons[anchorIndex]->h+applyButtonYOffset,
								leftClick);

			// Set location of Keep & Revert btns in view
			if (promptToKeep) {

				// Set location of Keep button
				anchorIndex = 12;
				float x = vButtons[anchorIndex]->x + vButtons[anchorIndex]->w+keepXOffset;
				float y = vButtons[anchorIndex]->y;
				vButtons[13]->Update(mex, mey, x, y, leftClick);

				// Set location of Revert button
				anchorIndex = 13;
				x = vButtons[anchorIndex]->x + vButtons[anchorIndex]->w+revertXOffset;
				y = vButtons[anchorIndex]->y;
				vButtons[14]->Update(mex, mey, x, y, leftClick);
			}

			// Hide keep & Revert btns
			else {
				vButtons[13]->Update(mex, mey, -2000, -2000, leftClick);
				vButtons[14]->Update(mex, mey, -2000, -2000, leftClick);
			}
		}

		// View Video buttons
		else {

			// Video buttons in view
			for (unsigned int i=startIndexVid; i<=stopIndexVid; i++) {
				vButtons[i]->Update(mex, mey, xPos, yPos + (i-startIndexVid)* btnSpacingH, leftClick);
			}

			// Audio buttons OUT of view
			for (unsigned int i=startIndexVol; i<=stopIndexVol; i++) {
				vButtons[i]->Update(mex, mey, -2000, -2000, leftClick);
			}

			// Set location of Apply button
			int anchorIndex = 11;
			vButtons[12]->Update(mex, mey, xPos, vButtons[anchorIndex]->y + vButtons[anchorIndex]->h+applyButtonYOffset,
								leftClick);

			// Set location of Keep & Revert btns in view
			if (promptToKeep) {

				// Set location of Keep button
				anchorIndex = 12;
				float x = vButtons[anchorIndex]->x + vButtons[anchorIndex]->w+keepXOffset;
				float y = vButtons[anchorIndex]->y;
				vButtons[13]->Update(mex, mey, x, y, leftClick);

				// Set location of Revert button
				anchorIndex = 13;
				x = vButtons[anchorIndex]->x + vButtons[anchorIndex]->w+revertXOffset;
				y = vButtons[anchorIndex]->y;
				vButtons[14]->Update(mex, mey, x, y, leftClick);
			}

			// Hide keep & Revert btns
			else {
				vButtons[13]->Update(mex, mey, -2000, -2000, leftClick);
				vButtons[14]->Update(mex, mey, -2000, -2000, leftClick);
			}
		}


		// Change options based based on mouse position
		{
			if (leftClick) {

				// Change volume buttons
				for (unsigned int i=startIndexVol; i<=stopIndexVol; i++)
				{
					if (vButtons[i]->pressed) {
						int barMaxValue;
						if (i == startIndexVol)
							barMaxValue = 100;
						else
							barMaxValue = 128;


						option[i-startIndexVol] = barMaxValue*(mex - (vButtons[i]->x-20))/(vButtons[i]->w+40);
					}
				}
			}
		}

		// Prompt timer
		if (promptToKeep) {
			promptTimer -= 1;

			// Time out from user, revert to old settings
			if (promptTimer <= 0) {
				promptToKeep = false;

				// Load old settings
				LoadAudioCFG();

				// Load old video settings
				LoadVideoCFG();

				// Apply old video settings to Window
				gWindow.applySettings(getResolutionValue(), getAntiAliasValue(), getVsyncValue(), getFullscreenValue());
			}
		}

		// Clear screen
		SDL_SetRenderDrawColor(gRenderer, 20, 50, 70, 255);
		SDL_RenderClear(gRenderer);

		// Render everything
		{
			// Render Show Audio btn
			// Render Show Video btn
			// Render Back btn
			// Render Menu btn
			// Render Exit btn
			for (unsigned int i=0; i<staticMax; i++)
			{
				vButtons[i]->Render(gRenderer);
			}

			// Render Audio buttons
			for (unsigned int i=startIndexVol; i<=stopIndexVol; i++)
			{
				// Render BG for sliders
				{
					int barVlue = option[i-startIndexVol];
					int barPixelWidth = vButtons[i]->w+40;
					int barMaxValue;
					if (i == startIndexVol)
						barMaxValue = 100;
					else
						barMaxValue = 128;
					int width = barPixelWidth*(barVlue)/barMaxValue;
					helper.RenderFillRect(gRenderer, vButtons[i]->x-20, vButtons[i]->y-10, width, vButtons[i]->h+20, {13,18,40});
				}

				// Render buttons
				vButtons[i]->Render(gRenderer, btnTextXOffset, btnTextYOffset);

				// Render value for buttons
				textNFont.RenderText(gRenderer, vButtons[i]->x+vButtons[i]->w, vButtons[i]->y, getValue(i-startIndexVol), 255, {255,255,255}, "left");
			}

			// Render Video buttons
			for (unsigned int i=startIndexVid; i<=stopIndexVid; i++)
			{
				// Used for all video bars
				int barPixelWidth = vButtons[i]->w+40;

				// If index on everything else EXCEPT resolution bar
				if (i != startIndexVid) {
					// Render BG for sliders
					{
						int barVlue = option[i-startIndexVol]+1;
						int barMaxValue = 2;
						int width = barPixelWidth*(barVlue)/barMaxValue+1;

						// Render rect on the left side of slider (On)
						if (barVlue == 1) {
							helper.RenderFillRect(gRenderer, vButtons[i]->x-20, vButtons[i]->y-10, width, vButtons[i]->h+20, {13,18,40});
						}

						// Render rect on the left side of slider (Off)
						else {
							helper.RenderFillRect(gRenderer, vButtons[i]->x+(width/2)-20, vButtons[i]->y-10, width/2, vButtons[i]->h+20, {13,18,40});
						}
					}

					// Render buttons
					vButtons[i]->Render(gRenderer, btnTextXOffset, btnTextYOffset);

					// Render value for buttons
					std::string name = "On";
					if (getValue(i-staticMax) == 1)
						name = "On";
					else
						name = "Off";

					// Render Text
					textNFont.RenderText(gRenderer, vButtons[i]->x+vButtons[i]->w, vButtons[i]->y, name.c_str(), 255, {255,255,255}, "left");
				}

				// If index on Resolution bar
				else {
					// Render BG for sliders
					{
						int barVlue = option[i-startIndexVol];
						int barMaxValue;
						barMaxValue = 5;
						int width = barPixelWidth*(barVlue)/barMaxValue;
						helper.RenderFillRect(gRenderer, vButtons[i]->x-20, vButtons[i]->y-10, width, vButtons[i]->h+20, {13,18,40});
					}

					// Rebder buttons
					vButtons[i]->Render(gRenderer, btnTextXOffset, btnTextYOffset);

					// Render value for buttons
					std::string resolutionValue;
					if (getValue(i-startIndexVol)==1){
						resolutionValue	= "1024x576";
					}else if (getValue(i-startIndexVol)==2){
						resolutionValue	= "1280x720";
					}else if (getValue(i-startIndexVol)==3){
						resolutionValue	= "1336x768";
					}else if (getValue(i-startIndexVol)==4){
						resolutionValue	= "1600x900";
					}else if (getValue(i-startIndexVol)==5){
						resolutionValue	= "1920x1080";
					}
					textNFont.RenderText(gRenderer, vButtons[i]->x+vButtons[i]->w, vButtons[i]->y, resolutionValue.c_str(), 255, {255,255,255}, "left");
				}
			}

			// Render Apply btn
			vButtons[12]->Render(gRenderer);

			// Render Keep btn
			vButtons[13]->Render(gRenderer);

			// Render Revert btn
			vButtons[14]->Render(gRenderer);

			// Render text to prompt user
			if (promptToKeep) {
				std::stringstream tempss;
				tempss << "Reverting settings in " << promptTimer/60 << ".";
				textNFont.RenderText(gRenderer, vButtons[12]->x, vButtons[12]->y+vButtons[12]->gText.getHeight()+20, tempss.str().c_str(), 255, {255,255,255}, "right");
			}

				std::stringstream tempss;
				tempss << "vButtons.size: " << vButtons.size();
				textNFont.RenderText(gRenderer, 0, 0, tempss.str().c_str(), 255, {255,255,255}, "right");


			// Render mouse location
			gCursor.render(gRenderer, mex, mey, 20, 20);
		}

		// Update screen
		SDL_RenderPresent(gRenderer);

		// fps
		helper.frame++;
		if((helper.cap == true ) && (helper.fps.get_ticks() < 1000 / helper.FRAMES_PER_SECOND ))
			SDL_Delay((1000 / helper.FRAMES_PER_SECOND ) - helper.fps.get_ticks());

	}
	FreeAudio();
	gCursor.free();
	textNFont.FreeFonts();

	// Load buttons
	for (unsigned int i=0; i<vButtons.size(); i++) {
		vButtons[i]->Free();
	}
	// Clear everytime we come back
	vButtons.clear();
}



// Mouse Pressed
void Settings::mousePressed(SDL_Event event)
{
	// If mouse button down
	if (event.type == SDL_MOUSEBUTTONDOWN) {
		if (event.button.button == SDL_BUTTON_LEFT) {
			leftClick = true;
			/*
			*/
		}
		if (event.button.button == SDL_BUTTON_RIGHT) {
			//
		}
	}
}

// Mouse Released
Settings::SettingsResult Settings::mouseReleased(LWindow &gWindow, SDL_Event event){
	Settings::SettingsResult result = Nothing;

	if (event.type == SDL_MOUSEBUTTONUP)
	{
		// If Left mouse button
		if (event.button.button == SDL_BUTTON_LEFT)
		{
			leftClick = false;

			// Change to view Audio or Video Settings, or Back or Exit
			for (unsigned int i=0; i<buttonsSize; i++)
			{
				if (vButtons[i]->CheckMouseReleased(mex, mey) == "Index1")
				{
					settingsIndex = 0;
				}
				if (vButtons[i]->CheckMouseReleased(mex, mey) == "Index2")
				{
					settingsIndex = 1;
				}
				if (vButtons[i]->CheckMouseReleased(mex, mey) == "Back")
				{
					result = Back;
				}
				if (vButtons[i]->CheckMouseReleased(mex, mey) == "Menu")
				{
					result	= ShowingMenu;
				}
				if (vButtons[i]->CheckMouseReleased(mex, mey) == "Exit")
				{
					result = Exit;
				}
				if (vButtons[i]->CheckMouseReleased(mex, mey) == "Apply")
				{
					// Stop prompt
					promptToKeep = true;
					promptTimer = 60*11;

					// Apply audio cfg to audio files
					ApplyAudioCfgToSFX();

					// Play a SFX so the user knows how loud it is
					Mix_PlayChannel(-1, sCast, 0);

					// Apply new video settings to Window
					gWindow.applySettings(getResolutionValue(), getAntiAliasValue(), getVsyncValue(), getFullscreenValue());
				}
				if (vButtons[i]->CheckMouseReleased(mex, mey) == "Keep")
				{
					// Stop prompt
					promptToKeep = false;
					promptTimer = 60*11;

					// Save new settings
					SaveAudioCFG();
					SaveVideoCFG();
				}
				if (vButtons[i]->CheckMouseReleased(mex, mey) == "Revert")
				{
					// Stop prompt
					promptToKeep = false;
					promptTimer = 60*11;

					// Load previous settings
					LoadAudioCFG();
					LoadVideoCFG();

					// Apply old video settings to Window
					gWindow.applySettings(getResolutionValue(), getAntiAliasValue(), getVsyncValue(), getFullscreenValue());
				}
			}


			// Change video buttons
			{
				for (unsigned int i=startIndexVid; i<=stopIndexVid; i++)
				{
					if (vButtons[i]->pressed) {

						// If resolution bar, there will be 5 chocies
						if (i == startIndexVid) {
							int barMaxValue;
							barMaxValue = 5;
							option[i-startIndexVol] = (barMaxValue*(mex - (vButtons[i]->x-20))/(vButtons[i]->w+40)) + 1;
						}

						// Everything else has only 2 choices: "Off" or "On"
						else {
							int barMaxValue = 2;

							// Set to off
							if (helper.checkCollision(mex, mey, 1, 1, vButtons[i]->x, vButtons[i]->y, vButtons[i]->w/2, vButtons[i]->h)) {
								option[i-startIndexVol] = barMaxValue*(mex - (vButtons[i]->x-20))/(vButtons[i]->w+40);
							}
							// Set to on
							else {
								option[i-startIndexVol] = barMaxValue*(mex - (vButtons[i]->x-20))/(vButtons[i]->w+40);
							}
						}
					}
				}
			}
		}
		// If Right mouse button
		if (event.button.button == SDL_BUTTON_RIGHT)
		{
		}
	}
	return result;
}

void Settings::ApplyAudioCfgToSFX() {
	//Mix_VolumeMusic(MUSIC_VOL*(MASTER_VOL*0.01));
	//Mix_VolumeChunk(sRockBreak, SFX_VOL);
	//Mix_VolumeMusic(getMasterVolume());
	Mix_VolumeChunk(sCast				, getSfxVolume());
	Mix_VolumeChunk(sSlash				, getSfxVolume());
	Mix_VolumeChunk(sDownStab			, getSfxVolume());
	Mix_VolumeChunk(sParry				, getSfxVolume());
	Mix_VolumeChunk(sParrySuccess		, getSfxVolume());
	Mix_VolumeChunk(sStep				, getSfxVolume());
	Mix_VolumeChunk(sDash				, getSfxVolume());
	Mix_VolumeChunk(sSlashTilec			, getSfxVolume());
	Mix_VolumeChunk(sDownStabHitTilec	, getSfxVolume());
	Mix_VolumeChunk(sSlashHitBoss		, getSfxVolume());
	Mix_VolumeChunk(sCastHitBoss		, getSfxVolume());
	Mix_VolumeChunk(sPotBreak			, getSfxVolume());
	Mix_VolumeChunk(sValidation			, getSfxVolume());

	for (int i=0; i<5; i++) {
		Mix_VolumeChunk(sGrunts[i]			, getSfxVolume());
	}
}

void Settings::ApplyAudioCfgToSFX(Settings &settings) {
	//Mix_VolumeMusic(MUSIC_VOL*(MASTER_VOL*0.01));
	//Mix_VolumeChunk(sRockBreak, SFX_VOL);
	//Mix_VolumeMusic(getMasterVolume());
	Mix_VolumeChunk(settings.sCast				, getSfxVolume());
	Mix_VolumeChunk(settings.sSlash				, getSfxVolume());
	Mix_VolumeChunk(settings.sDownStab			, getSfxVolume());
	Mix_VolumeChunk(settings.sParry				, getSfxVolume());
	Mix_VolumeChunk(settings.sParrySuccess		, getSfxVolume());
	Mix_VolumeChunk(settings.sStep				, getSfxVolume());
	Mix_VolumeChunk(settings.sDash				, getSfxVolume());
	Mix_VolumeChunk(settings.sSlashTilec		, getSfxVolume());
	Mix_VolumeChunk(settings.sDownStabHitTilec	, getSfxVolume());
	Mix_VolumeChunk(settings.sSlashHitBoss		, getSfxVolume());
	Mix_VolumeChunk(settings.sCastHitBoss		, getSfxVolume());
	Mix_VolumeChunk(settings.sPotBreak			, getSfxVolume());
	Mix_VolumeChunk(settings.sValidation		, getSfxVolume());

	for (int i=0; i<5; i++) {
		Mix_VolumeChunk(settings.sGrunts[i]		, getSfxVolume());
	}
}

/*


void Options::applyCustomAudioCFG(float MUSIC_VOL, float SFX_VOL) {
	//Mix_VolumeMusic(MUSIC_VOL*(MASTER_VOL*0.01));
	//Mix_VolumeChunk(sRockBreak, SFX_VOL*(MASTER_VOL*0.01));
	//Mix_VolumeChunk(sRockBreak, SFX_VOL);



}

void Options::applyOldAudioCFG() {
	//Mix_VolumeMusic(MUSIC_VOL);
	//Mix_VolumeChunk(sRockBreak, SFX_VOL);
}

void Options::applyMasterAudioCFG() {
	//Mix_VolumeMusic(MUSIC_VOL*(MASTER_VOL*0.01));
	//Mix_VolumeChunk(sRockBreak, SFX_VOL*(MASTER_VOL*0.01));
}




void Options::minusValues() {
	if (focusedOther) {
		if (indexOther == 0) {
			bar[indexOther].value -= 10;
		}
		else if (indexOther == 1) {
			bar[indexOther].value -= 8;
		}
		else if (indexOther == 2) {
			bar[indexOther].value -= 8;
		}
		else if (indexOther == 3) {
			if (bar[indexOther].value > 1) {
				bar[indexOther].value -= 1;
			}
		}
		else if (indexOther == 4) {
			bar[indexOther].value -= 1;
		}
		else if (indexOther == 5) {
			bar[indexOther].value -= 1;
		}
		else if (indexOther == 6) {
			bar[indexOther].value -= 1;
		}
		// fix going over
		if (bar[indexOther].value < 0) {
			bar[indexOther].value = 0;
		}
	}
}

void Options::addValues() {
	if (focusedOther) {
		if (indexOther == 0) {
			bar[indexOther].value += 10;
			if (bar[indexOther].value > 128) { bar[indexOther].value = 128; }
		}
		else if (indexOther == 1) {
			bar[indexOther].value += 8;
			if (bar[indexOther].value > 128) { bar[indexOther].value = 128; }
		}
		else if (indexOther == 2) {
			bar[indexOther].value += 8;
			if (bar[indexOther].value > 128) { bar[indexOther].value = 128; }
		}
		else if (indexOther == 3) {
			bar[indexOther].value += 1;
			if (bar[indexOther].value > 5) { bar[indexOther].value = 5; }
		}
		else if (indexOther == 4) {
			bar[indexOther].value += 1;
			if (bar[indexOther].value > 1) { bar[indexOther].value = 1; }
		}
		else if (indexOther == 5) {
			bar[indexOther].value += 1;
			if (bar[indexOther].value > 1) { bar[indexOther].value = 1; }
		}
		else if (indexOther == 6) {
			bar[indexOther].value += 1;
			if (bar[indexOther].value > 1) { bar[indexOther].value = 1; }
		}
	}
}

void Options::actionApplyAudio() {
	// Apply new Bar Settings and new SFX Bar Settings
	applyCustomAudioCFG(bar[1].value*(bar[0].value*0.01),
					    bar[2].value*(bar[0].value*0.01));

	// player preview of new settings
	Mix_PlayChannel(-1, sRockBreak, 0);

	// Display confirm prompt
	confirm			= true;
	timer 			= revertSettingsTimer;
	button = "a";
}



 */
