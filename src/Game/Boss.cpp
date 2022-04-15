/*
 * Boss.cpp
 *
 *  Created on: Jul 21, 2017
 *      Author: Carl
 */

#include "Boss.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

void Boss::Init(Boss boss[]) {
	count = 0;
	for (int i = 0; i < max; i++) {
		boss[i].x 					= 0;
		boss[i].y 					= 0;
		boss[i].w 					= 0;
		boss[i].h 					= 0;
		boss[i].vX 					= 0.0;
		boss[i].vY 					= 0.0;
		boss[i].speed 				= 0.0;
		boss[i].angle 				= 0.0;
		boss[i].angleFacingTarget   = 0.0;
		boss[i].health 				= 10000;
		boss[i].healthDecay 			= 10000;
		boss[i].maxHealth 			= 10000;
		boss[i].damage 				= 5;
		boss[i].distance 			= 1;
		boss[i].mouse 				= false;
		boss[i].flash 				= false;
		boss[i].alive 				= false;
		boss[i].collision 			= false;
		boss[i].onScreen 			= false;
		boss[i].alert 				= false;
		boss[i].renderInFront 		= false;
		boss[i].type 				= 0;
		boss[i].jumpstate			= "falling";
		boss[i].jumps				= 1;
		boss[i].jump 				= false;
		boss[i].animSpeed			= 15;
		boss[i].animTimer			= 0;
		boss[i].animFrame			= 0;
		boss[i].walkTimer			= 0;
		boss[i].walkSpeed			= 1;
		boss[i].walkFrame			= 0;
		boss[i].animState			= -1;
		boss[i].randomAttack		= 0;
		boss[i].coolDownTimer		= this->coolDownTimeStart;
		boss[i].coolDownTimeStart	= this->coolDownTimeStart;

		boss[i].chargingAttack 	= false;
		boss[i].chargeTime 		= this->chargeTimeStart;
		boss[i].constantFiringTimer = 0;
	}

	// Other classes:
	LoadFonts();
}

void Boss::Load(SDL_Renderer *gRenderer) {
	gTexture.loadFromFile(gRenderer, "resource/gfx/Boss/boss.png");
	gTextureFlashed.loadFromFile(gRenderer, "resource/gfx/Boss/boss_flash.png");
	gMiddleTurret.loadFromFile(gRenderer, "resource/gfx/Boss/middle-turret.png");
	gTurret.loadFromFile(gRenderer, "resource/gfx/Boss/turret.png");
	gBossShadow.loadFromFile(gRenderer, "resource/gfx/boss_shadow.png");
	sCast 			= Mix_LoadWAV("sounds/bfxr/snd_cast.wav");
}

void Boss::Free() {
	gTexture.free();
	gTextureFlashed.free();
	gBossShadow.free();
	gMiddleTurret.free();
	gTurret.free();
	Mix_FreeChunk(sCast);
	sCast 			= NULL;

	// Other classes:
	FreeFonts();
}

void Boss::RemoveAll(Boss boss[]) {
	count = 0;
	for (int i = 0; i < max; i++) {
		boss[i].alive = false;
	}
}


void Boss::Remove(Boss boss[]) {
	for (int i = 0; i < max; i++) {
		if (boss[i].alive){
			if (boss[i].mouse){
				boss[i].alive = false;
				count--;
			}
		}
	}
}

void Boss::Spawn(Boss boss[], float x, float y, float w, float h, float angle, float speed, int type) {
	for (int i = 0; i < max; i++) {
		if (!boss[i].alive) {

			///////////////////////////////////////////////////////////////////////////////////////
			//----------- When boss is spawned, spawn going a random direction -----------------//
			int randDir = rand() % 2;
			int dir;
			if (randDir == 0)
				dir 				= 1;
			else
				dir 				= -1;
			//----------- When boss is spawned, spawn going a random direction -----------------//
			///////////////////////////////////////////////////////////////////////////////////////

			///////////////////////////////////////////////////////////////////////////////////////
			//------------------------------ Save/load this data --------------------------------//
			boss[i].x 				= x;
			boss[i].y 				= y;
			boss[i].w 				= w;
			boss[i].h 				= h;
			boss[i].radius 		= w/2;
			boss[i].speed 			= speed;
			boss[i].type 			= type;
			boss[i].damage			= 5;
			boss[i].mouse 			= false;
			boss[i].flash 			= false;

			// Spawning normal boss, set default parameters
			if (type == 0) {
				boss[i].health 		= 10000;
				boss[i].maxHealth 	= 10000;
				boss[i].healthDecay 	= 10000;
			} else {
				boss[i].health 		= 20000;
				boss[i].maxHealth 	= 20000;
				boss[i].healthDecay 	= 20000;
			}
			boss[i].vX 			= dir;
			boss[i].vY 			= sin( (3.14159265/180)*(angle) );
			boss[i].angle 			= 0.0;

			//------------------------------ Save/load this data --------------------------------//
			///////////////////////////////////////////////////////////////////////////////////////

			//------------------------------------------------------------------------//
			boss[i].angleFacingTarget   = 0.0;
			/* initial centering of boss's circle collision shape */
			float radians 			= (3.1415926536/180)*(0.0);
			float Cos 				= floor(cos(radians)*10+0.5)/10;
			float Sin 				= floor(sin(radians)*10+0.5)/10;
			int newW 				= 11 * (-Cos);
			int newH 				= 11 * (-Sin);
			boss[i].x2 			= x+w/2 + newW - boss[i].radius;
			boss[i].y2 			= y+h/2 + newH - boss[i].radius;
			//------------------------------------------------------------------------//
			//boss[i].vX 				= cos( (3.14159265/180)*(angle) );
			boss[i].constantFiringTimer = 0;
			boss[i].x2 				= x + w/2;
			boss[i].y2 				= y + h/2;
			boss[i].animState			= -1;
			boss[i].coolDownTimer		= this->coolDownTimeStart;
			boss[i].coolDownTimeStart	= this->coolDownTimeStart;
			boss[i].chargingAttack 	= false;
			boss[i].chargingAttack 	= false;
			boss[i].chargeTime 		= this->chargeTimeStart;
			boss[i].distance 			= 1;
			boss[i].collision 			= false;
			boss[i].onScreen 			= false;
			setStatsBasedOnType(boss, i);
			boss[i].alive 			= true;
			count++;
			break;
		}
	}
}

