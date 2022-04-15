/*
 * ActSelection.cpp
 *
 *  Created on: May 15, 2017
 *      Author: Carl
 */

#include "ActSelection.h"

void ActSelection::Init()
{
	debug = false;
	LevelsCompleted = 1;	// How many levels player has unlocked
	levelIndex = 1;			// Current index for level
	selectedLevel = -1;		// Current index for level selected

	// Open saved file
	LoadLevelsUnlockedFile();

	// levelBoxes
	for (int i=0; i < levelMax; i++) {
		levelbox[i].x = -5000;
		levelbox[i].y = -5000;
		//levelbox[i].w = 420;
		//levelbox[i].h = 260;
		levelbox[i].w = screenWidth * 0.10;
		levelbox[i].h = screenHeight * 0.05;
		levelbox[i].hover = false;
		levelbox[i].selected = false;
	}

	// Center camera
	camx = 0;
	camy = 0;

	// Other classes
	StartGameBtn.Init("Start Game", "StartGame");
	EraseDataBtn.Init("Erase Data", "EraseData");
	BackBtn.Init("Back", "Back");
}

void ActSelection::Load(SDL_Renderer *gRenderer) {

	// Load textures
	gCursor.loadFromFile(gRenderer, "resource/gfx/cursor.png");

	// Load level preview textures
	gLevelPreviews[0].loadFromFile(gRenderer, "resource/gfx/levels/level1.png");
	gLevelPreviews[1].loadFromFile(gRenderer, "resource/gfx/levels/level2.png");
	gLevelPreviews[2].loadFromFile(gRenderer, "resource/gfx/levels/level3.png");
	gLevelPreviews[3].loadFromFile(gRenderer, "resource/gfx/levels/level4.png");
	gLevelPreviews[4].loadFromFile(gRenderer, "resource/gfx/levels/level5.png");
	gLockedLevel.loadFromFile(gRenderer, "resource/gfx/levels/locked-level.png");

	/*for (int i=0; i<5; i++) {
		std::stringstream filePath;
		filePath << "resource/gfx/levels/";
		filePath << "level" + (i+1);
		filePath << ".png";
	}*/

	// Other classes
	StartGameBtn.Load();
	EraseDataBtn.Load();
	BackBtn.Load();
	LoadFonts();
}

void ActSelection::Free() {

	// Free textures
	gCursor.free();

	// Other classes
	StartGameBtn.Free();
	EraseDataBtn.Free();
	BackBtn.Free();
	FreeFonts();
}

void ActSelection::LoadLevelsUnlockedFile() {
	// Open highscore first to check value
	unsigned int PreviousLevelsUnlocked = -1;

	// Open file
	{
		// Set file path
		std::stringstream filePath;
		filePath << "data/LevelsUnlocked.mp";

		// Open file
		std::fstream file(filePath.str().c_str());

		// Part #1
		// We run this code first to see if there is a file that exist
		// If there is no file that exist then we create one, with the default of level 1
		{
			if (!file.is_open()) {
				//std::cout<< "File does not exist for Levels Unlocked, creating new file.\n";
				{
					// Set file path
					std::stringstream filePath;
					filePath << "data/LevelsUnlocked.mp";

					// Create file
					std::ofstream fileSave;

					// Open newly made file
					fileSave.open(filePath.str().c_str());

					// Save to file
					fileSave << LevelsCompleted;

					// Close file
					fileSave.close();
				}
			}
			// Close file
			file.close();
		}

		// Part #2
		// Next we attempt to open the newly created file OR
		// We attempt to load a previously saved file.
		{
			// Open file again
			std::fstream fileAgain(filePath.str().c_str());

			// If file opened
			if (fileAgain.is_open())
			{
				// Store previous levels completed in this variable for checking
				fileAgain >> LevelsCompleted;
			}
			// Close file
			fileAgain.close();
		}
	}
}

void ActSelection::EraseFileData()
{
	// Set file path
	std::stringstream filePath;
	filePath << "data/LevelsUnlocked.mp";

	// Create file
	std::ofstream fileSave;

	// Open newly made file
	fileSave.open(filePath.str().c_str());

	// Save to file
	fileSave << 1;

	// Set default
	LevelsCompleted = 1;

	// Close file
	fileSave.close();
}

