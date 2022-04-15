/*
 * MainMenu.cpp
 *
 *  Created on: Apr 20, 2017
 *      Author: Carl
 */

#include <fstream>
#include <iostream>
#include <math.h>
#include <SDL2/SDL.h>

#include "LWindow.h"
#include "MainMenu.h"

void MainMenu::Init() {

}

void MainMenu::Load(SDL_Renderer *gRenderer)
{
	// Load Video CFG
	settings.LoadVideoCFG();

	// Load Audio CFG
	settings.LoadAudioCFG();

	// Textures
	gMenu.loadFromFile(gRenderer, "resource/gfx/menu.png");
	gCursor.loadFromFile(gRenderer, "resource/gfx/cursor.png");

	// Buttons
	buttonName[0] = "New Game";
	buttonName[1] = "How To Play";
	buttonName[2] = "Credits";
	buttonName[3] = "Options";
	buttonName[4] = "Exit";
    joy = SDL_JoystickOpen(0);

	// Other classes Fonts
	LoadFonts();
}

void MainMenu::Free() {
	gMenu.free();
	gCursor.free();
    SDL_JoystickClose(joy);
	joy = NULL;
}

void MainMenu::Show(LWindow &gWindow, SDL_Renderer *gRenderer, MainMenu::MenuResult &result) {

	// Upon entry
	quit = false;
	leftClick = false;
	confirmKey = false;
	shift = false;
	A = false;
	LAnalogTrigger = false;
	key	= 0;
	menuIndex = -1;
	result = Nothing;

	// Load resources
	Load(gRenderer);

	//std::cout << "Inside Act Selection Screen" << std::endl;

	// While loop
	while (!quit) {

		// Set level buttons
		for (int i=0; i<5; i++) {
			levelsBox[i].w = 336;
			levelsBox[i].h = 48;
			levelsBox[i].x = 20;
			levelsBox[i].y = 20+i*(levelsBox[i].h+8);
		}

		// Get mouse position
		SDL_GetMouseState(&mx, &my);

		mex = (screenWidth*mx)/gWindow.getWidth();				// New mouse coordinates, no relation to camera
		mey = (screenHeight*my)/gWindow.getHeight();				// New mouse coordinates, no relation to camera

		// Handle Events
		while (SDL_PollEvent(&event)) {

			// Handle window events
			gWindow.handleEvent(gRenderer, event);

			// switch key if controller moved
			if (event.type == SDL_JOYAXISMOTION) {
				key = 0;
			}
			// Controller button down
			if (event.type == SDL_JOYBUTTONDOWN){
				key = 0;
			}

			// Back Game
			if (event.type == SDL_QUIT) {
				result = Exit;
				quit = true;
				gCursor.free();
				Free();
				return;
			}else{
				// Key Pressed
				if (event.type == SDL_KEYDOWN && event.key.repeat == 0) {
					key = 0;
					switch (event.key.keysym.sym) {
					case SDLK_LSHIFT:
						shift = true;
						break;
					case SDLK_RSHIFT:
						shift = true;
						break;
					/////////////////////////////////////////////////////////////////////////////////
					///////////////////////// Level selection index navigation //////////////////////
					//-----------------------------------------------------------------------------//
					case SDLK_w:							// Cycle up through levels
						if (menuIndex > 0) {
							menuIndex--;
						}
						break;
					case SDLK_s:							// Cycle down through levels
						if (menuIndex < 4) {
							menuIndex++;
						}
						break;
					case SDLK_UP:							// Cycle up through levels
						if (menuIndex > 0) {
							menuIndex--;
						}
						break;
					case SDLK_DOWN:							// Cycle down through levels
						if (menuIndex < 4) {
							menuIndex++;
						}
						break;
					case SDLK_RETURN:							// Enter button action selection
						confirmKey = true;
						break;
					case SDLK_SPACE:							// Enter button action selection
						confirmKey = true;
						break;
					//-----------------------------------------------------------------------------//
					///////////////////////// Level selection index navigation //////////////////////
					/////////////////////////////////////////////////////////////////////////////////
					}
				}
				// Key Released
				else if (event.type == SDL_KEYUP && event.key.repeat == 0) {
					switch (event.key.keysym.sym) {
					case SDLK_LSHIFT:
						shift = false;
						break;
					case SDLK_RSHIFT:
						shift = false;
						break;
					case SDLK_RETURN:							// Enter button action selection
						confirmKey = false;
						if (menuIndex == 0) {
							result = NewGame;
						} else if (menuIndex == 1) {
							result = HowToPlay;
						} else if (menuIndex == 2) {
							result = Credits;
						} else if (menuIndex == 3) {
							// Other classes Fonts
							FreeFonts();

							settings.start(gWindow,gRenderer);

							// Other classes Fonts
							LoadFonts();
						} else if (menuIndex == 4) {
							result = Exit;
						}
						break;
					case SDLK_SPACE:							// Enter button action selection
						confirmKey = false;
						if (menuIndex == 0) {
							result = NewGame;
						} else if (menuIndex == 1) {
							result = HowToPlay;
						} else if (menuIndex == 2) {
							result = Credits;
						} else if (menuIndex == 3) {
							// Other classes Fonts
							FreeFonts();

							settings.start(gWindow,gRenderer);

							// Other classes Fonts
							LoadFonts();
						} else if (menuIndex == 4) {
							result = Exit;
						}
						break;
					}
				}

				// Mouse motion?
				if (event.type == SDL_MOUSEMOTION) {
					key = 1;
				}

				// Get results from mouse only if we are moving the mouse
				if (key == 1) {
					// Mouse Pressed
					mousePressed(event);

					// Mouse Released
					result = mouseReleased(gWindow, gRenderer, event);
				}

				// Get title index from keyboard or xbox controller
				updateJoystick(gRenderer, gWindow, &event, result);

				// Handle results
				switch (result)  {
					case NewGame:
						quit = true;
						break;
					case HowToPlay:
						quit = true;
						break;
					case Options:
						quit = true;
						break;
					case Credits:
						quit = true;
						break;
					case Exit:
						quit = true;
						break;
					case Nothing:

						break;
				}

				// Customize Character results
				switch (settings.settingsResult)  {
				case Settings::Exit:				// Exit Desktop
					result = MainMenu::Exit;
					quit = true;
					break;
				}
			}
		}

		// Update Variables
		// Set menuIndex equal to where ever the mouse is, else let the keyboard decide what index it is
		if (key == 1) {
			for (int i=0; i<5; i++) {
				if (checkCollision(mex, mey, 1, 1, levelsBox[i].x, levelsBox[i].y, levelsBox[i].w, levelsBox[i].h)) {
					menuIndex = i;
				}
			}
		}

		// Clear render screen
		SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
		SDL_RenderClear(gRenderer);

			//gMenu.render(gRenderer, 0, 0, 1920, 1080);

			Render(gRenderer);

			// Render mouse location
			gCursor.render(gRenderer, mex, mey, 20, 20);

		// Update screen
		SDL_RenderPresent(gRenderer);
	}

	// Free everything
	Free();
}