void Boss::Update(Boss boss[], Object &obj, Object object[],
		Particle particle[], Particle &p_dummy,
		Map &map, int mex, int mey,
		int camx, int camy, bool playerAlive) {
	for (int i = 0; i < this->max; i++) {
		if (boss[i].alive) {

			// boss screen check
			if (boss[i].x + boss[i].w >= camx && boss[i].x <= camx + screenWidth &&
				boss[i].y + boss[i].h >= camy && boss[i].y <= camy + screenHeight) {
				boss[i].onScreen = true;
			}else{
				boss[i].onScreen = false;
			}

			// Boss health decay
			if (boss[i].healthDecay > boss[i].health) {
				boss[i].healthDecay -= 1;
			}

			// boss circle collision check with other bosss
			/*for (int j = 0; j < this->max; j++) {
				if (i != j) {
					if (boss[j].alive) {
						float bmx = boss[j].x+boss[j].w/2;
						float bmy = boss[j].y+boss[j].h/2;
						float bmx2 = boss[i].x+boss[i].w/2;
						float bmy2 = boss[i].y+boss[i].h/2;
						float angle = atan2(bmy - bmy2,bmx - bmx2);
						angle = angle * (180 / 3.1416);
						if (angle < 0) {
							angle = 360 - (-angle);
						}
						float radians = (3.1415926536/180)*(angle);
						float Cos = floor(cos(radians)*10+0.5)/10;
						float Sin = floor(sin(radians)*10+0.5)/10;
						float distance = sqrt((bmx - bmx2) * (bmx - bmx2)+
											  (bmy - bmy2) * (bmy - bmy2));
						if (distance <= 0.01) {
							distance = 0.01;
						}
						if (distance < boss[i].w/2 + boss[j].w/2) {
							//boss[i].x -= 1 * Cos;
							//boss[i].vX = boss[i].vX * -1;

							// This will move both bosss away from each other so they don't overlap
							boss[j].x += 1 * Cos;

							// This still only bounces the boss thats being launched
							//boss[j].vX = boss[j].vX * -1.2;

							//boss[i].y -= 1 * Sin;
						}
					}
				}
			}*/

			// Animation
			boss[i].animTimer += boss[i].animSpeed;
			if (boss[i].animTimer > 60) {
				boss[i].animTimer = 0;
				boss[i].animFrame++;
			}

			if (boss[i].animFrame > 4) {
				boss[i].animFrame = 0;
				boss[i].animTimer = 0;
			}

			// Decelerate boss velocity every frame
			//boss[i].vX = boss[i].vX - boss[i].vX * 0.2;
			//boss[i].vY = boss[i].vY - boss[i].vY * 0.2;

			////////////////////////////////////////////////////////////////////////////
			/////////////////////////// Handle Boss Animations ////////////////////////

			// Cooldown animation
			if (boss[i].animState == -1)
			{
				// If cool down timer greater than 0
				if (boss[i].coolDownTimer > 0) {

					// Start countdown
					boss[i].coolDownTimer--;
				}

				// Countdown finished
				else {
					// Reset cool down timer
					boss[i].coolDownTimer = this->coolDownTimeStart;

					// Reset animation state
					boss[i].animState = 0;
				}
			}

			// Walking around
			else if (boss[i].animState == 0)
			{
				// If player is within 1000 distance from the boss, boss will follow player
				if (boss[i].distance < 1000)
				{
					// If player alive, have Boss follow player
					if (playerAlive) {

						// Change x velocity to go towards player
						boss[i].vX = 5 * (boss[i].bmx - boss[i].bmx2) / boss[i].distance;
						boss[i].vY = 5 * (boss[i].bmy - boss[i].bmy2) / boss[i].distance;
					}


					// Boss is on alert !
					boss[i].alert = true;
				} else
				{
					// Boss is NOT on alert
					boss[i].alert = false;
				}

				// Boss is on alert
				if (boss[i].alert)
				{
				}

				// If player is within 200 pixels, start charging animation
				if (boss[i].distance < 200 && !boss[i].chargingAttack)
				{
					// Start charge-attack animation
					boss[i].chargingAttack = true;

					// Stop moving boss
					boss[i].vX = 0.0;
					boss[i].vY = 0.0;

					// Choose random attack for Boss before starting Shooting animations
					boss[i].randomAttack = rand() % 2;

					// Change animation state
					int randNum = rand() % 2 + 2;

					boss[i].animState = randNum;	// random number from 2-3
				}
			}

			// Moving towards player animation
			else if (boss[i].animState == 1) {

			}

			// Charging-attack animation
			else if (boss[i].animState == 2) {

				// If Charge attack animation
				if (boss[i].chargingAttack)
				{
					// Typical Boss Attack
					for (int j=0; j<5; j++)
					{
						// Boss shoots based on what our chargeTime is
						if (boss[i].chargeTime == j * 6)
						{
							// Spawn particle effect
							int rands = 32;
							float tempX = boss[i].x + boss[i].w/2 - rands/2;
							float tempY = boss[i].y + boss[i].h/2 - rands/2;
							for (double h=0.0; h< 360.0; h+=rand() % 10 + 10){
								p_dummy.spawnParticleAngle(particle, 1, 1,
												   tempX,
												   tempY,
												   rands, rands,
												   h, randDouble(5, 5),
												   10, 0, 1,
												   {255, 255, 255, 255}, 1,
												   1, 1,
												   255, 0,
												   rand() % 50 + 90, 0,
												   true, randDouble(0.1, 0.7),
												   100, 10);
							}

							// Play SFX
							Mix_PlayChannel(-1, sCast, 0);
						}
					}

					// If count down has not reached 0 seconds
					if (boss[i].chargeTime > 0) {

						// Start counting down charge-attack animation
						boss[i].chargeTime--;
					}

					// Countdown reached 0 seconds.
					else {

						// Stop charge attack animation
						boss[i].chargingAttack = false;

						// Reset charge-attack count down
						boss[i].chargeTime = this->chargeTimeStart;

						// Set animation state to cooldown (-1)
						boss[i].animState = -1;

						///////////////////////////////////////////////////////////////////////////////////////////////////////
						//----------------------------- Spawn attack at the end of charge attack ----------------------------//
		                int width;

		                // Player on right side of Boss
		                if (boss[i].bmx > boss[i].x+boss[i].w/2) {

		                	// Set attack object's x pos to the right of player's body
			                width = 64;
		                }

		                // Player on left side of Boss
		                else{

		                	// Set attack object's x pos to the left of player's body
			                width = -64;
		                }
		                // Attack-object's width and height
		                int tempWidth = 64;
		                int tempHeight = 34;


		                // Spawn attack object (it will appear in the world for 1 frame then remove itself)
		                /*obj.spawn(object, boss[i].x+boss[i].w/2 - tempWidth/2,
		                				  boss[i].y-20,
										  tempWidth, tempHeight,
										  2,
										  i);*/		// Save Bosss index for reference later (can be used to knockback this boss for example)

		                // Spawn particle attack
		                // spawn particle
						/*p_dummy.spawnParticleAngle(particle, 1,
								boss[i].x+boss[i].w/2,
								boss[i].y+boss[i].h/2,
								20, 20,
								boss[i].angleFacingTarget, 20,
							   25,
							   {255, 255,0}, 1,
							   1, 1,
							   255, 0,
							   100, 2,
							   false, 0);*/

						// Spawn particle effect
						/*float tempX = boss[i].bmx2;
						float tempY = boss[i].bmy2;
						for (double i=0.0; i< 360.0; i+=rand() % 10 + 40){
							int rands = rand() % 11 + 3;


							p_dummy.spawnParticleAngle(particle, 1,
									tempX-rands/2,
									tempY-rands/2,
											   rands, rands,
											   i, randDouble(2.1, 5.1),
											   0.0,
											   {144, 144, 144, 255}, 1,
											   1, 1,
											   rand() % 100 + 150, rand() % 2 + 5,
											   rand() % 50 + 90, 0,
											   true, randDouble(0.1, 0.7),
											   100, 10);
						}*/

		                // Play boss hit sound effect
						// Play sound effect
						//Mix_PlayChannel(-1, sCast, 0);

						//----------------------------- Spawn attack at the end of charge attack ----------------------------//
						///////////////////////////////////////////////////////////////////////////////////////////////////////
					}
				}
			}

			///////////////////////////////////////////////////////////////////////////////////
			//-------------------------------------------------------------------------------//
			//---------------------------- Barrage Attack Part 1 ----------------------------//
			else if (boss[i].animState == 3) {

				// If Charge attack animation
				if (boss[i].chargingAttack)
				{
					for (int j=0; j<3; j++) {
						if (boss[i].chargeTime == j * 10) {

							// Spawn particle effect
							int rands = 32;
							float tempX = boss[i].x + boss[i].w/2 - rands/2;
							float tempY = boss[i].y + boss[i].h/2 - rands/2;
							for (double h=0.0; h< 360.0; h+=rand() % 10 + 5){

								//int rands = rand() % 11 + 3;

								p_dummy.spawnParticleAngle(particle, 1, 1,
												   tempX,
												   tempY,
												   rands, rands,
												   h, 5,
												   5, 0, 1,
												   {0, 254, 254, 255}, 1,
												   1, 1,
												   255, 0,
												   15, 1,
												   false, 0.0,
												   100, 10);
							}

							// Play SFX
							Mix_PlayChannel(-1, sCast, 0);
						}
					}

					// If count down has not reached 0 seconds
					if (boss[i].chargeTime > 0) {

						// Start counting down charge-attack animation
						boss[i].chargeTime--;
					}

					// Countdown reached 0 seconds.
					else {

						// Attack has 2 parts

						// Reset charge-attack count down
						boss[i].chargeTime = this->chargeTimeStart;

						// Set animation state
						boss[i].animState = 4;
					}
				}
			}
			//---------------------------- Barrage Attack Part 1 ----------------------------//
			//-------------------------------------------------------------------------------//
			///////////////////////////////////////////////////////////////////////////////////


			///////////////////////////////////////////////////////////////////////////////////
			//-------------------------------------------------------------------------------//
			//---------------------------- Barrage Attack Part 2 ----------------------------//
			else if (boss[i].animState == 4) {

				// If Charge attack animation
				if (boss[i].chargingAttack)
				{
					for (int j=0; j<15; j++) {
						if (boss[i].chargeTime == j * 2) {

							// Spawn particle effect
							int rands = 32;
							float tempX = boss[i].x + boss[i].w/2 - rands/2;
							float tempY = boss[i].y + boss[i].h/2 - rands/2;
							for (double h=0.0; h< 360.0; h+=rand() % 10 + 5){

								//int rands = rand() % 11 + 3;

								p_dummy.spawnParticleAngle(particle, 1, 1,
												   tempX,
												   tempY,
												   rands, rands,
												   h, randDouble(14, 16),
												   5, 0, 1,
												   {0, 254, 254, 255}, 1,
												   1, 1,
												   255, 0,
												   randDouble(5, 30), 0.2,
												   false, 0,
												   100, 10);
							}

							// Play SFX
							Mix_PlayChannel(-1, sCast, 0);
						}
					}

					// If count down has not reached 0 seconds
					if (boss[i].chargeTime > 0) {

						// Start counting down charge-attack animation
						boss[i].chargeTime--;
					}

					// Countdown reached 0 seconds.
					else {

						// Stop charge attack animation
						boss[i].chargingAttack = false;

						// Reset charge-attack count down
						boss[i].chargeTime = this->chargeTimeStart;

						// Set animation state to cooldown (-1)
						boss[i].animState = -1;
					}
				}
			}
			//---------------------------- Barrage Attack Part 2 ----------------------------//
			//-------------------------------------------------------------------------------//
			///////////////////////////////////////////////////////////////////////////////////

			// At all states, constantaly fire a barage of bullets from the side
			if (boss[i].animState == -1)
			{
				// Increase timer
				boss[i].constantFiringTimer += 0.5;

				// Reset timer every 60 seconds
				if (boss[i].constantFiringTimer > 60) {
					boss[i].constantFiringTimer = 0;

					// Spawn bullet
					int rands = 96;
					float tempX;
					float tempY;
					float speed = randDouble(15, 15);

					// Middle Turret of boss
					tempX = boss[i].x + boss[i].w/2 - rands/2;
					tempY = boss[i].y + boss[i].h/2 - rands/2;
					rands = 96;
					speed = 2.57;
					p_dummy.spawnParticleAngleFollow(particle, 1, 1,
									   tempX,
									   tempY,
									   rands, rands,
									   boss[i].angleFacingTarget, speed,
									   25, 0, 4,
									   {255, 255, 255, 255}, 1,
									   1, 1,
									   255, 0,
									   60*6, 1,
									   false, 0,
									   100, 10,
									   true, boss[i].xFollow, boss[i].yFollow);

					// Right Turret of boss
					/*tempX = boss[i].x + boss[i].w/2+134 - rands/2;
					tempY = boss[i].y + boss[i].h/2 - rands/2;
					p_dummy.spawnParticleAngle(particle, 1,
									   tempX,
									   tempY,
									   rands, rands,
									   0.0, speed,
									   0.0,
									   {144, 144, 144, 255}, 1,
									   1, 1,
									   rand() % 100 + 150, rand() % 2 + 5,
									   rand() % 50 + 90, 0,
									   true, randDouble(0.1, 0.7),
									   100, 10);

					// Bottom Turret of boss
					tempX = boss[i].x + boss[i].w/2 - rands/2;
					tempY = boss[i].y + boss[i].h/2+134 - rands/2;
					p_dummy.spawnParticleAngle(particle, 1,
									   tempX,
									   tempY,
									   rands, rands,
									   90.0, speed,
									   0.0,
									   {144, 144, 144, 255}, 1,
									   1, 1,
									   rand() % 100 + 150, rand() % 2 + 5,
									   rand() % 50 + 90, 0,
									   true, randDouble(0.1, 0.7),
									   100, 10);


					// Left Turret of boss
					tempX = boss[i].x + boss[i].w/2-134 - rands/2;
					tempY = boss[i].y + boss[i].h/2 - rands/2;
					p_dummy.spawnParticleAngle(particle, 1,
									   tempX,
									   tempY,
									   rands, rands,
									   180.0, speed,
									   0.0,
									   {144, 144, 144, 255}, 1,
									   1, 1,
									   rand() % 100 + 150, rand() % 2 + 5,
									   rand() % 50 + 90, 0,
									   true, randDouble(0.1, 0.7),
									   100, 10);

					// Top Turret of boss
					tempX = boss[i].x + boss[i].w/2 - rands/2;
					tempY = boss[i].y + boss[i].h/2-134 - rands/2;
					p_dummy.spawnParticleAngle(particle, 1,
									   tempX,
									   tempY,
									   rands, rands,
									   270.0, speed,
									   0.0,
									   {144, 144, 144, 255}, 1,
									   1, 1,
									   rand() % 100 + 150, rand() % 2 + 5,
									   rand() % 50 + 90, 0,
									   true, randDouble(0.1, 0.7),
									   100, 10);*/

					// Play SFX
					Mix_PlayChannel(-1, sCast, 0);
				}
			}



			/////////////////////////// Handle Boss Animations ////////////////////////
			////////////////////////////////////////////////////////////////////////////

			// Camera level bounds
			if( boss[i].x < 0 ){
				boss[i].x = 0;
			}
			if( boss[i].y < 0 ){
				boss[i].y = 0;
			}
			if( boss[i].x+boss[i].w > map.w ){
				boss[i].x = map.w-boss[i].w;
			}
			if( boss[i].y+boss[i].h > map.h ){
				boss[i].y = map.h-boss[i].h ;
			}
			if (
				boss[i].flash) {

				boss[i].flash = false;
			}
		}
	}

	// Other classes:
}

