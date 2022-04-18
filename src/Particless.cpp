/*
 * Particless.cpp
 *
 *  Created on: Dec 16, 2016
 *      Author: Carl
 */

#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mouse.h>

#include "Game/Players.h"
#include "Particless.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////// ENEMY BULLETS //////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------------------------------------------------//

void Particle::load(SDL_Renderer* gRenderer){
	setClips(cParticles[0], 0, 0, 8, 8);
	setClips(cParticles[1], 8, 0, 8, 8 );
	setClips(cParticles[2], 16, 0, 8, 8);
	setClips(cParticles[3], 24, 0, 8, 8);
	setClips(cParticles[4], 32, 0, 8, 8);
	setClips(cParticles[5], 48, 0, 8, 8);
	gParticles.loadFromFile(gRenderer, "resource/gfx/particles.png");
	gParticles.setBlendMode(SDL_BLENDMODE_ADD);
	gBullet.loadFromFile(gRenderer, "resource/gfx/player/gBullet.png");
	gSlash.loadFromFile(gRenderer, "resource/gfx/player/gSlash.png");
	blue_block.loadFromFile(gRenderer, "resource/gfx/blue_block.png");

	// From other class
	LoadFonts();
}

void Particle::free(){
	gParticles.free();
	gBullet.free();
	blue_block.free();
	gSlash.free();

	// From other class
	//textFont.Free();

	// Free textures
	gText.free();

	// From other class
	FreeFonts();
}

void Particle::init(Particle particle[]) {
	count = 0;
	for (int i = 0; i < max; i++) {
		particle[i].x 				= 0;
		particle[i].y 				= 0;
		particle[i].w 				= 4;
		particle[i].h 				= 4;
		particle[i].alpha 			= 255;
		particle[i].time 			= 0;
		particle[i].alphaspeed 		= 0;
		particle[i].deathTimer 		= 100;
		particle[i].deathTimerSpeed = 1;
		particle[i].speed 			= 0.00;
		particle[i].vX 				= 0.00;
		particle[i].vY 				= 0.00;
		particle[i].speed 			= 0.00;
		particle[i].angle 			= 0.00;
		particle[i].angleSpe 		= 0;
		particle[i].angleDir 		= 0;
		particle[i].onScreen 		= false;
		particle[i].collide 		= false;
		particle[i].alive 			= false;
		particle[i].sizeDeath 		= false;
		particle[i].splatSpeed 		= 0;
		particle[i].splatDistance 	= 0;
		particle[i].deathSpe 		= 0;
		particle[i].side 			= "";
		particle[i].renderType 		= 0;
		particle[i].hurtType 		= 0;
		particle[i].damage 			= 37;
		particle[i].dmgToParticles 	= 0;
		particle[i].color 			= { 255, 255, 255, 255 };
	}
}

