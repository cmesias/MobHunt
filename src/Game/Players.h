/*
 * Players.h
 *
 *  Created on: Dec 18, 2016
 *      Author: Carl
 */

#ifndef LOCAL_PLAYERS_H_
#define LOCAL_PLAYERS_H_

#include <SDL2/SDL_ttf.h>

#include "../LWindow.h"
#include "../LTexture.h"
#include "../Particless.h"
#include "../Helper.h"
#include "Maps.h"
#include "Tiles.h"
#include "Tilec.h"
#include "Object.h"
#include "TextNFont.h"

#include "../Settings.h"

//Player
class Players: public Helper, public TextNFont {
public:
	enum Result { QUIT, LOADMENU, PLAYGAME, QUITPLAYGAME };

	Settings *settings;
	void setSettings(Settings *settings);

public:
	// Joystick Sensitivity
	const int JOYSTICK_DEAD_ZONE = 8000;
	void OnKeyDown(SDL_Keycode sym );
	void OnKeyUp(SDL_Keycode sym );
	void mouseClickState(SDL_Event &e);

	// Mouse stuff
	bool leftclick;
	bool rightclick;
	bool test;

public:	// resources
	// Local resources
	LTexture gPlayer;
	LTexture gPlayerShadow;
	LTexture gShield;
	LTexture gSwords;

	/* Sword types
	 * 0  - 10: Fist & Swords
	 * 11 - 12: Rapier and Hammer
	 * 13 - 18: Better Swords
	 * 19 - 22: Broken Swords
	 * 23: 		Bomb
	 * 24: 		Heart
	 * 25: 		Coin
	 * 26: 		Silver key
	 * 27: 		Gold key
	 */
	SDL_Rect rSwords[28];

	/*
	 *
	 *
	// Walk-down
	    // 0:
		// 1:
		// 2:
		// 3:
		// 4:

		// 5: Walk-down slash 0
		// 6: Walk-down slash 1
		// 7: Walk-down slash 2
		 *
	// Walk-up
		// 8:
		// 9:
		// 10:
		// 11:
		// 12:

		// 13: Walk-up slash 0
		// 14: Walk-up slash 1
		// 15: Walk-up slash 2
		 *
	// Walk-right
		// 16:
		// 17:
		// 18:
		// 19:
		// 20:

		// 21: Walk-right slash 0
		// 22: Walk-right slash 1
		// 23: Walk-right slash 2
		 *
	// Walk-left
		// 24:
		// 25:
		// 26:
		// 27:
		// 28:

		// 29: Walk-left slash 0
		// 30: Walk-left slash 1
		// 31: Walk-left slash 2
	 */
	SDL_Rect rPlayer[32];
	SDL_Rect rShield[7];

	/*
	 * 0-3: Walk
	 * 4:   Before slash
	 * 5:   Slash
	 * 6:   Parry
	 * 7:   Down Stab
	 * 8:   Dash frame 0
	 * 9:   Dash frame 1
	 */
	int sprite_index;

	/*
	 * 0: Facing down block
	 * 7: Facing up block
	 * 15: Facing right
	 * 23: Facing left
	 */
	int sprite_dir = 0;

	/*
	 * 0: Keyboard
	 * 1: Keyboard + Mouse
	 */
	int ControlsPreference = 1;	// default: 0

public:	// variables

	/*
	 * Parrying small blue Bullet from Boss:		score += 1;
	 * Parrying small red Bullet from Boss:			score += 2;
	 * Parrying large red Bullet from Boss:			score += 4;
	 * Hitting Boss directly w/ Slash Projectile: 	score += 5
	 * Hitting Boss directly w/ Sword: 				score += 5
	 * Hitting Boss with counter is:				score += 10
	 */
	unsigned int score	= 0;
	unsigned int highscore = 0;

	std::string name;
	int realw = 64, realh = 64;			// object size in actual pixel size
	int w = 24, h = 24;					// object size in collision pixels
	float x = 0,
		  y = 0;
	float checkpointX, checkPointY;					// When Player enters a new area, this will be there spawn point
	float armX;							// This is where we will attach the sword to
	float armY;
	float x2, y2;						// player center
	float angle 		= 0.0,
		   radians 		= 0.0,
		   radianCos 	= 0.0,
		   radianSin 	= 0.0;
	bool camlocked 		= true;
	bool shift 			= false;
	bool deathScreen	= false;
	bool alive;

	double tempc, temps;
	SDL_Rect continueButton[3];

	// High-Score display
	std::string highList[10];
	int indx 		= 0;
	float travel 	= 0.0;
	int position	= 0;
	int position2	= 0;
	int dir 		= 1;

public: // Attack variables