void Boss::UpdateEditor(Boss boss[], int mex, int mey, int camx, int camy) {
	for (int i = 0; i < this->max; i++) {
		if (boss[i].alive) {

			//If the mouse is on the tile
			if (mex > boss[i].x && mex < boss[i].x + boss[i].w &&
				mey > boss[i].y && mey < boss[i].y + boss[i].h) {
				boss[i].mouse = true;
			} else {
				boss[i].mouse = false;
			}
		}
	}

	// Other classes:
}

void Boss::RenderShadow(SDL_Renderer *gRenderer, Boss boss[], int camx, int camy) {
	for (int i = 0; i < max; i++) {
		if (boss[i].alive) {
			// Render boss shadow
			gBossShadow.render(gRenderer, boss[i].x-3 - camx, boss[i].y+20 - camy,
										boss[i].w+6, boss[i].h+30);
		}
	}
}

void Boss::RenderBack(SDL_Renderer *gRenderer, Boss boss[], int camx, int camy) {
	for (int i = 0; i < max; i++) {
		if (boss[i].alive) {
			if (!boss[i].renderInFront) {

				// Walking around or idle
				if (boss[i].animState == -1)
				{
					gTexture.setColor(255, 255, 255);
				}

				// Walking around or idle
				else if (boss[i].animState == 0) {

					// If boss is on alert (sees player)
					if (boss[i].alert)
					{
						// Set color red
						gTexture.setColor(200, 0, 0);
					} else {
						// Set color normal
						gTexture.setColor(255, 255, 255);
					}
				}

				// Moving towards player animation
				else if (boss[i].animState == 1) {

				}

				// Charging-attack animation
				else if (boss[i].animState == 2) {
					gTexture.setColor(rand() % 255 + 1, rand() % 255 + 1, rand() % 255 + 1);
				}

				// Render boss
				if (boss[i].flash) {
					gTextureFlashed.setAlpha(255);
					gTextureFlashed.render(gRenderer, boss[i].x - camx, boss[i].y - camy,
												boss[i].w, boss[i].h, NULL,
												boss[i].angle);
				} else {
					gTexture.setAlpha(255);
					gTexture.render(gRenderer, boss[i].x - camx, boss[i].y - camy,
												boss[i].w, boss[i].h, NULL,
												boss[i].angle);
				}

				///////////////////////////////////////////////////////////////
				//-----------------------------------------------------------//
				//------------------------ Turrets --------------------------//

				// Render Middle Turret
				gMiddleTurret.setAlpha(255);
				gMiddleTurret.render(gRenderer, boss[i].x+boss[i].w/2-66/2 - camx,
												boss[i].y+boss[i].h/2-66/2 - camy,
												66, 66, NULL,
												boss[i].angleFacingTarget);

				float turretSize = 42;
				// Render Right Turret
				gTurret.setAlpha(255);
				float RightTurretX = 127;
				gTurret.render(gRenderer, boss[i].x+boss[i].w/2-turretSize/2 + RightTurretX - camx,
												boss[i].y+boss[i].h/2-turretSize/2 - camy,
												turretSize, turretSize, NULL,
												0.0);

				// Render Bottom Turret
				gMiddleTurret.setAlpha(255);
				float BotTurretY = 127;
				gTurret.render(gRenderer, boss[i].x+boss[i].w/2-turretSize/2 - camx,
												boss[i].y+boss[i].h/2-turretSize/2 + BotTurretY - camy,
												turretSize, turretSize, NULL,
												90.0);

				// Render Left Turret
				gTurret.setAlpha(255);
				float LeftTurretX = 127;
				gTurret.render(gRenderer, boss[i].x+boss[i].w/2-turretSize/2 - LeftTurretX - camx,
												boss[i].y+boss[i].h/2-turretSize/2 - camy,
												turretSize, turretSize, NULL,
												180.0);

				// Render Top Turret
				gTurret.setAlpha(255);
				float TopTurretY = 127;
				gTurret.render(gRenderer, boss[i].x+boss[i].w/2-turretSize/2 - camx,
												boss[i].y+boss[i].h/2-turretSize/2 - TopTurretY - camy,
												turretSize, turretSize, NULL,
												270.0);
				//------------------------ Turrets --------------------------//
				//-----------------------------------------------------------//
				///////////////////////////////////////////////////////////////
			}
		}
	}
}