// Spawn a Particle: moves based on angle given and fixed speed
void Particle::spawnParticleAngle(Particle particle[], int renderType, int hurtType,
		float spawnX, float spawnY,
		int spawnW, int spawnH,
		double angle, double speed,
		float damage, float dmgToParticles, float health,
		SDL_Color color, int layer,
		int angleSpe, int angleDir,
		float alpha, float alphaspeed,
		float deathTimer, float deathTimerSpeed,
		bool sizeDeath, float deathSpe,
		double splatDistance, double splatSpeed, float yOffsetShadow) {
	for (int i = 0; i < max; i++)
	{
		if (!particle[i].alive)
		{
			if (renderType == 0) {

				particle[i].yOffsetShadow 	= 30;
			}
			if (renderType == 1) {

				particle[i].yOffsetShadow 	= 20;
			}
			if (renderType == 2) {

				particle[i].yOffsetShadow 	= 30;
			}
			if (renderType == 3) {

				particle[i].yOffsetShadow 	= 7;
			}
			if (renderType == 4) {

				particle[i].yOffsetShadow 	= 30;
			}

			particle[i].renderType 		= renderType;
			particle[i].hurtType 		= hurtType;
			particle[i].damage 			= damage;
			particle[i].dmgToParticles 	= dmgToParticles;
			particle[i].health 			= health;
			particle[i].follow 			= false;
			particle[i].x 				= spawnX;
			particle[i].y 				= spawnY;
			particle[i].w 				= spawnW;
			particle[i].h 				= spawnH;
			particle[i].x2 				= spawnX + spawnW/2;
			particle[i].y2 				= spawnY + spawnH/2;
			particle[i].time 			= 0;
			particle[i].angle 			= angle;
			particle[i].speed 			= speed;
			particle[i].vX 				= (cos( (3.14159265/180)*(angle) ));
			particle[i].vY 				= (sin( (3.14159265/180)*(angle) ));
			//particle[i].x 				= spawnX + (rand() % 4 + 2 * (cos( (3.14159265/180)*(angle) )));
			//particle[i].y 				= spawnY + (rand() % 4 + 2 * (sin( (3.14159265/180)*(angle) )));
			//particle[i].x 				= spawnX + cos( (3.14159265/180)*(angle) );
			//particle[i].y 				= spawnY + sin( (3.14159265/180)*(angle) );


			particle[i].side 			= "";

			particle[i].onScreen 		= false;
			particle[i].collide 		= false;
			particle[i].splatDistance 	= splatDistance;
			particle[i].splatSpeed 		= splatSpeed;
			particle[i].color 			= color;
			particle[i].layer 			= layer;

			particle[i].alphaspeed 		= alphaspeed;
			particle[i].alpha 			= alpha;
			particle[i].deathTimer 		= deathTimer;
			particle[i].deathTimerSpeed = deathTimerSpeed;
			particle[i].angleSpe		= angleSpe;
			particle[i].angleDir		= angleDir;
			particle[i].sizeDeath 		= sizeDeath;
			particle[i].deathSpe 		= deathSpe;
			particle[i].alive 			= true;
			count++;
			break;
		}
	}
}

void Particle::spawnParticleAngleFollow(Particle particle[], int renderType, int hurtType,
		float spawnX, float spawnY,
		int spawnW, int spawnH,
		double angle, double speed,
		float damage, float dmgToParticles, float health,
		SDL_Color color, int layer,
		int angleSpe, int angleDir,
		int alpha, int alphaspeed,
		int deathTimer, int deathTimerSpeed,
		bool sizeDeath, float deathSpe,
		double splatDistance, double splatSpeed,
		bool follow, float *xFollow, float *yFollow,
		float yOffetShadow) {
	for (int i = 0; i < max; i++)
	{
		if (!particle[i].alive)
		{
			particle[i].renderType 			= renderType;
			particle[i].hurtType 			= hurtType;
			particle[i].damage 			= damage;
			particle[i].dmgToParticles 	= dmgToParticles;
			particle[i].health 			= health;
			particle[i].yOffsetShadow 	= yOffetShadow;
			particle[i].follow 			= follow;
			particle[i].xFollow 		= xFollow;
			particle[i].yFollow 		= yFollow;
			particle[i].x 				= spawnX;
			particle[i].y 				= spawnY;
			particle[i].w 				= spawnW;
			particle[i].h 				= spawnH;
			particle[i].x2 				= spawnX + spawnW/2;
			particle[i].y2 				= spawnY + spawnH/2;
			particle[i].time 			= 0;
			particle[i].angle 			= angle;
			particle[i].speed 			= speed;
			particle[i].vX 				= (cos( (3.14159265/180)*(angle) ));
			particle[i].vY 				= (sin( (3.14159265/180)*(angle) ));
			//particle[i].x 				= spawnX + (rand() % 4 + 2 * (cos( (3.14159265/180)*(angle) )));
			//particle[i].y 				= spawnY + (rand() % 4 + 2 * (sin( (3.14159265/180)*(angle) )));
			//particle[i].x 				= spawnX + cos( (3.14159265/180)*(angle) );
			//particle[i].y 				= spawnY + sin( (3.14159265/180)*(angle) );

			particle[i].side 			= "";

			particle[i].onScreen 		= false;
			particle[i].collide 		= false;
			particle[i].splatDistance 	= splatDistance;
			particle[i].splatSpeed 		= splatSpeed;
			particle[i].color 			= color;
			particle[i].layer 			= layer;

			particle[i].alphaspeed 		= alphaspeed;
			particle[i].alpha 			= alpha;
			particle[i].deathTimer 		= deathTimer;
			particle[i].deathTimerSpeed = deathTimerSpeed;
			particle[i].angleSpe		= angleSpe;
			particle[i].angleDir		= angleDir;
			particle[i].sizeDeath 		= sizeDeath;
			particle[i].deathSpe 		= deathSpe;
			particle[i].alive 			= true;
			count++;
			break;
		}
	}
}

