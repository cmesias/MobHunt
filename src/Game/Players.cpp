/*
 * Players.cpp
 *
 *  Created on: Dec 18, 2016
 *      Author: Carl
 */

#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_mouse.h>

//#include "Input.h"

#include "Players.h"

void Players::setSettings(Settings *settings) {
	this->settings = settings;
}

// Reset game
void Players::Init(float spawnX, float spawnY, std::string newName){
	this->x 				= spawnX;
	this->y 				= spawnY;
	this->name				= newName;

	this->sprite_index 		= 0;
	this->clash 			= false;
	this->score 			= 0;

	// Screen prompts
	this->deathScreen 		= false;
	this->alive 				= true;

	// Always reset these

	// Walking
	this->facing			= "down";
	this->moving			= false;
	this->moveleft 			= false;
	this->moveright 		= false;
	this->moveup 			= false;
	this->movedown			= false;
	this->walkTimer 		= 0;
	this->walkTimerVFX 		= 0;
	this->vX 				= 0.0;
	this->vY 				= 0.0;

	// Invulnerability ability
	this->invurnerableFrame	= 1;
	this->invurnerableTick	= 0;
	this->invurnerableT		= 300;
	this->invurnerable		= true;

	// Shoot Attack
	this->shootTimer 		= 0;
	this->shootDelay 		= false;
	this->initialshot 		= false;
	this->trigger 			= false;

	// Attack
	this->attackTimer 		= 0;
	this->attackFrame 		= 5;
	this->attack 			= false;
	this->spawnAttack 		= false;
	this->attackType		= -1;

	// Delay
	this->delayTimer		= 0;
	this->delay				= false;

	// Parry ability
	this->parry 			= false;
	this->parryTimer 		= 0;
	this->parryCDTimer 		= 0;
	this->parryCDMax		= 60*2.6;

	// Dash ability
	this->dash 				= false;
	this->dashSpeed 		= 10;
	this->dashLength 		= 5;
	this->dashCounter 		= 0;
	this->dashCoolCounter 	= 0;

	// Stunned ability
	this->stunned 			= false;
	this->stunTimer 		= 0;

	// Official player control keys
	this->pressedEquipKey 	= false;

	// TODO (3-9-2022) [ ] - Save these player stats in a file. This will be the players save data.

	// Inventory
	this->silverKeys			= 0;
	this->goldKeys				= 0;
	this->coins					= 0;

	// Health
	this->hearts 				= 3;
	this->healthDecay 			= 100;
	this->health				= 100;
	this->healthMax				= 100;

	// Damage
	this->castDamage			= 15;
	this->damageMultipler		= 1;
	this->castAtkSpe 			= 6.87;

	// Mana
	this->maxMana				= 100;
	this->mana					= this->maxMana;
	this->manaRegenTimer		= 0;
	this->manaRegenSpeed		= 8.75;
	this->manaGainOnParry		= 5.25;

	// Parry
	this->parryLength			= 15;

	// Other stats
	this->knockBackPower		= 1.58;

	// Set default sword: fists
	ResetLivesAndPlayer();

	// Load Player save data
	this->LoadData();
}

void Players::RespawnPlayer() {
	this->ActivateInvurnerability();
	this->StopMovement();
	this->StopSlashAttack();
	this->StopDashing();
	this->ResetDashing();
	this->health				= this->healthMax;
	this->walkTimer 			= 0;
	this->walkTimerVFX 			= 0;
}

void Players::ResetLivesAndPlayer() {

	// Name
	std::string newName;
	newName="AAA";

	// Set default sword: fists
	this->EquipSword(0, 10, 5);

	// Reset score and lives, and turn player alive
	this->score 				= 0;

	this->LoadData();
}



// Check collision between 2 objects
bool Players::checkCollision(int x, int y, int w, int h, int x2, int y2, int w2, int h2){
	bool collide;
	if (x+w > x2 && x < x2 + w2 && y+h > y2 && y < y2 + h2){
		collide = true;
	}else{
		collide = false;
	}
	return collide;
}

// Load asteroid resources
void Players::Load(SDL_Renderer* gRenderer)
{
    // load textures
	gPlayer.loadFromFile(gRenderer, "resource/gfx/zelda_pack/Character_SpriteSheet.png");
	gPlayerShadow.loadFromFile(gRenderer, "resource/gfx/player/player_shadow.png");
	gShield.loadFromFile(gRenderer, "resource/gfx/player/shield.png");
	gSwords.loadFromFile(gRenderer, "resource/gfx/author_0x72/0x72_16x16DungeonTileset_swords.png");

	// Clip player texture

	// Walk-down
		// 0:
		// 1:
		// 2:
		// 3:
		// 4:

		// 5: Walk-down slash 0
		// 6: Walk-down slash 1
		// 7: Walk-down slash 2
	rPlayer[0] = {0,0,64,64};
	rPlayer[1] = {64*1,0,64,64};
	rPlayer[2] = {64*2,0,64,64};
	rPlayer[3] = {64*3,0,64,64};
	rPlayer[4] = {64*4,0,64,64};
	rPlayer[5] = {64*5,0,64,64};
	rPlayer[6] = {64*6,0,64,64};
	rPlayer[7] = {64*7,0,64,64};

	// Walk-up
		// 8:
		// 9:
		// 10:
		// 11:
		// 12:

		// 13: Walk-up slash 0
		// 14: Walk-up slash 1
		// 15: Walk-up slash 2
	rPlayer[8] = {0,64*1,64,64};
	rPlayer[9] = {64*1,64*1,64,64};
	rPlayer[10] = {64*2,64*1,64,64};
	rPlayer[11] = {64*3,64*1,64,64};
	rPlayer[12] = {64*4,64*1,64,64};
	rPlayer[13] = {64*5,64*1,64,64};
	rPlayer[14] = {64*6,64*1,64,64};
	rPlayer[15] = {64*7,64*1,64,64};

	// Walk-right
		// 16:
		// 17:
		// 18:
		// 19:
		// 20:

		// 21: Walk-right slash 0
		// 22: Walk-right slash 1
		// 23: Walk-right slash 2
	rPlayer[16] = {0,64*2,64,64};
	rPlayer[17] = {64*1,64*2,64,64};
	rPlayer[18] = {64*2,64*2,64,64};
	rPlayer[19] = {64*3,64*2,64,64};
	rPlayer[20] = {64*4,64*2,64,64};
	rPlayer[21] = {64*5,64*2,64,64};
	rPlayer[22] = {64*6,64*2,64,64};
	rPlayer[23] = {64*7,64*2,64,64};

	// Walk-left
		// 24:
		// 25:
		// 26:
		// 27:
		// 28:

		// 29: Walk-left slash 0
		// 30: Walk-left slash 1
		// 31: Walk-left slash 2
	rPlayer[24] = {0,64*3,64,64};
	rPlayer[25] = {64*1,64*3,64,64};
	rPlayer[26] = {64*2,64*3,64,64};
	rPlayer[27] = {64*3,64*3,64,64};
	rPlayer[28] = {64*4,64*3,64,64};
	rPlayer[29] = {64*5,64*3,64,64};
	rPlayer[30] = {64*6,64*3,64,64};
	rPlayer[31] = {64*7,64*3,64,64};

	// Shield clips
	for (int i=0; i<7; i++){setClips(rShield[i], i*48, 0, 48, 48);}

	// Clip swords texture
	{
		for (int i=0; i<11; i++) {
			rSwords[i] = {i*10,0,10,21};
		}
		rSwords[11] = {110,0,7,17};
		rSwords[12] = {117,0,10,21};

		// Second row of swords
		rSwords[13] = {0,30,10,25};
		rSwords[14] = {11,30,8,27};
		rSwords[15] = {20,30,10,29};
		rSwords[16] = {32,30,6,27};
		rSwords[17] = {40,30,10,24};
		rSwords[18] = {50,30,10,24};

		// Broken swords
		rSwords[19] = {60,30,6,9};
		rSwords[20] = {70,30,6,12};
		rSwords[21] = {80,30,6,12};
		rSwords[22] = {90,30,6,12};

		// Bomb
		rSwords[23] = {100,30,14,13};

		// Heart
		rSwords[24] = {60,50,10,10};		// Heart (Used in UI, not in inventory or maybe it should? TODO)

		// Coin
		rSwords[25] = {70,50,10,10};

		// Silver key
		rSwords[26] = {80,45,7,15};

		// Golden key
		rSwords[27] = {87,45,7,15};
	}

	// Other classes
	LoadFonts();
}

// Free asteroid resources
void Players::Free(){

    // Free textures
	gPlayer.free();
    gPlayerShadow.free();
    gShield.free();
    gSwords.free();

	// Other classes
	FreeFonts();
}