void Boss::RenderFront(SDL_Renderer *gRenderer, Boss boss[], int camx, int camy) {
	for (int i = 0; i < max; i++) {
		if (boss[i].alive) {
			if (boss[i].renderInFront) {

				// Walking around or idle
				if (boss[i].animState == -1)
				{
					gTexture.setColor(255, 255, 255);
				}

				// Walking around or idle
				else if (boss[i].animState == 0) {

					// If boss is on alert (sees player)
					if (boss[i].alert)
					{
						// Set color red
						gTexture.setColor(200, 0, 0);
					} else {
						// Set color normal
						gTexture.setColor(255, 255, 255);
					}
				}

				// Moving towards player animation
				else if (boss[i].animState == 1) {

				}

				// Charging-attack animation
				else if (boss[i].animState == 2) {
					gTexture.setColor(rand() % 255 + 1, rand() % 255 + 1, rand() % 255 + 1);
				}

				// Render boss
				if (boss[i].flash) {
					gTextureFlashed.setAlpha(255);
					gTextureFlashed.render(gRenderer, boss[i].x - camx, boss[i].y - camy,
												boss[i].w, boss[i].h, NULL,
												boss[i].angle);
				} else {
					gTexture.setAlpha(255);
					gTexture.render(gRenderer, boss[i].x - camx, boss[i].y - camy,
												boss[i].w, boss[i].h, NULL,
												boss[i].angle);
				}

				///////////////////////////////////////////////////////////////
				//-----------------------------------------------------------//
				//------------------------ Turrets --------------------------//

				// Render Middle Turret
				gMiddleTurret.setAlpha(255);
				gMiddleTurret.render(gRenderer, boss[i].x+boss[i].w/2-66/2 - camx,
												boss[i].y+boss[i].h/2-66/2 - camy,
												66, 66, NULL,
												boss[i].angleFacingTarget);

				float turretSize = 42;
				// Render Right Turret
				gTurret.setAlpha(255);
				float RightTurretX = 127;
				gTurret.render(gRenderer, boss[i].x+boss[i].w/2-turretSize/2 + RightTurretX - camx,
												boss[i].y+boss[i].h/2-turretSize/2 - camy,
												turretSize, turretSize, NULL,
												0.0);

				// Render Bottom Turret
				gMiddleTurret.setAlpha(255);
				float BotTurretY = 127;
				gTurret.render(gRenderer, boss[i].x+boss[i].w/2-turretSize/2 - camx,
												boss[i].y+boss[i].h/2-turretSize/2 + BotTurretY - camy,
												turretSize, turretSize, NULL,
												90.0);
				// Render Left Turret
				gTurret.setAlpha(255);
				float LeftTurretX = 127;
				gTurret.render(gRenderer, boss[i].x+boss[i].w/2-turretSize/2 - LeftTurretX - camx,
												boss[i].y+boss[i].h/2-turretSize/2 - camy,
												turretSize, turretSize, NULL,
												180.0);

				// Render Top Turret
				gTurret.setAlpha(255);
				float TopTurretY = 127;
				gTurret.render(gRenderer, boss[i].x+boss[i].w/2-turretSize/2 - camx,
												boss[i].y+boss[i].h/2-turretSize/2 - TopTurretY - camy,
												turretSize, turretSize, NULL,
												270.0);
				//------------------------ Turrets --------------------------//
				//-----------------------------------------------------------//
				///////////////////////////////////////////////////////////////
			}
		}
	}
}