void Particle::spawnBloodVFX(Particle particle[], float targetX, float targetY,
												  float targetW, float targetH,
												  SDL_Color color)
{
	for (double i=0.0; i< 360.0; i+=rand() % 10 + 10){
		int rands = rand() % 9 + 2;
		float newX = targetX+targetW/2;
		float newY = targetY+targetH/2;
		spawnParticleAngle(particle, 2, 2,
							newX-rands/2,
							newY-rands/2,
						   rands, rands,
						   i, randDouble(2.1, 5.1),
						   0.0, 0, 0,
						   color, 1,
						   1, 1,
						   rand() % 100 + 150, rand() % 2 + 5,
						   rand() % 50 + 90, 0,
						   true, 0.11,
						   rand() % 9 + 2, 1);
	}
}

void Particle::spawnParryVFX(Particle particle[], float targetX, float targetY,
												  float targetW, float targetH)
{
	for (double i=0.0; i< 360.0; i+=rand() % 10 + 40){
		int rands = rand() % 11 + 3;
		float newX = targetX+targetW/2;
		float newY = targetY+targetH/2;
		spawnParticleAngle(particle, 2, 2,
							newX-rands/2,
							newY-rands/2,
						   rands, rands,
						   i,randDouble(2.1, 5.1),
						   0.0, 0, 0,
						   {192, 202, 206, 255}, 1,
						   1, 1,
						   rand() % 100 + 150, rand() % 2 + 5,
						   rand() % 50 + 90, 0,
						   true, randDouble(0.1, 0.7),
						   100, 10);
	}
}


void Particle::spawnNoMoreManaVFX(Particle particle[], float centerX, float centerY)
{
	for (double i=0.0; i< 90.0; i+= rand() % 10 + 20){
		int rands = rand() % 11 + 3;
		float newX = centerX;
		float newY = centerY;
		spawnParticleAngle(particle, 2, 2,
							newX-rands/2,
							newY-rands/2,
						   rands, rands,
						   angle -45 + i, randDouble(2.1, 5.1),
						   0.0, 0, 0,
						   {244, 144, 0, 255}, 1,
						   1, 1,
						   rand() % 100 + 150, rand() % 2 + 5,
						   rand() % 50 + 90, 0,
						   true, randDouble(0.1, 0.7),
						   100, 10);
	}
}

void Particle::spawnSlashAttackProjectile(Particle particle[], int hurtType,
		float spawnX, float spawnY,
		float spawnW, float spawnH,
		float damage, float angle)
{
	spawnParticleAngle(particle, 0, hurtType,
			spawnX, spawnY,
			spawnW, spawnH,
		   angle, 21,
		   damage, damage, 0,
		   {255, 255,0}, 1,
		   1, 1,
		   255, 0,
		   100, 2,
		   false, 0);
}

void Particle::spawnProjectileAttack(Particle particle[], int hurtType, float spawnX, float spawnY,
		float spawnW, float spawnH,
		float angle, float damage, float speed,
		SDL_Color color) {

	spawnParticleAngle(particle, 1, hurtType,
			spawnX,
			spawnY,
			spawnW, spawnH,
			angle, speed,
			damage, damage, 50,
			color, 1,
			   1, 1,
			   255, 0,
			   60*6, 1,
			   false, 0,
			   100, 10);
}

void Particle::spawnRedProjectileAttack(Particle particle[], int hurtType, float spawnX, float spawnY,
		float spawnW, float spawnH,
		float angle, float damage, float speed) {

	spawnParticleAngle(particle, 1, hurtType,
			spawnX,
			spawnY,
			spawnW, spawnH,
			angle, speed,
			damage, damage, 1,
			   {255, 255, 255, 255}, 1,
			   1, 1,
			   255, 0,
			   60*6, 1,
			   false, 0,
			   100, 10);
}