// Player shoot
void Players::fire(Particle particle[], Particle &p_dummy, Mix_Chunk *sCastSFX, int mx, int my){

	// Determine controls
	trigger = initialshot;

	// Get player angle based on mouse coordinates
	angle = atan2(my - y-h/2,mx - x-w/2);
	angle = angle * (180 / 3.1416);
	//Set player angle max limits
	if (angle < 0) {
		angle = 360 - (-angle);
	}

	// Get radiant from angle
	radians = (3.1415926536/180)*(angle);
	radianCos = floor(cos(radians)*10+0.5)/10;
	radianSin = floor(sin(radians)*10+0.5)/10;

	// Shoot particle
	if (trigger)
	{
		// If we have enough mana
		if (this->mana >= 5)
		{
			if (!shootDelay)
			{
				shootDelay = true;

				// play audio
				Mix_PlayChannel(-1, settings->sCast, 0);

				// Offset to spawn the Slash Attack
				int offSetX =0;
				if (facing == "left") {
					offSetX = 32;
				}
				else if (facing == "right") {
					offSetX = 0;
				}
				float spawnX = x+w/2-offSetX;
				float spawnY = y+w/2;

				// Spawn Slash Attack particle
				p_dummy.spawnRedProjectileAttack(particle, 0,
						spawnX, spawnY,
						16, 16,
						this->angle, this->castDamage, 9);

				// Subtract mana
				this->mana -= 2;
			}
		}

		// If no more mana
		else {

			// Spawn no more mana VFX
			//p_dummy.spawnNoMoreManaVFX(particle, x2, y2);
		}
	}

	// Shoot delay
	if (shootDelay) {

		// Start timer
		shootTimer += castAtkSpe;

		// After 1 second
		if (shootTimer > 60) {

			// Stop delay
			shootDelay = false;

			// Reset shoot timer
			shootTimer = 0;
		}
	}
}