void Boss::RenderUI(SDL_Renderer *gRenderer, Boss boss[], int camx, int camy) {
	for (int i = 0; i < max; i++) {
		if (boss[i].alive) {
			const float yOffsetBar = 30;
			const float barHeight = 12;
			const float barWidth = boss[i].w*1.25;
			float uiX = boss[i].x + boss[i].w/2 - barWidth/2;
			float uiY = boss[i].y - barHeight - yOffsetBar;

			// Health Decay bar on Bosses
			{
				// Health Decay bar, bg
				RenderFillRect(gRenderer, uiX-camx, uiY-camy, (barWidth*boss[i].maxHealth)/boss[i].maxHealth, barHeight, {0, 0, 0} );

				// Render Decay health
				RenderFillRect(gRenderer, uiX-camx, uiY-camy, (barWidth*boss[i].healthDecay)/boss[i].maxHealth, barHeight, {60, 30, 30} );
			}

			// Health bar on Bosses
			{
				// Render health
				RenderFillRect(gRenderer, uiX-camx, uiY-camy, (barWidth*boss[i].health)/boss[i].maxHealth, barHeight, {200, 30, 30} );
			}

			// Boss UI
			// Text
			{
				// Render Text
				int uiX = screenWidth/2-gText.getWidth()/2;
				int uiY = 50;

				std::stringstream tempss;
				tempss << "THE FATTEST BOSS FIGHT EVER";
				gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {255, 255, 255}, gFont20);
				gText.render(gRenderer, uiX, uiY, gText.getWidth(), gText.getHeight());
			}

			// Health Decay top of screen
			{
				// Render health
				float barWidth = screenWidth * 0.85;

				float uiX = screenWidth/2 - barWidth/2;
				float uiY = 10;

				// Health Decay bar, bg
				RenderFillRect(gRenderer, uiX, uiY, (barWidth*boss[i].maxHealth)/boss[i].maxHealth, 35, {0, 0, 0} );

				// Render Decay health
				RenderFillRect(gRenderer, uiX, uiY, (barWidth*boss[i].healthDecay)/boss[i].maxHealth, 35, {60, 30, 30} );
			}

			// Health top of screen
			{
				// Render health
				float barWidth = screenWidth * 0.85;

				float uiX = screenWidth/2 - barWidth/2;
				float uiY = 10;

				// Render health
				RenderFillRect(gRenderer, uiX, uiY, (barWidth*boss[i].health)/boss[i].maxHealth, 35, {200, 30, 30} );
			}

		}
	}
}