void Particle::spawnBlackVoidProjectileAttack(Particle particle[], int hurtType, float spawnX, float spawnY,
		float spawnW, float spawnH,
		float angle, float damage, float speed) {
	spawnParticleAngle(particle, 1, hurtType,
			spawnX,
			spawnY,
			spawnW, spawnH,
			angle, speed,
			damage, damage, 1,
		   {106, 13, 173, 255}, 1,
		   1, 1,
		   255, 0,
		   rand() % 50 + 90, 0,
		   false, 0,
		   100, 10);
}

void Particle::spawn360RedAttack(Particle particle[], int hurtType, float spawnX, float spawnY,
		float spawnW, float spawnH,
		float h, float damage, float speed, SDL_Color color) {

	spawnParticleAngle(particle, 1, hurtType,
			spawnX,
			spawnY,
			spawnW, spawnH,
			h, speed,
		   damage, 0, 1,
		   color, 1,
		   1, 1,
		   255, 0,
		   rand() % 50 + 90, 0,
		   false, 0,
		   100, 10);
}

void Particle::spawnQuick360BlueAttackPart1(Particle particle[],
		int hurtType, float casterX, float casterY, float casterW, float casterH,
		float sizeW, float sizeH,
		float h, float damage, float speed, SDL_Color color) {

	for (double h=0.0; h< 360.0; h+=20){

		spawnParticleAngle(particle, 1, hurtType,
				casterX + casterW/2 - sizeW/2,
				casterY + casterH/2 - sizeH/2,
				sizeW, sizeH,
				h, speed,
			   5, 0, 1,
			   color, 1,
			   1, 1,
			   255, 0,
			   15, 3,
			   false, 0.0,
			   100, 10);
	}


}

void Particle::spawnQuick360BlueAttackPart2(Particle particle[],
		int hurtType, float casterX, float casterY, float casterW, float casterH,
		float sizeW, float sizeH,
		float h, float damage, float speed, SDL_Color color) {

	for (double h=0.0; h< 360.0; h+=20){

		spawnParticleAngle(particle, 1, hurtType,
				casterX + casterW/2 - sizeW/2,
				casterY + casterH/2 - sizeH/2,
				sizeW, sizeH,
				h, speed,
				damage, 0, 1,
			   color, 1,
			   1, 1,
			   255, 0,
			   60 * 10, 3,
			   false, 0.0,
			   100, 10);
	}
}

void Particle::spawnExplosion(Particle particle[], float x, float y, int size) {
	for (double j=0.0; j< 360.0; j+=rand() % 10 + 10){
		double f = (double)rand() / RAND_MAX;
		double num = 2.1 + f * (5.1 - 2.1);
		spawnParticleAngle(particle, 2, 2,
							x-size/2,
							y-size/2,
							size, size,
						   j, num,
						   0.0, 0, 100,
						   {255, 0, 255, 255}, 1,
						   1, 1,
						   rand() % 100 + 150, rand() % 2 + 5,
						   rand() % 50 + 90, 0,
						   true, 0.1,
						   rand() % 15 + 2, 1);
	}
}

void Particle::spawnTileHitVFX(Particle particle[], float targetX, float targetY,
												  float targetW, float targetH)
{
	for (double i=0.0; i< 360.0; i+=rand() % 10 + 40){
		int rands = rand() % 9 + 2;
		float newX = targetX+targetW/2;
		float newY = targetY+targetH/2;
		spawnParticleAngle(particle, 3, 2,
							newX-rands/2,
							newY-rands/2,
						   rands, rands,
						   i, randDouble(2.1, 5.1),
						   0.0, 0, 0,
						   {244, 144, 80}, 1,
						   1, 1,
						   rand() % 100 + 150, rand() % 2 + 5,
						   rand() % 50 + 90, 0,
						   true, 0.11,
						   rand() % 9 + 2, 1);
	}
}

void Particle::RemoveAll(Particle particle[]) {
	count = 0;
	for (int i = 0; i < this->max; i++) {
		if (particle[i].alive){
			particle[i].alive = false;
		}
	}
}

