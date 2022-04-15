/*
 * CustomizeCharacter.cpp
 *
 *  Created on: Apr 20, 2017
 *      Author: Carl
 */

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "LWindow.h"
#include "CustomizeCharacter.h"

void CustomizeCharacter::Show(LWindow &gWindow, SDL_Renderer *gRenderer, CustomizeCharacter::Result &result) {

	// Upon entry
	quit = false;
	leftClick = false;
	shift = false;

	// Load resources
	load(gRenderer);

	// While loop
	while (!quit) {

		// Set buttons, Quit Save Character, and Start Game
		w = 384;
		h = 96;
		x = 72;
		y = screenHeight-h-72;
		mW = 384;
		mW = 0;
		button[0].x = 72;								// Quit
		button[1].x = screenWidth/2-384/2;		// Save Character
		button[2].x = screenWidth-384-72;		// Start Game

		for (int i=0; i<3; i++) {
			button[i].w = w;
			button[i].h = h;
			button[i].y = y;
		}

		// Get mouse position
		SDL_GetMouseState(&mx, &my);
		mx = (screenWidth*mx)/gWindow.getWidth();					// New mouse coordinates, no relation to camera
		my = (screenHeight*my)/gWindow.getHeight();				// New mouse coordinates, no relation to camera

		// Handle Events
		while (SDL_PollEvent(&event)) {

			// Handle window events
			gWindow.handleEvent(gRenderer, event);

			// Back Game
			if (event.type == SDL_QUIT) {
				result = Exit;
				quit = true;
				free();
				return;
			}else{
				// Key Pressed
				if (event.type == SDL_KEYDOWN && event.key.repeat == 0) {
					switch (event.key.keysym.sym) {
					case SDLK_s: {

						//SDL_Surface* loadedSurface = IMG_Load("player.png");
						//SDL_SaveBMP(loadedSurface, "temp.bmp");


						//SDL_Surface *ss = SDL_CreateRGBSurface(0, 700, 700, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
						/*std::string test = "resource/player.png";
						SDL_Surface* loadedSurface = IMG_Load(test.c_str());
						IMG_SavePNG(loadedSurface, "temp.png");*/
						//SDL_RenderReadPixels(gRenderer, NULL, SDL_PIXELFORMAT_ABGR8888, ss, 0 );
						//SDL_SaveBMP(loadedSurface, "test.bmp");
						/*std::string test = "resource/player.png";
						SDL_Surface* loadedSurface = IMG_Load(test.c_str());
						std::stringstream testt;
						testt << IMG_Load(test.c_str());

						std::ofstream fileS;
						fileS.open("test.png");
						fileS << testt.str().c_str();
						fileS.close();

						std::ofstream fileS2;
						fileS2.open("testSTR.png");
						fileS2 << loadedSurface;
						fileS2.close();*/
						break;
					}
					case SDLK_LSHIFT:
						shift = true;
						break;
					case SDLK_RSHIFT:
						shift = true;
						break;
					case SDLK_q:		// save-index decrease
						index++;
						if (index > 2)
							index = 0;
						break;
					case SDLK_x:
						if (shift) {	// Load CFG
							if (index==0) {									// Load eye-color position
								loadCFG("cc.cfg", eyes.x, eyes.y, 1);
							}else if (index==1) {							// Load body-color position
								loadCFG("cc.cfg", body.x, body.y, 2);
							}else if (index==2) {							// Load body-color position
								loadCFG("cc.cfg", hair.x, hair.y, 3);
							}
						}else{			// Save CFG
							if (index==0) {									// Save eye-color position
								eyes.x = mx;
								eyes.y = my;
								saveCFG("cc.cfg", eyes.x, eyes.y, 1);
							}else if (index==1) {							// Save body-color position
								body.x = mx;
								body.y = my;
								saveCFG("cc.cfg", body.x, body.y, 2);
							}else if (index==2) {							// Save hair-color position
								hair.x = mx;
								hair.y = my;
								saveCFG("cc.cfg", hair.x, hair.y, 3);
							}
						}
						break;
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
				}
			}
		}

		// Update Variables
		eyes.update(leftClick);
		body.update(leftClick);
		hair.update(leftClick);


		// Set render target
		gBlank.setAsRenderTarget(gRenderer);
		//Clear render screen, white color
		SDL_SetRenderDrawColor(gRenderer, 0, 255, 255, 255);
		SDL_RenderClear(gRenderer);
			// Render Player Texture
			// Body
			gPlayer[0].setColor(body.color[0], body.color[1], body.color[2]);
			gPlayer[0].render(gRenderer, 0, 0, 144, 144);
			// Eyes
			gPlayer[1].setColor(eyes.color[0], eyes.color[1], eyes.color[2]);
			gPlayer[1].render(gRenderer, 0, 0, 144, 144);
			// Hair
			gPlayer[2].setColor(hair.color[0], hair.color[1], hair.color[2]);
			gPlayer[2].render(gRenderer, 0, 0, 144, 144);
		//Reset render target
		SDL_SetRenderTarget( gRenderer, NULL );


		// Clear render screen
		SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
		SDL_RenderClear(gRenderer);

			// render background
			gBG.render(gRenderer, 0, 0, screenWidth, screenHeight);

			// Show Player's new edited sprite
			gBlank.render( gRenderer, screenWidth/2-128/2, screenHeight/2-128/2, 144, 144);

			//Show rendered to texture
			SDL_Rect tempr = {0, 96, 48, 48};
			gBlank.render( gRenderer, 111, 111, 80, 80, &tempr);

			// Render bars
			eyes.render(gRenderer);
			body.render(gRenderer);
			hair.render(gRenderer);

			// Render buttons
			for (int i=0; i<3; i++) {
				if (mx+1 >= button[i].x && mx <= button[i].x+button[i].w &&
					my+1 >= button[i].y && my <= button[i].y+button[i].h) {
					// Mouse click, hover
					if (leftClick) {
						gButtons.setAlpha(255);
						gButtons.setColor(244,144,20);
					}
					// No mouse click, hover
					else{
						gButtons.setAlpha(255);
						gButtons.setColor(200,200,200);
					}
				}else{
					// No mouse click, no hover
					gButtons.setAlpha(255);
					gButtons.setColor(255,255,255);
				}
				gButtons.render(gRenderer, button[i].x, button[i].y, button[i].w, button[i].h, &bClip[i]);

				/*SDL_Rect tempRect = {button[i].x, button[i].y, button[i].w, button[i].h};
				SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
				SDL_RenderDrawRect(gRenderer, &tempRect);

				std::stringstream tempss;
				tempss << buttonName[i];
				gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {255,255,255}, gFont21);
				gText.setAlpha(255);
				gText.render(gRenderer, button[i].x+button[i].w/2 - gText.getWidth()/2,button[i].y+button[i].h/2 - gText.getHeight()/2, gText.getWidth(), gText.getHeight());*/
			}

			// Render text, color-part index
			std::stringstream tempss;
				std::string tempS[3];
				tempS[0] = "Eyes";
				tempS[1] = "Body";
				tempS[2] = "Hair";
				tempss << tempS[index];
				gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {255,255,255}, gFont20);
				gText.setAlpha(255);
			gText.render(gRenderer, 10 , 10, gText.getWidth(), gText.getHeight());

		// Update screen
		SDL_RenderPresent(gRenderer);
	}
	// Free everything
	free();
}


