/*
 * PlayGame.h
 *
 *  Created on: May 15, 2017
 *      Author: Carl
 */

#ifndef PLAYGAME_H_
#define PLAYGAME_H_

#include "Game/Boss.h"
#include "Game/Maps.h"
#include "Game/Players.h"
#include "Game/Spawners.h"
#include "Game/Object.h"
#include "Game/TileBar.h"
#include "Game/Tiles.h"
#include "Game/Tilec.h"
#include "Game/Item.h"
#include "Game/Mob.h"
#include "Game/TextNFont.h"
#include "Helper.h"
#include <cassert>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>
#include "Settings.h"


class PlayGame : public Helper {

private:
	TextNFont fonts;
	Settings settings;
	void LoadAudioSettings();

public:	// Global World Variables
	double gravity = 0.6;
	bool RestartLevel = false;

public: // global functions
	bool checkCollisionRect( SDL_Rect a, SDL_Rect b );

public:	// globals
	bool shift;
	bool editor;
	bool debug;
	/* 0: Textured tiles
	 * 1: Collision Tiles
	 * 2: Boss
	 * 3: Item
	 * 4: Mobs
	 *
	 */
	int place_type;
	int newMx, newMy;
	int mex, mey;
	bool leftClick = false;
	bool rightClick = false;

public:	// Objectives

	// mob.count
	std::vector<Tile> jarsLeft;

public:
    // camera
    float camx;
    float camy;
    bool camlock;
	bool camUp = false;
	bool camDown = false;
	bool camLeft = false;
	bool camRight = false;

	// Debug text
	int xOffetDebug = 0;
	int yOffetDebug = 50;

	/* This will bring the player
	 * to life. At least, that's what
	 * it'll look like. This will rustle
	 * the player's armor as they move
	 */
	float rustleW;
	float rustleSpe;
	float rustleDirX;
    bool camshake;

	void ShakeCamera();
	bool playerCallingToShakeCamera = false;

public: // System variables
	enum Result { Back, Nothing, StartGame, ShowingMenu, Exit };
	void Show(LWindow &gWindow, SDL_Renderer *gRenderer,
			  PlayGame::Result &result, int &LevelToLoad);
	PlayGame::Result mousePressed(SDL_Event event);
	PlayGame::Result mouseReleased(SDL_Event event);

public:	// Scene textures
	LTexture gBG;
	LTexture gBG2;
	LTexture gTargetTexture;
	LTexture gRect;
	LTexture gCircle;
	LTexture gShadow;

	// Tile breaking
	LTexture gTileBreak;
	SDL_Rect rTileBreak[10];

	// Tile jars
	LTexture gJar;
	SDL_Rect rJar[6];

	// Mouse cursor
	LTexture gCursor;

	class Text {
	public:
		int count;
		float x;
		float y;
		float w;
		float h;
		float alpha;
		float timer;
		bool alive;
		float vX;
		float vY;
		SDL_Color color;

		/*
		 * 0: General information logs like debug logs
		 * 1: These will have an x & y coordinate somewhere
		 */
		int type;

		std::string textfield;
		void init(Text text[]) {
			count =0;
			for (int i=0; i<100; i++) {
				text[i].alive = false;
			}
		}
		void spawn(SDL_Renderer *gRenderer, Text text[], float x, float y,
				float vX, float vY, int alpha,
				std::string textfield, int type, SDL_Color color={244,144,40}) {
			for (int i=0; i<100; i++) {
				if (!text[i].alive) {
					//std::cout << "Spawning text object." << std::endl;
					text[i].type = type;
					text[i].color = color;
					text[i].x = x;
					text[i].y = y;
					text[i].w = 139;
					text[i].h = 96;
					text[i].vX = vX;
					text[i].vY = vY;
					text[i].timer = timer;
					text[i].alpha = alpha;
					text[i].textfield = textfield;
					text[i].alive = true;
					count++;
					break;
				}
			}
		}
		void updateDebugText(Text text[]) {
			for (int i=0; i<100; i++) {
				if (text[i].alive && text[i].type == 0) {
					text[i].x += text[i].vX;
					text[i].y += text[i].vY;

					text[i].alpha -= 2;
					if (text[i].alpha <= 0) {
						text[i].alive = false;
						count--;
					}
				}
			}
		}

		void updateDamageText(Text text[]) {
			for (int i=0; i<100; i++) {
				if (text[i].alive && text[i].type == 1) {
					text[i].x += text[i].vX;
					text[i].y += text[i].vY;

					text[i].alpha -= 2;
					if (text[i].alpha <= 0) {
						text[i].alive = false;
						count--;
					}
				}
			}
		}
	};

	Text tex;
	Text text[100];

public:	// Other classes
	Map map;
	Particle part;
	Spawner spaw;
	//Pause pau;
	//Pause::Result pauseResult;
	Players player;

	// Class instance, multiple
	Particle particles[800];
	Spawner spawner[200];
	//Mixed mix;
	//Controls cont;
	//static Items items[10];

	// Objects (Player attack collisions
	Object obj;
	Object object[10];

	// Boss
	Boss bos;
	Boss boss[100];

	// Mob
	Mob mb;
	Mob mob[100];

	// Items (mostly swords)
	Item ite;
	Item item[100];

public: // Tile Variables