// Spawn a Particle: moves based on a fixed velocity given and fixed speed
/*void Particle::fireParticle(Particle particle[], int type, int damage,
									   int spawnx, int spawny, int w, int h,
									   double speed, float vX, float vY,
									   int alphaspeed, int alpha,
									   int deathTimer, int deathTimerSpeed, double angle, double angleSpe, double angleDir,
									   SDL_Color color, int layer) {
	for (int i = 0; i < 1024; i++) {
		if (!particle[i].alive) {
			particle[i].damage 			= damage;
			particle[i].type 			= type;
			particle[i].alphaspeed 		= alphaspeed;
			particle[i].alpha  			= alpha;
			particle[i].x  				= spawnx;
			particle[i].y  				= spawny;
			particle[i].w  				= w;
			particle[i].h  				= h;
			particle[i].speed  			= speed;
			particle[i].angle 			= angle;
			particle[i].angleSpe		= angleSpe;
			particle[i].angleDir		= angleDir;
			particle[i].vX  			= vX;
			particle[i].vY  			= vY;
			particle[i].deathTimer 		= deathTimer;
			particle[i].deathTimerSpeed = deathTimerSpeed;
			particle[i].sizeDeath 		= false;
			particle[i].deathSpe 		= 0;
			particle[i].time 			= 0;
			particle[i].side 			= "";
			particle[i].onScreen 		= false;
			particle[i].collide 		= false;
			particle[i].alive 			= true;
			particle[i].color 			= color;
			particle[i].layer 			= layer;
			//particle[i].angle 		= atan2 ( radianSin, radianCos) * 180 / PI;
			PARTICLES++;
			break;
		}
	}
}*/

// Generate Stars
void Particle::genStars(Particle particle[], int screnWidth, int screenHeight){
	/*this->ptimer += 1;
	if (this->ptimer > 10){
		this->ptimer = 0;
		for (int i=0; i<65; i++){
			int randAlpha = rand() % 100+150;
			int cW = screnWidth + 200;
			int cH = screenHeight + 200;
			int randW = rand() % cW;
			int randH = rand() % cH;
			SDL_Color tempColor = { rand() % 255 + 1, rand() % 255 + 1, rand() % 255 + 1};
			int randl = rand() % 5 + 5;
			int randw = rand() % 2 + 2;

			fireParticle(particle, 2, 0,
									0-100 + randW, 0-100 + randH,
									10/randl, 10/randl,
									0,
									0.0,0.0, 1, randAlpha,
									100, 1, 0.0, 0.0, 0.0,
									tempColor, randl);
		}
	}*/
}

// Spawn a Particle: moves based on location of a target given
/*void Particle::fireParticle2(Particle particle[], int type, int damage, int spawnx, int spawny, int w, int h, int targetx,int targety,
										int alphaspeed, int alpha,
										int deathTimer, int deathTimerSpeed,
										double speed,
										SDL_Color color, int layer) {
	for (int i = 0; i < 1024; i++) {
		if (!particle[i].alive) {
			particle[i].damage 			= damage;
			particle[i].alpha  			= alpha;
			particle[i].deathTimer 		= deathTimer;
			particle[i].alphaspeed 		= alphaspeed;
			particle[i].deathTimerSpeed = deathTimerSpeed;
			particle[i].type 			= type;
			particle[i].x 				= spawnx;
			particle[i].y 				= spawny;
			particle[i].w 				= w;
			particle[i].h 				= h;
			particle[i].speed 			= speed;
			float bmx 					= targetx;
			float bmy 					= targety;
			float distance 				= sqrt((bmx - spawnx) * (bmx - spawnx - particle[i].w / 2) +
											   (bmy - spawny) * (bmy - spawny - particle[i].h / 2));
			particle[i].vX 				= particle[i].speed  *  (bmx - spawnx - particle[i].w / 2) / distance;
			particle[i].vY 				= particle[i].speed  *  (bmy - spawny - particle[i].h / 2) / distance;
			particle[i].angle 			= atan2(bmy - particle[i].y, bmx - particle[i].x);
			particle[i].angle 			= particle[i].angle * (180 / 3.1416);
			particle[i].angleSpe		= 0;
			particle[i].angleDir		= 0;
			particle[i].sizeDeath 		= false;
			particle[i].deathSpe 		= 0;
			particle[i].time 			= 0;
			particle[i].side 			= "";
			particle[i].collide 		= false;
			particle[i].onScreen 		= false;
			particle[i].alive 			= true;
			particle[i].color 			= color;
			particle[i].layer 			= layer;
			PARTICLES++;
			break;
		}
	}
}*/

