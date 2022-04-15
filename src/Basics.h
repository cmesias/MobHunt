/*
 * Basics.h
 *
 *  Created on: May 15, 2017
 *      Author: Carl
 */

#ifndef BASICS_H_
#define BASICS_H_

class Basics {
public:
	enum Result { Back, Nothing, StartGame, Exit };
	//void Show(LWindow &gWindow, SDL_Renderer *gRenderer, Basics::Result &result);
	Basics::Result mousePressed(SDL_Event event);
	Basics::Result mouseReleased(SDL_Event event);
public:

	// Initialize
	void Init();

	// Load resources
	void Load(SDL_Renderer *gRenderer);

	// Free resources
	void Free();

	// Show Node to Engine
private:
	int mx, my;
	bool leftClick;
	bool quit;
	bool shift;
	SDL_Event event;
};

#endif /* BASICS_H_ */
