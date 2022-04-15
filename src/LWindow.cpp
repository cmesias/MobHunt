/*
 * LWindow.cpp
 *
 *  Created on: Apr 18, 2017
 *      Author: healt
 */


#include "LWindow.h"


LWindow::LWindow() {
	//Initialize non-existant window
	mWindow = NULL;
	mMouseFocus = false;
	mKeyboardFocus = false;
	mFullScreen = false;
	mMinimized = false;
	mWidth = 0;
	mHeight = 0;
	name = "window name here";
}

/* The only time this is called is in the beginning of
 * the program.
 *
 * This will create a window, then will apply the right
 * configurations based on the file loaded.
 *
 * If you would like to change the video settings
 * after calling this function, start using applySettings()
 */
bool LWindow::create(std::string newName, int RESOLUTION, int ANTI_ALIS, int FULLSCREEN, int VSYNC) {
	// Screen dimensions
	int Width = 0;
	int Height = 0;

	Uint32 flags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;

	// Resolution
	if (RESOLUTION==1){
		Width 	= 1024;
		Height 	= 576;
	}else if (RESOLUTION==2){
		Width 	= 1280;
		Height 	= 720;
	}else if (RESOLUTION==3){
		Width 	= 1336;
		Height 	= 768;
	}else if (RESOLUTION==4){
		Width 	= 1600;
		Height 	= 900;
	}else if (RESOLUTION==5){
		Width 	= 1920;
		Height 	= 1080;
	}

	// Vsync
	// Handled in Game.cpp when creating a renderer

	//Create window
	name = newName.c_str();
	mWindow = SDL_CreateWindow(name.c_str(), 12, 12, Width, Height, flags);
	if (mWindow != NULL) {
		mMouseFocus = true;
		mKeyboardFocus = true;
		mWidth = Width;
		mHeight = Height;
	}

	// Turn FullScreen off so we can change resolution
	//SDL_SetWindowFullscreen(mWindow, SDL_FALSE);

	// Center screen
	SDL_SetWindowPosition(mWindow, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

	// FullScreen
	if (FULLSCREEN==1){
	//	SDL_SetWindowFullscreen(mWindow, SDL_TRUE);
	}else{
	//	SDL_SetWindowFullscreen(mWindow, SDL_FALSE);
	}

	return mWindow != NULL;
}

bool LWindow::init(std::string newName, int screenWidth, int screenHeight, Uint32 flags) {
	//Create window
	name = newName.c_str();
	mWindow = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, flags);
	if (mWindow != NULL) {
		mMouseFocus = true;
		mKeyboardFocus = true;
		mWidth = screenWidth;
		mHeight = screenHeight;
	}

	return mWindow != NULL;
}

SDL_Renderer* LWindow::createRenderer(Uint32 flags) {
	return SDL_CreateRenderer(mWindow, -1, flags);
}

void LWindow::handleEvent(SDL_Renderer *gRenderer, SDL_Event& e) {
	//Window event occured
	if (e.type == SDL_WINDOWEVENT) {
		//Caption update flag
		bool updateCaption = false;

		switch (e.window.event) {
		//Get new dimensions and repaint on window size change
		case SDL_WINDOWEVENT_SIZE_CHANGED:
			mWidth = e.window.data1;
			mHeight = e.window.data2;
			SDL_RenderPresent(gRenderer);
			break;

			//Repaint on exposure
		case SDL_WINDOWEVENT_EXPOSED:
			SDL_RenderPresent(gRenderer);
			break;

			//Mouse entered window
		case SDL_WINDOWEVENT_ENTER:
			mMouseFocus = true;
			updateCaption = true;
			break;

			//Mouse left window
		case SDL_WINDOWEVENT_LEAVE:
			mMouseFocus = false;
			updateCaption = true;
			break;

			//Window has keyboard focus
		case SDL_WINDOWEVENT_FOCUS_GAINED:
			mKeyboardFocus = true;
			updateCaption = true;
			break;

			//Window lost keyboard focus
		case SDL_WINDOWEVENT_FOCUS_LOST:
			mKeyboardFocus = false;
			updateCaption = true;
			break;

			//Window minimized
		case SDL_WINDOWEVENT_MINIMIZED:
			mMinimized = true;
			break;

			//Window maxized
		case SDL_WINDOWEVENT_MAXIMIZED:
			mMinimized = false;
			break;

			//Window restored
		case SDL_WINDOWEVENT_RESTORED:
			mMinimized = false;
			break;
		}

		//Update window caption with new data
		if (updateCaption) {
			std::stringstream caption;
			/*caption << "Boxes (Server) - MouseFocus:"
					<< ((mMouseFocus) ? "On" : "Off") << " KeyboardFocus:"
					<< ((mKeyboardFocus) ? "On" : "Off");*/
			SDL_SetWindowTitle(mWindow, name.c_str());
		}
	}
	//Enter exit full screen on return key
	//else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN) {
	else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_F11) {
		if (mFullScreen) {
			SDL_SetWindowFullscreen(mWindow, SDL_FALSE);
			mFullScreen = false;
			SDL_MaximizeWindow(mWindow);
		} else {
			SDL_SetWindowFullscreen(mWindow, SDL_TRUE);
			mFullScreen = true;
			mMinimized = false;
			SDL_MaximizeWindow(mWindow);
		}
	}
}

// Apply new window settings
void LWindow::applySettings(int RESOLUTION, int ANTI_ALIS, int FULLSCREEN, int VSYNC){
	//SDL_SetWindowFullscreen(mWindow, SDL_FALSE);

	/*  WARNING
	 *  Do not start the program and immediately Fullscreen,
	 *  if you do this then you will not be able to
	 *  change the resolution and will create many other issues.
	 */

	// Turn FullScreen off so we can change resolution
	//SDL_SetWindowFullscreen(mWindow, SDL_FALSE);

	// Resolution
	if (RESOLUTION==1){
		SDL_SetWindowSize(mWindow, 1024, 576);
	}else if (RESOLUTION==2){
		SDL_SetWindowSize(mWindow, 1280, 720);
	}else if (RESOLUTION==3){
		SDL_SetWindowSize(mWindow, 1336, 768);
	}else if (RESOLUTION==4){
		SDL_SetWindowSize(mWindow, 1600, 900);
	}else if (RESOLUTION==5){
		SDL_SetWindowSize(mWindow, 1920, 1080);
	}

	// Vsync
	// Handled in Game.cpp when creating a renderer

	// Center screen
	SDL_SetWindowPosition(mWindow, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

	// FullScreen
	if (FULLSCREEN==1){


		//SDL_SetWindowFullscreen(mWindow, SDL_TRUE);
		///mFullScreen = true;
		//mMinimized = false;
		//SDL_MaximizeWindow(mWindow);
	}else{
		//SDL_SetWindowFullscreen(mWindow, SDL_FALSE);
		//mFullScreen = false;
		//SDL_MaximizeWindow(mWindow);
	}
}

void LWindow::free() {
	if (mWindow != NULL) {
		SDL_DestroyWindow(mWindow);
	}

	mMouseFocus = false;
	mKeyboardFocus = false;
	mWidth = 0;
	mHeight = 0;
}

int LWindow::getWidth() {
	return mWidth;
}

int LWindow::getHeight() {
	return mHeight;
}

SDL_Window* LWindow::getWindow() {
	return mWindow;
}

bool LWindow::hasMouseFocus() {
	return mMouseFocus;
}

bool LWindow::hasKeyboardFocus() {
	return mKeyboardFocus;
}

bool LWindow::isMinimized() {
	return mMinimized;
}


