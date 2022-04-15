/*
 * SplashScreen.h
 *
 *  Created on: Apr 19, 2017
 *      Author: Carl
 */

#ifndef SPLASHSCREEN_H_
#define SPLASHSCREEN_H_

class SplashScreen : public Helper {
public:
	enum SplashResult { Nothing, Exit, Play };
	void Show(LWindow &gWindow, SDL_Renderer *gRenderer, SplashScreen::SplashResult &result);
private:
	void free();
	LTexture gSplash;

};

#endif /* SPLASHSCREEN_H_ */
