/*
 * Maps.h
 *
 *  Created on: Dec 20, 2016
 *      Author: Carl
 */

#ifndef LOCAL_MAPS_H_
#define LOCAL_MAPS_H_


//Map size
class Map {
public:
	int x = 0;
	int y = 0;
	//int w = 1536;
	//int h = 640;
	//const int w = 1536;
	//const int h = 624;
	//const int w = 2304;
	//const int w = 6912;
	//const int h = 1152;
	//const int w = 2500;
	//const int h = 3500;
	int w = 512;
	int h = 512;
	// Render map
	void render(SDL_Renderer* gRenderer, int camx, int camy);

	void IncreaseWidth(int width);

	void IncreaseHeight(int height);

	int getWidth();

	int getHeight();
};

#endif /* LOCAL_MAPS_H_ */