// Update Player
void Players::Update(Map &map,
					Particle particle[], Particle &p_dummy,
					Tile &tl, Tile tile[],
					Tilec &tlc, Tilec tilec[],
					Object &obj, Object object[],
					int mex, int mey, int camx, int camy,
					float spawnX, float spawnY,
					LWindow gWindow, SDL_Renderer* gRenderer,
					SDL_Color color,
					bool &RestartLevel,
					int LevelWeLoaded, bool &ShakeCamera)
{
	// Player center
	this->x2 = this->x+this->w/2;
	this->y2 = this->y+this->h/2;

	// Set arm location
	this->armX = this->x;
	this->armY = this->y - this->h;

	// Player alive
	if (this->alive)
	{

		// Player health decay
		if (this->healthDecay > this->health) {
			this->healthDecay -= 0.25;
		}
		if (this->healthDecay < this->health) {
			this->healthDecay += 0.25;
		}

		////////////////////////////////////////////////////////////////////////////////////
		//--------------------------------------------------------------------------------//
		//----------------------------------- Movement -----------------------------------//
		{
			// X Axis movement
			{
				// Move left
				if (this->moveleft && !this->attack && !this->parry && !this->dash) {
					if (this->vX > -this->velMax) {
						this->vX -= this->velSpeed;
					}
					this->moving = true;
					if (this->ControlsPreference == 0) {
				        if (!this->shift) {
				        	this->facing = "left";
				        	this->sprite_dir = 24;
				        }
					}
				}
				// Move right
				if (this->moveright && !this->attack && !this->parry && !this->dash) {
					if (this->vX < this->velMax) {
						this->vX += this->velSpeed;
					}
					this->moving = true;
					if (this->ControlsPreference == 0) {
				        if (!this->shift) {
				        	this->facing = "right";
				        	this->sprite_dir = 16;
				        }
					}
				}
			}

			// Y Axis movement
			{
				// Move up
				if ((this->moveup && !this->attack && !this->parry && !this->dash)) {
					if (this->vY > -this->velMax) {
						this->vY -= this->velSpeed;
					}
					this->moving = true;
					if (this->ControlsPreference == 0) {
				        if (!this->shift) {
				        	this->facing = "up";
				        	this->sprite_dir = 8;
				        }
					}
				}
				// Move down
				if (this->movedown && !this->attack && !this->parry && !this->dash) {
					if (this->vY < this->velMax) {
						this->vY += this->velSpeed;
					}
					this->moving = true;
					if (this->ControlsPreference == 0) {
				        if (!this->shift) {
				        	this->facing = "down";
				        	this->sprite_dir = 0;
				        }
					}
				}
			}

			// If ControlsPreference is "1" then we will override the direction above ^
			if (this->ControlsPreference == 1) {
				if (mex+camx > this->x) {
		        	this->facing = "right";
				}else {
		        	this->facing = "left";
				}

				// Facing right
				if (this->angle >= 315 || this->angle < 45) {
		        	this->facing = "right";
		        	this->sprite_dir = 16;
				}

				// Facing down
				else if (this->angle >= 45 && this->angle < 135) {
		        	this->facing = "down";
		        	this->sprite_dir = 0;
				}

				// Facing left
				else if (this->angle >= 135 && this->angle < 225) {
		        	this->facing = "left";
		        	this->sprite_dir = 24;
				}

				// Facing up
				else if (this->angle >= 225 && this->angle < 315) {
		        	this->facing = "up";
		        	this->sprite_dir = 8;
				}
			}

			// If not dashing
			if (!this->dash) {
				// Max X speed
				if (this->vX < -this->velMax) {
			        vX = vX - vX * 0.05;
				}
				if (this->vX > this->velMax) {
			        vX = vX - vX * 0.05;
				}
				// Max Y speed
				if (this->vY < -this->velMax) {
			        vY = vY - vY * 0.05;
				}
				if (this->vY > this->velMax) {
			        vY = vY - vY * 0.05;
				}
			}
		}
		//----------------------------------- Movement -----------------------------------//
		//--------------------------------------------------------------------------------//
		////////////////////////////////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////////////////////////////////
		//--------------------------------------------------------------------------------//
		//--------------------------- Movement & Collision w/ Tiles ----------------------//

		// Handle movement of player, and handle collision with Tiles
		tl.checkCollisionXY(tile,
				this->x, this->y,
				this->w, this->h,
				this->vX, this->vY);

		//--------------------------- Movement & Collision w/ Tiles ----------------------//
		//--------------------------------------------------------------------------------//
		////////////////////////////////////////////////////////////////////////////////////


	    ////////////////////////////////////////////////////////////////////////////
	    //------------------------------------------------------------------------//
	    //--------------------------------- Do !Parrying -------------------------//

		// Idle animation if not doing anything
		if (!this->attack && !this->dash && !this->parry) {

			// Idle animation
			if (!this->moving) {
				this->sprite_index = 0;

			// Moving animation
			} else {

				// If not attacking
				//if (!this->attack) {

					///////////////////////////////////////////////////////////////////////////
					//-----------------------------------------------------------------------//
					//----------------------------- Do walkTimer ----------------------------//
					{
						// Increment animation timer
						this->walkTimer += this->walkTimerSpe;

						// Increment current animation frame
						if (this->walkTimer > 60)
						{
							// Reset timer
							this->walkTimer = 0;
							// Go to next animation frame
							this->sprite_index++;
						}

						// Reset sprite
						if (this->sprite_index > 3) {
							this->sprite_index = 0;
						}

						// Adjust player shadow based on what frame we're currently rendering
						/*if (sprite_index == 0) {
							shadowW = 44;
						}else if (sprite_index == 1) {
							shadowW = 32;
						}else if (sprite_index == 2) {
							shadowW = 38;
						}else if (sprite_index == 3) {
							shadowW = 42;
						}*/
					}
					//----------------------------- Do walkTimer ----------------------------//
					//-----------------------------------------------------------------------//
					///////////////////////////////////////////////////////////////////////////

					///////////////////////////////////////////////////////////////////////////
					//-----------------------------------------------------------------------//
					//--------------------------- Do walkTimerVFX ---------------------------//
					{
						//	Increase walkTimerVFX
						walkTimerVFX += 1;

						// If walkTimer is at 30 frames
						if (walkTimerVFX == 15 || walkTimerVFX == 30 || walkTimerVFX == 45 || walkTimerVFX == 60)
						{
							// Visual and audio effects
							{
								// Spawn particle
								int tempAngel = 0;
								float adjustX = 0;
								if (facing == "left" ) {
									tempAngel = 0;
									adjustX = 8;
								} else if (facing == "right" ) {
									tempAngel = 180;
									adjustX = -8;
								}

								// Spawn size and pos
								int randSize = rand() % 5 + 5;

								float spawnX = this->x+this->w/2 + adjustX;
								float spawnY = this->y+this->h;

								// Spawn particle effect at feet
								p_dummy.spawnParticleAngle(particle, 3, 2,
													spawnX-randSize/2,
													spawnY-randSize/2,
													randSize, randSize,
												   tempAngel, randDouble(0.1, 0.4),
												   0.0, 0, 0,
												   {255, 255, 255, 255}, 1,
												   1, 1,
												   rand() % 100 + 150, rand() % 2 + 5,
												   rand() % 50 + 90, 0,
												   true, randDouble(0.1, 0.7),
												   100, 10);

								// Play sound effect
								Mix_PlayChannel(-1, settings->sStep, 0);
							}
						}

						// Reset walkTimerVFX
						if (walkTimerVFX > 60)
						{
							walkTimerVFX = 0;
						}
					}
					//--------------------------- Do walkTimerVFX ---------------------------//
					//-----------------------------------------------------------------------//
					///////////////////////////////////////////////////////////////////////////
				//}
			}
		}

		// Animations
		{
			///////////////////////////////////////////////////////////////////////////
			//-----------------------------------------------------------------------//
			//------------------------------- Do Dash -------------------------------//
			if (this->dash) {

				if (dashCounter >= 0 && dashCounter < 2) {
					sprite_index = 4;
				}
				else if (dashCounter >= 2 && dashCounter < 4) {
					sprite_index = 4;
				}
				else if (dashCounter >= 4 && dashCounter < 6) {
					sprite_index = 4;
				}
				else if (dashCounter >= 6 && dashCounter < 8) {
					sprite_index = 4;
				}
				else if (dashCounter >= 8 && dashCounter < 10) {
					sprite_index = 4;
				}

				int rands = rand() % 9 + 2;
				float newX = x+w/2;
				float newY = y+h;
				p_dummy.spawnParticleAngle(particle, 3, 2,
									newX-rands/2,
									newY-rands/2,
								   rands, rands,
								   0, randDouble(0.1, 0.3),
								   0.0, 0, 0,
								   {255, 255, 255, 255}, 1,
								   1, 1,
								   rand() % 100 + 150, rand() % 2 + 5,
								   rand() % 50 + 90, 0,
								   true, 0.11,
								   rand() % 9 + 2, 1);

				// If dash counter is greater than 0
				if (dashCounter > 0) {

					// Subtract dash counter by 1 every frame
					dashCounter -= 1;
				}
				// If dash counter goes lower than 0
				else {

					// Stop player movement
					StopMovement();

					// Dash on cool down
					dash = false;

					// Start dash cool down timer
					dashCoolCounter = dashCooldown;
				}
			}

			//------------------------------- Do Dash -------------------------------//
			//-----------------------------------------------------------------------//
			///////////////////////////////////////////////////////////////////////////
			///////////////////////////////////////////////////////////////////////////
			//-----------------------------------------------------------------------//
			//----------------------------- Do Attacking ----------------------------//
			// If attacking
			else if (this->attack)
			{
				if (!this->delay)
					{
					// If attack-type: Slash Attack
					if (this->attackType == 0)
					{
						// Stop player movement
						this->StopMovement();

						// Increase attack timer/frames
						this->attackTimer += this->atkAnimSpe;

						// If timer after 60 seconds
						if (this->attackTimer > 60) {
							this->attackTimer = 0;

							// Go to next attack frame
							this->sprite_index++;

							// Attack animation finished
							if (sprite_index == 7) {
								// Immediatly stop attacking
								{
									// Stop attacking
									this->spawnAttack = false;

									// Reset attack-type
									this->attackType = -1;
									this->attackTimer = 0;
									this->attack = false;

									// Stop shooting slashes
									this->initialshot = false;
								}

								// Spawn Player Attack Object
								{
									float spawnX = 0;
									float spawnY = 0;
									float spawnW = 64;
									float spawnH = 50;

									// Down
									if (this->sprite_dir == 0) {
										spawnX = 0;
										spawnY = 21;
										spawnW = 64;
										spawnH = 30;
									}
									// Up
									if (this->sprite_dir == 8) {
										spawnX = 0;
										spawnY = 0;
										spawnW = 64;
										spawnH = 50;
									}
									// Right
									if (this->sprite_dir == 16) {
										spawnX = 21;
										spawnY = 0;
										spawnW = 50;
										spawnH = 50;
									}
									// Left
									if (this->sprite_dir == 24) {
										spawnX = 0;
										spawnY = 0;
										spawnW = 50;
										spawnH = 50;
									}


									// Down
									/*if (this->sprite_dir == 0) {
										spawnX = 0;
										spawnY = 21;
										spawnW = 20;
										spawnH = 20;
									}
									// Up
									if (this->sprite_dir == 8) {
										spawnX = 0;
										spawnY = -21;
										spawnW = 20;
										spawnH = 20;
									}
									// Right
									if (this->sprite_dir == 16) {
										spawnX = this->w;
										spawnY = 0;
										spawnW = 20;
										spawnH = 20;
									}
									// Left
									if (this->sprite_dir == 24) {
										spawnX = -this->w;
										spawnY = 0;
										spawnW = 20;
										spawnH = 20;
									}*/

									obj.spawn(object, this->x+this->w/2-this->realw/2+spawnX,
												      this->y+this->h/2-this->realh/2+spawnY,
													  spawnW, spawnH, 0);

									// Spawn attack object (it wzill appear in the world for 1 frame then remove itself)
									//obj.spawn(object, this->x+this->w/2-spawnW/2 + spawnX,
									//				  this->y+this->h/2-spawnH/2 + spawnY,
									//				  spawnW, spawnH, 0);

									// Play slash sound effect
									Mix_PlayChannel(-1, settings->sSlash, 0);

									// Start delay
									this->delay = true;
								}
							}
						}

						// Set pre-slashing sprite(s)
						/*if (this->attackTimer <= 30) {

							// Set slashing sprite
							sprite_index = 5;
						}

						// Spawn attack here
						else if (this->attackTimer > 30 && this->attackTimer < 33) {

							// Set slashing sprite
							sprite_index = 6;

							// If are spawning an attack-object
							if (spawnAttack) {

								// Immediatly stop attacking
								spawnAttack = false;

								// Spawn attack object


								// Spawn swords attribute
								{
									// Wood Sword
									if (itemIndex == 1) {

										// Spawn projectile
										int rands  = 24;
										float speed  = 21;

										for (double j=0.0; j< 45.0; j+=9){
											p_dummy.spawnProjectileAttack(particle, 0,
													this->x + this->w/2 - rands/2,
													this->y + this->h/2 - rands/2,
													rands, rands, this->angle-22.5+j, this->castDamage, speed,
													{102, 71, 31});
										}

										// play audio
										Mix_PlayChannel(1, settings->sCast, 0);
									}
									// Rusty Sword
									if (itemIndex == 2) {

										// Spawn projectile
										int rands  = 24;
										float speed  = 21;

										for (double j=0.0; j< 45.0; j+=9){
											p_dummy.spawnProjectileAttack(particle, 0,
													this->x + this->w/2 - rands/2,
													this->y + this->h/2 - rands/2,
													rands, rands, this->angle-22.5+j, this->castDamage, speed,
													{72, 112, 133});
										}

										// play audio
										Mix_PlayChannel(1, settings->sCast, 0);
									}
									// Iron Sword
									if (itemIndex == 3) {

										// Spawn projectile
										int rands  = 24;
										float speed  = 21;

										for (double j=0.0; j< 45.0; j+=5){
											p_dummy.spawnProjectileAttack(particle, 0,
													this->x + this->w/2 - rands/2,
													this->y + this->h/2 - rands/2,
													rands, rands, this->angle-22.5+j, this->castDamage, speed,
													{120, 110, 121});
										}

										// play audio
										Mix_PlayChannel(1, settings->sCast, 0);
									}

									// Iron Sword w/ Red in center
									if (itemIndex == 4) {

										// Spawn projectile
										int rands  = 24;
										float speed  = 21;

										for (double j=0.0; j< 45.0; j+=5){
											p_dummy.spawnProjectileAttack(particle, 0,
													this->x + this->w/2 - rands/2,
													this->y + this->h/2 - rands/2,
													rands, rands, this->angle-22.5+j, this->castDamage, speed,
													{200, 95, 101});
										}

										// play audio
										Mix_PlayChannel(1, settings->sCast, 0);
									}

									// Silver Sword
									if (itemIndex == 5) {

										// Spawn projectile
										int rands  = 24;
										float speed  = 21;

										for (double j=0.0; j< 45.0; j+=5){
											p_dummy.spawnProjectileAttack(particle, 0,
													this->x + this->w/2 - rands/2,
													this->y + this->h/2 - rands/2,
													rands, rands, this->angle-22.5+j, this->castDamage, speed,
													{106, 95, 101});
										}

										// play audio
										Mix_PlayChannel(1,settings->sCast, 0);
									}

									// Yellow Sword
									if (itemIndex == 6) {

										// Spawn projectile
										int rands  = 24;
										float speed  = 21;

										for (double j=0.0; j< 45.0; j+=5){
											p_dummy.spawnProjectileAttack(particle, 0,
													this->x + this->w/2 - rands/2,
													this->y + this->h/2 - rands/2,
													rands, rands, this->angle-22.5+j, this->castDamage, speed,
													{244, 144, 25});
										}



										// play audio
										Mix_PlayChannel(1, settings->sCast, 0);
									}

									// Yellow Sword
									if (itemIndex == 7) {

										// Spawn projectile
										int rands  = 24;
										float speed  = 21;

										for (double j=0.0; j< 45.0; j+=5){
											p_dummy.spawnProjectileAttack(particle, 0,
													this->x + this->w/2 - rands/2,
													this->y + this->h/2 - rands/2,
													rands, rands, this->angle-22.5+j, this->castDamage, speed,
													{255, 188, 120});
										}



										// play audio
										Mix_PlayChannel(1, settings->sCast, 0);
									}

									// Black sword
									if (itemIndex == 8) {
										// Spawn projectile
										int rands  = 24;
										float speed  = 21;
										p_dummy.spawnBlackVoidProjectileAttack(particle, 0,
												this->x + this->w/2 - rands/2,
												this->y + this->h/2 - rands/2,
												rands, rands, this->angle, this->castDamage, speed);

										// play audio
										Mix_PlayChannel(1,settings->sCast, 0);
									}

									// Green sword
									if (itemIndex == 9) {

										// Spawn projectile
										int rands  = 24;
										float speed  = 21;

										for (double j=0.0; j< 45.0; j+=9){
											p_dummy.spawnProjectileAttack(particle, 0,
													this->x + this->w/2 - rands/2,
													this->y + this->h/2 - rands/2,
													rands, rands, this->angle-22.5+j, this->castDamage, speed,
													{20, 200, 20});
										}



										// play audio
										Mix_PlayChannel(1,settings->sCast, 0);

									}

									// Red sword
									if (itemIndex == 10) {
										// Spawn projectile
										int rands  = 24;
										float speed  = 21;
										p_dummy.spawnRedProjectileAttack(particle, 0,
												this->x + this->w/2 - rands/2,
												this->y + this->h/2 - rands/2,
												rands, rands, this->angle, this->castDamage, speed);

										// play audio
										Mix_PlayChannel(1,settings->sCast, 0);
									}

									// If we have a rapier or long rapier
									if (itemIndex == 11 || itemIndex == 14) {

										// Spawn Slash Attack particle
										p_dummy.spawnSlashAttackProjectile(particle, 0, armX, armY,
												this->particleW, this->particleH, this->castDamage, angle);

										// play audio
										Mix_PlayChannel(1,settings->sCast, 0);

									}

									// Shake camera
									//ShakeCamera = true;
								}
							}
						}

						// Set slashing sprite(s)
						else if (this->attackTimer <= 60) {

							// Set slashing sprite
							sprite_index = 6;
						}

						// At frame 16, spawn attack collision
						else{

							// Set slashing sprite
							sprite_index = 7;
						}
						// Attack over
						if (attackTimer > 60) {
							// Reset attack-type
							attackType = -1;
							attackTimer = 0;
							attack = false;

							// Stop shooting slashes
							this->initialshot = false;
						}*/
					}

					// Handle Down-Stab attack
					/*else if (attackType == 1)
					{
						// Set animation until Player hits the ground
						sprite_index = 7;

						// If are spawning an attack-object
						if (spawnAttack) {
							spawnAttack = false;

							// Spawn down-stab object
							obj.spawnFollow(object, x, y, 28, 20, 1, &swordX, &swordY);
						}

						// If player is on the ground
						if (jumpstate=="ground") {
							//attack = false;

							// If we can play the sound effect
							if (playSFXDownStabHittingFloor) {
								// Stop playing sound effect
								playSFXDownStabHittingFloor = false;

								// Player sound effect
								//Mix_PlayChannel(-1, sDownStab, 0);
							}

							// Attack timer
							attackTimer++;

							// After 0.25 second or 15 frames
							// Attack over
							if (attackTimer > 15)
							{
								// Reset attack-type
								attackType = -1;

								// Enable play sound effect next time we down stab
								playSFXDownStabHittingFloor = true;

								// Stop attack
								attackTimer = 0;
								attack = false;
							}
						}
					}*/
				}
			}

			// Start delay after attacking
			if (this->delay)
			{
				// Start delay (the faster the attakc speed, the sooner the delay ends
				this->delayTimer += this->atkSpeed;

				// Delay over
				if (this->delayTimer > 60)
				{
					// Stop delay
					this->delayTimer = 0;
					this->delay = false;
				}
			}
			//----------------------------- Do Attacking ----------------------------//
			//-----------------------------------------------------------------------//
			///////////////////////////////////////////////////////////////////////////

			///////////////////////////////////////////////////////////////////////////
			//-----------------------------------------------------------------------//
			//------------------------------ Do Parrying ----------------------------//
			// Parrying animation
			if (this->parry)
			{
				// Stop movement
				StopMovement();

				// Determine direction
				this->sprite_index = 3;

				// Start Parrying timer
				this->parryTimer++;

				// Parry for 15 frames
				if (this->parryTimer > this->parryLength){
					this->parryTimer = 0;
					this->parry = false;
					//this->StopParrying();
				}
			// Parry cool-down
			} else if (!this->parry) {
				if (this->parryCDTimer > 0) {
					this->parryCDTimer -= 1;
				}
			}
			//------------------------------ Do Parrying ----------------------------//
			//-----------------------------------------------------------------------//
			///////////////////////////////////////////////////////////////////////////

			///////////////////////////////////////////////////////////////////////////
			//-----------------------------------------------------------------------//
			//----------------------------- Do NOT Dash -----------------------------//

			// If dash on cooldown
			if (!this->dash) {

				// Start cooldown countdown
				if (this->dashCoolCounter > 0) {
					this->dashCoolCounter -= 1;
				}
			}

			//----------------------------- Do NOT Dash -----------------------------//
			//-----------------------------------------------------------------------//
			///////////////////////////////////////////////////////////////////////////


		} // end Animations



		//------------------------------ Do Parrying ----------------------------//
		//-----------------------------------------------------------------------//
		///////////////////////////////////////////////////////////////////////////

		///////////////////////////////////////////////////////////////////////////
		//-----------------------------------------------------------------------//
		//------------------------------- Do Stunned ----------------------------//
	    // Stunned
	    /*if (this->stunned) {
	        if (this->facing == "left") {
	        	this->sprite_index = 3;
	        }
	        if (facing == "right") {
	        	this->sprite_index = 3;
	        }
	        this->stunTimer += 1;
	        if (this->stunTimer > 90) {
	        	this->stunTimer = 0;
	            this->stunned = false;
	        }
	    }*/
		//------------------------------- Do Stunned ----------------------------//
		//-----------------------------------------------------------------------//
		///////////////////////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////////////////////////////////
		//--------------------------------------------------------------------------------//
		//-------------------------------- Stop Movement ---------------------------------//

		// Player not moving X
		if (!moveleft && !moveright && !dash) {
	        vX = vX - vX * 0.6;
		}

		// Player not moving Y
		if (!moveup && !movedown && !dash) {
	        vY = vY - vY * 0.6;
		}

		// Player not moving
		if (!moveup && !movedown && !moveleft && !moveright && !dash) {
			moving = false;
		}

		//-------------------------------- Stop Movement ---------------------------------//
		//--------------------------------------------------------------------------------//
		////////////////////////////////////////////////////////////////////////////////////

		// Player shoot
		this->fire(particle, p_dummy, settings->sCast, mex+camx, mey+camy);

		// Player shield
		if (this->invurnerable){
			this->invurnerableT-=1;
			this->invurnerableTick += 20;
			if (this->invurnerableTick>60){
				this->invurnerableTick = 0;
				this->invurnerableFrame += 1;
			}
			if (this->invurnerableFrame>7){
				this->invurnerableFrame = 1;
				this->invurnerableTick = 0;
			}
			if (this->invurnerableT<=0){
				this->invurnerableT= 300;
				this->invurnerable = false;
			}
		}

		// Check high score MAX
		if (this->score > 999999999){
			this->score = 999999999;
		}

		// Player death, lose hearts
		if (this->health <=0)
		{
			// Spawn blood VFX
			p_dummy.spawnBloodVFX(particle, this->x, this->y, this->w, this->h, {255,0,0});

			// Take away hearts
			this->hearts-= 1;

			// Reset Player
			this->RespawnPlayer();

			// Player ran out of hearts, que Death Screen
			if (this->hearts <= 0 ){

				// Load last checkpoint
				this->x = this->checkpointX;
				this->y = this->checkPointY;

				// Bring player back to life
				this->alive 			= false;

				// Show deathscreen
				this->deathScreen 	= true;

				// Save high score
				SaveHighScore(LevelWeLoaded);

			}
		}

		// Update Player score and Wave
		if (this->score > this->highscore){
			this->highscore = this->score;
		}

		// Player level boundaries
		if( this->x < map.x ){
			this->x = map.x;
		}
		if( this->y < map.y ){
			this->vY = 0.0;
			this->y = map.y;
		}
		if( this->x+this->realw > map.w ){
			this->x = map.w-this->realw;
		}
		if( this->y+realh > map.h ){
			this->y = map.h-this->realh;
		}

		// Regain mana if not attacking
		if (!this->attack) {
			// Every frame player regenerates mana
			if (this->mana < this->maxMana)
			{
				// Start timer
				this->manaRegenTimer += 1;

				// After 1 second or 60 frames
				if (this->manaRegenTimer > 60) {

					// Reset timer
					this->manaRegenTimer = 0;

					// Increase mana based on mana regen speed
					this->mana += this->manaRegenSpeed;
				}
			}
		}

		// Check if mana spilling over max
		if (this->mana > this->maxMana)
		{
			this->mana = this->maxMana;
		}

		// Every frame reduce damage multiplier by 2%
		if (this->damageMultipler > 1.0025) {
			this->damageMultipler -= 0.0025;
		}


	}else{
		// High-Score moving
		this->travel += 0.05 * this->dir;
		if (this->travel > 10){
			this->dir = -1;
		}
		if (this->travel < -10){
			this->dir = 1;
		}

		// Continue YES or NO Screen
		if (this->deathScreen)
		{
			// Set button position
			continueButton[0] = {0 + screenWidth/2 -96/2-100, screenHeight/2-gText.getHeight()/2, 96, 33};
			continueButton[1] = {0 + screenWidth/2 -96/2+100, screenHeight/2-gText.getHeight()/2, 96, 33};
			continueButton[2] = {0 + screenWidth/2 -256/2 , screenHeight-gText.getHeight()-72, 256, 39};

			// High Score display position
			position  = 0+travel;
			position2 = continueButton[1].x+continueButton[1].w-gText.getWidth()+travel;

			// Left click
			if (leftclick)
			{
				// Option: Yes, reset everything
				if (checkCollision(mex, mey, 1, 1, continueButton[0].x, continueButton[0].y, continueButton[0].w, continueButton[0].h))
				{
					// Restores players base stats and health
					leftclick			= false;

					// Reset Player
					ResetLivesAndPlayer();
					RestartLevel = true;
				}

				// Option: No, go to Main Menu
				if (checkCollision(mex, mey, 1, 1, continueButton[1].x, continueButton[1].y, continueButton[1].w, continueButton[1].h))
				{
					score = 0;
					leftclick  = false;
					deathScreen = false;
				}

				// Option: Reset high scores
				if (checkCollision(mex, mey, 1, 1, continueButton[2].x, continueButton[2].y, continueButton[2].w, continueButton[2].h))
				{
					// Reset high scores
					ResetHighScore(LevelWeLoaded);
				}
			}
		// Menu Screen
		}else{
			SDL_ShowCursor(true);
			// Set button position
			continueButton[0] = {screenWidth/2-96/2, screenHeight/2-gText.getHeight()/2, 96, 33};
			continueButton[2] = {screenWidth/2-256/2 , screenHeight-gText.getHeight()-72, 256, 39};

			// High Score display position
			position 	= continueButton[0].w/2-gText.getWidth()/2-100+travel;
			position2 = continueButton[0].x+continueButton[0].w/2-gText.getWidth()/2+100+travel;

			// Left click
			if (leftclick)
			{
				// Option: Play
				if (checkCollision(mex, mey, 1, 1, continueButton[0].x, continueButton[0].y, continueButton[0].w, continueButton[0].h))
				{
					leftclick			= false;

					// Reset Player
					ResetLivesAndPlayer();
					RestartLevel = true;
				}

				// Option: Reset high scores
				if (checkCollision(mex, mey, 1, 1, continueButton[2].x, continueButton[2].y, continueButton[2].w, continueButton[2].h))
				{
					// Reset high scores
					ResetHighScore(LevelWeLoaded);
				}
			}
		}
	}

	// Max values
	if (this->coins > this->coinsMax) {
		this->coins = this->coinsMax;
	}
}

