/*
 * Boss.h
 *
 *  Created on: Jul 21, 2017
 *      Author: Carl
 */

#ifndef GAME_BOSS_H_
#define GAME_BOSS_H_

#include "Object.h"

#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include "../LWindow.h"
#include "../LTexture.h"

#include "../Helper.h"
#include "Monster.h"
#include "Maps.h"
#include "TextNFont.h"
#include "../Particless.h"

class Boss: public Monster, public Helper, public TextNFont {
public:
	int count;
	const int max = 100;
	LTexture gTexture;
	LTexture gTextureFlashed;
	LTexture gMiddleTurret;
	LTexture gTurret;
	LTexture gBossShadow;
	Mix_Chunk *sCast 			= NULL;

public: // variables
	double walkTimer;
	double walkSpeed;
	double walkFrame;

public:	// Core Functions
	void Init(Boss boss[]);
	void Load(SDL_Renderer *gRenderer);
	void Free();
	void RemoveAll(Boss boss[]);
	void Remove(Boss boss[]);
	void Spawn(Boss boss[], float x, float y, float w, float h, float angle, float speed, int type);

	void Update(Boss boss[],
			Object &obj, Object object[],
			Particle particle[], Particle &p_dummy,
			Map &map, int mex, int mey,
			int camx, int camy, bool playerAlive);

	void UpdateEditor(Boss boss[], int mex, int mey, int camx, int camy);

	void RenderShadow(SDL_Renderer *gRenderer, Boss boss[], int camx, int camy);

	void RenderBack(SDL_Renderer *gRenderer, Boss boss[], int camx, int camy);

	void RenderFront(SDL_Renderer *gRenderer, Boss boss[], int camx, int camy);

	void RenderUI(SDL_Renderer *gRenderer, Boss boss[], int camx, int camy);

	void RenderDebug(SDL_Renderer *gRenderer, Boss boss[], int camx, int camy);

	void RenderHand(SDL_Renderer *gRenderer, Boss boss[], int newMx, int newMy,
						  int mex, int mey, int camx, int camy);


public:	// Setter functions

	void setStatsBasedOnType(Boss boss[], int i);

public:	// Functions that work with other classes

	void GetDistanceOfPlayer(Boss boss[], float targetX, float targetY,
				float targetW, float targetH,
				float *xFollow, float *yFollow);

public:	// Animations

	float *xFollow;
	float *yFollow;

	// Target center
	float bmx, bmy;

	// Self center
	float bmx2, bmy2;

	bool renderInFront;
	bool mouse;
	bool flash;				// flash bosses texture on getting hit by players attacks

	/*
	 * 0: Attack 1
	 * 1: Attack 2
	 */
	int randomAttack;	// Depracated, get rid of this



	/*
	 * -1: 	Cooldown state (can't attack)
	 * 0: 	Walking around
	 * 1: 	Moving towards player
	 * 2: 	Normal 4 shot Attack
	 * 3: 	Pre BARRAGE Attack!!!		- Part 1 of Barrage Attack
	 * 4: 	BARRAGE Attack!				- Part 2 of Barrage Attack
	 */
	int animState;
	bool chargingAttack;
	float chargeTime;					// After 3 seconds, attack
	const float chargeTimeStart = 30;	// Change this to change the charge-attack animation start time

	float constantFiringTimer;

	float coolDownTimer;
	float coolDownTimeStart = 180;

public: // Saving & loading functions

	void LoadData(Boss boss[], std::fstream &fileTileDataL);		// Load data

	std::string SaveData(Boss boss[]);		// Save data

public: // Other classes: Objects


};

#endif /* GAME_BOSS_H_ */