	SDL_Rect rTiles[UniqueTileBarTiles];

	// Tilebar
	TileBar tb;
	TileBar tilebar[UniqueTileBarTiles];

	// Tiles
	Tile tl;
	Tile tile[5000];

	// Collision Tiles
	Tilec tlc;
	Tilec tilec[1000];

public:	// Player variables
	// Player spawn point
	float spawnX = 0, spawnY = 0;
	std::string saveSpawnPoint();
	std::string saveMapSize();
	void loadSpawnPoint();

public:	// Initial functions

	// Initialize
	void Init();

	// Load resources
	void Load(LWindow &gWindow, SDL_Renderer *gRenderer);

	// Free resources
	void Free();

public:	// Core functions
	// Update everything
	void Update(LWindow &gWindow, SDL_Renderer *gRenderer);

	// Render foreground
	void RenderFG(SDL_Renderer *gRenderer, LWindow &gWindow);

	// Render shadows
	void RenderShadows(SDL_Renderer *gRenderer, LWindow &gWindow);

	// Render game objects
	void Render(SDL_Renderer *gRenderer, LWindow &gWindow);

	// Render UI
	void RenderUI(SDL_Renderer *gRenderer, LWindow &gWindow);

	// Render debug information
	void RenderDebug(SDL_Renderer *gRenderer);

	// Render text
	void RenderText(SDL_Renderer *gRenderer, LWindow &gWindow);

public:	// Functions mixed with other classes

	/* Update number of Jars left
	 * 1. When LoadLevel() is called
	 * 2. When attempting to open a floor
	 * 	  door Tile but there are mobs/jars left
	 * 3. When a jar is broken
	 */
	void UpdateJarsLeft() {
		// Get number of Tile jars left on the map
		jarsLeft.clear();
		for (int j = 0; j < tl.max; j++) {
			if (tile[j].alive){
				if (tile[j].id == 1) {
					jarsLeft.push_back(tile[j]);
				}
			}
		}
	}


	/* Notes on some orders:
	 *
	 * 0. obj.Update()
	 * 1. bos.GetDistanceOfPlayer()
	 * 2. bos.Update()
	 * 3. player.Update()
	 *
	 *
	 */

	//------------------ Collision

	// Check collision between Player & Item
	void checkCollisionPlayerItem();

	// Check collision between Player & Tilec
	void checkPlayerTilceCollision();

	// Check collision between Player & Tile
	/* Tile references
	 *
	 * 0: Chest Tile
	 */
	const int chestTile = 0;
	std::vector<int> doorTiles;

	void checkPlayerTileCollision();

	// Check collision between Mob & Tile
	void checkCollisionTileMob();

	// Check collision between Boss & Tile
	void checkBossTileCollision();

	//------------------ Mobs

	//------------------ Collision


	//------------------ Mobs




	//------------------ Hurts Mobs
	// Check collision between Mobs & Particle
	void checkCollisionParticleMob();

	//------------------ Hurts Mobs
	// Check collision between Mobs & Player attacks
	void checkPlayerAttacksCollisionMob();




	// Check if Mobs have vision of Player
	void checkMobPlayerVision();

	//------------------ Hurts Tiles
	// Player Attack Objects & Tiles
	void checkPlayerAttacksTileCollision();

	// Check collision between Particle & Tile
	//------- Hurts Tiles
	void checkCollisionParticleTile();




	//------------------ Hurts Boss
	// Check collision between Player attacks & Boss
	void checkPlayerAttacksCollisionBoss();

	//------------------ Hurts Boss
	// Check collision between Particle & Boss
	void checkCollisionParticleBoss();

	//------------------ Hurts Particles by Attack Object
	// Check collision between Player attacks & Boss/Mob projectiles
	void checkPlayerAttacksBossParticleCollision();



	//---------- Hurts Player
	// Boss/Mob projectiles & Player
	void checkCollisionParticlePlayer();

	//---------- Hurts Player
	// Boss attack objects & Player
	void checkBossAttacksCollisionPlayer();




	// Check collision between Boss & Player
	void checkCollisionBossPlayer();

	// Check collision between Player Particle & Boss Particle
	void checkCollisionParticleParticle();

	//---------- Boss deafeated? Update levelsCompleted.mp
	void checkBossOrMobDied();

	// Renders
	void RenderTileBreakingBehind();
	void RenderTileBreakingFront();

public:	// Controls
	// Editor controls
	void editorOnKeyDown(SDL_Keycode sym);
	void editorOnKeyUp(SDL_Keycode sym);

private:	// used for some debugging
	int mx, my;
	bool quit;
	bool promptConfirm;
	/*
	 * 0: Loading
	 * 1: Saving
	 */
	int promptType;
	SDL_Event event;

private:	// Load level

	// Boss died, update levelsUnlocked.mp
	void SaveLevelsCompleted();

	// Load config files
	void LoadCFG();

	// Load previous high-score for Level
	void LoadHighScore();

	SDL_Renderer *gRenderer;
	unsigned int previousLevel = -1;
	unsigned int LevelToLoad;
	float lastKnownPositionX, lastKnownPositionY;

	// Load level
	void LoadLevel();
};

#endif /* PLAYGAME_H_ */
