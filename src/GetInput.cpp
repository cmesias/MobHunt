/*
 * GetInput.cpp
 *
 *  Created on: Apr 6, 2022
 *      Author: healt
 */

#include "GetInput.h"

std::string GetInput::PromptInput(LWindow &gWindow, SDL_Renderer *gRenderer, bool &mainLoop, std::string hint){

	// Loop bool
	bool getInputSave = true;

	// Store hint hear to give the Editor an idea of what they are typing
	std::string textField = "128 128";

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

	textNFont.LoadFonts();

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
				getInputResult	= Exit;
			}

			if (typing == "inputBox"){
				if (e.type == SDL_KEYDOWN) {
					if (e.key.keysym.sym == SDLK_BACKSPACE&& textField.length() > 0) {
						textField.pop_back();
					}
					else if (e.key.keysym.sym== SDLK_c&& SDL_GetModState() & KMOD_CTRL) {
						SDL_SetClipboardText(textField.c_str());
					}
					else if (e.key.keysym.sym== SDLK_v&& SDL_GetModState() & KMOD_CTRL) {
						textField = SDL_GetClipboardText();
					}
				}
				else if (e.type == SDL_TEXTINPUT) {
					if (!((e.text.text[0] == 'c' || e.text.text[0] == 'C')&& (e.text.text[0] == 'v' || e.text.text[0] == 'V')&& SDL_GetModState() & KMOD_CTRL)) {
						textField += e.text.text;
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
					getInputSave = false;
				}
			}

			//Left mouse click
			if (e.type == SDL_MOUSEBUTTONUP) {
				if (e.button.button == SDL_BUTTON_LEFT) {
					//
				}
				if ( helper.checkCollision(mex, mey, 1, 1, customRect.x,customRect.y, customRect.w, customRect.h)  ){
					if (typing != "inputBox"){
						typingAnim = 30;
						typing = "inputBox";
					}
				} else{
					typingAnim = 0;
					typing = "nothing";
				}
			}
		}

		//Typing timer animation
		if (typing == "inputBox"){
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
		if (typing == "inputBox") {
			tempSS << textField.c_str() << slash;
		} else {
			tempSS << textField.c_str();
		}

		//Clear screen for render
		SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 100);
		SDL_RenderClear(gRenderer);

			//Render text tip
			textNFont.gText.loadFromRenderedText(gRenderer, hint.c_str(), {255,255,255}, textNFont.gFont12);
			textNFont.gText.render(gRenderer, 5, 5,textNFont.gText.getWidth(), textNFont.gText.getHeight());

			//Render text input
			textNFont.gText.loadFromRenderedText(gRenderer, tempSS.str().c_str(), {0,255,0}, textNFont.gFont12);
			customRect.x = 10;
			customRect.y = 20;
			textNFont.gText.render(gRenderer, customRect.x+4, customRect.y,textNFont.gText.getWidth(), textNFont.gText.getHeight());
			customRect.w = textNFont.gText.getWidth()+8;
			customRect.h = textNFont.gText.getHeight();
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
	textNFont.FreeFonts();
	return textField.c_str();
}