void ActSelection::Show(LWindow &gWindow, SDL_Renderer *gRenderer,
						ActSelection::Result &result, int &LevelToLoad) {

	// Upon entry
	quit = false;
	leftClick = false;
	shift = false;

	// Init resources
	Init();

	// Load resources
	Load(gRenderer);

	// While loop
	while (!quit) {

		// Get mouse location
		SDL_GetMouseState(&mx, &my);
		mex = (screenWidth*mx)/gWindow.getWidth();				// New mouse coordinates, no relation to camera
		mey = (screenHeight*my)/gWindow.getHeight();			// New mouse coordinates, no relation to camera

		// Handle Events
		while (SDL_PollEvent(&event)) {

			// Handle window events
			gWindow.handleEvent(gRenderer, event);

			// Back Game
			if (event.type == SDL_QUIT) {
				result = Exit;
				quit = true;
				Free();
				return;
			}else{
				// Key Pressed
				if (event.type == SDL_KEYDOWN && event.key.repeat == 0) {
					// Set controls focus to Keyboard
					this->controls = 0;

					// Handle keyboard keys
					switch (event.key.keysym.sym) {
					case SDLK_LSHIFT:
						shift = true;
						break;
					case SDLK_RSHIFT:
						shift = true;
						break;
					case SDLK_LEFT:
						if (levelIndex > 1) {
							levelIndex--;
						}
						break;
					case SDLK_RIGHT:
						if (levelIndex < 5) {
							levelIndex++;
						}
						break;
					case SDLK_SPACE:
						// Select the current level-preview Texture
						for (int i=0; i<levelMax; i++)
						{
							if (levelIndex-1 == i) {
								levelbox[i].selected = true;
								selectedLevel = i+1;
								LevelToLoad = selectedLevel;
							} else {
								levelbox[i].selected = false;
							}
						}
						break;
					case SDLK_RETURN:
						// Select the current level-preview Texture
						for (int i=0; i<levelMax; i++)
						{
							if (levelIndex-1 == i) {
								levelbox[i].selected = true;
								selectedLevel = i+1;
								LevelToLoad = selectedLevel;
							} else {
								levelbox[i].selected = false;
							}
						}
						break;
					case SDLK_ESCAPE:
						result = Back;
						quit = true;
						Free();
						return;
					}
				}
				// Key Released
				else if (event.type == SDL_KEYUP && event.key.repeat == 0) {
					switch (event.key.keysym.sym) {
					case SDLK_h:
						//debug = (!debug);
						break;
					case SDLK_LSHIFT:
						shift = false;
						break;
					case SDLK_RSHIFT:
						shift = false;
						break;
					}
				}

				// Mouse Pressed
				mousePressed(event);

				// Mouse Released
				result = mouseReleased(event);

				// Customize Character results
				switch (result)  {
					case Back:				// Back (to Main Menu)
						quit = true;
						break;
					case Nothing:			// Nothing

						break;
					case StartGame:			// Start Game
						quit = true;
						break;
					case Exit:			// Start Game
						quit = true;
						Free();
						return;
				}
			}
		}

		// Update Variables

		// Levels boxes
		for (int i=0; i<levelMax; i++)
		{
			// Keyboard controls
			if (this->controls == 0) {

				// Player has i Level-preview Textures selected
				if (levelIndex-1 == i) {
					//levelIndex = i+1;
					levelbox[i].hover = true;
				}
				else {
					levelbox[i].hover = false;
				}
			}

			// Mouse controls
			else if (this->controls == 1) {
				// If mouse is hovering on level selection box
				if (checkCollision(mex, mey, 1, 1, levelbox[i].x, levelbox[i].y, levelbox[i].w, levelbox[i].h))
				{
					levelIndex = i+1;
					levelbox[i].hover = true;
				} else {
					levelbox[i].hover = false;
				}
			}

			// If left mouse click
			if (leftClick) {
				// If mouse is clicking on level selection box
				if (checkCollision(mex, mey, 1, 1, levelbox[i].x, levelbox[i].y, levelbox[i].w, levelbox[i].h))
				{
					levelbox[i].selected = true;
					selectedLevel = i+1;

					// Set level to load to selection
					LevelToLoad = selectedLevel;
				} else {

					// If we dont click on above levelbox then if the selectedLevel is
					// not the level we have selected, set the levelbox selection to off
					if (selectedLevel != i+1) {
						levelbox[i].selected = false;
					}

				}
			}
		}

		// Level-preview Textures
		for (int i=0; i < levelMax; i++) {

			// Static position for Level-preview Textures
			//levelbox[i].y = gWindow.getHeight()/2 - levelbox[i].h/2;

			// Keep Level-preview Textures off of screen
			//levelbox[i].x = -500;

			// Player has i Level-preview Textures selected
			//if (levelIndex-1 == i) {

				// Center Level-preview Texture
				//levelbox[i].x = gWindow.getWidth()/2 - levelbox[i].w/2;
			//}

			// Center all boxes on screen
			float xAnchorPos = screenWidth * 0.05;
			levelbox[i].x = xAnchorPos + (i * widthSpacing);
			levelbox[i].y = 80;
			levelbox[i].w = screenWidth * 0.10;
			levelbox[i].h = screenHeight * 0.09;
		}

		// Set position of button: Start Game Button
		StartGameBtn.Update(mex, mey,
							screenWidth * 0.90 - StartGameBtn.gText.getWidth(),
							screenHeight * 0.90 - StartGameBtn.gText.getHeight(),
							leftClick);

		// Set position of button: Erase Data Button
		EraseDataBtn.Update(mex, mey,
							screenWidth * 0.5 - EraseDataBtn.gText.getWidth()/2,
							screenHeight * 0.90 - EraseDataBtn.gText.getHeight(),
							leftClick);


		// Set position of button: Back Button
		BackBtn.Update(mex, mey,
							screenWidth * 0.10,
							screenHeight * 0.90 - StartGameBtn.gText.getHeight(),
							leftClick);



		// Clear render screen
		SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
		SDL_RenderClear(gRenderer);

			///////////////////////////////////////////////////////////////////////////
			//-----------------------------------------------------------------------//
			//-------------------------- Render textures ----------------------------//
			{
				//SDL_Rect tempr = {gWindow.getWidth()/2-64, gWindow.getHeight()/2-64, 128, 128};
				//SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
				//SDL_RenderFillRect(gRenderer, &tempr);

				// Render Level-preview Textures
				for (int i=0; i<levelMax; i++)
				{
					// Render light-orange box around level-preview Texture
					if (levelbox[i].hover) {
						SDL_Rect tempr = {levelbox[i].x-10, levelbox[i].y-10, levelbox[i].w+20, levelbox[i].h+20};
						SDL_SetRenderDrawColor(gRenderer, 255, 144, 50, 255);
						SDL_RenderFillRect(gRenderer, &tempr);
					}

					// Render dark-orange box around level-preview Texture
					if (levelbox[i].selected) {
						SDL_Rect tempr = {levelbox[i].x-5, levelbox[i].y-5, levelbox[i].w+10, levelbox[i].h+10};
						SDL_SetRenderDrawColor(gRenderer, 200, 100, 20, 255);
						SDL_RenderFillRect(gRenderer, &tempr);

						// Render preview in the center of the screen
						gLevelPreviews[i].render(gRenderer, screenWidth/2 - levelbox[i].w,
															screenHeight/2 - levelbox[i].h,
															levelbox[i].w*2,
															levelbox[i].h*2);
						// Render locked level if not unlocked yet
						if (i+1 > LevelsCompleted) {
							gLockedLevel.render(gRenderer, (screenWidth/2 - levelbox[i].w) +40,
															(screenHeight/2 - levelbox[i].h) + 35,
															(levelbox[i].w*2) - 80,
															(levelbox[i].h*2)   - 70);
						}
					}

					// Render Level-preview Texture selected
					gLevelPreviews[i].render(gRenderer, levelbox[i].x, levelbox[i].y, levelbox[i].w, levelbox[i].h);

					// Render locked level if not unlocked yet
					if (i+1 > LevelsCompleted) {
						gLockedLevel.render(gRenderer, levelbox[i].x+40, levelbox[i].y+35+5,
													   levelbox[i].w-80, levelbox[i].h-70);
					}
				}
			}
			//-------------------------- Render textures ----------------------------//
			//-----------------------------------------------------------------------//
			///////////////////////////////////////////////////////////////////////////


			///////////////////////////////////////////////////////////////////////////
			//-----------------------------------------------------------------------//
			//--------------------------- Render buttons ----------------------------//

			// Render buttons
			StartGameBtn.Render(gRenderer);
			EraseDataBtn.Render(gRenderer);
			BackBtn.Render(gRenderer);

			//--------------------------- Render buttons ----------------------------//
			//-----------------------------------------------------------------------//
			///////////////////////////////////////////////////////////////////////////


			///////////////////////////////////////////////////////////////////////////
			//-----------------------------------------------------------------------//
			//----------------------------- Render text -----------------------------//
			{
				if (debug) {
					// Render text: Current scene
					{
						std::stringstream tempss;
						tempss << "_gameState: Act Selection Scene.";
						gText.loadFromRenderedText(gRenderer, tempss.str().c_str(),
												   {255,255,255}, gFont12);
						gText.setAlpha(255);
						gText.render(gRenderer, gWindow.getWidth() - gText.getWidth(),
												0,
												gText.getWidth(),
												gText.getHeight());
					}

					// Render text: Levels unlocked
					{
						std::stringstream tempss;
						tempss << "Completed Levels: " << LevelsCompleted;
						gText.loadFromRenderedText(gRenderer, tempss.str().c_str(),
												   {255,255,255}, gFont12);
						gText.setAlpha(255);
						gText.render(gRenderer, gWindow.getWidth()/2 - gText.getWidth(),
												gWindow.getHeight() * 0.90 - gText.getHeight(),
												gText.getWidth(),
												gText.getHeight());
					}

					// Render text: Current level selected
					{
						std::stringstream tempss;
						tempss << "Level hovered: " << levelIndex;
						gText.loadFromRenderedText(gRenderer, tempss.str().c_str(),
												   {255,255,255}, gFont12);
						gText.setAlpha(255);
						gText.render(gRenderer, gWindow.getWidth()/2 - gText.getWidth(),
												gWindow.getHeight() * 0.90 - gText.getHeight() + 14,
												gText.getWidth(),
												gText.getHeight());

						tempss.str(std::string());
						tempss << "selectedLevel: " << selectedLevel;
						gText.loadFromRenderedText(gRenderer, tempss.str().c_str(),
												   {255,255,255}, gFont12);
						gText.setAlpha(255);
						gText.render(gRenderer, gWindow.getWidth()/2 - gText.getWidth(),
												gWindow.getHeight() * 0.90 - gText.getHeight() + 28,
												gText.getWidth(),
												gText.getHeight());
					}
				}
			}
			//----------------------------- Render text -----------------------------//
			//-----------------------------------------------------------------------//
			///////////////////////////////////////////////////////////////////////////

			// Render mouse location
			gCursor.render(gRenderer, mex-camx, mey-camy, 20, 20);
		//}

		// Update screen
		SDL_RenderPresent(gRenderer);
	}
	// Free everything
	Free();
}


