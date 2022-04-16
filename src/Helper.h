/*
 * Helper.h
 *
 *  Created on: May 18, 2017
 *      Author: Carl
 */

#ifndef LOCAL_HELPER_H_
#define LOCAL_HELPER_H_

#include "Game/Timers.h"
#include <iostream>

static const std::string GameName = "Jar Destroyer";


class Helper {


public:
	struct Point
	{
		int x;
		int y;
	};

public:
    Timer fps;

public:
    // fps
    bool cap 					= true;
	int frame 					= 0;
    const int FRAMES_PER_SECOND = 60;

    // Tiles
    static const int UniqueTileBarTiles = 100;

    //int screenWidth = 1920, screenHeight = 1080;
    //const int screenWidth = 1280, screenHeight = 720;
    //const int screenWidth = 960, screenHeight = 540;
    const int screenWidth = 800, screenHeight = 450;
    //const int screenWidth = 576, screenHeight = 320;


    //int screenWidth = 1920;
    //int screenHeight = 1080;
    //const int screenWidth = 1280;
    //const int screenHeight = 720;

    //const int screenWidth = 1008;
    // const int screenHeight = 624;
	//const int screenWidth = 2304;
	//const int screenHeight = 1296;
	//const int screenWidth = 2880;
    //const int screenHeight = 1620;
     //const int screenWidth = 640;
    //const int screenHeight = 360;
    //const int screenWidth = 480;
    //const int screenHeight = 270;

public:

	// Generate a random double number
	double randDouble(double fMin, double fMax);

	// Set Texture clips
	void setClips(SDL_Rect &tempRect, int x, int y, int w, int h);

	// Check sollision
	bool checkCollision(int x, int y, int w, int h, int x2, int y2, int w2, int h2);

	bool checkCollisionRect( SDL_Rect a, SDL_Rect b );

	float getDistance( float x, float y, float x2, float y2 );

	float getCenter( float pos, float size);

	// Doesnt work properly (3/17/2022)
	void ChangeRenderResolution(SDL_Renderer *gRenderer, int changeTo);

	void RenderRect(SDL_Renderer *gRenderer, float x, float y, float w, float h, SDL_Color color);

	void RenderFillRect(SDL_Renderer *gRenderer, float x, float y, float w, float h, SDL_Color color);

public:	// Check if 2 line segments intersect

	bool onSegment(Point p, Point q, Point r);

	int orientation(Point p, Point q, Point r);

	bool doIntersect(Point p1, Point q1, Point p2, Point q2);

};

#endif /* LOCAL_HELPER_H_ */