void Boss::RenderDebug(SDL_Renderer *gRenderer, Boss boss[], int camx, int camy) {
	for (int i = 0; i < max; i++) {
		if (boss[i].alive) {

			// DEBUG SQAURES UNDER HERE

			// Render box around boss
			SDL_Rect tempRect = {boss[i].x-camx, boss[i].y-camy, boss[i].w, boss[i].h};
			SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
			SDL_RenderDrawRect(gRenderer, &tempRect);

			// Boss imaginary line that checks if there is a block in the next step
			// if not, then the boss turns around
			// Where we check this is inside 'checkBossTilecCollision()' in PlayGame class
			//SDL_Rect rectMiddle = {boss[i].x+boss[i].w/2-1-camx, boss[i].y+boss[i].h-camy, 2, boss[i].h+2};
			//SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255);
			//SDL_RenderDrawRect(gRenderer, &rectMiddle);

			// If mouse is hoveirng on Boss
			if (boss[i].mouse) {
				// Render Green box
				SDL_Rect tempr = {boss[i].x+3 - camx, boss[i].y+3 - camy, boss[i].w-6, boss[i].h-6};
				SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
				SDL_RenderDrawRect(gRenderer, &tempr);
			} else {
				// Render Red box
				SDL_Rect tempr = {boss[i].x+3 - camx, boss[i].y+3 - camy, boss[i].w-6, boss[i].h-6};
				SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255);
				SDL_RenderDrawRect(gRenderer, &tempr);
			}


			/// TEXT UNDER HERE

			// Render Text
			std::stringstream tempss;
			//tempss << "D: " << boss[i].distance << ", AS: " << boss[i].animState << ", AL: " << boss[i].alert << ", CH: " <<  boss[i].chargingAttack;
			tempss << boss[i].health;
			gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {255, 255, 255}, gFont);
			gText.render(gRenderer, boss[i].x-camx, boss[i].y-gText.getHeight()-camy, gText.getWidth(), gText.getHeight());

		}
	}
}