// Render Player
void Players::Render(int mx, int my, int camx, int camy, LWindow gWindow, SDL_Renderer* gRenderer,
					 SDL_Color color, int &PARTICLES) {
	gText.setAlpha(255);
	// If alive
	if (alive){

		// render player
		{
			// Render shadow
			{
				/*int shadowW = 24;
				int shadowH = 10;
				gPlayerShadow.setAlpha(110);
				gPlayerShadow.render(gRenderer, x+w/2-realw/2-shadowW/2-camx,
												y+h+yOffset-shadowH/2-camy,
												shadowW, shadowH, NULL, 0.0, NULL);*/
			}

			// Render player
			gPlayer.render(gRenderer, x+w/2 - realw/2-camx, y + yOffset-camy, realw, realh, &rPlayer[sprite_index + this->sprite_dir]);

		}

		// Render Shield
		if (invurnerable){
			gShield.render(gRenderer, x+w/2 - 64/2 - camx, y+realh/2 + yOffset - 64/2 - camy, 64, 64, &rShield[invurnerableFrame], 180);
		}
	}
}

void Players::RenderUI(SDL_Renderer *gRenderer, int camX, int camY, int CurrentLevel)
{
	// Player UI
	{
		// Show death screen if not alive
		if (!this->alive){
			// Continue YES or NO Screen
			if (deathScreen)
			{
				// Render Text
				gText.loadFromRenderedText(gRenderer, "You have died. Continue?", {255,255,255}, gFont12);
				gText.render(gRenderer, screenWidth/2-gText.getWidth()/2, screenHeight/2-gText.getHeight()/2-50, gText.getWidth(), gText.getHeight());

				// Render buttons: Yes
				SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
				SDL_RenderDrawRect(gRenderer, &continueButton[0]);

				// Render buttons: No
				SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255);
				SDL_RenderDrawRect(gRenderer, &continueButton[1]);

				// Render buttons: ResetHighScore
				SDL_SetRenderDrawColor(gRenderer, 0, 255, 255, 255);
				SDL_RenderDrawRect(gRenderer, &continueButton[2]);

				// Render button texts: Yes or No
				gText.loadFromRenderedText(gRenderer, "Yes", {255,255,255}, gFont12);
				gText.render(gRenderer,  continueButton[0].x+continueButton[0].w/2-gText.getWidth()/2,
										 continueButton[0].y+continueButton[0].h/2-gText.getHeight()/2,
										 gText.getWidth(), gText.getHeight());

				gText.loadFromRenderedText(gRenderer, "No", {255,255,255}, gFont12);
				gText.render(gRenderer,  continueButton[1].x+continueButton[1].w/2-gText.getWidth()/2,
										 continueButton[1].y+continueButton[1].h/2-gText.getHeight()/2,
										 gText.getWidth(), gText.getHeight());

				// Render Text
				gText.loadFromRenderedText(gRenderer, "Reset High Scores", {255,255,255}, gFont12);
				gText.render(gRenderer, continueButton[2].x+continueButton[2].w/2-gText.getWidth()/2,
										 continueButton[2].y+continueButton[2].h/2-gText.getHeight()/2,
										 gText.getWidth(), gText.getHeight());
			// Player Menu screen
			}else{

				// Render buttons: Play
				SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
				SDL_RenderDrawRect(gRenderer, &continueButton[0]);

				// Render buttons: ResetHighScore
				SDL_SetRenderDrawColor(gRenderer, 0, 255, 255, 255);
				SDL_RenderDrawRect(gRenderer, &continueButton[2]);

				// Render Text
				gText.loadFromRenderedText(gRenderer, "PLAY", {255,255,255}, gFont12);
				gText.render(gRenderer, continueButton[0].x+continueButton[0].w/2-gText.getWidth()/2,
										 continueButton[0].y+continueButton[0].h/2-gText.getHeight()/2,
										 gText.getWidth(), gText.getHeight());

				// Render Text
				//gText.loadFromRenderedText(gRenderer, "Reset High Scores", color, gFont12);
				//gText.render(gRenderer, continueButton[2].x+continueButton[2].w/2-gText.getWidth()/2,
				//						 continueButton[2].y+continueButton[2].h/2-gText.getHeight()/2,
				//						 gText.getWidth(), gText.getHeight());
			}
		}


		// Health
		{
			// Health bar
			/*int uiX = screenWidth * 0.95 - 100 - 10;
			int uiY = screenHeight * 0.96 - 20 - 20 - 48 - 6;

			gText.loadFromRenderedText(gRenderer, "Health", {255,255,255}, gFont13);
			gText.render(gRenderer,  uiX-gText.getWidth()-2, uiY, gText.getWidth(), gText.getHeight());

			// Render health, bg
			int barWidth = 150;
			SDL_Rect tempRect = {uiX, uiY, (barWidth*this->healthMax)/this->healthMax, 24};
			SDL_SetRenderDrawColor(gRenderer, 60, 60, 60, 255);
			SDL_RenderFillRect(gRenderer, &tempRect);

			// Render health
			tempRect = {uiX + 1, uiY + 1, ((barWidth*this->health)/this->healthMax) - 2, 24-2};
			SDL_SetRenderDrawColor(gRenderer, 255, 30, 30, 255);
			SDL_RenderFillRect(gRenderer, &tempRect);

			// Render health, border
			tempRect = {uiX, uiY, (barWidth*this->healthMax)/this->healthMax, 24};
			SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
			SDL_RenderDrawRect(gRenderer, &tempRect);*/

			const float yOffsetBar = 7;
			const float barHeight = 12;
			const float barWidth = this->w*1.75;
			float uiX = this->x + this->w/2 - barWidth/2;
			float uiY = this->y - this->h - barHeight - yOffsetBar;

			// Health Decay bar on Mobes
			{
				// Health Decay bar, bg
				RenderFillRect(gRenderer, uiX-camX, uiY-camY, (barWidth*this->healthMax)/this->healthMax, barHeight, {0, 0, 0} );

				// Render Decay health
				RenderFillRect(gRenderer, uiX-camX, uiY-camY, (barWidth*this->healthDecay)/this->healthMax, barHeight, {30, 60, 30} );
			}

			// Health bar on Mobes
			{
				// Render health
				RenderFillRect(gRenderer, uiX-camX, uiY-camY, (barWidth*this->health)/this->healthMax, barHeight, {30, 200, 30} );
			}


		}

		/*// Mana
		{
			// Mana bar
			int uiX = screenWidth * 0.95 - 100 - 10;
			int uiY = screenHeight * 0.96 - 20 - 20 - 24 - 6;

			gText.loadFromRenderedText(gRenderer, "Mana", {255,255,255}, gFont13);
			gText.render(gRenderer,  uiX-gText.getWidth()-2, uiY, gText.getWidth(), gText.getHeight());

			// Render mana, bg
			int barWidth = 150;
			SDL_Rect tempRect = {uiX, uiY, (barWidth*this->maxMana)/this->maxMana, 24};
			SDL_SetRenderDrawColor(gRenderer, 60, 60, 60, 255);
			SDL_RenderFillRect(gRenderer, &tempRect);

			// Render mana
			tempRect = {uiX + 1, uiY + 1, ((barWidth*this->mana)/this->maxMana) - 2, 24-2};
			SDL_SetRenderDrawColor(gRenderer, 0, 90, 200, 255);
			SDL_RenderFillRect(gRenderer, &tempRect);

			// Render mana, border
			tempRect = {uiX, uiY, (barWidth*this->maxMana)/this->maxMana, 24};
			SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
			SDL_RenderDrawRect(gRenderer, &tempRect);
		}

		// Parry CD
		{
			// Parry bar
			int uiX = screenWidth * 0.95 - 100 - 10;
			int uiY = screenHeight * 0.96 - 20 - 20;

			gText.loadFromRenderedText(gRenderer, "Parry CD ", {255,255,255}, gFont13);
			gText.render(gRenderer,  uiX-gText.getWidth()-2, uiY, gText.getWidth(), gText.getHeight());

			// Render Parry, bg
			int barWidth = 150;
			SDL_Rect tempRect = {uiX, uiY, (barWidth*this->parryCDMax)/this->parryCDMax, 24};
			SDL_SetRenderDrawColor(gRenderer, 60, 60, 60, 255);
			SDL_RenderFillRect(gRenderer, &tempRect);

			// Render Parry
			int tempN = this->parryCDTimer - this->parryCDMax;
			tempRect = {uiX + 1, uiY + 1, ((barWidth*(-tempN))/this->parryCDMax) - 2, 24-2};
			SDL_SetRenderDrawColor(gRenderer, 0, 220, 220, 255);
			SDL_RenderFillRect(gRenderer, &tempRect);

			// Render Parry, border
			tempRect = {uiX, uiY, (barWidth*this->parryCDMax)/this->parryCDMax, 24};
			SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
			SDL_RenderDrawRect(gRenderer, &tempRect);
		}

		// Dash counter
		{
			// Dash bar
			int uiX = screenWidth * 0.95 - 100 - 10;
			int uiY = screenHeight * 0.96 - 20 - 20 + 24 + 6;

			gText.loadFromRenderedText(gRenderer, "Dash CD ", {255,255,255}, gFont13);
			gText.render(gRenderer,  uiX-gText.getWidth()-2, uiY, gText.getWidth(), gText.getHeight());

			// Render dash, bg
			int barWidth = 150;
			SDL_Rect tempRect = {uiX, uiY, (barWidth*this->maxMana)/this->maxMana, 24};
			SDL_SetRenderDrawColor(gRenderer, 60, 60, 60, 255);
			SDL_RenderFillRect(gRenderer, &tempRect);

			// Render dash
			int tempN = this->dashCooldown - this->dashCoolCounter;
			tempRect = {uiX + 1, uiY + 1, ((barWidth*tempN)/this->dashCooldown) - 2, 24-2};
			SDL_SetRenderDrawColor(gRenderer, 200, 200, 20, 255);
			SDL_RenderFillRect(gRenderer, &tempRect);

			// Render dash, border
			tempRect = {uiX, uiY, (barWidth*this->maxMana)/this->maxMana, 24};
			SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
			SDL_RenderDrawRect(gRenderer, &tempRect);
		}*/
	}

	// Hearts
	{
		// Render number of hearts left
		int marginW = 16;
		int marginH = 16;
		int size = 32;
		for (int i=0; i<hearts; i++){
			//int tempX = screenWidth - ((1+i)*32) - marginW;
			int tempX = marginW + i*32;
			int tempY = marginH;
			gSwords.render(gRenderer, tempX, tempY, size, size,
									  &rSwords[24], 0, NULL);
		}
	}

	// Used by all 3 below
	int marginW = 32;
	int marginH = 32*2 + 4;
	int tempX = marginW;
	//int tempX = screenWidth - marginW;
	int tempY;
	std::stringstream tempsi;

	// Coins
	{
		tempX = screenWidth * 1.00;
		tempY = screenHeight * 1.00;
		// Render Coins
		tempsi.str( std::string() );
		if (this-> coins >= 0 && this-> coins <= 9) {
			tempsi  << "00" << this->coins;
		} else if (this-> coins >= 10 && this-> coins < 99) {
			tempsi  << "0" << this->coins;
		} else {
			tempsi << this->coins;
		}

		gText.loadFromRenderedText(gRenderer, tempsi.str().c_str(), {255, 255, 255}, gFont12);
		gText.render(gRenderer,
				tempX - gText.getWidth() - 30,
				tempY - gText.getHeight() - 3 - 15,
				gText.getWidth(), gText.getHeight());

		// Render Icon
		for (int i=0; i<coins+1; i++){
			gSwords.render(gRenderer,
					tempX - 24 - gText.getWidth() - 30 - 10,
					tempY - 24 - 15,
					24, 24,
					&rSwords[25], 0, NULL);
		}

		// Render "x"
		//tempsi.str( std::string() );
		//tempsi  << "x";
		//gText.loadFromRenderedText(gRenderer, tempsi.str().c_str(), {255, 255, 255}, gFont12);
		//gText.render(gRenderer, screenWidth * 0.23 + 24/2 - gText.getWidth()/2, tempY  + 24 + gText.getHeight()/6, gText.getWidth(), gText.getHeight());
	}

	// Silver keys
	{
		// Render Icon
		int marginW = 24;
		int marginH = 16;
		int spacing = 10;
		int sizeW = 16;
		int sizeH = 32;
		int sizeOfHearts = 32;
		for (int i=0; i<silverKeys; i++){
			int tempX = marginW + i*sizeW;
			int tempY = marginH + sizeOfHearts + spacing;
			gSwords.render(gRenderer, tempX, tempY, sizeW, sizeH,
									  &rSwords[26], 0, NULL);
		}

		// Render Text
		/*tempsi.str( std::string() );
		tempsi << " x " <<this->silverKeys;
		gText.loadFromRenderedText(gRenderer, tempsi.str().c_str(), {255, 255, 255}, gFont12);
		gText.render(gRenderer, tempX + 32 + 4, tempY, gText.getWidth(), gText.getHeight());*/
	}

	// Gold keys
	{
		// Render Icon
		int marginW = 24;
		int marginH = 16;
		int spacing = 5;
		int sizeW = 16;
		int sizeH = 32;
		int sizeOfHearts = 32;
		for (int i=0; i<goldKeys; i++){
			int tempX = marginW + i*sizeW;
			int tempY = (marginH + sizeOfHearts)*2 + spacing;
			gSwords.render(gRenderer, tempX, tempY, sizeW, sizeH,
									  &rSwords[27], 0, NULL);
		}

		// Render Text
		/*tempsi.str( std::string() );
		tempsi  << " x " << this->goldKeys;
		gText.loadFromRenderedText(gRenderer, tempsi.str().c_str(), {255, 255, 255}, gFont12);
		gText.render(gRenderer, tempX + 32 + 4, tempY, gText.getWidth(), gText.getHeight());*/
	}

	// Bombs
	{
		// Render Icon
		int marginW = 24;
		int marginH = 16;
		int spacing = 5;
		int sizeW = 16;
		int sizeH = 32;
		int sizeOfHearts = 32;
		for (int i=0; i<bombs; i++){
			int tempX = marginW + i*sizeW;
			int tempY = (marginH + sizeOfHearts)*3 + spacing;
			gSwords.render(gRenderer, tempX, tempY, sizeW, sizeH,
									  &rSwords[23], 0, NULL);
		}

		// Render Text
		/*tempsi.str( std::string() );
		tempsi  << " x " << this->bombs;
		gText.loadFromRenderedText(gRenderer, tempsi.str().c_str(), {255, 255, 255}, gFont12);
		gText.render(gRenderer, tempX + 32 + 4, tempY, gText.getWidth(), gText.getHeight());*/
	}


	// Text UI
	{
		// Highscore text
		/*tempsi.str( std::string() );
		tempsi << "Highscore: " << this->highscore;
		gText.loadFromRenderedText(gRenderer, tempsi.str().c_str(), {244, 144, 20}, gFont12);
		gText.render(gRenderer, 0, 75+28*2, gText.getWidth(), gText.getHeight());

		tempsi.str( std::string() );
		tempsi << "Score: " << this->score;
		gText.loadFromRenderedText(gRenderer, tempsi.str().c_str(), {255, 255, 255}, gFont12);
		gText.render(gRenderer, screenWidth-gText.getWidth()-15, 75+28*4, gText.getWidth(), gText.getHeight());

		tempsi.str( std::string() );
		tempsi << "Level: " << CurrentLevel;
		gText.loadFromRenderedText(gRenderer, tempsi.str().c_str(), {255, 255, 255}, gFont12);
		gText.render(gRenderer, screenWidth-gText.getWidth()-15, 75+28*5, gText.getWidth(), gText.getHeight());

		tempsi.str( std::string() );
		tempsi << "Damage +" << this->damage;
		gText.loadFromRenderedText(gRenderer, tempsi.str().c_str(), {255, 255, 255}, gFont12);
		gText.render(gRenderer, screenWidth-gText.getWidth()-15, 75+28*13, gText.getWidth(), gText.getHeight());

		tempsi.str( std::string() );
		tempsi << "Cast Damage +" << this->castDamage;
		gText.loadFromRenderedText(gRenderer, tempsi.str().c_str(), {255, 255, 255}, gFont12);
		gText.render(gRenderer, screenWidth-gText.getWidth()-15, 75+28*14, gText.getWidth(), gText.getHeight());*/
	}
}