// Mouse Pressed
CustomizeCharacter::Result CustomizeCharacter::mousePressed(SDL_Event event){
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
CustomizeCharacter::Result CustomizeCharacter::mouseReleased(SDL_Event event){
	CustomizeCharacter::Result result = Nothing;
	if (event.type == SDL_MOUSEBUTTONUP) {
		if (event.button.button == SDL_BUTTON_LEFT) {
			leftClick = false;
			// Perform actions
			for (int i=0; i<3; i++) {
				// Check Mouse hover w/ Menu item
				SDL_Rect a = {mx, my, 1, 1};
				SDL_Rect b = {button[i].x, button[i].y, button[i].w, button[i].h};
				// If mouse is hovering over menu item then render specifically
				if (checkCollision(a, b)) {
					if (i == 0) {			// Back
						result = Back;
					}else if (i == 1) {		// Save Character
						// function here to save customized character to file
						result = Nothing;
					}else if (i == 2) {		// Start Game
						result = StartGame;
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

bool CustomizeCharacter::checkCollision(SDL_Rect a, SDL_Rect b){
	bool collide;
	if (a.x+a.w > b.x && a.x < b.x + b.w &&
		a.y+a.h > b.y && a.y < b.y + b.h){
		collide = true;
	}else{
		collide = false;
	}
	return collide;
}


void CustomizeCharacter::load(SDL_Renderer *gRenderer) {
	////////////////////////////////////////////////////////////////////////////////////////////////
	//---------------------------------------- Initialize ----------------------------------------//
	// Create RGB bars
	eyes.init("Eyes", 128, 24);
	body.init("Body", 128, 24);
	hair.init("Hair", 128, 24);
	loadCFG("cc.cfg", eyes.x, eyes.y, 1);
	loadCFG("cc.cfg", body.x, body.y, 2);
	loadCFG("cc.cfg", hair.x, hair.y, 3);

	// Buttons
	buttonName[0] = "BACK";
	buttonName[1] = "SAVE CHARACTER";
	buttonName[2] = "START GAME";

	// Save index
	index = 0;

	////////////////////////////////////////////////////////////////////////////////////////////////
	//------------------------------------------- Load -------------------------------------------//

	// Load Texture scenes
	//Load texture target
	if( !gBlank.createBlank( gRenderer, 144, 144, SDL_TEXTUREACCESS_TARGET ) )
	{
		printf( "Failed to create target texture!\n" );
	}
	gBG.loadFromFile(gRenderer, "resource/gfx/bg-menu.png");
	gPlayer[0].loadFromFile(gRenderer, "resource/gfx/player/player_body.png");
	gPlayer[1].loadFromFile(gRenderer, "resource/gfx/player/player_eyes.png");
	gPlayer[2].loadFromFile(gRenderer, "resource/gfx/player/player_hair.png");

	gButtons.loadFromFile(gRenderer, "resource/gfx/cc-buttons.png");
	for (int i=0; i<3; i++) {
		bClip[i].x = 0;
		bClip[i].y = 0+i*96;
		bClip[i].w = 384;
		bClip[i].h = 96;
	}

	int i = 0;
	for (int h=0; h<3; h++) {
		for (int w=0; w<3; w++) {
			clip[i].x = 0+w*48;
			clip[i].y = 0+h*48;
			clip[i].w = 48;
			clip[i].h = 48;
			i++;
		}
	}
	LoadFonts();

	//initTexture(gTileBorder, 32, 32, white, 110);
	// Load Menu starting coordinates
	/*std::ifstream fileTileNumL("data/cfg/menu.cfg");
	if (fileTileNumL.is_open()) {
		fileTileNumL >> x >> y >>
					 w >> h>>
					 mW >> mH;
	}*/
}


void CustomizeCharacter::save() {
	// Load Menu starting coordinates
	/*std::ofstream fileTileNumL("data/cfg/menu.cfg");
	if (fileTileNumL.is_open()) {
		fileTileNumL << x  << " " <<
					    y  << " " <<
						w  << " " <<
						h  << " " <<
						mW << " " <<
						mH;
	}*/
}

void CustomizeCharacter::free() {
	eyes.free();
	body.free();
	hair.free();
	// Free resources
	gBlank.free();
	gBG.free();
	gButtons.free();
	gPlayer[0].free();
	gPlayer[1].free();
	gPlayer[2].free();
	FreeFonts();
}