void Boss::RenderHand(SDL_Renderer *gRenderer, Boss boss[], int newMx, int newMy, int mex, int mey, int camx, int camy){
	// Render boss in hand
	gTexture.setAlpha(110);
	gTexture.render(gRenderer, newMx, newMy, 384, 384, NULL);

	// Render mouse coordinates snapped to grid
	SDL_Rect tempr = {newMx, newMy, 384, 384};
	SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
	SDL_RenderDrawRect(gRenderer, &tempr);
}



void Boss::setStatsBasedOnType(Boss boss[], int i) {
	// Set defeault parameters depending on what mobs were spawning
	if (boss[i].type == 0) {
		boss[i].w 			= 128*2;
		boss[i].h 			= 128*2;
		boss[i].health 		= 500;
		boss[i].maxHealth 	= 500;
		boss[i].healthDecay = 500;
		//boss[i].setSightRange(64*10);
		//boss[i].setAtkRange(64*4);
	}

}

// Functions that work with other classes
void Boss::GetDistanceOfPlayer(Boss boss[], float targetX, float targetY, float targetW, float targetH, float *xFollow, float *yFollow) {
	for (int i = 0; i < max; i++) {
		if (boss[i].alive) {

			////////////////////////////////////////////////////////////////////////////
			/////////////////////////// GET DISTANCE OF PLAYER /////////////////////////
			// Get center of attack-object (spawned by the player attacking)
			boss[i].xFollow = xFollow;
			boss[i].yFollow = yFollow;

			////////////////////////////////////////////////////////////////////////////
			/////////////////////////// GET DISTANCE OF PLAYER /////////////////////////
			// Get center of attack-object (spawned by the player attacking)
			boss[i].bmx = targetX+targetW/2;
			boss[i].bmy = targetY+targetH/2;

			// Get center of boss
			boss[i].bmx2 = boss[i].x+boss[i].w/2;
			boss[i].bmy2 = boss[i].y+boss[i].h/2;

			// Get angle of boss relative to attack-object
			float angle = atan2(boss[i].bmy - boss[i].bmy2, boss[i].bmx - boss[i].bmx2);
			angle = angle * (180 / 3.1416);
			if (angle < 0) {
				angle = 360 - (-angle);
			}

			// Get angle of boss relative to attack-object
			boss[i].angleFacingTarget = atan2(boss[i].bmy - boss[i].bmy2, boss[i].bmx - boss[i].bmx2);
			boss[i].angleFacingTarget = boss[i].angleFacingTarget * (180 / 3.1416);
			if (boss[i].angleFacingTarget < 0) { boss[i].angleFacingTarget = 360 - (-boss[i].angleFacingTarget); }

			// Handle radians, cos, sin
			float radians = (3.1415926536/180)*(angle);
			float Cos = floor(cos(radians)*10+0.5)/10;
			float Sin = floor(sin(radians)*10+0.5)/10;

			// Distance of boss relative to attack-object
			boss[i].distance = sqrt((boss[i].bmx - boss[i].bmx2) * (boss[i].bmx - boss[i].bmx2)+
								  (boss[i].bmy - boss[i].bmy2) * (boss[i].bmy - boss[i].bmy2));

			// If it goes less than 0, the game crashes
			if (boss[i].distance <= 0.01) {
				boss[i].distance = 0.01;
			}

			float distanceW = sqrt((bmx - bmx2) * (bmx - bmx2));
			float distanceH = sqrt((bmy - bmy2) * (bmy - bmy2));
			float tempVX 	= 0.5 * (bmx - bmx2) / distanceW;
			float tempVY 	= 0.5 * (bmy - bmy2) / distanceH;

			/////////////////////////// GET DISTANCE OF PLAYER /////////////////////////
			////////////////////////////////////////////////////////////////////////////

			// Check if player is in front of boss or not
			if (boss[i].y+boss[i].h > targetY+targetH){
				boss[i].renderInFront = true;
			} else {
				boss[i].renderInFront = false;
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////// LOAD-SAVE ///////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////
//------------------------------------ Load Boss data --------------------------------------//
// Load Boss data
void Boss::LoadData(Boss boss[], std::fstream &fileTileDataL)
{
	// Reset all bosss
	Init(boss);

	//std::cout << "Attempting to load... Boss" << std::endl;
	// Load Boss count data
	fileTileDataL >> this->count;
	//std::cout << this->count << std::endl;

	// Load rest of Boss data
	for (int h = 0; h < this->count; h++) {
		if (!boss[h].alive) {

			fileTileDataL >>
				boss[h].x 		>>
				boss[h].y	 	>>
				boss[h].w 		>>
				boss[h].h 		>>
				boss[h].radius >>
				boss[h].speed 	>>
				boss[h].type 	>>
				boss[h].damage >>
				boss[h].health >>
				boss[h].maxHealth >>
				boss[h].vX 	>>
				boss[h].vY 	>>
				boss[h].angle 	>>
				boss[h].alive;

			//std::cout << "Boss h: " << h << ", x: " << boss[h].x << ", y: " << boss[h].y << std::endl;

			// When we load bosss, spawn them 1 pixel from the ground so that we dont have glitches or problems
			boss[h].y-=2;

			// Defaults on loading
			{
				setStatsBasedOnType(boss, h);
			}
		}
		//break;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////
//------------------------------------ Save Boss data --------------------------------------//
// Save Tile data
std::string Boss::SaveData(Boss boss[])
{
	/////////////////////////////////////////////////////////////////////////////////
	//-----------------------------------------------------------------------------//
	//----------------------------- Save Everything in One File -------------------//

	// Save all tile data in a string first
	std::stringstream tempsstb;
	for (int i = 0; i < max; i++) {
		if (boss[i].alive) {
			tempsstb
				<< boss[i].x 			<< " "
				<< boss[i].y 			<< " "
				<< boss[i].w  			<< " "
				<< boss[i].h  			<< " "
				<< boss[i].radius  		<< " "
				<< boss[i].speed  		<< " "
				<< boss[i].type  		<< " "
				<< boss[i].damage  		<< " "
				<< boss[i].health  		<< " "
				<< boss[i].maxHealth  	<< " "
				<< boss[i].vX  			<< " "
				<< boss[i].vY  			<< " "
				<< boss[i].angle  		<< " "
			   	<< boss[i].alive 		<< "\n";
		}
	}
	std::string temps;
	temps = tempsstb.str().c_str();
	//temps = temps.substr(0, temps.size()-1);

	// Save number of tiles in World
	std::stringstream aVeryLongString;
	aVeryLongString << count << "\n";

	// Save the data of all Tiles
	aVeryLongString << temps.c_str();
	//----------------------------- Save Everything in One File -------------------//
	//-----------------------------------------------------------------------------//
	/////////////////////////////////////////////////////////////////////////////////

	return aVeryLongString.str().c_str();
}
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////// LOAD-SAVE ///////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////