void Players::RenderDebug(SDL_Renderer *gRenderer, int camX, int camY)
{
	////////////////////////////////////////////////////////////
	//--------------------- Render boxes ---------------------//
	//--------------------------------------------------------//

	// Original box, untouched
	SDL_Rect tempRect = {x-camX, y-camY, w, h};
	SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
	SDL_RenderDrawRect(gRenderer, &tempRect);

	// Original box, untouched
	//tempRect = {x-camX, y-camY, realw, realh};
	//SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
	//SDL_RenderDrawRect(gRenderer, &tempRect);

	////////////////////////////////////////////////////////////
	//---------------------- Render text ---------------------//
	//--------------------------------------------------------//

	std::stringstream tempsi;
	tempsi << "attack: " << attack << ", delay: " << delay;
	gText.loadFromRenderedText(gRenderer, tempsi.str().c_str(), {255,255,255}, gFont12);
	gText.render(gRenderer, x-camX, y-h-camY-gText.getHeight(), gText.getWidth(), gText.getHeight());

	/*tempsi.str( std::string() );
	tempsi << "Health: " << health;
	gText.loadFromRenderedText(tempsi.str().c_str(), {0,255,0}, gFont12, gRenderer);
	gText.render(gRenderer, screenWidth-gText.getWidth()-15, 58, gText.getWidth(), gText.getHeight());*/

	// Check what keys are pressed
	/*for (int i=0; i<25; i++){
		std::std::stringstream tempss;
		tempss << "i: " << i << " -        " << SDL_JoystickGetButton(joy, i);
		gText.loadFromRenderedText(tempss.str().c_str(), {255, 255, 255, 255}, gFont, gRenderer);
		gText.render(gRenderer, 5, 5+i*15, gText.getWidth(), gText.getHeight());
	}

	std::std::stringstream tempss;
	tempss.str(std::string());
	tempss << "Axis: " << 0 << " -        " << SDL_JoystickGetAxis(joy, 0);
	gText.loadFromRenderedText(tempss.str().c_str(), {255, 255, 255, 255}, gFont, gRenderer);
	gText.render(gRenderer, 60, 5, gText.getWidth(), gText.getHeight());

	tempss.str(std::string());
	tempss << "Axis: " << 1 << " -        " << SDL_JoystickGetAxis(joy, 1);
	gText.loadFromRenderedText(tempss.str().c_str(), {255, 255, 255, 255}, gFont, gRenderer);
	gText.render(gRenderer, 60, 20, gText.getWidth(), gText.getHeight());

	tempss.str(std::string());
	tempss << "Axis: " << 2 << " -        " << SDL_JoystickGetAxis(joy, 2);
	gText.loadFromRenderedText(tempss.str().c_str(), {255, 255, 255, 255}, gFont, gRenderer);
	gText.render(gRenderer, 60, 35, gText.getWidth(), gText.getHeight());

	tempss.str(std::string());
	tempss << "Axis: " << 3 << " -        " << SDL_JoystickGetAxis(joy, 3);
	gText.loadFromRenderedText(tempss.str().c_str(), {255, 255, 255, 255}, gFont, gRenderer);
	gText.render(gRenderer, 60, 50, gText.getWidth(), gText.getHeight());

	tempss.str(std::string());
	tempss << "Axis: " << 4 << " -        " << SDL_JoystickGetAxis(joy, 4);
	gText.loadFromRenderedText(tempss.str().c_str(), {255, 255, 255, 255}, gFont, gRenderer);
	gText.render(gRenderer, 60, 65, gText.getWidth(), gText.getHeight());

	tempss.str(std::string());
	tempss << "Axis: " << 5 << " -        " << SDL_JoystickGetAxis(joy, 5);
	gText.loadFromRenderedText(tempss.str().c_str(), {255, 255, 255, 255}, gFont, gRenderer);
	gText.render(gRenderer, 60, 80, gText.getWidth(), gText.getHeight());*/
}