void Particle::Update(Particle particle[], int mapX, int mapY, int mapW, int mapH) {
	for (int i = 0; i < max; i++) {
		if (particle[i].alive)
		{
			// Center
			particle[i].x2 = particle[i].x + particle[i].w/2;
			particle[i].y2 = particle[i].y + particle[i].h/2;

			// Radius
			particle[i].radius = particle[i].w;

			// Particle death timer, Time
			particle[i].time += particle[i].deathTimerSpeed;

			// Particle death, Time
			if (particle[i].time > particle[i].deathTimer) {
				particle[i].alive = false;
				count--;
			}

			// Particle spin
			particle[i].angle += particle[i].angleSpe * particle[i].angleDir;

			// Update particles angle based on its X and Y velocities
			particle[i].angle = atan2 ( particle[i].vY, particle[i].vX) * 180 / 3.14159265;

			// Particle death timer, transparency
			particle[i].alpha -= particle[i].alphaspeed;
			if (particle[i].alpha < 0) {
				particle[i].alive = false;
				count--;
			}

			// Particle map collision
			if (particle[i].x+particle[i].w < mapX) {
				particle[i].x = mapX+mapW-particle[i].w;
			}
			if (particle[i].x > mapX+mapW) {
				particle[i].x = mapX-particle[i].w;
			}
			if (particle[i].y+particle[i].h < mapY) {
				particle[i].y = mapY+mapH-particle[i].h;
			}
			if (particle[i].y > mapY+mapH) {
				particle[i].y = mapY-particle[i].h;
			}
		}
	}
}

// Update Particles
void Particle::UpdateBullets(Particle particle[], int mapX, int mapY, int mapW, int mapH) {
	for (int i = 0; i < max; i++) {
		if (particle[i].alive)
		{

			// Player particle, and Player CounterAttack Particle
			if (particle[i].hurtType == -1 || particle[i].hurtType == 0)
			{

				// Particle movement
				particle[i].x += particle[i].vX * particle[i].speed;
				particle[i].y += particle[i].vY * particle[i].speed;
			}

			// Enemy particle
			if (particle[i].hurtType == 1)
			{
				// Follow target
				if (particle[i].follow) {
					float bmx = *particle[i].xFollow;
					float bmy = *particle[i].yFollow;
					float bmx2 = particle[i].x + particle[i].w/2;
					float bmy2 = particle[i].y + particle[i].h/2;
					float distance = sqrt((bmx - bmx2) * (bmx - bmx2)+
										  (bmy - bmy2) * (bmy - bmy2));
					particle[i].vX 	= particle[i].speed * (bmx - bmx2) / distance;
					particle[i].vY 	= particle[i].speed * (bmy - bmy2) / distance;

					// Particle movement
					particle[i].x += particle[i].vX;
					particle[i].y += particle[i].vY;
				} else {

					// Particle movement
					particle[i].x += particle[i].vX * particle[i].speed;
					particle[i].y += particle[i].vY * particle[i].speed;
				}

				// Particle health death
				if (particle[i].health <= 0) {
					particle[i].alive = false;
					count--;
				}
			}
		}
	}
}

// Update Particles
void Particle::UpdateVFX(Particle particle[], int mapX, int mapY, int mapW, int mapH) {
	for (int i = 0; i < max; i++) {
		if (particle[i].alive)
		{
			// Star particles
			if (particle[i].hurtType == 2 || particle[i].hurtType == 3)
			{
				// Particle movement
				if (particle[i].splatDistance > 1) {
					particle[i].splatDistance -= particle[i].splatSpeed;
					particle[i].x += particle[i].vX * particle[i].speed;
					particle[i].y += particle[i].vY * particle[i].speed;
				}

				// Particle death upon size
				if (particle[i].sizeDeath) {
					particle[i].w -= particle[i].deathSpe;
					particle[i].h -= particle[i].deathSpe;

					if (particle[i].w <= 0 || particle[i].h <= 0){
						particle[i].alive = false;
						count--;
					}
				}
			}
			// Blue block bits particles
			if (particle[i].hurtType == 4)
			{
				// Particle movement
				if (particle[i].splatDistance > 1) {
					particle[i].splatDistance -= particle[i].splatSpeed;
					particle[i].x += particle[i].vX * particle[i].speed;
					particle[i].y += particle[i].vY * particle[i].speed;
				}

				// Particle death upon size
				if (particle[i].sizeDeath) {
					particle[i].w -= particle[i].deathSpe;
					particle[i].h -= particle[i].deathSpe;

					if (particle[i].w <= 0 || particle[i].h <= 0){
						particle[i].alive = false;
						count--;
					}
				}
			}
		}
	}
}