void MainMenu::Render(SDL_Renderer *gRenderer)
{
	// render level titles and button reactions
	for (int i=0; i<5; i++) {
		// Render background
		SDL_Rect tempr = {levelsBox[i].x, levelsBox[i].y, levelsBox[i].w, levelsBox[i].h};
		SDL_SetRenderDrawColor(gRenderer, 65, 65, 65, 255);
		SDL_RenderFillRect(gRenderer, &tempr);

		// Render border around title menu item

		// Controller stuff
		if (key == 0) {
			if (menuIndex==i) {
				if (confirmKey || A) {
					SDL_Rect tempr = {levelsBox[i].x+1, levelsBox[i].y+1, levelsBox[i].w-2, levelsBox[i].h-2};
					SDL_SetRenderDrawColor(gRenderer, 0, 200, 0, 255);
					SDL_RenderDrawRect(gRenderer, &tempr);
				}else{
					SDL_Rect tempr = {levelsBox[i].x+1, levelsBox[i].y+1, levelsBox[i].w-2, levelsBox[i].h-2};
					SDL_SetRenderDrawColor(gRenderer, 255, 144, 25, 255);
					SDL_RenderDrawRect(gRenderer, &tempr);
				}
			}
		}

		// Mouse and keyboard stuff
		else{
			if (checkCollision(mex, mey, 1, 1, levelsBox[i].x, levelsBox[i].y, levelsBox[i].w, levelsBox[i].h)) {
				if (leftClick) {
					//SDL_Rect tempr = {levelsBox[i].x+1, levelsBox[i].y+1, levelsBox[i].w-2, levelsBox[i].h-2};
					//SDL_SetRenderDrawColor(gRenderer, 0, 200, 0, 255);
					//SDL_RenderDrawRect(gRenderer, &tempr);
					SDL_Rect tempr = {levelsBox[i].x, levelsBox[i].y, levelsBox[i].w, levelsBox[i].h};
					SDL_SetRenderDrawColor(gRenderer, 0, 200, 0, 255);
					SDL_RenderDrawRect(gRenderer, &tempr);
				}else{
					//SDL_Rect tempr = {levelsBox[i].x+1, levelsBox[i].y+1, levelsBox[i].w-2, levelsBox[i].h-2};
					//SDL_SetRenderDrawColor(gRenderer, 255, 144, 25, 255);
					//SDL_RenderDrawRect(gRenderer, &tempr);
					SDL_Rect tempr = {levelsBox[i].x, levelsBox[i].y, levelsBox[i].w, levelsBox[i].h};
					SDL_SetRenderDrawColor(gRenderer, 255, 144, 25, 255);
					SDL_RenderDrawRect(gRenderer, &tempr);
				}
			}
		}

		// Render level index we are currently on
		/*if (menuIndex==i) {
			if (leftClick) {
				SDL_Rect tempr = {levelsBox[i].x+1, levelsBox[i].y+1, levelsBox[i].w-2, levelsBox[i].h-2};
				SDL_SetRenderDrawColor(gRenderer, 0, 200, 0, 255);
				SDL_RenderDrawRect(gRenderer, &tempr);
			}else{
				SDL_Rect tempr = {levelsBox[i].x+1, levelsBox[i].y+1, levelsBox[i].w-2, levelsBox[i].h-2};
				SDL_SetRenderDrawColor(gRenderer, 255, 144, 25, 255);
				SDL_RenderDrawRect(gRenderer, &tempr);
			}
		}else{
			SDL_Rect tempr = {levelsBox[i].x, levelsBox[i].y, levelsBox[i].w, levelsBox[i].h};
			SDL_SetRenderDrawColor(gRenderer, 100, 100, 100, 255);
			SDL_RenderDrawRect(gRenderer, &tempr);
		}*/

		//Render text input
		std::stringstream tempSS;
		tempSS << buttonName[i];
		gText.loadFromRenderedText(gRenderer, tempSS.str().c_str(), {255,255,255}, gFont12);
			int newWidth = gText.getWidth();
			int newHeight = gText.getHeight();
		gText.render(gRenderer, levelsBox[i].x+5,
				levelsBox[i].y + levelsBox[i].h/2 - newHeight/2,
				newWidth, newHeight);

		//Render text input
		tempSS.str(std::string());
		tempSS << GameName.c_str();
		gText.loadFromRenderedText(gRenderer, tempSS.str().c_str(), {255,255,255}, gFont12);
		gText.render(gRenderer, screenWidth-gText.getWidth(), screenHeight-gText.getHeight(), gText.getWidth(), gText.getHeight());
	}
}