// Key Pressed
void Players::OnKeyDown(SDL_Keycode sym )
{
	switch (sym)
	{
	case SDLK_a:
		this->moveleft = true;
		break;
	case SDLK_d:
		this->moveright = true;
		break;
	case SDLK_w:
		this->moveup = true;
		break;
	case SDLK_s:
		this->movedown = true;
		break;
	case SDLK_e:
		//this->pressedEquipKey = true;
		break;
	case SDLK_h:
		//debug = ( !debug );
		break;
	case SDLK_y:
		this->camlocked = ( !this->camlocked );
		break;
	case SDLK_m:						// Dodge
		// Future code to dodge
	case SDLK_j:						// Attack (spell attack? This is not finalized.)

		break;
	case SDLK_LSHIFT:
		this->shift = true;
		break;
	case SDLK_n:						// Attack

		// Attack
		SlashAttack();
		break;
	case SDLK_c:						// Parry

		// Activate Parry
		ActivateParry();
		break;
	case SDLK_SPACE: 					// Dash

		// Activate Dash
		ActivateDash();
		break;
	case SDLK_1:
		itemIndex = 0;
		break;
	case SDLK_2:
		itemIndex = 1;
		break;
	case SDLK_3:
		itemIndex = 2;
		break;
	}
}

// Key Released
void Players::OnKeyUp(SDL_Keycode sym )
{
	switch (sym){
	case SDLK_a:
		this->moveleft = false;
		break;
	case SDLK_d:
		this->moveright = false;
		break;
	case SDLK_w:
		this->moveup = false;
		break;
	case SDLK_s:
		this->movedown = false;
		break;
	case SDLK_e:
		this->pressedEquipKey = true;
		break;
	case SDLK_j:

		break;
	case SDLK_LSHIFT:
		this->shift 		= false;
		break;
	case SDLK_SPACE:
    	//
		break;
	}
}