	bool clash;

private:	// Abilities

	// Fire-rate
	double particleW 	= 48;
	double particleH 	= 48;

private:

	// Walking
	std::string facing;
	bool moveleft 		= false,
		 moveright 		= false,
		 moveup 		= false,
		 movedown		= false;
	bool moving = false;
	float walkTimerVFX;
	const float walkTimerSpe = 10;
	float walkTimer;
	float vX 			= 0.0;
	float vY 			= 0.0;
	const float velSpeed= 0.5;
	const float velMax 	= 2;

	// Invulnerability ability
	int invurnerableFrame;
	int invurnerableTick;
	int invurnerableT;
	bool invurnerable;

	// Shoot Attack
	float shootTimer	= 0;
	bool shootDelay 	= false;
	bool initialshot 	= false;
	bool trigger 		= false;

	// Attack
	float attackTimer;
	const float atkAnimSpe = 60;
	int attackFrame = 5;
	int attackType;	// 0: slash, 1: down stab
	bool attack;
	bool spawnAttack;
	float knockBackPower;	// Knock back power

	// Delay
	float atkSpeed = 4.27;	// Player's attack speed
	float delayTimer;
	bool delay;

	// Parry ability
	bool parry;
	float parryTimer;		// Parry timer
	float parryCDTimer;		// Parry cooldown timer tracker
	float parryCDMax;		// Parry cooldown, default: 60*3

	// Dash ability
	bool dash;							// Dash ability
	float dashSpeed;					// Default: 15
	float dashLength;					// Default: 5
	float dashCounter;					// Default: 0
	float dashCoolCounter;				// Default: 0
	const float dashCooldown = 60;		// Default: 60

	// Stunned ability
	int stunTimer;
	bool stunned;

	// Official player control keys
	bool pressedEquipKey;

	// Inventory
	int silverKeys;						// Keys
	int goldKeys;						// Keys
	int coins;							// Coins
	const int silverKeysMax = 999;		// Keys,  	Default: 99
	const int goldKeysMax = 999;		// Keys,  	Default: 99
	const int coinsMax = 999;			// Coins,  	Default: 99
	int bombs;							// Bombs
	const int bombsMax = 3;				// Bombs,	Deafult: 99

	// Health
	int hearts;							// Default: 3
	const int heartsMax = 3;			// Default: 99
	float health;						// Default: 100
	float healthDecay;					// Default: 100
	float healthMax = 100;				// Default: 100

	// Damage
	float damage;			// Sword damage
	float castDamage;		// Cast damage
	float damageMultipler;	// Parrying will increase damage multiplier
	float castAtkSpe;		// Cast atk speed - deprecated, delete?

	// Mana
	float mana;				// Mana
	float maxMana;			// Max mana
	float manaRegenTimer;	// Timer for mana regen
	float manaRegenSpeed;	// Mana
	float manaGainOnParry;	// Gain mana if you parry successfully

	// Parry
	float parryLength;		// Parry length: 15 default

private:	// TODO [ ] - Save in file

	/* Save into player save data
	 *
	 * damage 			= 10;
	 * atkSpeed			= 4.27;
	 * heartsMax		= 3;
	 * healthMax 		= 100;
	 * castDamage 		= 100;
	 * castAtkSpe 		= 1;
	 * maxMana			= 100;
	 * manaRegenSpeed	= 8.75;
	 * manaGainOnParry	= 5.25;
	 * parryLength		= 15;
	 * dashSpeed		= 10;
	 * dashLength		= 5;
	 * alive 			= true;
	 *
	 */

private:	// Variables used in Textures

	const float xOffset = 0;
	const float yOffset = -29;

	/* Sword Types (TODO [ ] (3-25-2022) - save this in player data)
	 *
	 * 0: Wooden Sword
	 * 1: Rusty Sword
	 * 2: Iron Sword
	 *
	 *
	 * 10: Red Sword					10 x 21
	 *
	 *
	 * 11: Tool? 						7 x 17
	 * 12: Hammer 						10 x 21
	 *
	 * 13: Thicc Sword 					10 x 25
	 * 14: Long Rapier 					8 x 27
	 * 15: Golden Long Sword 			10 x 29
	 * 16: Silver Sword 				6 x 27
	 * 17: Silver Thicc Sword 			10 x 24
	 * 18: Silver Thicc Sword w/ Gold 	10 x 24
	 *
	 * 19: Broken Iron sword short 		6 x 9
	 * 20: Broken Iron sword 			6 x 12
	 * 21: Broken Golden sword 			6 x 12
	 * 22: Broken Silver sword 			6 x 12
	 *
	 * 23: Bomb							14 x 13
	 * 24: Heart 						10 x 10
	 * 25: Coin 						10 x 10
	 * 26: Silver key 					5 x 10
	 * 27: Gold key 					5 x 10
	 */
	int itemIndex = 0;

public:	// Functions to do stuff?

