/*
 * Helper.cpp
 *
 *  Created on: May 18, 2017
 *      Author: Carl
 */

#include <stdlib.h>
#include <SDL2/SDL.h>
#include "Helper.h"
#include <math.h>

// Set Texture clips
void Helper::setClips(SDL_Rect &tempRect, int x, int y, int w, int h){
	tempRect.x = x;
	tempRect.y = y;
	tempRect.w = w;
	tempRect.h = h;
}

// Generate a random double number
double Helper::randDouble(double fMin, double fMax)
{
	double f = (double)rand() / RAND_MAX;
	return fMin + f * (fMax - fMin);
}

bool Helper::checkCollision(int x, int y, int w, int h, int x2, int y2, int w2, int h2) {
	bool collide;
	if (x+w > x2 && x < x2 + w2 && y+h > y2 && y < y2 + h2) {
		collide = true;
	}else{
		collide = false;
	}
	return collide;
}

bool Helper::checkCollisionRect( SDL_Rect a, SDL_Rect b )
{
    //The sides of the rectangles
    int leftA,   leftB;
    int rightA,  rightB;
    int topA, 	 topB;
    int bottomA, bottomB;

    //Calculate the sides of rect A
    leftA 	= a.x;
    rightA 	= a.x + a.w;
    topA 	= a.y;
    bottomA = a.y + a.h;

    //Calculate the sides of rect B
    leftB 	= b.x;
    rightB 	= b.x + b.w;
    topB 	= b.y;
    bottomB = b.y + b.h;

    //If any of the sides from A are outside of B
    if( bottomA < topB )
    {
        return false;
    }

    if( topA > bottomB )
    {
        return false;
    }

    if( rightA < leftB )
    {
        return false;
    }

    if( leftA > rightB )
    {
        return false;
    }

    //If none of the sides from A are outside B
    return true;
}

float Helper::getDistance( float targetX, float targetY, float selfX, float selfY ) {

	float distance = 1;

	distance = sqrt((targetX - selfX) * (targetX - selfX)+
					(targetY - selfY) * (targetY - selfY));

	return distance;
}

float Helper::getCenter(float pos, float size) {

	float centerPos = pos + size/2;

	return centerPos;
}

void Helper::ChangeRenderResolution(SDL_Renderer *gRenderer, int changeTo) {

	/*if (changeTo == 1) {
	    screenWidth = 1920;
	    screenHeight = 1080;
	}

	else if (changeTo == 2) {
	    screenWidth = 1600;
	    screenHeight = 900;
	}

	else if (changeTo == 3) {
	    screenWidth = 1280;
	    screenHeight = 720;
	}

	else if (changeTo == 4) {
	    screenWidth = 800;
	    screenHeight = 600;
	}*/
}

void Helper::RenderRect(SDL_Renderer *gRenderer, float x, float y, float w, float h, SDL_Color color) {
	SDL_Rect tempRect = {x, y, w, h};
	SDL_SetRenderDrawColor(gRenderer, color.r, color.g, color.b, 255);
	SDL_RenderDrawRect(gRenderer, &tempRect);
}

void Helper::RenderFillRect(SDL_Renderer *gRenderer, float x, float y, float w, float h, SDL_Color color) {
	SDL_Rect tempRect = {x, y, w, h};
	SDL_SetRenderDrawColor(gRenderer, color.r, color.g, color.b, 255);
	SDL_RenderFillRect(gRenderer, &tempRect);
}

// Given three colinear points p, q, r, the function checks if
// point q lies on line segment 'pr'
bool Helper::onSegment(Point p, Point q, Point r)
{
    if (q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) &&
        q.y <= std::max(p.y, r.y) && q.y >= std::min(p.y, r.y))
       return true;

    return false;
}

// To find orientation of ordered triplet (p, q, r).
// The function returns following values
// 0 --> p, q and r are colinear
// 1 --> Clockwise
// 2 --> Counterclockwise
int Helper::orientation(Point p, Point q, Point r)
{
    // See http://www.geeksforgeeks.org/orientation-3-ordered-points/
    // for details of below formula.
    int val = (q.y - p.y) * (r.x - q.x) -
              (q.x - p.x) * (r.y - q.y);

    if (val == 0) return 0;  // colinear

    return (val > 0)? 1: 2; // clock or counterclock wise
}

// The main function that returns true if line segment 'p1q1'
// and 'p2q2' intersect.
bool Helper::doIntersect(Point p1, Point q1, Point p2, Point q2)
{
    // Find the four orientations needed for general and
    // special cases
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);

    // General case
    if (o1 != o2 && o3 != o4)
        return true;

    // Special Cases
    // p1, q1 and p2 are colinear and p2 lies on segment p1q1
    if (o1 == 0 && onSegment(p1, p2, q1)) return true;

    // p1, q1 and p2 are colinear and q2 lies on segment p1q1
    if (o2 == 0 && onSegment(p1, q2, q1)) return true;

    // p2, q2 and p1 are colinear and p1 lies on segment p2q2
    if (o3 == 0 && onSegment(p2, p1, q2)) return true;

     // p2, q2 and q1 are colinear and q1 lies on segment p2q2
    if (o4 == 0 && onSegment(p2, q1, q2)) return true;

    return false; // Doesn't fall in any of the above cases
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