// Player Mouse Pressed
void Players::mouseClickState(SDL_Event &e){
	if (e.type == SDL_MOUSEBUTTONDOWN) {
		if (e.button.button == SDL_BUTTON_LEFT) {
			this->leftclick = true;

			// Attack
			SlashAttack();
		}
		if (e.button.button == SDL_BUTTON_RIGHT) {
			this->rightclick = true;
		}
	}else if (e.type == SDL_MOUSEBUTTONUP) {
		if (e.button.button == SDL_BUTTON_LEFT) {
			this->leftclick = false;

			// Shoot bullet
			this->initialshot = false;
		}
		if (e.button.button == SDL_BUTTON_RIGHT) {
			this->rightclick = false;
		}
	}
}

void Players::SlashAttack() {
	if (!this->delay) {
		if (!this->attack && !this->stunned && !this->parry) {

			// If currently dashing
			if (this->dash)
			{
				// Stop dashing
				StopDashing();
			}

			// Set attack parameters
			this->sprite_index = 5;
			this->clash = false;
			this->attack = true;
			this->spawnAttack = true;
			this->attackType = 0;

			// Shoot bullet
			this->initialshot = true;

			// Do normal Slash Attack
			/*if (!this->movedown) {
				this->attackType = 0;
			}
			// Do Stab-Down attack
			else {
				this->attackType = 1;
			}*/
		}
	}
}

void Players::ActivateParry() {
    if (!this->parry && this->parryCDTimer <= 0 && !this->stunned)
    {
		// Stop attacking
		if (this->attack) {
			StopSlashAttack();
		}

		// Stop dashing
		if (this->dash) {
			StopDashing();
		}

    	// enable parrying
    	this->parry = true;

    	// Have parry on CD
    	this->parryCDTimer = this->parryCDMax;

    	// Play sound effect
    	Mix_PlayChannel(-1, settings->sParry, 0);
    }
}