	void SlashAttack();

	void ActivateParry();

	void ActivateDash();

public:	// Mutator functions

	float setVx(float value);

	float setVy(float value);

	// Move x pos
	float moveX(float value);

	// Move y pos
	float moveY(float value);

	// Applies invurnerability to Player
	void ActivateInvurnerability();

	// Stops parry but goes on cool down
	void StopParrying();

	// Stops parry but reset cool down to 0
	void ResetParry();

	// Extend parry duration, and Gain mana back
	void ExtendParryDuration();

	// Increase Silver Keys
	void IncreaseSilverKeys(int value = 1);

	// Increase Gold Keys
	void IncreaseGoldKeys(int value = 1);

	// Increase Coins
	void IncreaseCoins(int value = 1);

	// Increase Bombs
	void IncreaseBombs(int value = 1);

	// Increase Hearts
	void IncreaseHearts(int value = 1);

	// Increase Health
	void IncreaseHealth(float value);

	// Increase Score
	void IncreaseScore(float value);

	// Shorten parry cool down
	void ShortenParryCD(float value);

	// Stop movement
	void StopMovement();

	// Stop Slash Attack
	void StopSlashAttack();

	// Stop dashing
	void StopDashing();

	// Reset dashing
	void ResetDashing();

	// Equip a sword
	void EquipSword(int swordInHand_Index, float damage, float slashAtkSpe);

	// Set false to equip key state
	void stopEquipState();

public:	// Accessor functions

	// Get x pos
	float getX();

	// Get y pos
	float getY();

	// Get width
	float getW();

	// Get height
	float getH();

	// Get left pos
	float getLeftSide();

	// Get right pos
	float getRightSide();

	// Get top pos
	float getTopSide();

	// Get bottom pos
	float getBottomSide();

	// Get player damage
	float getDamage();

	// Get player cast damage
	float getCastDamage();

	// Get x center of player
	float getCenterX();

	// Get y center of player
	float getCenterY();

	// Get value depending on direction facing
	float getXDir();

	// Get knockback power
	float getKnockBackPower();

	// Get status of invurnerability
	float getInvurnerableStatus();

	// Get status of parrying
	float getParryStatus();

	// Get status of dashing
	float getDashStatus();

	// Get score
	float getScore();

	// Get equip key state
	bool getEquipState();

	// Check if item in hand is this index
	int getItemEqipped(int checkThisIndex);

	// Get item index in hand
	int getItemID();

	// Return number of Coins keys for Player
	int getCoins();

	// Return number of Silver keys for Player
	int getSilverKeys();

	// Return number of Gold keys for Player
	int getGoldKeys();

public:

	// Reset High Scores
	void ResetHighScore(int LevelWeLoaded);

	// Load previous high score from PlayGame.cpp
	void ApplyHighScore(float previousHighScore);

	// Save current highs-core for current Level
	void SaveHighScore(int LevelToLoad);

	// Save player stats
	void SaveData();

	// Load player stats
	void LoadData();

public:	// functions
	// Functions

	void Load(SDL_Renderer* gRenderer);

	void Free();

	void Init(float spawnX, float spawnY, std::string newName);

	// Called when player loses all their health
	void RespawnPlayer();

	// Called when player loses all their lives
	void ResetLivesAndPlayer();

	bool checkCollision(int x, int y, int w, int h, int x2, int y2, int w2, int h2);

	// Get audio files
	//void getAudio(Mix_Chunk* sLazer);

	// Player shoot
	void fire(Particle particle[], Particle &p_dummy, Mix_Chunk* sCastSFX, int mx, int my);

	void Update(Map &map,
				Particle particle[], Particle &p_dummy,
				Tile &tl, Tile tile[],
				Tilec &tlc, Tilec tilec[],
				Object &obj, Object object[],
				int mx, int my, int camx, int camy,
				float spawnX, float spawnY,
				LWindow gWindow, SDL_Renderer* gRenderer,
				SDL_Color color,
				bool &RestartLevel,
				int LevelWeLoaded, bool &ShakeCamera);

	void Render(int mx, int my, int camX, int camY, LWindow gWindow,
				SDL_Renderer* gRenderer,
				SDL_Color color, int &PARTICLES);

	void RenderUI(SDL_Renderer *gRenderer, int camX, int camY, int CurrentLevel);

	void RenderDebug(SDL_Renderer* gRenderer, int camX, int camY);

};

#endif /* LOCAL_PLAYERS_H_ */