// Mouse Pressed
MainMenu::MenuResult MainMenu::mousePressed(SDL_Event event){
	if (event.type == SDL_MOUSEBUTTONDOWN) {
		if (event.button.button == SDL_BUTTON_LEFT) {
			leftClick = true;
			for (int i=0; i<5; i++) {
				if (checkCollision(mex, mey, 1, 1, levelsBox[i].x, levelsBox[i].y, levelsBox[i].w, levelsBox[i].h)) {
					menuIndex = i;
				}
			}
		}
		if (event.button.button == SDL_BUTTON_RIGHT) {
			//
		}
	}
	return Nothing;
}

// Mouse Released
MainMenu::MenuResult MainMenu::mouseReleased(LWindow gWindow, SDL_Renderer *gRenderer, SDL_Event event){
	MainMenu::MenuResult result = Nothing;
	if (event.type == SDL_MOUSEBUTTONUP) {
		if (event.button.button == SDL_BUTTON_LEFT) {
			leftClick = false;
			// Perform actions
			for (int i=0; i<5; i++) {
				// If mouse is hovering over menu item then render specifically
				if (checkCollision(mex, mey, 1, 1, levelsBox[i].x, levelsBox[i].y, levelsBox[i].w, levelsBox[i].h)) {
					if (i == 0) {
						result = NewGame;
					} else if (i == 1) {
						result = HowToPlay;
					} else if (i == 2) {
						result = Credits;
					} else if (i == 3) {

						// TODO [ ] - find out why it keeps crashing when freeing a font that is used in that class
						// Other classes Fonts
						FreeFonts();

						settings.start(gWindow,gRenderer);

						// Other classes Fonts
						LoadFonts();
					} else if (i == 4) {
						result = Exit;
					}
				}
			}
		}
		if (event.button.button == SDL_BUTTON_RIGHT) {
			//
		}
	}
	return result;
}