void Players::ActivateDash() {
	// If Dash is not on cool down
	if (this->dashCoolCounter <= 0 && this->dashCounter <=0) {

		// Make sure we are not parrying before dashing
		// Because we dont want to stop the animation of parrying
		// if we dash.
		if (!this->parry) {

			// Stop attacking
			if (this->attack) {
				StopSlashAttack();
			}

			// Depending on which way the player is moving,
			if (this->moveleft) {
				this->vX -= this->dashSpeed;
			}else if (moveright) {
				this->vX += this->dashSpeed;
			}
			if (this->moveup) {
				this->vY -= this->dashSpeed;
			}else if (movedown) {
				this->vY += this->dashSpeed;
			}
			this->dash = true;
			this->dashCounter = this->dashLength;

			// Play dash sound effect
			Mix_PlayChannel(-1, settings->sDash, 0);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// MUTATOR FUNCTIONS ///////////////////////////////


float Players::setVx(float value) {
	return this->vX += value;
}

float Players::setVy(float value) {
	return this->vY += value;
}

float Players::moveX(float value) {
	return this->x += value;
}

float Players::moveY(float value) {
	return this->y += value;
}

// Applies a shield to Player
void Players::ActivateInvurnerability(){
	invurnerableFrame			= 1;
	invurnerableTick			= 0;
	invurnerableT				= 300;
	invurnerable				= true;
}

void Players::StopParrying()
{
	// Reset parry CD
	this->parryTimer = 0;
	this->parry = false;
	this->parryCDTimer = this->parryCDMax;
}

void Players::ResetParry()
{
	// Reset parry CD
	this->parryTimer = 0;
	this->parry = false;
	this->parryCDTimer = 0;

	// Increase damage multiplier
	//this->damageMultipler += 0.1;
}

void Players::ExtendParryDuration()
{
	//  Extend parry
	this->parryTimer = 0;

	// Gain mana back
	this->mana += this->manaGainOnParry;

	// Increase damage multiplier
	//this->damageMultipler += 0.1;
}

void Players::IncreaseSilverKeys(int value) {
	this->silverKeys += value;
	if (this->silverKeys > this->silverKeysMax) {
		this->silverKeys = this->silverKeysMax;
	}
}

void Players::IncreaseGoldKeys(int value) {
	this->goldKeys += value;
	if (this->goldKeys > this->goldKeysMax) {
		this->goldKeys = this->goldKeysMax;
	}
}

void Players::IncreaseCoins(int value) {
	this->coins += value;
	if (this->coins > this->coins) {
		this->coins = this->coinsMax;
	}
}

void Players::IncreaseBombs(int value) {
	this->bombs += value;
	if (this->bombs > this->bombsMax) {
		this->bombs = this->bombsMax;
	}
}

void Players::IncreaseHearts(int value) {
	this->hearts += value;
	if (this->hearts > this->heartsMax) {
		this->hearts = this->heartsMax;
	}
}

void Players::IncreaseHealth(float value) {
	this->health += value;
	if (this->health > this->healthMax) {
		this->health = this->healthMax;
	}
}

void Players::IncreaseScore(float value) {
	this->score += value;
}

void Players::ShortenParryCD(float value) {
	this->parryCDTimer -= value;

	if (this->parryCDTimer <= 0) {
		this->parryCDTimer = 0;
	}
}

void Players::StopMovement()
{
	// Stop player movement
    this->vX = 0.0;
    this->vY = 0.0;
    this->moving = false;
}

void Players::StopSlashAttack()
{
	// Reset slash attack timers
	attackType = -1;
    attackTimer = 0;
    attack = false;
}

void Players::StopDashing()
{
	// Reset dash timers
	this->dash = false;
	this->dashCounter 	= 0;
	this->dashCoolCounter = this->dashCooldown;
}

void Players::ResetDashing()
{
	// Reset dash timers
	this->dash = false;
	this->dashCounter 	= 0;
	this->dashCoolCounter = 0;
}

void Players::EquipSword(int itemIndex, float itemDamage, float itemAtkSpeed)
{
	this->itemIndex = itemIndex;
	this->damage = itemDamage;
	this->atkSpeed = itemAtkSpeed;
}

void Players::stopEquipState() {
	this->pressedEquipKey = false;
}

///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// GETTER FUNCTIONS ////////////////////////////////

float Players::getX() {
	return this->x;
}

float Players::getY() {
	return this->y;
}

float Players::getW() {
	return this->w;
}

float Players::getH() {
	return this->h;
}

float Players::getLeftSide() {
	return this->x;
}

float Players::getRightSide() {
	return this->x+this->w;
}

float Players::getTopSide() {
	return this->y;
}

float Players::getBottomSide() {
	return this->y+this->h;
}

float Players::getDamage() {
	return this->damage * this->damageMultipler;
}

float Players::getCastDamage() {
	return this->castDamage;
}


// Get x center of player
float Players::getCenterX() {
	return this->x + this->w/2;
}

// Get y center of player
float Players::getCenterY() {
	return this->y + this->h/2;
}

// Get value depending on direction facing
float Players::getXDir() {
	// Knockback player
	float xDir = 1;
	if (this->facing=="left") {
		xDir = -1;
	}
	if (this->facing=="right") {
		xDir = 1;
	}
	return xDir;
}

// Get knockback power
float Players::getKnockBackPower() {
	return this->knockBackPower;
}


float Players::getInvurnerableStatus() {
	return this->invurnerable;
}

float Players::getParryStatus() {
	return this->parry;
}

float Players::getDashStatus() {
	return this->dash;
}

float Players::getScore() {
	return this->score;
}

bool Players::getEquipState() {
	return this->pressedEquipKey;
}
int Players::getItemEqipped(int checkThisIndex) {
	if (checkThisIndex == itemIndex) {
		return true;
	} else{
		return false;
	}
}
int Players::getItemID() {
	return itemIndex;
}

int Players::getCoins() {
	return this->coins;
}

int Players::getSilverKeys() {
	return this->silverKeys;
}

int Players::getGoldKeys() {
	return this->goldKeys;
}

void Players::ResetHighScore(int LevelWeLoaded){

	// Set a file path for score.txt
	std::stringstream filePath;
	filePath << "data/maps/highscore";
	filePath << LevelWeLoaded;
	filePath << ".txt";

	// Overwrite "highscores.txt"
	std::ofstream fileSave;
	fileSave.open(filePath.str().c_str());
	fileSave << 0;
	fileSave.close();

	// Reset scores
	this->highscore = 0;
	this->score = 0;
}

void Players::ApplyHighScore(float previousHighScore) {
	this->highscore = previousHighScore;
}

void Players::SaveHighScore(int LevelWeLoaded) {

	// Open highscore first to check value
	unsigned int tempScore = -1;

	// Open file, and get previous High Score
	{
		std::stringstream filePath;
		filePath << "data/maps/highscore";
		filePath << LevelWeLoaded;
		filePath << ".txt";

		std::fstream fileOpen(filePath.str().c_str());

		// If a file exist
		if (fileOpen.is_open()) {
			// Store previous High score in this variable
			fileOpen >> tempScore;


			// Check if current Score is higher than previous High score
			if (this->score > tempScore) {

				// Save current Score as new High Score
				std::stringstream filePath;
				filePath << "data/maps/highscore";
				filePath << LevelWeLoaded;
				filePath << ".txt";

				std::ofstream fileSave;
				fileSave.open(filePath.str().c_str());
				fileSave << this->score;
				fileSave.close();
			}
		}

		// If no file exists, create new highscore
		else {

			std::cout<< "File does not exist on Saving Highscore, creating new file.\n";
			{
				std::stringstream filePath;
				filePath << "data/maps/highscore";
				filePath << LevelWeLoaded;
				filePath << ".txt";

				std::ofstream fileSave;
				fileSave.open(filePath.str().c_str());
				fileSave << this->score;
				fileSave.close();
			}
		}
		fileOpen.close();
	}
}

void Players::SaveData() {

	// Open file, and get previous High Score
	{
		std::stringstream filePath;
		filePath << "data/savefile.mp";
		std::fstream fileOpen(filePath.str().c_str());

		// If a file exist
		if (fileOpen.is_open()) {
			// Save current Score as new High Score
			std::stringstream filePath;
			filePath << "data/savefile.mp";

			std::ofstream fileSave;
			fileSave.open(filePath.str().c_str());

			fileSave << this->damage 			<< " "
					 << this->atkSpeed 			<< " "
					 << this->hearts 			<< " "
					 << this->healthMax 		<< " "
					 << this->castDamage 		<< " "
					 << this->castAtkSpe 		<< " "
					 << this->maxMana  			<< " "
					 << this->manaRegenSpeed  	<< " "
					 << this->manaGainOnParry  	<< " "
					 << this->parryLength  		<< " "
					 << this->dashSpeed  		<< " "
					 << this->dashLength  		<< " "
			   	   	 << true 					<< "\n";

			fileSave.close();
		}

		// If no file exists, create new highscore
		else {

			//std::cout<< "File does not exist on Saving Highscore, creating new file.\n";
			{
				std::stringstream filePath;
				filePath << "data/savefile.mp";

				std::ofstream fileSave;
				fileSave.open(filePath.str().c_str());

				fileSave << 10 		<< " "
						 << 4.27 	<< " "
						 << 3 		<< " "
						 << 100 	<< " "
						 << 100 	<< " "
						 << 1 		<< " "
						 << 100  	<< " "
						 << 8.75  	<< " "
						 << 5.25  	<< " "
						 << 15  	<< " "
						 << 10  	<< " "
						 << 5  		<< " "
				   	   	 << true 	<< "\n";

				fileSave.close();
			}
		}
		fileOpen.close();
	}
}

void Players::LoadData() {

	std::stringstream filePath;
	filePath << "data/savefile.mp";

	std::fstream fileSave;
	fileSave.open(filePath.str().c_str());

	// File opened, load data
	if (fileSave.is_open()) {
		fileSave >>  	  this->damage 				>>
						  this->atkSpeed 			>>
						  this->hearts 				>>
						  this->healthMax 			>>
						  this->castDamage 			>>
						  this->castAtkSpe 			>>
						  this->maxMana 			>>
						  this->manaRegenSpeed 		>>
						  this->manaGainOnParry 	>>
						  this->parryLength 		>>
						  this->dashSpeed 			>>
						  this->dashLength			>>
						  this->alive;
	}

	// File didn't open, create a new file, then set default parameters
	else {

		std::cout<< "File does not exist on Saving Highscore, creating new file.\n";
		{
			std::stringstream filePath;
			filePath << "data/savefile.mp";

			std::ofstream fileNewSave;
			fileNewSave.open(filePath.str().c_str());

			fileNewSave << 10 		<< " "
					 << 4.27 	<< " "
					 << 3 		<< " "
					 << 100 	<< " "
					 << 100 	<< " "
					 << 1 		<< " "
					 << 100  	<< " "
					 << 8.75  	<< " "
					 << 5.25  	<< " "
					 << 15  	<< " "
					 << 10  	<< " "
					 << 5  		<< " "
			   	   	 << true 	<< "\n";

			fileNewSave.close();
		}

		// Set default parameters after create a new file save
		this->damage 			= 10;
		this->atkSpeed 			= 4.27;
		this->hearts 			= 3;
		this->healthMax 		= 100;
		this->castDamage 		= 100;
		this->castAtkSpe 		= 1;
		this->maxMana 			= 100;
		this->manaRegenSpeed 	= 8.75;
		this->manaGainOnParry 	= 5.25;
		this->parryLength 		= 15;
		this->dashSpeed 		= 10;
		this->dashLength		= 5;
		this->alive				= true;
	}
	fileSave.close();


	// Set default parameters
	//setStatsBasedOnType();
}