// Mouse Pressed
ActSelection::Result ActSelection::mousePressed(SDL_Event event)
{
	// If mouse moved
	if (event.type == SDL_MOUSEMOTION) {
		// Set controls focus to Keyboard
		this->controls = 1;
	}

	// If mouse button down
	if (event.type == SDL_MOUSEBUTTONDOWN) {
		if (event.button.button == SDL_BUTTON_LEFT) {
			leftClick = true;
		}
		if (event.button.button == SDL_BUTTON_RIGHT) {
			//
		}
	}
	return Nothing;
}

// Mouse Released
ActSelection::Result ActSelection::mouseReleased(SDL_Event event){
	ActSelection::Result result = Nothing;

	// If mouse button released
	if (event.type == SDL_MOUSEBUTTONUP)
	{
		// If left mouse button
		if (event.button.button == SDL_BUTTON_LEFT) {
			leftClick = false;

			// Start game button
			if (StartGameBtn.CheckMouseReleased(mex, mey) == "StartGame")
			{
				// If the Player tries to Play a level that they have unlocked, then player game
				if (selectedLevel <= LevelsCompleted) {
					result = StartGame;
				}

			}

			// Erase data Button
			else if (EraseDataBtn.CheckMouseReleased(mex, mey) == "EraseData")
			{
				EraseFileData();
			}

			// Back button
			else if (BackBtn.CheckMouseReleased(mex, mey) == "Back")
			{
				result = Back;
			}
		}
		// If right mouse button
		if (event.button.button == SDL_BUTTON_RIGHT) {
			//
		}
	}
	return result;
}