void MainMenu::updateJoystick(SDL_Renderer *gRenderer, LWindow &gWindow, SDL_Event *e, MainMenu::MenuResult &result) {
	////////////////// Xbox 360 Controls /////////////
	if (e->type == SDL_CONTROLLERAXISMOTION) {
		//controls = 1;
	}
	/* Xbox 360 Controls */
	// Left Analog
	if ( ((SDL_JoystickGetAxis(joy, 0) < -8000) || (SDL_JoystickGetAxis(joy, 0) > 8000)) ||
		 ((SDL_JoystickGetAxis(joy, 1) < -8000) || (SDL_JoystickGetAxis(joy, 1) > 8000)) ){
		LAngle = atan2(SDL_JoystickGetAxis(joy, 1), SDL_JoystickGetAxis(joy, 0)) * ( 180.0 / M_PI );
	}
	// Right Analog
	if ( ((SDL_JoystickGetAxis(joy, 3) < -8000) || (SDL_JoystickGetAxis(joy, 3) > 8000)) ||
		 ((SDL_JoystickGetAxis(joy, 4) < -8000) || (SDL_JoystickGetAxis(joy, 4) > 8000)) ){
		RAngle = atan2(SDL_JoystickGetAxis(joy, 4), SDL_JoystickGetAxis(joy, 3)) * ( 180.0 / M_PI );
	}
	if (LAngle < 0) { LAngle = 360 - (-LAngle); }
	if (RAngle < 0) { RAngle = 360 - (-RAngle); }

	//// Left Analog/////
	// Move left, x-axis
	if (SDL_JoystickGetAxis(joy, 0)/30 < -500){
		//
	}
	// Move right, x-axis
	if (SDL_JoystickGetAxis(joy, 0)/30 > 500){
		//
	}
	// joy range between -500 and 500, no moving
	if (SDL_JoystickGetAxis(joy, 0)/30 >= -500 && SDL_JoystickGetAxis(joy, 0)/30 <= 500){
		//
	}
	// Move up, y-axis
	if (SDL_JoystickGetAxis(joy, 1) < -JOYSTICK_DEAD_ZONE){
		if (!LAnalogTrigger) {
			LAnalogTrigger = true;
			if (menuIndex > 0) {
				menuIndex--;
			}
		}
	}
	// Move down, y-axis
	else if (SDL_JoystickGetAxis(joy, 1) > JOYSTICK_DEAD_ZONE){
		if (!LAnalogTrigger) {
			LAnalogTrigger = true;
			if (menuIndex < 4) {
				menuIndex++;
			}
		}
	}else{
		LAnalogTrigger = false;
	}
	// joy range between -500 and 500, no moving
	if (SDL_JoystickGetAxis(joy, 1)/30 >= -500 && SDL_JoystickGetAxis(joy, 1)/30 <= 500){
		//
	}

	//// Right Analog/////
	// Face left, x-axis
	if (SDL_JoystickGetAxis(joy, 3)/30 < -500){
	//	moveLeft = true;
	}
	// Face right, x-axis
	if (SDL_JoystickGetAxis(joy, 3)/30 > 500){
	//	moveRight = true;
	}
	// Face up, y-axis
	if (SDL_JoystickGetAxis(joy, 4)/30 < -500){
	//	moveUp = true;
	}
	// Face down, y-axis
	if (SDL_JoystickGetAxis(joy, 4)/30 > 500){
	//	moveDown = true;
	}

	//// Triggers Analog/////
	// Left Trigger
	if (SDL_JoystickGetAxis(joy, 2) > -LTRIGGER_DEAD_ZONE){
		//
	}
	// Right Trigger
	if (SDL_JoystickGetAxis(joy, 5) > -RTRIGGER_DEAD_ZONE){
		//
	}
	//// DPAD Triggers ////
	if (SDL_JoystickGetHat(joy, 0) == SDL_HAT_UP) {
		if (menuIndex > 0) {
			menuIndex--;
		}
	}
	if (SDL_JoystickGetHat(joy, 0) == SDL_HAT_DOWN) {
		if (menuIndex < 4) {
			menuIndex++;
		}
	}
	if (SDL_JoystickGetHat(joy, 0) == SDL_HAT_LEFT) {
		//
	}
	if (SDL_JoystickGetHat(joy, 0) == SDL_HAT_RIGHT) {
		//
	}

	if (SDL_JoystickGetHat(joy, 0) == SDL_HAT_LEFTUP) {
		//
	}
	else if (SDL_JoystickGetHat(joy, 0) == SDL_HAT_RIGHTUP) {
		//
	}
	else if (SDL_JoystickGetHat(joy, 0) == SDL_HAT_LEFTDOWN) {
		//
	}
	else if (SDL_JoystickGetHat(joy, 0) == SDL_HAT_RIGHTDOWN) {
		//
	}

	// Xbox 360 Controls
	if (e->type == SDL_JOYBUTTONDOWN && e->jbutton.state == SDL_PRESSED && e->jbutton.which == 0){
		key = 0;
		switch(e->jbutton.button){
		case SDL_CONTROLLER_BUTTON_DPAD_UP:
			//
			break;
		case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
			//
			break;
		case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
			//
			break;
		case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
			//
			break;
		case SDL_CONTROLLER_BUTTON_A:
			A = true;
			break;
		case SDL_CONTROLLER_BUTTON_B:
			//
			break;
		case SDL_CONTROLLER_BUTTON_X:
			//
			break;
		case SDL_CONTROLLER_BUTTON_Y:
			//
			break;
		}
	}else if (e->type == SDL_JOYBUTTONUP && e->jbutton.state == SDL_RELEASED && e->jbutton.which == 0){
		switch(e->jbutton.button){
		case SDL_CONTROLLER_BUTTON_DPAD_UP:
			//
			break;
		case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
			//
			break;
		case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
			//
			break;
		case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
			//
			break;
		case SDL_CONTROLLER_BUTTON_A:
			A = false;
			if (menuIndex == 0) {
				result = NewGame;
			} else if (menuIndex == 1) {
				result = HowToPlay;
			} else if (menuIndex == 2) {
				result = Credits;
			} else if (menuIndex == 3) {
				// Other classes Fonts
				FreeFonts();
				settings.start(gWindow,gRenderer);
				// Other classes Fonts
				LoadFonts();
			} else if (menuIndex == 4) {
				result = Exit;
			}
			break;
		case SDL_CONTROLLER_BUTTON_B:
			//
			break;
		case SDL_CONTROLLER_BUTTON_X:
			//
			break;
		case SDL_CONTROLLER_BUTTON_Y:
			//
			break;
		}
	}
}
