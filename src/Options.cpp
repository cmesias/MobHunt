/*
 * Options.cpp
 *
 *  Created on: May 24, 2017
 *      Author: Carl
 */


#include "Options.h"

/*
//Get's input from user and returns it
void Options::SaveLevel(LWindow &gWindow, SDL_Renderer *gRenderer, bool &mainLoop,
						std::string SpawnCoordinatesData,
						std::string TileSaveData,
						std::string ItemSaveData,
						std::string MonsterSaveData){

	// Loop bool
	bool getInputSave = true;

	// Store user input in string
	std::string inputDirectory = "file name...";

	// What you are currently typing
	std::string typing = "nothing";
	int typingAnim = 0;
	std::string slash = "";

	//Start getting input from user
	SDL_StartTextInput();

	//Mouse properties
	int mex, mey;

	//input text box
	SDL_Rect customRect = {0,0,0,0};
	SDL_Event e;

	while (getInputSave) {

		// Start FPS cap
		helper.fps.start();

		//Mouse Position
		SDL_GetMouseState(&mex, &mey);

		// get new mouse coordinates based on render size, and actual screen size
		int renderW = 0;
		int renderHDummey = 0;
		SDL_GetRendererOutputSize(gRenderer,&renderW,&renderHDummey);
		int en = renderW * 0.4375;
		int renderH = renderW - en;
		mex = (helper.screenWidth*mex)/renderW;	// New mouse coordinates, no relation to camera
		mey = (helper.screenHeight*mey)/renderH;	// New mouse coordinates, no relation to camera

		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				getInputSave = false;
				optionsResult	= Exit;
			}

			if (typing == "filename"){
				if (e.type == SDL_KEYDOWN) {
					if (e.key.keysym.sym == SDLK_BACKSPACE&& inputDirectory.length() > 0) {
						inputDirectory.pop_back();
					}
					else if (e.key.keysym.sym== SDLK_c&& SDL_GetModState() & KMOD_CTRL) {
						SDL_SetClipboardText(inputDirectory.c_str());
					}
					else if (e.key.keysym.sym== SDLK_v&& SDL_GetModState() & KMOD_CTRL) {
						inputDirectory = SDL_GetClipboardText();
					}
				}
				else if (e.type == SDL_TEXTINPUT) {
					if (!((e.text.text[0] == 'c' || e.text.text[0] == 'C')&& (e.text.text[0] == 'v' || e.text.text[0] == 'V')&& SDL_GetModState() & KMOD_CTRL)) {
						inputDirectory += e.text.text;
					}
				}
			}
			//Handle window events
			gWindow.handleEvent(gRenderer, e);

			//If Key Pressed
			if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
				switch (e.key.keysym.sym) {
				case SDLK_ESCAPE:
					getInputSave = false;			//quit current loop
					break;
				case SDLK_RETURN:
					std::stringstream defaultDir;
					defaultDir << "resource/data/maps/";
					///////////////////////////////////////////////////////////////////////////////
					///////////////////////////////////////////////////////////////////////////////
					//----------------------------- Save Spawn Point ----------------------------//
					//Save # of Blocks
					std::ofstream spawnPointFile;
					std::stringstream PathDir;
					// Set directory to save
					PathDir << defaultDir.str().c_str();
					// Set file name and extension
					PathDir << inputDirectory.c_str() << "spawn.mp";
					// Open File
					spawnPointFile.open(PathDir.str().c_str());
					// Store data given from Editor to store in File
					spawnPointFile << SpawnCoordinatesData;
					// Close
					spawnPointFile.close();
					//----------------------------- Save Spawn Point ----------------------------//
					///////////////////////////////////////////////////////////////////////////////
					///////////////////////////////////////////////////////////////////////////////
					///////////////////////////////////////////////////////////////////////////////
					///////////////////////////////////////////////////////////////////////////////
					//------------------------------ Save Tile Data -----------------------------//
					//Save # of Blocks
					std::ofstream newTileFile;
					PathDir.str(std::string());
					// Set directory to save
					PathDir << defaultDir.str().c_str();
					// Set file name and extension
					PathDir << inputDirectory.c_str() << "Tile.mp";
					// Open File
					newTileFile.open(PathDir.str().c_str());
					// Store data given from Editor to store in File
					newTileFile << TileSaveData;
					// Close
					newTileFile.close();
					//------------------------------ Save Tile Data -----------------------------//
					///////////////////////////////////////////////////////////////////////////////
					///////////////////////////////////////////////////////////////////////////////
					///////////////////////////////////////////////////////////////////////////////
					///////////////////////////////////////////////////////////////////////////////
					//-------------------------- Save Collision Tile Data -----------------------//
					///Save # of Blocks
					std::ofstream newTileCFile;
					PathDir.str(std::string());
					// Set directory to save
					PathDir << defaultDir.str().c_str();
					// Set file name and extension
					PathDir << inputDirectory.c_str() << "TileC.mp";
					// Open File
					newTileCFile.open(PathDir.str().c_str());
					// Store data given from Editor to store in File
					newTileCFile << CollisionTileSaveData;
					// Close
					newTileCFile.close();
					//-------------------------- Save Collision Tile Data -----------------------//
					///////////////////////////////////////////////////////////////////////////////
					///////////////////////////////////////////////////////////////////////////////
					///////////////////////////////////////////////////////////////////////////////
					///////////////////////////////////////////////////////////////////////////////
					//------------------------------ Save Object Data ---------------------------//
					//Save # of Blocks
					std::ofstream newObjectFile;
					PathDir.str(std::string());
					// Set directory to save
					PathDir << defaultDir.str().c_str();
					// Set file name and extension
					PathDir << inputDirectory.c_str() << "Item.mp";
					// Open File
					newObjectFile.open(PathDir.str().c_str());
					// Store data given from Editor to store in File
					newObjectFile << ItemSaveData;
					// Close
					newObjectFile.close();
					//------------------------------ Save Object Data ---------------------------//
					///////////////////////////////////////////////////////////////////////////////
					///////////////////////////////////////////////////////////////////////////////
					///////////////////////////////////////////////////////////////////////////////
					//----------------------------- Save Monster Data ---------------------------//
					//Save # of Blocks
					std::ofstream newMonsterFile;
					PathDir.str(std::string());
					// Set directory to save
					PathDir << defaultDir.str().c_str();
					// Set file name and extension
					PathDir << inputDirectory.c_str() << "Monster.mp";
					// Open File
					newMonsterFile.open(PathDir.str().c_str());
					// Store data given from Editor to store in File
					newMonsterFile << MonsterSaveData;
					// Close
					newMonsterFile.close();
					//----------------------------- Save Monster Data ---------------------------//
					///////////////////////////////////////////////////////////////////////////////
					///////////////////////////////////////////////////////////////////////////////
					getInputSave = false;
				}
			}

			//Left mouse click
			if (e.type == SDL_MOUSEBUTTONUP) {
				if (e.button.button == SDL_BUTTON_LEFT) {
					//
				}
				if ( helper.checkCollision(mex, mey, 1, 1, customRect.x,customRect.y, customRect.w, customRect.h)  ){
					if (typing != "filename"){
						typingAnim = 30;
						typing = "filename";
					}
				} else{
					typingAnim = 0;
					typing = "nothing";
				}
			}
		}

		//Typing timer animation
		if (typing == "filename"){
			typingAnim += 1;
			if (typingAnim >= 0 && typingAnim < 60) {
				slash = "_";
			}
			if (typingAnim >= 60 && typingAnim <= 120) {
				slash = " ";
			}
			if (typingAnim > 120) {
				typingAnim = 0;
			}
		}
		//Add the slash animation only when the player is typing the file name
		std::stringstream tempSS;
		if (typing == "filename") {
			tempSS << inputDirectory.c_str() << slash;
		} else {
			tempSS << inputDirectory.c_str();
		}

		//Clear screen for render
		SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 100);
		SDL_RenderClear(gRenderer);

			//Render text tip
			textNFont.gText.loadFromRenderedText(gRenderer, "Enter level directory (ending with a '/'):", {255,255,255}, textNFont.gFont13);
			int newWidth = textNFont.gText.getWidth()/2;
			int newHeight = textNFont.gText.getHeight()/2;
			textNFont.gText.render(gRenderer, 5, 5, newWidth, newHeight);

			//Render text input
			textNFont.gText.loadFromRenderedText(gRenderer, tempSS.str().c_str(), {0,255,0}, textNFont.gFont13);
			customRect.x = 10;
			customRect.y = 15;
			newWidth = textNFont.gText.getWidth()/2;
			newHeight = textNFont.gText.getHeight()/2;
			textNFont.gText.render(gRenderer, customRect.x+4, customRect.y +newHeight - 2, newWidth, newHeight);
			customRect.w = newWidth+8;
			customRect.h = newHeight * 2;
			if ( helper.checkCollision(mex, mey, 1, 1, customRect.x,customRect.y, customRect.w, customRect.h)  ){
				SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
				SDL_RenderDrawRect(gRenderer, &customRect);
			}else{
				SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
				SDL_RenderDrawRect(gRenderer, &customRect);
			}

		//Update screen
		SDL_RenderPresent(gRenderer);

		// fps
		helper.frame++;
		if((helper.cap == true ) && (helper.fps.get_ticks() < 1000 / helper.FRAMES_PER_SECOND ))
			SDL_Delay((1000 / helper.FRAMES_PER_SECOND ) - helper.fps.get_ticks());
	}
}*/
