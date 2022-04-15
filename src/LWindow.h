/*
 * LWindow.h
 *
 *  Created on: Apr 18, 2017
 *      Author: healt
 */

#ifndef LWINDOW_H_
#define LWINDOW_H_

#include <iostream>
#include <sstream>
#include <SDL2/SDL.h>

class LWindow {
public:
	void applySettings(int RESOLUTION, int ANTI_ALIS, int FULLSCREEN, int VSYNC);
public:
	//Intializes internals
	LWindow();

	bool create(std::string newName, int RESOLUTION, int ANTI_ALIS, int FULLSCREEN, int VSYNC);

	//Creates window
	bool init(std::string newName, int screenWidth, int screenHeight, Uint32 flags);

	//Creates renderer from internal window
	SDL_Renderer* createRenderer(Uint32 flags);

	//Handles window events
	void handleEvent(SDL_Renderer *gRenderer, SDL_Event& e);

	//Deallocates internals
	void free();

	//Window dimensions
	int getWidth();
	int getHeight();

	// Window
	SDL_Window* getWindow();

	//Window focii
	bool hasMouseFocus();
	bool hasKeyboardFocus();
	bool isMinimized();

private:
	//Window data
	SDL_Window* mWindow;

	//Window name
	std::string name;

	//Window dimensions
	int mWidth;
	int mHeight;

	//Window focus
	bool mMouseFocus;
	bool mKeyboardFocus;
	bool mFullScreen;
	bool mMinimized;
};

#endif /* LWINDOW_H_ */