// Render bullets
void Particle::RenderBullets(Particle particle[], int camX, int camY, float playerZ, SDL_Renderer* gRenderer) {
	for (int i = 0; i < max; i++) {
		if (particle[i].alive) {
			int shadowSizeW = particle[i].w/2;
			int shadowSizeH = particle[i].h/2;

			// Render Slash texture
			if (particle[i].renderType == 0)
			{

				// Render texture
				gSlash.setBlendMode(SDL_BLENDMODE_BLEND);
				gSlash.setAlpha(255);
				gSlash.render(gRenderer, particle[i].x - camX,
										 particle[i].y - particle[i].yOffsetShadow - camY,
										 particle[i].w,
										 particle[i].h,
										 NULL, particle[i].angle);
			}

			// Render Red Projectile texture
			if (particle[i].renderType == 1)
			{
				// Render texture
				gBullet.setBlendMode(SDL_BLENDMODE_BLEND);
				gBullet.setColor(particle[i].color.r, particle[i].color.g, particle[i].color.b);
				gBullet.setAlpha(particle[i].alpha);
				gBullet.render(gRenderer, particle[i].x - camX,
										  particle[i].y - particle[i].yOffsetShadow - camY, particle[i].w,
											 particle[i].h,
										     NULL, particle[i].angle);
			}
		}
	}
}

// Render bullets
void Particle::RenderBulletDebug(Particle particle[], int camX, int camY, SDL_Renderer* gRenderer) {
	for (int i = 0; i < max; i++) {
		if (particle[i].alive) {
			int shadowSizeW = particle[i].w/2;
			int shadowSizeH = particle[i].h/2;

			if (particle[i].renderType == 0 || particle[i].renderType == 1)
			{
				// Cyan box around particles
				SDL_Rect tempRect = {particle[i].x - camX,  particle[i].y - camY,  particle[i].w, particle[i].h};
				SDL_SetRenderDrawColor(gRenderer, 0,245,245,255);
				SDL_RenderDrawRect(gRenderer, &tempRect);

				// Render angle Text
				if (particle[i].follow) {
					//std::stringstream tempss;
					//tempss << "x: " << *particle[i].xFollow << ", y: " << *particle[i].yFollow;
					//gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {255,255,255}, gFont20);
					//gText.render(gRenderer, particle[i].x - camX,
					//						particle[i].y-gText.getHeight() - camY,
					//						gText.getWidth(), gText.getHeight());
				}
			}
		}
	}
}

// Render stars
void Particle::RenderVFX(Particle particle[], int camX, int camY, float playerZ, SDL_Renderer* gRenderer) {
	for (int i = 0; i < max; i++) {
		if (particle[i].alive) {
			int shadowSizeW = particle[i].w/2;
			int shadowSizeH = particle[i].h/2;

			// Render Star particle
			if (particle[i].renderType == 2 || particle[i].renderType == 3) {
				gParticles.setColor(particle[i].color.r, particle[i].color.g, particle[i].color.b);
				gParticles.setAlpha(particle[i].alpha);
				gParticles.render(gRenderer, particle[i].x - camX/particle[i].layer,
											 particle[i].y - particle[i].yOffsetShadow - camY/particle[i].layer,
											 particle[i].w, particle[i].h,
											 &cParticles[4], particle[i].angle);
			}

			// Render Tile hit particle
			if (particle[i].renderType == 4)
			{
				blue_block.setColor(particle[i].color.r, particle[i].color.g, particle[i].color.b);
				blue_block.setAlpha(particle[i].alpha);
				blue_block.render(gRenderer, particle[i].x - camX/particle[i].layer,
											 particle[i].y - particle[i].yOffsetShadow - camY/particle[i].layer,
											 particle[i].w, particle[i].h,
											 NULL, particle[i].angle);
			}
		}
	}
}

//-------------------------------------------------------------------------------------------------------------------------------//
////////////////////////////////////////////////////////// ENEMY BULLETS //////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

