/*
 * PlayGame.cpp
 *
 *  Created on: May 15, 2017
 *      Author: Carl
 */

#include <fstream>
#include <iostream>
#include <limits>
#include <math.h>
#include <sstream>
#include <iomanip>
#include <memory>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include "LTexture.h"
#include "Particless.h"

#include "PlayGame.h"

// TODO [ ] 4/9/2022: change chest tiles to correct id
//		[ ] - fix texture breaking sprite for jars barrels
// 		[ ] - Mob keeps shooting at Player even when Player is not in sight of Mob
// 		[ ] - Sound for sValidation does not apply properly the volume we have in config
//		[ ] - fix crashing when about to remove all jars

void PlayGame::Init() {

	// Upon entry
	place_type 			= 0;
    debug 				= true;
	editor	 			= true;
	quit 				= false;
	leftClick 			= false;
	shift 				= false;
	frame 				= 0;
    cap 				= true;
	int i = 0;
	lastKnownPositionX  = 100;
	lastKnownPositionY  = 0;
	xOffetDebug			= 0;
	yOffetDebug			= 0;
	promptConfirm		= false;

	// Initialize
	bos.Init(boss);
	mb.Init(mob);
	part.init(particles);
	spaw.init(spawner);
	player.Init(map.x+map.w/2-player.w/2, map.y+map.h/2-player.h/2, "Player1");
	tex.init(text);
	tl.initTile(tile);
	tlc.Init(tilec);
	tb.init(tilebar);
	tb.placeTileBar(tilebar);
	obj.init(object);
	ite.Init(item);

	// Initialize Audio for certain classes
	mb.setSettings(&settings);
	player.setSettings(&settings);

	// Camera stuff
	// Initial camera center
	//camx = map.x + map.w / 2 - screenWidth/2;
	//camy = map.y + map.h / 2 - screenHeight/2;
	camx = map.x;
	camy = map.y + screenHeight/2 + 100;
	camlock		= false;

	// Tiles
	{
		for (int h = 0; h < tb.HowManyTilesVertical; h++) {
			for (int w = 0; w < tb.HowManyTilesHorizontal; w++) {
				rTiles[i].x = 0 + w * tb.TilesCutSizeW;
				rTiles[i].y = 0 + (h * tb.TilesCutSizeH);
				rTiles[i].w = tb.TilesCutSizeW;
				rTiles[i].h = tb.TilesCutSizeH;
				i++;
			}
		}
		// Do specific tiles

		// Top door
		rTiles[2] = {64, 0, 64, 64};
		rTiles[3] = {64, 0, 64, 64};
		rTiles[12] = {64, 0, 64, 64};
		rTiles[13] = {64, 0, 64, 64};

		// Bottom door
		rTiles[22] = {64, 64, 64, 64};
		rTiles[23] = {64, 64, 64, 64};
		rTiles[32] = {64, 64, 64, 64};
		rTiles[44] = {64, 64, 64, 64};

		// Left door
		rTiles[40] = {0, 128, 64, 64};
		rTiles[41] = {0, 128, 64, 64};
		rTiles[50] = {0, 128, 64, 64};
		rTiles[51] = {0, 128, 64, 64};

		// Right door
		rTiles[42] = {64, 128, 64, 64};
		rTiles[43] = {64, 128, 64, 64};
		rTiles[52] = {64, 128, 64, 64};
		rTiles[53] = {64, 128, 64, 64};

		doorTiles.push_back(2);
		doorTiles.push_back(3);
		doorTiles.push_back(12);
		doorTiles.push_back(13);

		doorTiles.push_back(22);
		doorTiles.push_back(23);
		doorTiles.push_back(32);
		doorTiles.push_back(44);

		doorTiles.push_back(40);
		doorTiles.push_back(41);
		doorTiles.push_back(50);
		doorTiles.push_back(51);

		doorTiles.push_back(42);
		doorTiles.push_back(43);
		doorTiles.push_back(52);
		doorTiles.push_back(53);

		// Barrel Tile
		//rTiles[4] = {128, 32, 34};
		//rTiles[14] = {128, 32, 34};

		//272, 128, tile 306
	}
}
/*
void PlayGame::saveCFG(std::string fileName){
	std::ofstream fileSettings;
	fileSettings.open( fileName.c_str() );
		std::stringstream tempss;
		tempss << MASTER_VOL << " "
			   << MUSIC_VOL  << " "
			   << SFX_VOL 	 << " "
			   << RESOLUTION << " "
			   << FULLSCREEN << " "
			   << VSYNC;
		fileSettings << tempss.str().c_str();
	fileSettings.close();
}

void PlayGame::loadCFG(std::string fileName){
	std::fstream fileSettings( fileName.c_str() );
	if (fileSettings.is_open()) {
		fileSettings >> MASTER_VOL >> MUSIC_VOL >> SFX_VOL >> RESOLUTION >> FULLSCREEN >> VSYNC;
		std::cout << "Loaded config.cfg" << std::endl;
	}else{
		std::cout << "Error opening config.cfg." << std::endl;
		std::cout << "\tCreating default config.cfg..." << std::endl;
		std::ofstream newConfigFile("cfg/config.cfg");
		if (newConfigFile.is_open()) {
			std::cout << "\t\tSuccessfully created config.cfg." << std::endl;
			newConfigFile << "100 128 128 2 2 2";
		}else{
			std::cout << "\t\tError creating config.cfg." << std::endl;
			std::cout << "\t\t\tManually setting default config settings." << std::endl;
			MASTER_VOL			= 100;
			MUSIC_VOL			= 128;
			SFX_VOL				= 128;
			RESOLUTION			= 2;
			FULLSCREEN			= 2;
			VSYNC				= 2;
		}
		newConfigFile.close();
	}
	fileSettings.close();

	// Set Audio Settings
	Mix_VolumeMusic(MUSIC_VOL*(MASTER_VOL*0.01));
	Mix_VolumeChunk(sRockBreak, SFX_VOL*(MASTER_VOL*0.01));
	Mix_VolumeChunk(sCast, SFX_VOL*(MASTER_VOL*0.01));
	Mix_VolumeChunk(sAtariBoom, SFX_VOL*(MASTER_VOL*0.01));

	// Set Video Settings
	gWindow.applySettings(RESOLUTION, FULLSCREEN, VSYNC);
}*/

// Load
void PlayGame::Load(LWindow &gWindow, SDL_Renderer *gRenderer)
{
	// load textures
	gBG.loadFromFile(gRenderer, 		"resource/gfx/bg.png");
	gBG2.loadFromFile(gRenderer, 		"resource/gfx/bg2.png");
	gRect.loadFromFile(gRenderer, 		"resource/gfx/rect.png");
	gCircle.loadFromFile(gRenderer, 	"resource/gfx/circle.png");
	gShadow.loadFromFile(gRenderer, 	"resource/gfx/shadow.png");
	gCursor.loadFromFile(gRenderer, "resource/gfx/cursor.png");
	gTileBreak.loadFromFile(gRenderer, "resource/gfx/tile-break.png");
	gTileBreak.setBlendMode(SDL_BLENDMODE_ADD);

	// Tile Jars
	gJar.loadFromFile(gRenderer, "resource/gfx/jar_spritesheet.png");
	rJar[0] = {64*0,0,64,64};
	rJar[1] = {64*1,0,64,64};
	rJar[2] = {64*2,0,64,64};
	rJar[3] = {64*3,0,64,64};
	rJar[4] = {64*4,0,64,64};
	rJar[5] = {64*5,0,64,64};

	// Tile break clips
	int j = 0;
	for (int w = 0; w < 10; w++) {
		rTileBreak[j].x = 0 + w * 16;
		rTileBreak[j].y = 0;
		rTileBreak[j].w = 16;
		rTileBreak[j].h = 16;
		j++;
	}

	//Load texture target
	gTargetTexture.createBlank( gRenderer, screenWidth, screenHeight, SDL_TEXTUREACCESS_TARGET );

	// load media for other classes
	bos.Load(gRenderer);
	mb.Load(gRenderer);

	part.load(gRenderer);
	spaw.load(gRenderer);

	player.Load(gRenderer);

	tl.load(gRenderer, &gJar);
	tb.load(gRenderer);

	tlc.Load();
	ite.Load(gRenderer);

	// Other classes Fonts
	fonts.LoadFonts();
	settings.LoadAudio();
	settings.LoadAudioCFG();
	settings.ApplyAudioCfgToSFX(settings);
}

void PlayGame::Free() {
	// free textures
	gBG.free();
	gBG2.free();
	gRect.free();
	gCircle.free();
	gShadow.free();
	gCursor.free();
	gTileBreak.free();

	// free media from other classes
	bos.Free();
	mb.Free();
	player.Free();
	part.free();
	spaw.free();
	tb.free();
	tl.free();
	tlc.Free();
	ite.Free();

	// Other classes Fonts
	fonts.FreeFonts();
	settings.FreeAudio();
}

void PlayGame::ShakeCamera()
{
	playerCallingToShakeCamera = false;
	camshake = true;
	rustleW = 2;
	rustleSpe = 1;

	rustleDirX = rand() % 3 + 1;
}


void PlayGame::Show(LWindow &gWindow, SDL_Renderer *gRenderer,
					PlayGame::Result &result, int &LevelToLoad) {


	// Set renderer
	this->gRenderer = gRenderer;

	// Set level to load to a local variable
	// that is only available inside PlayGame.cpp
	this->LevelToLoad = LevelToLoad;

	// Previous level
	this->previousLevel = LevelToLoad;

	// Initialize
	Init();

	// Load resources
	Load(gWindow, gRenderer);

	// Load save configurations
	LoadCFG();

	// Load Player last high score for current Level
	LoadHighScore();

	// Load level selected from ActSelection.cpp
	LoadLevel();

    // Play Music, looped
	//Mix_FadeInMusic(sAmbientMusic, -1, 0);
	//Mix_PlayMusic( sAmbientMusic, -1);

	//SDL_ShowCursor(false);

	// Load Audio CFG here (This needs to be loaded here)
	//LoadAudioSettings();

	// While loop
	while (!quit) {

		// Start FPS cap
		fps.start();

		// Handle Events
		while (SDL_PollEvent(&event)) {

			// Handle window events
			gWindow.handleEvent(gRenderer, event);

			// Close program
			if (event.type == SDL_QUIT) {
				result = Exit;
				quit = true;
				Free();
				return;
			}else{
				/// Key Pressed
				if (event.type == SDL_KEYDOWN && event.key.repeat == 0)
				{
					/* Global Controls */
					switch (event.key.keysym.sym)
					{
						case SDLK_LSHIFT:
							shift = true;
							break;
						case SDLK_RSHIFT:
							shift = true;
							break;
						case SDLK_p:
							//editor = (!editor);
							if (editor) {
								// Disable editor
								editor = false;
								camlock = true;
							} else {
								// Enable editor
								editor = true;
								camlock = false;
							}
							break;
						case SDLK_h:
							debug = (!debug);
							break;
						case SDLK_ESCAPE:	// pause menu
							//start(gWindow, gRenderer);

							// Leaving PlayGame.cpp, free these
							settings.FreeAudio();

							// Load Settings.cpp
							settings.start(gWindow, gRenderer);

							// Returning to PlayGame.cpp, load these
							LoadAudioSettings();
							break;
						case SDLK_F1:							// Set render size 1
							//SDL_RenderSetLogicalSize(gRenderer,1920,1080);

							//this->LevelToLoad = 1;
							//LevelToLoad = 1;
							//LoadLevel();
							break;
						case SDLK_F2:							// Set render size 2
							//SDL_RenderSetLogicalSize(gRenderer,1600,900);

							//this->LevelToLoad = 2;
							//LevelToLoad = 2;
							//LoadLevel();
							break;
						case SDLK_F3:							// Set render size 3
							//SDL_RenderSetLogicalSize(gRenderer,1280,720);
							break;
						case SDLK_F4:							// Set render size 4
							//SDL_RenderSetLogicalSize(gRenderer,800,600);
							break;
					}

					/* Editor Controls */
					if (editor) {
						// Local
						switch (event.key.keysym.sym)
						{
							//
						}
						editorOnKeyDown(event.key.keysym.sym);
					}
					/* Player Controls */
					else{
						// Local
						switch (event.key.keysym.sym)
						{
							case SDLK_ESCAPE:	// pause menu
								//start(gWindow, gRenderer);
								break;
						}

						// Player key down
						player.OnKeyDown(event.key.keysym.sym);
					}	// end editor check
				}       // end key pressed
				// Key Released
				else if (event.type == SDL_KEYUP && event.key.repeat == 0)
				{
					/* Global Controls */
					switch (event.key.keysym.sym) {
					case SDLK_LSHIFT:
						shift = false;
						break;
					case SDLK_RSHIFT:
						shift = false;
						break;
					}

					/* Editor Controls */
					if (editor) {
						// Local
						switch (event.key.keysym.sym)
						{
							//
						}
						editorOnKeyUp(event.key.keysym.sym);
					}
					/* Player Controls */
					else{
						// Local
						switch (event.key.keysym.sym)
						{
							//
						}

						// Player Keys Released, categorized
						player.OnKeyUp(event.key.keysym.sym);
					} // end editor
				}	  // end key released

				//////////////////////////////////////

				/////////////////////////////////////

				// Update Player click state
				if (!editor) {
					// Player Mouse Click state
					player.mouseClickState(event);
				}

				// Global mouse presses
				mousePressed(event);

				// Global mouse release
				mouseReleased(event);

				// Mouse Released
				//result = mouseReleased(event);

				// Handle exit on Settings.cpp
				switch (settings.settingsResult)  {
				case Settings::Back:				// Back (to Main Menu)
					// Do nothing
					break;
				case Settings::ShowingMenu:				// Back (to Main Menu)
					quit = true;
					break;
				case Settings::Exit:				// Back (to Main Menu)
					result = Exit;
					quit = true;
					Free();
					return;
					break;
				}

			}
		}

		// update everything
		Update(gWindow, gRenderer);

		//Set our Texture as our render target
		gTargetTexture.setAsRenderTarget(gRenderer);

		// Clear render screen
		SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
		SDL_RenderClear(gRenderer);

			//SDL_Rect backgroundRect = {0, 0, screenWidth, screenHeight};
			//SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0);
			//SDL_RenderFillRect(gRenderer, &backgroundRect);

			// Render particles
			part.RenderVFX(particles, camx, camy, 1, gRenderer);
			part.RenderBullets(particles, camx, camy, 1, gRenderer);

			// Render UI
			//player.renderUI();
			//konoko.renderUI(npc[1], textBoxPrincess[indexP]);

			// Render foreground
			/*RenderFG(gRenderer, gWindow);*/

			// Render objects
			Render(gRenderer, gWindow);

			// TODO [ ] - fix lights 3/17/2022
			///gBG2.setBlendMode(SDL_BLENDMODE_ADD);
			//gBG2.render( gRenderer, 0, 0, 1280, 720, NULL, 0.0);

			//gBG.setBlendMode(SDL_BLENDMODE_BLEND);
			//gBG.render( gRenderer, 0, 0, 1280, 720, NULL, 0.0);

			// Render background
			//gBG2.setBlendMode(SDL_BLENDMODE_ADD);
			//gBG2.render( gRenderer, 0, 0, 1280, 720, NULL, 0.0);

			// Render UI for objects
			RenderUI(gRenderer, gWindow);

			// Render text
			RenderDebug(gRenderer);

			// Render text
			RenderText(gRenderer, gWindow);

			// Original box, untouched
			SDL_Rect tempRect = {0, 0, screenWidth, screenHeight};
			SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
			SDL_RenderDrawRect(gRenderer, &tempRect);

		//Reset render target
		SDL_SetRenderTarget( gRenderer, NULL );

		//Show rendered to texture
		gTargetTexture.render( gRenderer, 0, 0, screenWidth, screenHeight, NULL, 0.0);
		//gTargetTexture.render( gRenderer, 0, 0, gWindow.getWidth(), gWindow.getHeight(), NULL, 0.0);

		// Update screen
		SDL_RenderPresent(gRenderer);

		// fps
		frame++;
		if((cap == true ) && (fps.get_ticks() < 1000 / FRAMES_PER_SECOND ))
			SDL_Delay((1000 / FRAMES_PER_SECOND ) - fps.get_ticks());
	}
	// Free everything
	Free();
	//tl.free();
	//tb.free();
}

// Update everything
void PlayGame::Update(LWindow &gWindow, SDL_Renderer *gRenderer) {
	// Variable limits

	// Tiles: layers
	if (tl.layer < -1) {
		tl.layer = 6;
	}
	if (tl.layer > 6) {
		tl.layer = -1;
	}

	// Tiles: id
	if (tl.id < 0) {
		tl.id = tb.TILES_UNIQUE;
	}
	if (tl.id > tb.TILES_UNIQUE) {
		tl.id = 0;
	}

	// Tilecs: idm layer, newPos
	if (tlc.id > 2) {
		tlc.id = 0;
	}
	if (tlc.layer < 0) {
		tlc.layer = 0;
	}
	if (tlc.layer > 2) {
		tlc.layer = 0;
	}
	if (tlc.newPos < 0) {
		tlc.newPos = 3;
	}
	if (tlc.newPos > 3) {
		tlc.newPos = 0;
	}


	// Item: id
	if (ite.id < 0) {
		ite.id = ite.ITEMS_UNIQUE-1;
	}
	if (ite.id > ite.ITEMS_UNIQUE-1) {
		ite.id = 0;
	}

	// Tilecs: LevelToLoad
	if (tlc.LevelToLoad < -1) {
		tlc.LevelToLoad = 5;
	}
	if (tlc.LevelToLoad > 5) {
		tlc.LevelToLoad = -1;
	}

	// place_type
	if (place_type > 4) {
		place_type = 0;
	}

	// Get mouse position
	SDL_GetMouseState(&mx, &my);
	/* get render width and height
	 * but (mostly used to get actual render height)
	 */
	int renderW = 0;
	int renderHDummy = 0;
	SDL_GetRendererOutputSize(gRenderer,&renderW,&renderHDummy);
	int en = renderW * 0.4375;
	int renderH = renderW - en;

	// Grid like coordinates of mouse
	// get new mouse coordinates based on render size, and actual screen size
	/*mx = (screenWidth*mx)/gWindow.getWidth();	// New mouse coordinates, no relation to camera
	my = (screenHeight*my)/gWindow.getHeight();	// New mouse coordinates, no relation to camera

	//int mex = (1280*mx)/gWindow.getWidth();
	//int mey = (720*my)/gWindow.getHeight();
	int oldMX = mx+camx;								// New mouse coordinates, relation to camera
	int oldMY = my+camy;								// New mouse coordinates, relation to camera
	int clampSize = 32;									// Magnet pixel size
	int remainderW = oldMX % clampSize;
	int remainderH = oldMY % clampSize;
	newMx = mx - remainderW;						// New mouse coordinates, locked on x32 coordinates
	newMy = my - remainderH;						// New mouse coordinates, locked on x32 coordinates*/

	// Grid like coordinates of mouse
	mex = (screenWidth*mx)/gWindow.getWidth();				// New mouse coordinates, no relation to camera
	mey = (screenHeight*my)/gWindow.getHeight();				// New mouse coordinates, no relation to camera
	//int mex = (1280*mx)/gWindow.getWidth();
	//int mey = (720*my)/gWindow.getHeight();
	int oldMX = mex+camx;								// New mouse coordinates, relation to camera
	int oldMY = mey+camy;								// New mouse coordinates, relation to camera
	int clampSize = tb.tileSizeInWorldW;									// Magnet pixel size
	if (editor) {
		if (place_type == 0) {
			if (shift) {
				clampSize = tb.tileSizeInWorldW/2;
			}else{
				clampSize = tb.tileSizeInWorldW;
			}
		}else if (place_type == 1) {
			if (shift) {
				clampSize = 4;
			}else{
				clampSize = tb.tileSizeInWorldW;
			}
		}else if (place_type == 3) {
			if (shift) {
				clampSize = ite.getItemSizeW()/2;
			}else{
				clampSize = ite.getItemSizeW();
			}
		}else if (place_type == 4) {
			if (shift) {
				clampSize = mb.pixelSizeW/2;
			}else{
				clampSize = mb.pixelSizeW;
			}
		}
	}
	int remainderW = oldMX % clampSize;
	int remainderH = oldMY % clampSize;
	newMx = mex - remainderW;						// New mouse coordinates, locked on x32 coordinates
	newMy = mey - remainderH;						// New mouse coordinates, locked on x32 coordinates

	// Update Variables
	bool test;
	test = true;

	// Generate Stars
	if (test){
	//	part.genStars(particle, gWindow.getWidth(), gWindow.getHeight());
	}

	// Update tiles
	tl.updateTile(tile, gWindow, player.getX(), player.getY(), player.getW(), player.getH(),
							   newMx+camx, newMy+camy,
							   mex+camx, mey+camy,
							   camx, camy,
							   &rTiles[0]);

	// Update tiles
	tlc.Update(tilec, map, newMx+camx, newMy+camy, mex+camx, mey+camy, camx, camy);

	// Update Tile bar, newMx+camx, newMy+camy, mex+camx, mey+camy, camx, camy
	tb.update(tilebar, gWindow, mex, mey, camx, camy);

	// Editor
	if (editor) {

		////////////////////////////////////////////////////////////////////////////////////////////////////
		//------------------------------------------------------------------------------------------------//
		//--------------------------- Update editor Updates from other classes ---------------------------//

		// Boss.cpp class

		// Update Editor boss
		bos.UpdateEditor(boss, mex+camx, mey+camy, camx, camy);

		// Update Editor mob
		mb.UpdateEditor(mob, mex+camx, mey+camy, camx, camy);

		// Update items
		ite.UpdateEditor(item, newMx+camx, newMy+camy, mex+camx, mey+camy, camx, camy);

		//--------------------------- Update editor Updates from other classes ---------------------------//
		//------------------------------------------------------------------------------------------------//
		////////////////////////////////////////////////////////////////////////////////////////////////////

		// If Left Mouse Button is being held down
		if (leftClick) {
			// If not on Tile-bar, place other tiles
			if (!tb.touching) {

				// Remove Tiles
				if (place_type == 0) {

					// Remove Tiles if left clicking, "0" in second argument
					// Also removes them by how many Tiles we are going to place horiz. and verti.
					tl.removeTile(tile, 0);
				}

				// Remove Tilecs
				else if (place_type == 1) {
					tlc.RemoveTile(tilec, 0);
				}

				// Remove Bosses
				else if (place_type == 2) {
					bos.Remove(boss);
				}

				// Remove Items
				else if (place_type == 3) {
					ite.Remove(item, 0);
				}

				// Remove Items
				else if (place_type == 4) {
					mb.Remove(mob);
				}
			}else{
				if (shift) {
					tb.selectBlockMultiple(tilebar, tl.id, mex, mey);
				// Pen Tool, select a Tile from the TileBar
				}else{
					tb.selectBlock(tilebar, tl.id);
				}
			}
		}

		// If Right Mouse Button is being held down
		if (rightClick)
		{
			// If not on Tile-bar, place other tiles
			if (!tb.touching) {
				// Tiles
				if (place_type == 0) {
					tl.spawnTile(tile, newMx, newMy, camx, camy, &rTiles[0]);
				}

				// Tilecs
				else if (place_type == 1) {
					tlc.SpawnAll(tilec, newMx, newMy, camx, camy);
				}

				// Boss
				else if (place_type == 2) {
					// Do nothing here
				}

				// Items
				else if (place_type == 3) {
					ite.SpawnAll(item, newMx, newMy, camx, camy);
				}
			}else{
				// Pen Tool, select a Tile from the TileBar
				tb.selectBlock(tilebar, tl.id);
			}
		}
	}

	// Update debug texts
	tex.updateDebugText(text);

	// Update Particles
	part.Update(particles, map.x, map.y, map.w, map.h);
	part.UpdateVFX(particles, map.x, map.y, map.w, map.h);
	part.UpdateBullets(particles, map.x, map.y, map.w, map.h);

	// Update Asteroids
	/*aste.updateAsteroid(asteroid, particles, part,
								  player.alive, player.getInvurnerableStatus(), player.score, enem.mileScore, player.health,
								  mx, my, camx, camy, gWindow.getWidth(), gWindow.getHeight(),
								  map.x, map.y, map.w, map.h);*/

	// Update Spawner: spawns an Asteroid
	//spaw.update(spawner, asteroid, aste, player.x+player.w/2, player.y+player.h/2, mx, my, camx, camy);


	// Check collision between Player & Tiles
	//tl.checkCollision(tile, player.x, player.y, player.realw, player.realh, player.jumpstate, player.vX, player.vY, player.jumps);

	// If not in editor mode
	// If player mode
	if (!editor) {

		// Update damage texts
		tex.updateDamageText(text);

		// If Player is alive
		if (player.alive) {
			// update zombies
			/*zom.update(zombie, particles, part, map, player, sAtariBoom, camx, camy);

			// Update Enemy
			enem.update(enemy,
					particles, part,
						player.alive,
						player.x+player.w/2, player.y+player.h/2, player.w/2,
						player.score, map.x, map.y, map.w, map.h,
						sAtariBoom, sAtariBoom);*/
		}

		// update zombie manually
		/*for (int i = 0; i < 128; i++) {
			if (asteroid[i].alive){
				int rands = rand() % 3 + 1;
				double newX = asteroid[i].x+asteroid[i].w/2;
				double newY = asteroid[i].y+asteroid[i].h/2;
				part.spawnExplosion(particles, newX, newY, rands);
			}
		}*/

		// Update Player & Asteroid collision check
		//checkCollisionPlayerAsteroid(player, asteroid);

		// Update Player Particle & Asteroid collision check
		//checkCollisionParticleAsteroid(particles, part, asteroid, player);

		// Collision, particle & zombie
		/*checkCollisionParticleZombie();

		// Update Asteroids: Wave re-spawn
		spawnAsteroidsNow2();*/

		// Move boss towards player
		bos.GetDistanceOfPlayer(boss, player.getX(), player.getY(), player.getW(), player.getH(), &player.x2, &player.y2);

		// Update boss
		bos.Update(boss, obj, object, particles, part, map, mex+camx, mey+camy, camx, camy, player.alive);

		// Move Mob towards player
		mb.GetDistanceOfPlayer(mob, player.getX(), player.getY(), player.getW(), player.getH(), &player.x2, &player.y2);

		// Update Mob
		mb.Update(mob, obj, object, particles, part, map, mex+camx, mey+camy, camx, camy, player.alive);

		// Collision check Player & Item
		checkCollisionPlayerItem();

		// Mobs.cpp
		{
			checkCollisionParticleMob();
			checkPlayerAttacksCollisionMob();
			checkMobPlayerVision();
			checkCollisionTileMob();
		}

		// Update items
		ite.Update(item, newMx+camx, newMy+camy, mex+camx, mey+camy, camx, camy,
				         player.getX(), player.getY(), player.getW(), player.getH());

		// Update Player
		player.Update(map,
					  particles, part,
					  tl, tile,
					  tlc, tilec,
					  obj, object,
					  mex, mey, camx, camy,
					  spawnX, spawnY,
					  gWindow, gRenderer,
					  {255,255,255},
					  RestartLevel,
					  LevelToLoad, playerCallingToShakeCamera);

		// Shake camera if player wants to, but right not not used
		if (playerCallingToShakeCamera) {
			ShakeCamera();
		}

		// If we get a true from Player.cpp that we should restart the levelm then restart the level
		if (RestartLevel) {
			RestartLevel = false;

			// Load level selected from ActSelection.cpp
			LoadLevel();
		}

		// Update objects
		obj.update(object);

		// Check collision between Particle & Tile
		checkCollisionParticleTile();

		// Check collision between Particle & Boss
		checkCollisionParticleBoss();

		// Check collision between Boss & Tile
		checkBossTileCollision();

		// Player and Tilec collision check with LoadNextLevel Tilec's
		checkPlayerTilceCollision();

		// Unlock Locked Tile doors
		checkPlayerTileCollision();

		// Check collision between Player attacks & Boss
		checkPlayerAttacksCollisionBoss();

		// Check collision between Player attacks & Tile
		checkPlayerAttacksTileCollision();

		// Check collision between Player attacks & Boss Particle
		checkPlayerAttacksBossParticleCollision();

		// Check collision between Boss attacks & Player
		//checkBossAttacksCollisionPlayer();

		// Check collision between Boss & Player
		//checkCollisionBossPlayer();

		// Update Enemy Particle & Player collision check
		checkCollisionParticlePlayer();

		// Check collision between Player Particle & Boss Particle
		checkCollisionParticleParticle();

		//---------- Boss deafeated? Update levelsCompleted.mp
		checkBossOrMobDied();

		// Damage text: for zombie
		tex.updateDamageText(text);
	}

	// Damage text: for zombie
	tex.updateDebugText(text);

	// Update camera
	if (camLeft) {
		camx -= 8;
	}
	if (camRight) {
		camx += 8;
	}
	if (camUp) {
		camy -= 8;
	}
	if (camDown) {
		camy += 8;
	}
	//camx = map.x + map.w / 2 - screenWidth/2;
	//camy = map.y + map.h / 2 - screenHeight/2;

	// center camera on target
	{
		//camx = player.x + player.w / 2 - gWindow.getWidth()/2;
		//camy = player.y + player.h / 2 - gWindow.getHeight()/2;
		if (camlock) {

			// Get center of player
			float bmx, bmy;
			bmx  = player.x+player.w/2-screenWidth/2;
			bmy  = player.y+player.h/2-screenHeight/2;

			// Get distnace from player
			float distance = getDistance(bmx, bmy, camx, camy);

			// Camera target
			float vX, vY;

			// If camera distance from player is > 1, go to the player!
			if (distance >= 1){
				vX 	= 2 * (10*distance/256) * (bmx - camx) / distance;
				vY 	= 2 * (10*distance/256) * (bmy - camy) / distance;

				camx += vX;
				camy += vY;
			}

			// The reason we call camera bnounds before camera shaking is because the
			// screen wont shake if called after handling screen shaking
			// Camera bounds
			if (!editor) {
				if( camx < 0 ){
					camx = 0;
				}
				if( camy < 0 ){
					camy = 0;
				}
				if( camx+screenWidth > map.w ){
					camx = map.w-screenWidth;
				}
				if( camy+screenHeight > map.h ){
					camy = map.h-screenHeight ;
				}
			}

			// Handle camera shake
			if (camshake) {

				// Shake going right
				if (rustleDirX == 1) {

					// Shake camera
					camx += rustleW;

					// Next direction to shake
					rustleDirX = 2;

					// Lower rustle of camera every frame
					if (rustleW > 0) {
						rustleW -= rustleSpe;
					} else {
						// Stop shaking
						camshake = false;
					}
				}

				// Shake going down
				else if (rustleDirX == 2) {

					// Shake camera
					camy += rustleW;

					// Next direction to shake
					rustleDirX = 3;

					// Lower rustle of camera every frame
					if (rustleW > 0) {
						rustleW -= rustleSpe;
					} else {
						// Stop shaking
						camshake = false;
					}
				}

				// Shake going left
				else if (rustleDirX == 3) {

					// Shake camera
					camx -= rustleW;

					// Next direction to shake
					rustleDirX = 4;

					// Lower rustle of camera every frame
					if (rustleW > 0) {
						rustleW -= rustleSpe;
					} else {
						// Stop shaking
						camshake = false;
					}
				}

				// Shake going up
				else if (rustleDirX == 4) {

					// Shake camera
					camy -= rustleW;

					// Next direction to shake
					rustleDirX = 1;

					// Lower rustle of camera every frame
					if (rustleW > 0) {
						rustleW -= rustleSpe;
					} else {
						// Stop shaking
						camshake = false;
					}
				}
			}
		}else{
			if (editor) {
				if (mex <= 0) {
				//	camx -= 10;
				}
				if (mey <= 0) {
				//	camy -= 10;
				}
				if (mex + 2 > screenWidth) {
				//	camx += 10;
				}
				if (mey + 2 > screenHeight) {
				//	camy += 10;
				}
			}
		}
	}
}

// Render foreground
void PlayGame::RenderFG(SDL_Renderer *gRenderer, LWindow &gWindow) {
	// render map
	for (int j=0; j<6; j++) {
		for (int i=0; i<6; i++) {
			gBG.render(gRenderer, map.x+i*837-camx, map.y+j*837-camy, 837, 837);
		}
	}
}

// Render shadows
void PlayGame::RenderShadows(SDL_Renderer *gRenderer, LWindow &gWindow) {

	// Render shadow for Tiles.cpp
	{
		for (int i = 0; i < tl.max; i++) {
			if (tile[i].alive) {
				if (tile[i].id == 1) {
					if (!tile[i].startJarBreaking) {

						gShadow.setAlpha(110);
						int shadowSizeW = tile[i].w * 0.80;
						int shadowSizeH = tile[i].w * 0.50;
						int yOffsetShadow = -5;
						gShadow.render(gRenderer,
								tile[i].x+tile[i].w/2 - shadowSizeW/2 - camx,
								tile[i].y+tile[i].h - shadowSizeH/2+yOffsetShadow - camy,
								shadowSizeW, shadowSizeH);
					}
				}
			}
		}
	}

	// Render shadow for Item.cpp
	{
		for (int i = 0; i < ite.max; i++) {
			if (item[i].alive) {
				gShadow.setAlpha(110);
				int shadowSize = item[i].w/2 + item[i].hoverAmount;
				int yOffsetShadow = -(shadowSize/4);
				yOffsetShadow +=  item[i].yOffset;
				gShadow.render(gRenderer, item[i].x+item[i].w/2-shadowSize/2 - camx,
										  item[i].y+item[i].h-shadowSize/2+yOffsetShadow - camy,
										  shadowSize, shadowSize);
			}
		}
	}

	// Render Boss Shadow on floor
	bos.RenderShadow(gRenderer, boss, camx, camy);

	// Render Mob Shadow on floor
	mb.RenderShadow(gRenderer, mob, camx, camy);

	// Render player shadow
	{
		int shadowSizeW = 24;
		int shadowSizeH = 10;
		gShadow.setAlpha(110);
		gShadow.render(gRenderer, player.getX()- camx,
								  player.getY()+player.getH()-shadowSizeH/2-5 - camy,
								  shadowSizeW, shadowSizeH);
	}

	// Render particle shadow
	{
		for (int i = 0; i < part.max; i++) {
			if (particles[i].alive) {
				gShadow.setAlpha(110);
				int shadowSizeW = particles[i].w/2;
				int shadowSizeH = particles[i].h/2;
				gShadow.render(gRenderer, particles[i].x+particles[i].w/2-shadowSizeW/2 - camx, particles[i].y - camy,
						shadowSizeW,
						shadowSizeH);
			}
		}
	}
}

// Render everything
void PlayGame::Render(SDL_Renderer *gRenderer, LWindow &gWindow) {

	// Render Colonies that spawn Asteroids
	spaw.render(spawner, camx, camy, gRenderer);

	// Render different layers
	{

		/* Layers
		 * -1: Floor
		 * 0: ???
		 * 1: Walls				- Walls. Only rendered on top of Player when player.y+player.h < tile.x+tile.w
		 * 2: Appliance			- Always rendered on top of Player
		 * 3: Roof 				- Third block. Always rendered on top of Player.
		 */


		// Render tile, ground below ground
		tl.renderTile(gRenderer, tile, -1, camx, camy, &rJar[0]);


		// Render tile, ground
		tl.renderTile(gRenderer, tile, 0, camx, camy, &rJar[0]);

		// Render Tile in behind player sprite
		tl.RenderBehindPlayer(gRenderer, tile, 1, camx, camy, &rTiles[0], &rJar[0]);

			// Render shadows
			RenderShadows(gRenderer, gWindow);

		// Render Tile in behind player sprite
		tl.RenderBehindPlayer(gRenderer, tile, 2, camx, camy, &rTiles[0], &rJar[0]);

			// Render items
			ite.RenderBehindPlayer(gRenderer, item, camx, camy);

			// Render Tile breaking
			RenderTileBreakingBehind();

			// Render Mob
			mb.RenderBack(gRenderer, mob, camx, camy);

			// Render Boss
			bos.RenderBack(gRenderer, boss, camx, camy);

			// Render our player
			player.Render(mex, mey, camx, camy, gWindow,
					  gRenderer, {255,255,255}, part.count);

			// Render Mob
			mb.RenderFront(gRenderer, mob, camx, camy);

			// Render Boss
			bos.RenderFront(gRenderer, boss, camx, camy);

		// Render items
		ite.RenderOnTopOfPlayer(gRenderer, item, camx, camy);

		// Render Tile on top of player
		tl.RenderOnTopOfPlayer(gRenderer, tile, 1, camx, camy, &rTiles[0], &rJar[0]);

		// Render Tile on top of player
		tl.RenderOnTopOfPlayer(gRenderer, tile, 2, camx, camy, &rTiles[0], &rJar[0]);

		// Render Tile breaking
		RenderTileBreakingFront();

			// Render Star particles
			part.RenderVFX(particles, camx, camy, 1, gRenderer);

			// Render Particles
			part.RenderBullets(particles, camx, camy, 1, gRenderer);

		// Render Tile, ceiling
		tl.renderTile(gRenderer, tile, 3, camx, camy, &rJar[0]);

		// Render Tile, ceiling
		tl.renderTile(gRenderer, tile, 4, camx, camy, &rJar[0]);

		// Render Tile, ceiling
		tl.renderTile(gRenderer, tile, 5, camx, camy, &rJar[0]);

		// Render Tile, ceiling
		tl.renderTile(gRenderer, tile, 6, camx, camy, &rJar[0]);

	}
}


// Render everything
void PlayGame::RenderUI(SDL_Renderer *gRenderer, LWindow &gWindow)
{

	// Render "E" prompt on doors to new areas
	tlc.RenderUI(gRenderer, tilec, camx, camy);

	// Render Boss Health
	bos.RenderUI(gRenderer, boss, camx, camy);

	// Render Mob Health
	mb.RenderUI(gRenderer, mob, camx, camy);

	// Render item texts
	ite.RenderUI(gRenderer, item, camx, camy);

	// Render "E" prompt on doors to new areas
	tl.RenderUI(gRenderer, tile, camx, camy);

	// Render Player Health
	player.RenderUI(gRenderer, camx, camy, this->LevelToLoad);

	// Render number of enemies left
	std::stringstream tempss;
	tempss << "Eliminate: "<< mb.count;
	fonts.gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {244, 144, 20}, fonts.gFont12);
	fonts.gText.setAlpha(255);
	fonts.gText.render(gRenderer, screenWidth * 0.95 - fonts.gText.getWidth(), screenHeight * 0.25,
			fonts.gText.getWidth(), fonts.gText.getHeight());

	// Render number of jars left
	{
		tempss.str(std::string());
		tempss << "Destroy: "<< jarsLeft.size();
		fonts.gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {244, 144, 20}, fonts.gFont12);
		fonts.gText.setAlpha(255);
		fonts.gText.render(gRenderer, screenWidth * 0.95 - fonts.gText.getWidth(), screenHeight * 0.25 + (fonts.gText.getHeight()*1),
				fonts.gText.getWidth(), fonts.gText.getHeight());
	}
}

// Render debug information
void PlayGame::RenderDebug(SDL_Renderer *gRenderer)
{
	// If debugging, show debug info
	if (debug)
	{
		// Render Player debug
		player.RenderDebug(gRenderer, camx, camy);

		// Render Particle debug
		part.RenderBulletDebug(particles, camx, camy, gRenderer);

		// Render player attack collisions
		obj.render(gRenderer, object, camx, camy);

		// Render collision tiles
		tlc.Render(gRenderer, tilec, 0, camx, camy);

		// Render Boss text
		bos.RenderDebug(gRenderer, boss, camx, camy);

		// Render Mob text
		mb.RenderDebug(gRenderer, mob, camx, camy);

		// Render Item text
		ite.RenderDebug(gRenderer, item, camx, camy);

		// Render item text
		for (int i = 0; i < ite.max; i++)
		{
			if (item[i].alive)
			{
				// Render text
				std::stringstream tempss;
				tempss << "id: " << item[i].id << ", d: " << item[i].damage;
				fonts.gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {255,255,255}, fonts.gFont12);
				fonts.gText.setAlpha(255);
				fonts.gText.render(gRenderer, item[i].x-camx, item[i].y-fonts.gText.getHeight()-camy, fonts.gText.getWidth(), fonts.gText.getHeight());

			}
		}

		// Render object text
		for (int i = 0; i < obj.max; i++)
		{
			if (object[i].alive)
			{
				if (object[i].id == 1)
				{
					// Render text
					std::stringstream tempss;
					tempss << "x: " << *object[i].xFollow << ", y: " << *object[i].yFollow;
					fonts.gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {255,255,255}, fonts.gFont12);
					fonts.gText.setAlpha(255);
					fonts.gText.render(gRenderer, object[i].x-camx, object[i].y-fonts.gText.getHeight()-camy, fonts.gText.getWidth(), fonts.gText.getHeight());

				}

			}
		}
		// Render Tile debug
		tl.renderTileDebug(gRenderer, tile, newMx, newMy, mex, mey, camx, camy, &rTiles[0]);

		// Render Tilec debug texts
		tlc.RenderDebug(gRenderer, tilec, camx, camy);

		// Render which collision-tile we have selected for placement]
		/*int NumberOfCollisionTilesWidth = 32 * 3;
		for (int i=0; i<3; i++)
		{
			if (i == 0) {
				SDL_Rect tempr = {0 + NumberOfCollisionTilesWidth  + i*32, screenHeight-34, 32, 32};
				SDL_SetRenderDrawColor(gRenderer, 0, 0, 255, 255);
				SDL_RenderDrawRect(gRenderer, &tempr);
			}else if (i == 1) {
				SDL_Rect tempr = {0 + NumberOfCollisionTilesWidth + i*32, screenHeight-34, 32, 32};
				SDL_SetRenderDrawColor(gRenderer, 255, 255, 0, 255);
				SDL_RenderDrawRect(gRenderer, &tempr);
			}else if (i == 2) {
				SDL_Rect tempr = {0 + NumberOfCollisionTilesWidth + i*32, screenHeight-34, 32, 32};
				SDL_SetRenderDrawColor(gRenderer, 255, 0, 255, 255);
				SDL_RenderDrawRect(gRenderer, &tempr);
			}
		}*/

		// Render Player spawn point
		SDL_Rect tempRect = {spawnX-camx, spawnY-camy, tb.tileSizeInWorldW, tb.tileSizeInWorldH};
		SDL_SetRenderDrawColor(gRenderer, 0, 255, 200, 255);
		SDL_RenderDrawRect(gRenderer, &tempRect);

		// Render level size
		SDL_Rect tempr = {map.x-camx, map.y-camy, map.w, map.h};
		SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
		SDL_RenderDrawRect(gRenderer, &tempr);

		// Render hand debug info
		std::stringstream tempss;
		tempss << "mapW: " 		<< map.w 		<< ", mapH: " 	<< map.h;
		tempss << ", destructible: " 		<< tl.destructible 		<< ", LevelToLoad: " 	<< this->LevelToLoad;
		tempss << ", previousLevel: " 		<< previousLevel 		<< ", LevelToLoad: " 	<< this->LevelToLoad;
		tempss << ", place_type: " 			<< place_type 			<< ", tl.id: " 			<< tl.id;
		tempss << ", mb.count: " 			<< mb.count 			<< ", tileCount: " 			<< tl.tileCount;
		tempss << ", tl.collisionTile: " 	<< tl.collisionTile 	<< ", layer: " 			<< tl.layer;
		tempss << ", tlc.newPos: " << tlc.newPos;


				/*	   << ", layer: " 		<< tl.layer<< ", tlc.layer: " << tlc.layer << ", editor: " << editor
			   << ", tl.multiW: " 	<< tl.multiW << ", tl.multiH: " << tl.multiH << ", tl.count: " << tl.tileCount;
		tempss << ", tlc.multiW: " 	<< tlc.multiW << ", tlc.multiH: " << tlc.multiH << ", tlc.count: " << tlc.count;*/
		fonts.gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {255,255,255}, fonts.gFont12, 250);
		fonts.gText.setAlpha(255);
		fonts.gText.render(gRenderer, xOffetDebug+screenWidth-fonts.gText.getWidth(), yOffetDebug, fonts.gText.getWidth(), fonts.gText.getHeight());
	}

	// Editor debug menu
	if (editor) {

		// Render tile debug
		if (debug){

			// Render Tiles in Hand
			if (place_type == 0)
			{
				tl.RenderHand(gRenderer, tile, newMx, newMy, mex, mey, camx, camy, &rTiles[0]);
			}

			// Render Tilecs in Hand
			else if (place_type == 1)
			{
				tlc.RenderHand(gRenderer, tilec, newMx, newMy, mex, mey);
			}

			// Render Boss in Hand
			else if (place_type == 2)
			{
				bos.RenderHand(gRenderer, boss, newMx, newMy, mex, mey, camx, camy);
			}

			// Render Item (Sword) in Hand
			else if (place_type == 3)
			{
				ite.RenderHand(gRenderer, item, newMx, newMy, mex, mey);
			}

			// Render Mob in Hand
			else if (place_type == 4)
			{
				mb.RenderHand(gRenderer, mob, newMx, newMy, mex, mey, camx, camy);
			}
		}

		// Render Tile Bar
		tb.render(gRenderer, tilebar, tl.id);

		// Render confirmation prompt
		if (promptConfirm) {

			// Render box around text

			// Render: "Are you sure you want to save?"
			std::stringstream text;
			text << "Are you sure you want to ";
			if (promptType == 0)
				text << "Load level " << LevelToLoad << "?";
			else
				text << "Save level " << LevelToLoad << "?";

			// Prompt position
			int promptX;
			int promptY;
			gRect.setAlpha(200);
			gRect.setColor(240, 159, 80);

			// Prompt
			{
				// Get text
				fonts.gText.loadFromRenderedText(gRenderer, text.str().c_str(), {255,255,255}, fonts.gFont12);
				promptX = screenWidth * 0.5 - fonts.gText.getWidth()/2;
				promptY = screenHeight * 0.90 - fonts.gText.getHeight();

				// Render BG texture
				gRect.render(gRenderer, promptX-25, promptY-20, fonts.gText.getWidth()+50, fonts.gText.getHeight()+40);

				// Render text
				fonts.gText.render(gRenderer, promptX, promptY, fonts.gText.getWidth(), fonts.gText.getHeight());
			}

			// Render "YES"
			{
				// Get text
				fonts.gText.loadFromRenderedText(gRenderer, "YES", {0,144,0}, fonts.gFont12);
				promptX = screenWidth * 0.25 - fonts.gText.getWidth()/2;
				promptY = screenHeight * 0.50 - fonts.gText.getHeight()/2;

				// Render BG texture
				gRect.render(gRenderer, promptX-screenWidth/4, promptY-(screenWidth/3)/2, screenWidth/2, screenWidth/3);

				// Render text
				fonts.gText.render(gRenderer, promptX, promptY, fonts.gText.getWidth(), fonts.gText.getHeight());
			}

			// Render "NO"
			{
				// Get text
				promptX = screenWidth * 0.75 - fonts.gText.getWidth()/2;
				promptY = screenHeight * 0.50 - fonts.gText.getHeight()/2;
				fonts.gText.loadFromRenderedText(gRenderer, "NO", {180,0,0}, fonts.gFont12);

				// Render BG texture
				gRect.render(gRenderer, promptX-screenWidth/4, promptY-(screenWidth/3)/2, screenWidth/2, screenWidth/3);

				// Render text
				fonts.gText.render(gRenderer, promptX, promptY, fonts.gText.getWidth(), fonts.gText.getHeight());
			}
		}
	}
}

// Render text
void PlayGame::RenderText(SDL_Renderer *gRenderer, LWindow &gWindow)
{
	// Render map
	//map.render(gRenderer, camx, camy);

	// Render text that spawn into world then dissapear after x amount of time
	for (int i = 0; i < 100; i++)
	{
		if (text[i].alive)
		{
			// Render text
			std::stringstream tempss;
			tempss << text[i].textfield.c_str();

			// original but commented out
			//gText.render(gRenderer, 0, 0+i*28, gText.getWidth(), gText.getHeight());
			//gText.render(gRenderer, text[i].x-camx, text[i].y+i*28-camy, gText.getWidth(), gText.getHeight());

			// Render always on the top left corner of the screen (general debug information)
			if (text[i].type == 0) {
				fonts.gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), text[i].color, fonts.gFont12);
				text[i].w = fonts.gText.getWidth();
				text[i].h = fonts.gText.getHeight();
				fonts.gText.setAlpha(text[i].alpha);
				fonts.gText.render(gRenderer, 0, 0 + i*28, text[i].w, text[i].h);
			}

			// These texts have an x & y coordinate, render these (like damage text)
			else if (text[i].type == 1) {
				fonts.gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), text[i].color, fonts.gFont12);
				fonts.gText.setAlpha(text[i].alpha);
				fonts.gText.render(gRenderer, text[i].x-fonts.gText.getWidth()/2-camx, text[i].y-camy,
						fonts.gText.getWidth(), fonts.gText.getHeight());
			}
		}
	}

	// Render mouse location
	gCursor.render(gRenderer, mex, mey, 20, 20);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////// Global Functions ////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------------------------------------------------//

void PlayGame::checkCollisionParticleTile()
{
	// Particle
	for (int i = 0; i < part.max; i++)
	{
		if (particles[i].alive)
		{
			if (particles[i].hurtType >= -1 && particles[i].hurtType <= 1)
			{
				for (int j = 0; j < tl.max; j++)
				{
					if (tile[j].alive)
					{
						if (tile[j].collisionTile)
						{
							// Particle target center
							float bmx = tile[j].x + tile[j].w/2;
							float bmy = tile[j].y + tile[j].h/2;

							// Particle center
							float bmx2 = particles[i].x + particles[i].w/2;
							float bmy2 = particles[i].y + particles[i].h/2;
							float distance = sqrt((bmx - bmx2) * (bmx - bmx2)+
												  (bmy - bmy2) * (bmy - bmy2));

							// Circle Collision
							if (distance < tile[j].w/2 + particles[i].w/2)
							{
								if (tile[j].id == 1 || tile[j].id == 4) {
									// If tile destructible, lower health
									if (tile[j].destructible) {
										if (!tile[j].startJarBreaking) {
											// Shake camera
											ShakeCamera();

											// Damage health of Tile
											tile[j].health -= particles[i].damage;

											// Damage Tile
											if (tile[j].health <= 0) {
												// Spawn random item
												{
													// Spawn health or coin, or power up 1 or power up 2
													int randNum = rand() % 10;
													int tempID;

													if (randNum >= 0 && randNum <= 2) {
														tempID = 25;
														ite.SpawnAndThrowItem(item,
																tile[j].x+tile[j].w/2-ite.rSwords[tempID].w/2,
																tile[j].y+tile[j].h/2-ite.rSwords[tempID].h/2,
																tempID, 0.0, randDouble(1, 2));
													} else if (randNum >= 3 && randNum <= 5) {
														tempID = 28;
														ite.SpawnAndThrowItem(item,
																tile[j].x+tile[j].w/2-ite.rSwords[tempID].w/2,
																tile[j].y+tile[j].h/2-ite.rSwords[tempID].h/2,
																tempID, 0.0, randDouble(1, 2));
													} else if (randNum == 6) {
														tempID = 29;
														ite.SpawnAndThrowItem(item,
																tile[j].x+tile[j].w/2-ite.rSwords[tempID].w/2,
																tile[j].y+tile[j].h/2-ite.rSwords[tempID].h/2,
																tempID, 0.0, randDouble(1, 2));
													} else if (randNum == 7) {
														tempID = 30;
														ite.SpawnAndThrowItem(item,
																tile[j].x+tile[j].w/2-ite.rSwords[tempID].w/2,
																tile[j].y+tile[j].h/2-ite.rSwords[tempID].h/2,
																tempID, 0.0, randDouble(1, 2));
													}

													// If jar Tile
													if (tile[j].id == 1) {

														// Start breaking animation for Jar Tile
														tile[j].startJarBreaking = true;

														// Remove a jar from the vector
														jarsLeft.pop_back();
													}

													// If Barrel Tile
													else if (tile[j].id == 4) {

														// Remove Tile
														tile[j].alive = false;
														tl.tileCount--;
													}
												}
											}
											// Spawn particle effect
											part.spawnTileHitVFX(particles,
													getCenter(particles[i].x, particles[i].w),
													getCenter(particles[i].y, particles[i].h),
													particles[i].w, particles[i].h);

											// Remove particle
											particles[i].time = 0;
											particles[i].alive = false;
											part.count--;

											// Play hit sound effect
								            Mix_PlayChannel(-1, settings.sPotBreak, 0);
								            Mix_PlayChannel(-1, settings.sValidation, 0);
										}
									}
								}

								// If every other Tile
								else {


									// Spawn particle effect
									part.spawnTileHitVFX(particles,
											getCenter(particles[i].x, particles[i].w),
											getCenter(particles[i].y, particles[i].h),
											particles[i].w, particles[i].h);

									// Remove particle
									particles[i].time = 0;
									particles[i].alive = false;
									part.count--;

									// Play hit sound effect
									Mix_PlayChannel(-1, settings.sParrySuccess, 0);
								}

							}
						}
					}
				}
			}	// end Particle
		}
	}		// end Zombie
}

void PlayGame::checkCollisionParticleBoss()
{
	for (int j = 0; j < part.max; j++)
	{
		if (particles[j].alive)
		{
			if (particles[j].hurtType == -1 || particles[j].hurtType == 0)
			{
				for (int i = 0; i < bos.max; i++)
				{
					if (boss[i].alive)
					{
						// Get center of attack-particle (spawned by the player attacking)
						float bmx = boss[i].x+boss[i].w/2;
						float bmy = boss[i].y+boss[i].h/2;

						// Get center of particles
						float bmx2 = particles[j].x+particles[j].w/2;
						float bmy2 = particles[j].y+particles[j].h/2;

						// Get distance
						float distance = sqrt((bmx - bmx2) * (bmx - bmx2)+
											  (bmy - bmy2) * (bmy - bmy2));

						// If distance is less than 50 pixels
						//if (distance < 50)
						//{
							// Get angle of boss relative to attack-particle
							float angle = atan2(bmy - bmy2,bmx - bmx2);
							angle = angle * (180 / 3.1416);
							if (angle < 0) {
								angle = 360 - (-angle);
							}

							// Handle radians, cos, sin
							float radians = (3.1415926536/180)*(angle);
							float Cos = floor(cos(radians)*10+0.5)/10;
							float Sin = floor(sin(radians)*10+0.5)/10;

							////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//--------------------------------------------------------------------------------------------------------------------------------//
							//----------------------------- Collision Detection based on player-attack hit-box and Boss hit-box -----------------------------//
							// If particle slash if within X distance then move the boss away
							// collision occurred
							if (distance < boss[i].w/2 + particles[j].w/2)
							{
								// Shake camera
								ShakeCamera();

								// Flash Bosses sprite
								boss[i].flash = true;

								// Remove particle
								particles[j].time = 0;
								particles[j].alive = false;
								part.count--;

								// New velocity going away from Bullet Particle
								float newvX = 0.25 * (bmx - bmx2) / distance;
								float newvY = 0.25 * (bmy - bmy2) / distance;

								// Move the boss in someway
								boss[i].vX += newvX;
								boss[i].vY += newvY;
								//boss[i].vX += player.getKnockBackPower()/2 * xDir;

								// Play hit sound effect
				                Mix_PlayChannel(-1, settings.sCastHitBoss, 0);

				                // Subtract boss health
				                boss[i].health -= player.getCastDamage();

				                // Increase player score based on projectile that hit Boss
				                if (particles[j].hurtType == -1) {
					                player.IncreaseScore(10);
				                }
				                if (particles[j].hurtType == 0) {
					                player.IncreaseScore(5);
				                }

				                // Show damage text (it will print how much damage the player did to the boss)
				    			std::stringstream tempss;
				    			tempss << player.getCastDamage();
				    			tex.spawn(gRenderer, text, boss[i].x+boss[i].w/2, boss[i].y+boss[i].w/2-15, 0.0, -0.5, 150,
				    					tempss.str().c_str(), 1, {255, 255, 0, 255});
							}
							//----------------------------- Collision Detection based on player-attack hit-box and Boss hit-box -----------------------------//
							////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//--------------------------------------------------------------------------------------------------------------------------------//
						//}
					}
				}
			}
		}
	}
}

void PlayGame::checkCollisionPlayerItem() {
	// If player is alive
	if (player.alive) {
		for (int i = 0; i < ite.max; i++) {
			if (item[i].alive) {

				// Collision happening
				if (checkCollision(item[i].x, item[i].y, item[i].w, item[i].h,
								   player.getX(), player.getY(), player.getW(), player.getH())) {

					// Fists and Swords only
					if (item[i].id < 23) {

						// Prompt self, this will render an "E" above the Item to equip it
						item[i].promptSelf = true;

						// If player is pressing equip
						if (player.getEquipState())
						{
							// Drop old sword
							int old_sword_id = player.getItemID();

							// Equip new sword
							player.EquipSword(item[i].id, item[i].damage, item[i].slashAtkSpe);

							// Remove item from floor
							item[i].alive = false;
							ite.count--;

							// Drop old sword
							ite.SpawnAndThrowItem(item,
									player.getCenterX()-ite.rSwords[old_sword_id].w/2,
									player.getCenterY(),
									old_sword_id,
									0.0, 0.5);

							// play sound effect
							Mix_PlayChannel(-1, settings.sCastHitBoss, 0);
						}
					}

					// Bombs
					else if (item[i].id == 23) {

						// Remove item
						item[i].alive = false;
						ite.count--;

						// Increase player bombs
						player.IncreaseBombs();

						// play sound effect
						Mix_PlayChannel(-1, settings.sCastHitBoss, 0);

					}

					// Hearts
					else if (item[i].id == 24) {

						// Remove item
						item[i].alive = false;
						ite.count--;

						// Increase player hearts
						player.IncreaseHearts();

						// play sound effect
						Mix_PlayChannel(-1, settings.sCastHitBoss, 0);

					}

					// Coins
					else if (item[i].id == 25) {

						// Remove item
						item[i].alive = false;
						ite.count--;

						// Increase player Coins
						player.IncreaseCoins();

						// play sound effect
						Mix_PlayChannel(-1, settings.sValidation, 0);
					}

					// Silver keys
					else if (item[i].id == 26) {

						// Remove item
						item[i].alive = false;
						ite.count--;

						// Increase player Silver keys
						player.IncreaseSilverKeys();

						// play sound effect
						Mix_PlayChannel(-1, settings.sCastHitBoss, 0);
					}

					// Gold keys
					else if (item[i].id == 27) {

						// Remove item
						item[i].alive = false;
						ite.count--;

						// Increase player Gold keys
						player.IncreaseGoldKeys();

						// play sound effect
						Mix_PlayChannel(-1, settings.sCastHitBoss, 0);
					}

					// Green health
					else if (item[i].id == 28) {

						// Remove item
						item[i].alive = false;
						ite.count--;

						// Increase player Gold keys
						player.IncreaseHealth(25);

						// play sound effect
						Mix_PlayChannel(-1, settings.sCastHitBoss, 0);
					}

					// Power up 1
					else if (item[i].id == 29) {

						// Double shot power up
						player.setPowerUp(1);

						// Remove item
						item[i].alive = false;
						ite.count--;

						// play sound effect
						Mix_PlayChannel(-1, settings.sValidation, 0);
					}

					// Power up 2
					else if (item[i].id == 30) {

						// Triple shot power up
						player.setPowerUp(2);

						// Remove item
						item[i].alive = false;
						ite.count--;

						// play sound effect
						Mix_PlayChannel(-1, settings.sValidation, 0);
					}
				}

				// No collision
				else {
					item[i].promptSelf = false;
				}
			}
		}
	}
}

void PlayGame::checkPlayerTilceCollision() {
	for (int i = 0; i < tlc.max; i++) {
		if (tilec[i].alive){
			if (tilec[i].LevelToLoad >= 1)
			{
				// If player collides with a Tilec that can load levels
				if (checkCollision(player.getX(), player.getY(), player.getW(), player.getH(),
								   tilec[i].x, tilec[i].y, tilec[i].w, tilec[i].h))
				{
					// Prompt self, this will render an "E" above the Item to equip it
					tilec[i].promptSelf = true;

					// If player is pressing equip
					if (player.getEquipState()) {

						// Update number of jars left
						UpdateJarsLeft();

						// If no more mods
						if (mb.count == 0 && jarsLeft.size() == 0) {
							// Save last known position for loading if we come back
							lastKnownPositionX = player.getX();
							lastKnownPositionY = player.getY();

							// Save our current level in our previousLevel variables
							previousLevel = LevelToLoad;

							// Set next level or stage to whatever the Tilec has stored
							this->LevelToLoad = tilec[i].LevelToLoad;

							// Load next level or stage
							LoadLevel();

							// play sound effect
							Mix_PlayChannel(-1, settings.sCastHitBoss, 0);
						}

						// Some mobs are still alive, notify player to eliminate all mobs
						else {

							// If there are more mobs, notify User
							if (mb.count != 0) {

								std::stringstream tempss;
								tempss << mb.count << " mobs left!";
								tex.spawn(gRenderer, text,
										tilec[i].x+tilec[i].w/2,
										tilec[i].y-22,
										0.0, -0.4, 150,
										tempss.str().c_str(), 1, {250, 30, 20, 255});
							}

							// If there are more jars, notify User
							if (jarsLeft.size() != 0) {

								std::stringstream tempss;
								tempss << jarsLeft.size() << " jars left!";
								tex.spawn(gRenderer, text,
										tilec[i].x+tilec[i].w/2,
										tilec[i].y-10,
										0.0, -0.4, 150,
										tempss.str().c_str(), 1, {30, 250, 250, 255});
							}

							// play sound effect
							Mix_PlayChannel(-1, settings.sDownStabHitTilec, 0);
						}
					}
				} else {
					tilec[i].promptSelf = false;
				}
			}
		}
	}
}

void PlayGame::checkPlayerTileCollision()
{
	for (int i = 0; i < tl.max; i++)
	{
		if (tile[i].alive)
		{
			if (tile[i].collisionTile)
			{
				// Floor door Tile
				if (tile[i].id == 5)
				{
					// If collision happened
					if (checkCollision(player.getX(), player.getY(), player.getW(), player.getH(),
									   tile[i].x, tile[i].y, tile[i].w, tile[i].h+10))
					{
						// Prompt self, this will render an "E" above the Item to equip it
						tile[i].promptSelf = true;

						// If player has enough keys
						/*if (player.getSilverKeys() > 0) {

							// If player is pressing equip
							if (player.getEquipState()) {

								player.IncreaseSilverKeys(-1);

								// Chang Tile into unlocked door Tile
								//tile[i].promptSelf = false;
								//tile[i].id = 309;
								//tile[i].clip = rTiles[309];
								//tile[i].collisionTile = false;

								// play sound effect
								Mix_PlayChannel(-1, settings.sCastHitBoss, 0);
							}
						}*/
					} else {
						tile[i].promptSelf = false;
					}
				}

				// Silver chest
				if (tile[i].id == this->chestTile)
				{
					// If collision happened
					if (checkCollision(player.getX(), player.getY(), player.getW(), player.getH(),
									   tile[i].x, tile[i].y, tile[i].w, tile[i].h+10))
					{
						// Prompt self, this will render an "E" above the Item to equip it
						tile[i].promptSelf = true;

						// If player has enough keys
						//if (player.getSilverKeys() > 0) {

							// If player is pressing equip
							if (player.getEquipState()) {

								// Reduce silver keys
								//player.IncreaseSilverKeys(-1);

								// Chang Tile into an unlocked chest
								tile[i].promptSelf = false;
								tile[i].id = 10;
								tile[i].clip = rTiles[10];

								// Spawn random item out form chest
								//int randItem = rand() % 11 + 1;
								int randItem = 25;
								ite.SpawnAndThrowItem(item, tile[i].x+tile[i].w/2-ite.rSwords[randItem].w/2, tile[i].y+tile[i].h,
										rand() % 11 + 1,
										0.0, randDouble(4, 5));

								// play sound effect
								Mix_PlayChannel(-1, settings.sCastHitBoss, 0);
							}
						//}
					} else {
						tile[i].promptSelf = false;
					}
				}

				// Jar Tile
				if (tile[i].id == 1)
				{
					// If collision happened
					if (checkCollision(player.getX(), player.getY(), player.getW(), player.getH(),
									   tile[i].x-3, tile[i].y-3, tile[i].w+6, tile[i].h+6))
					{
						// If player is dashing
						if (player.getDashStatus()) {

							// Remove a jar from the vector
							jarsLeft.pop_back();

							// Remove Jar
							tile[i].alive = false;
							tl.tileCount--;

							// Play SFX
				            Mix_PlayChannel(1, settings.sPotBreak, 0);
				            Mix_PlayChannel(2, settings.sValidation, 0);
						}
					}
				}
			}
		}
	}

	// After checking for equipping state, every frame, stop equipping for Player
	player.stopEquipState();
}

void PlayGame::checkCollisionParticleMob()
{
	bool playHitSFX = false;

	for (int j = 0; j < part.max; j++)
	{
		if (particles[j].alive)
		{
			if (particles[j].hurtType == -1 || particles[j].hurtType == 0)
			{
				for (int i = 0; i < mb.max; i++)
				{
					if (mob[i].alive)
					{
						// Get center of attack-particle (spawned by the player attacking)
						float bmx = mob[i].x+mob[i].w/2;
						float bmy = mob[i].y+mob[i].h/2;

						// Get center of particles
						float bmx2 = particles[j].x+particles[j].w/2;
						float bmy2 = particles[j].y+particles[j].h/2;

						// Get distance
						float distance = sqrt((bmx - bmx2) * (bmx - bmx2)+
											  (bmy - bmy2) * (bmy - bmy2));

						// If distance is less than 50 pixels
						//if (distance < 50)
						//{
							// Get angle of mob relative to attack-particle
							float angle = atan2(bmy - bmy2,bmx - bmx2);
							angle = angle * (180 / 3.1416);
							if (angle < 0) {
								angle = 360 - (-angle);
							}

							// Handle radians, cos, sin
							float radians = (3.1415926536/180)*(angle);
							float Cos = floor(cos(radians)*10+0.5)/10;
							float Sin = floor(sin(radians)*10+0.5)/10;

							////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//--------------------------------------------------------------------------------------------------------------------------------//
							//----------------------------- Collision Detection based on player-attack hit-box and Mob hit-box -----------------------------//
							// If particle slash if within X distance then move the mob away
							// collision occurred
							if (distance < mob[i].w/2 + particles[j].w/2)
							{
								// Shake camera
								ShakeCamera();

								// Flash Mobes sprite
								mob[i].flash = true;

								// Remove particle
								particles[j].time = 0;
								particles[j].alive = false;
								part.count--;

								// Spawn blood VFX
								part.spawnBloodVFX(particles, mob[i].x, mob[i].y, mob[i].w, mob[i].h, {255,0,0});

								// New velocity going away from Bullet Particle
								float newvX = 0.25 * (bmx - bmx2) / distance;
								float newvY = 0.25 * (bmy - bmy2) / distance;

								// Move the mob in someway
								mob[i].vX += newvX;
								mob[i].vY += newvY;
								//mob[i].vX += player.getKnockBackPower()/2 * xDir;

								// Play hit sound effect
								playHitSFX = true;

				                // Subtract mob health
				                mob[i].health -= player.getCastDamage();

				                // Increase player score based on projectile that hit Boss
				                if (particles[j].hurtType == -1) {
					                player.IncreaseScore(10);
				                }
				                if (particles[j].hurtType == 0) {
					                player.IncreaseScore(5);
				                }

				                // Show damage text (it will print how much damage the player did to the mob)
				    			std::stringstream tempss;
				    			tempss << player.getCastDamage();
				    			tex.spawn(gRenderer, text, mob[i].x+mob[i].w/2, mob[i].y+mob[i].w/2-15, 0.0, -0.5, 150,
				    					tempss.str().c_str(), 1, {255, 255, 0, 255});
							}
							//----------------------------- Collision Detection based on player-attack hit-box and Mob hit-box -----------------------------//
							////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//--------------------------------------------------------------------------------------------------------------------------------//
						//}
					}
				}
			}
		}
	}

	// Play hit sound effect
	if (playHitSFX) {

		// Play hit sound effect
		Mix_PlayChannel(-1, settings.sParrySuccess, 0);
	}
}



void PlayGame::checkCollisionTileMob()
{
	for (int i = 0; i < mb.max; i++) {
		if (mob[i].alive) {
			bool MonsterIsColliding = false;

			// Update collision with Tiles
			// Player Velocity X Axis
			mob[i].x += mob[i].vX;

			// Move only in x-axis
			if (mob[i].targetDistanceX >= mob[i].targetDistanceY) {
				if (mob[i].alert) {
					// If Monster has vision of target
					//if (mob[i].hasVision) {
						//mob[i].x += mob[i].velX;
					//}
				}
				SDL_Rect rectA;
				rectA.x = mob[i].x;
				rectA.y = mob[i].y;
				rectA.w = mob[i].w;
				rectA.h = mob[i].h;
				bool moveBack;
				moveBack = false;
				for (int j = 0; j < tl.max; j++) {
					if (tile[j].alive){
						if (tile[j].collisionTile) {
							SDL_Rect rectB;
							rectB.x = tile[j].x;
							rectB.y = tile[j].y;
							rectB.w = tile[j].w;
							rectB.h = tile[j].h;
							if  ( checkCollisionRect( rectA, rectB )) {
								// Continue handling collision
								moveBack = true;

								// If Mob's target Y is lower than Tile's Y
								if (mob[i].bmy2 < tile[j].y+tile[j].h/2) {
									// If colliding with a Tile, move in Y direction towards Player
									mob[i].y -= 2;
									//mob[i].vY -= 5;
								} else {
									// If colliding with a Tile, move in Y direction towards Player
									mob[i].y += 2;
								}
							}
						}
					}
				}
				if (moveBack){
					mob[i].x -= mob[i].vX;
					//mob[i].x -= mob[i].velX;
					MonsterIsColliding = true;
				}
			}

			// Move only in y-axis
			else{
				mob[i].y += mob[i].vY;
				// Player Velocity Y Axis
				if (mob[i].alert) {
					// If Monster has vision of target
					//if (mob[i].hasVision) {
					//}
				}
				//mob[i].y += mob[i].velY;
				SDL_Rect rectA;
				rectA.x = mob[i].x;
				rectA.y = mob[i].y;
				rectA.w = mob[i].w;
				rectA.h = mob[i].h;
				bool moveBack;
				moveBack = false;
				for (int j = 0; j < tl.max; j++) {
					if (tile[j].alive){
						if (tile[j].collisionTile) {
							SDL_Rect rectB;
							rectB.x = tile[j].x;
							rectB.y = tile[j].y;
							rectB.w = tile[j].w;
							rectB.h = tile[j].h;
							if  ( checkCollisionRect( rectA, rectB )) {
								// Continue handling collision
								moveBack = true;

								// If Mob's target Y is lower than Tile's Y
								if (mob[i].bmx2 < tile[j].x+tile[j].w/2) {
									// If colliding with a Tile, move in Y direction towards Player
									mob[i].x -= 2;
									//mob[i].vY -= 5;
								} else {
									// If colliding with a Tile, move in Y direction towards Player
									mob[i].x += 2;
								}
							}
						}
					}
				}
				if (moveBack){
					mob[i].y -= mob[i].vY;
					//mob[i].y -= mob[i].velY;
					MonsterIsColliding = true;
				}
			}

			//-------------------------------------------------------------------------------------------//
			//-------------------------------------------------------------------------------------------//
			//-------------------------------------------------------------------------------------------//

			// This is used to determine if we should find the shortest route to the Player
			if (MonsterIsColliding) {
				mob[i].collision = true;
			}else{
				mob[i].collision = false;
			}

			// Decrease knockback velocity each frame
			mob[i].vX = mob[i].vX - mob[i].vX * 0.08;
			mob[i].vY = mob[i].vY - mob[i].vY * 0.08;
		}
	}
}

void PlayGame::checkPlayerAttacksCollisionMob() {

	bool playHurtMob = false;

	// Objects
	for (int j = 0; j < obj.max; j++)
	{
		if (object[j].alive)
		{
			if (object[j].id != 2)
			{
				// Mobs
				for (int i = 0; i < mb.max; i++)
				{
					if (mob[i].alive)
					{
						// Get center of attack-particle (spawned by the player attacking)
						float bmx = mob[i].x+mob[i].w/2;
						float bmy = mob[i].y+mob[i].h/2;

						// Get center of object
						float bmx2 = object[j].x+object[j].w/2;
						float bmy2 = object[j].y+object[j].h/2;

						// Get distance
						float distance = sqrt((bmx - bmx2) * (bmx - bmx2)+
											  (bmy - bmy2) * (bmy - bmy2));

						// If distance is less than 50 pixels
						if (distance < 384/2 +  50)
						{
							////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//--------------------------------------------------------------------------------------------------------------------------------//
							//----------------------------- Collision Detection based on player-attack hit-box and Mob hit-box -----------------------------//
							// Check collision between object and mob
							if (checkCollision(object[j].x, object[j].y, object[j].w, object[j].h, mob[i].x, mob[i].y, mob[i].w, mob[i].h))
							{
								// Knockback Mob
								{
									float distanceW = sqrt((bmx - bmx2) * (bmx - bmx2));
									float distanceH = sqrt((bmy - bmy2) * (bmy - bmy2));
									float tempVX 	= 0.8 * (bmx - bmx2) / distanceW;
									float tempVY 	= 0.8 * (bmy - bmy2) / distanceH;

									mob[i].vX += tempVX;
									mob[i].vY += tempVY;
								}

								// Shake camera
								ShakeCamera();

								// Broken swords have lifesteal? // TODO delete this lol
								if (player.getItemID() >= 19 && player.getItemID() <= 22) {
									player.IncreaseHealth(5);
								}

								// Spawn blood VFX
								part.spawnBloodVFX(particles, mob[i].x, mob[i].y, mob[i].w, mob[i].h, {204,57,123});

								// if difficulty: easy
								{
									mb.Stun(mob, i);
								}

								// Flash Bosses sprite
								mob[i].flash = true;

				                // Subtract mob health
				                mob[i].health -= player.getDamage();

				                // Increase player score
				                player.IncreaseScore(10);

								// Play hit sound effect: Slash attack
				                playHurtMob = true;

				                // Show damage text (it will print how much damage the player did to the mob)
				    			std::stringstream tempss;
				    			tempss << player.getDamage();
				    			tex.spawn(gRenderer, text, mob[i].x+mob[i].w/2, mob[i].y-15, 0.0, -0.5, 150,
				    					tempss.str().c_str(), 1, {255, 255, 0, 255});
							}
							//----------------------------- Collision Detection based on player-attack hit-box and Mob hit-box -----------------------------//
							////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//--------------------------------------------------------------------------------------------------------------------------------//
						}
					}
				}
			}
		}
	}

	// Play hit sound effect: Slash attack
	if (playHurtMob) {

		// play sound effect
		Mix_PlayChannel(-1, settings.sSlashHitBoss, 0);
	}
}

void PlayGame::checkMobPlayerVision() {
	for (int i=0; i<mb.max; i++) {
		if (mob[i].alive) {
			Point p1, q1;
			p1.x = mob[i].x2;
			p1.y = mob[i].y2;
			q1.x = player.getCenterX();
			q1.y = player.getCenterY();

			bool hasVision = true;

			// If distance from player is less than Mob sight distance
			for (int j=0; j<tl.max; j++) {
				if (tile[j].alive) {
					if (tile[j].collisionTile) {
						// a: top left
						// b: top right
						// c: bottom left
						// d: bottom right
						Point a, b, c, d;
						// top left
						a.x = tile[j].x;
						a.y = tile[j].y;
						// top right
						b.x = tile[j].x+tile[j].w;
						b.y = tile[j].y;
						// bottom left
						c.x = tile[j].x;
						c.y = tile[j].y+tile[j].h;
						// bottom right
						d.x = tile[j].x+tile[j].w;
						d.y = tile[j].y+tile[j].h;

						// top side line
						/*SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255);
						SDL_RenderDrawLine(gRenderer, a.x-camx, a.y-camy, b.x-camx, b.y-camy);

						// right side line
						SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
						SDL_RenderDrawLine(gRenderer, b.x-camx, b.y-camy, d.x-camx, d.y-camy);

						// bottom side line
						SDL_SetRenderDrawColor(gRenderer, 0, 0, 255, 255);
						SDL_RenderDrawLine(gRenderer, d.x-camx, d.y-camy, c.x-camx, c.y-camy);

						// left side line
						SDL_SetRenderDrawColor(gRenderer, 255, 0, 255, 255);
						SDL_RenderDrawLine(gRenderer, a.x-camx, a.y-camy, c.x-camx, c.y-camy);*/

						if (doIntersect(p1, q1, a, b)) {
							hasVision = false;
						}else if (doIntersect(p1, q1, b, d)) {
							hasVision = false;
						}else if (doIntersect(p1, q1, d, c)) {
							hasVision = false;
						}else if (doIntersect(p1, q1, c, a)) {
							hasVision = false;
						}
					}
				}
			}
			mob[i].hasVision = hasVision;
		}
	}
}

void PlayGame::checkBossTileCollision()
{


	for (int i = 0; i < bos.max; i++) {
		if (boss[i].alive) {
			bool MonsterIsColliding = false;

			// Update collision with Tiles
			// Player Velocity X Axis
			boss[i].x += boss[i].vX;

			// Move only in x-axis
			//if (boss[i].targetDistanceX >= boss[i].targetDistanceY) {
				if (boss[i].alert) {
					// If Monster has vision of target
					//if (boss[i].hasVision) {
						//boss[i].x += boss[i].velX;
					//}
				}
				SDL_Rect rectA;
				rectA.x = boss[i].x;
				rectA.y = boss[i].y;
				rectA.w = boss[i].w;
				rectA.h = boss[i].h;
				bool moveBack;
				moveBack = false;
				for (int j = 0; j < tl.max; j++) {
					if (tile[j].alive){
						if (tile[j].collisionTile) {
							SDL_Rect rectB;
							rectB.x = tile[j].x;
							rectB.y = tile[j].y;
							rectB.w = tile[j].w;
							rectB.h = tile[j].h;
							if  ( checkCollisionRect( rectA, rectB )) {
								// Continue handling collision
								moveBack = true;

								// If Boss's target Y is lower than Tile's Y
								if (boss[i].bmy2 < tile[j].y+tile[j].h/2) {
									// If colliding with a Tile, move in Y direction towards Player
									boss[i].y -= 2;
									//boss[i].vY -= 5;
								} else {
									// If colliding with a Tile, move in Y direction towards Player
									boss[i].y += 2;
								}
							}
						}
					}
				}
				if (moveBack){
					boss[i].x -= boss[i].vX;
					//boss[i].x -= boss[i].velX;
					MonsterIsColliding = true;
				}
			//}

			// Move only in y-axis
			else{
				boss[i].y += boss[i].vY;
				// Player Velocity Y Axis
				if (boss[i].alert) {
					// If Monster has vision of target
					//if (boss[i].hasVision) {
					//}
				}
				//boss[i].y += boss[i].velY;
				SDL_Rect rectA;
				rectA.x = boss[i].x;
				rectA.y = boss[i].y;
				rectA.w = boss[i].w;
				rectA.h = boss[i].h;
				bool moveBack;
				moveBack = false;
				for (int j = 0; j < tl.max; j++) {
					if (tile[j].alive){
						if (tile[j].collisionTile) {
							SDL_Rect rectB;
							rectB.x = tile[j].x;
							rectB.y = tile[j].y;
							rectB.w = tile[j].w;
							rectB.h = tile[j].h;
							if  ( checkCollisionRect( rectA, rectB )) {
								// Continue handling collision
								moveBack = true;

								// If Boss's target Y is lower than Tile's Y
								if (boss[i].bmx2 < tile[j].x+tile[j].w/2) {
									// If colliding with a Tile, move in Y direction towards Player
									boss[i].x -= 2;
									//boss[i].vY -= 5;
								} else {
									// If colliding with a Tile, move in Y direction towards Player
									boss[i].x += 2;
								}
							}
						}
					}
				}
				if (moveBack){
					boss[i].y -= boss[i].vY;
					//boss[i].y -= boss[i].velY;
					MonsterIsColliding = true;
				}
			}

			//-------------------------------------------------------------------------------------------//
			//-------------------------------------------------------------------------------------------//
			//-------------------------------------------------------------------------------------------//

			// This is used to determine if we should find the shortest route to the Player
			if (MonsterIsColliding) {
				boss[i].collision = true;
			}else{
				boss[i].collision = false;
			}

			// Decrease knockback velocity each frame
			boss[i].vX = boss[i].vX - boss[i].vX * 0.08;
			boss[i].vY = boss[i].vY - boss[i].vY * 0.08;
		}
	}






	// Boss and Tile Collision
	// X Axis Collision
/*
	// Boss
	for (int i = 0; i < bos.max; i++) {
		if (boss[i].alive) {
			// Tiles
			for (int j = 0; j < tl.max; j++) {
				if (tile[j].alive){
					if (tile[j].collisionTile && tile[j].id != 223)
					{
						// Get center of attack-particle (spawned by the player attacking)
						float bmx = boss[i].x+boss[i].w/2;
						float bmy = boss[i].y+boss[i].h/2;

						// Get center of particles
						float bmx2 = tile[j].x+particles[j].w/2;
						float bmy2 = tile[j].y+particles[j].h/2;

						// Get distance
						float distance = sqrt((bmx - bmx2) * (bmx - bmx2)+
											  (bmy - bmy2) * (bmy - bmy2));

						// If distance is less than 50 pixels
						//if (distance < 50)
						//{
							// Get angle of boss relative to attack-particle
							float angle = atan2(bmy - bmy2,bmx - bmx2);
							angle = angle * (180 / 3.1416);
							if (angle < 0) {
								angle = 360 - (-angle);
							}

							// Handle radians, cos, sin
							float radians = (3.1415926536/180)*(angle);
							float Cos = floor(cos(radians)*10+0.5)/10;
							float Sin = floor(sin(radians)*10+0.5)/10;

							////////////////////////////////////////////////////////////////////////
							//--------------------------------------------------------------------//
							//-------------------- Circle Collision Detection --------------------//
							// If particle slash if within X distance then move the boss away
							// collision occurred
							if (distance < boss[i].w/2 + particles[j].w/2)
							{
								// New velocity going away from Bullet Particle
								float newvX = 2 * (bmx - bmx2) / distance;
								float newvY = 2 * (bmy - bmy2) / distance;

								// Move the boss away from Tile
								boss[i].vX = boss[i].vX * -1;
								boss[i].vY = boss[i].vY * -1;
							}
					}
				}
			}
			//-------------------- Circle Collision Detection --------------------//
			//--------------------------------------------------------------------//
			////////////////////////////////////////////////////////////////////////

		}
	}*/
}

void PlayGame::checkPlayerAttacksCollisionBoss() {

	bool playHitSFX = false;

	// Objects
	for (int j = 0; j < obj.max; j++)
	{
		if (object[j].alive)
		{
			if (object[j].id != 2)
			{
				// Bosss
				for (int i = 0; i < bos.max; i++)
				{
					if (boss[i].alive)
					{
						// Get center of attack-particle (spawned by the player attacking)
						float bmx = boss[i].x+boss[i].w/2;
						float bmy = boss[i].y+boss[i].h/2;

						// Get center of object
						float bmx2 = object[j].x+object[j].w/2;
						float bmy2 = object[j].y+object[j].h/2;

						// Get distance
						float distance = sqrt((bmx - bmx2) * (bmx - bmx2)+
											  (bmy - bmy2) * (bmy - bmy2));

						// If distance is less than 50 pixels
						if (distance < 384/2 +  50)
						{
							////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//--------------------------------------------------------------------------------------------------------------------------------//
							//----------------------------- Collision Detection based on player-attack hit-box and Boss hit-box -----------------------------//
							// Check collision between object and boss
							if (checkCollision(object[j].x, object[j].y, object[j].w, object[j].h, boss[i].x, boss[i].y, boss[i].w, boss[i].h))
							{
								// Shake camera
								ShakeCamera();

								// Knockback Boss
								{
									float distanceW = sqrt((bmx - bmx2) * (bmx - bmx2));
									float distanceH = sqrt((bmy - bmy2) * (bmy - bmy2));
									float tempVX 	= 0.8 * (bmx - bmx2) / distanceW;
									float tempVY 	= 0.8 * (bmy - bmy2) / distanceH;

									boss[i].vX += tempVX;
									boss[i].vY += tempVY;
								}

				                // Subtract boss health
				                boss[i].health -= player.getDamage();

				                // Increase player score
				                player.IncreaseScore(10);

								// Play hit sound effect: Slash attack
				                playHitSFX = true;

				                // Show damage text (it will print how much damage the player did to the boss)
				    			std::stringstream tempss;
				    			tempss << player.getDamage();
				    			tex.spawn(gRenderer, text, boss[i].x+boss[i].w/2, boss[i].y-15, 0.0, -0.5, 150,
				    					tempss.str().c_str(), 1, {255, 255, 0, 255});
							}
							//----------------------------- Collision Detection based on player-attack hit-box and Boss hit-box -----------------------------//
							////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//--------------------------------------------------------------------------------------------------------------------------------//
						}
					}
				}
			}
		}
	}

	// Play hit sound effect: Slash attack
    if (playHitSFX) {
        Mix_PlayChannel(-1, settings.sSlashHitBoss, 0);
    }
}

void PlayGame::checkPlayerAttacksTileCollision() {

	bool playHitTile = false;
	int playPotSFX = false;

	for (int j = 0; j < obj.max; j++) {
		if (object[j].alive) {
			for (int i = 0; i < tl.max; i++) {
				if (tile[i].alive) {
					if (tile[i].collisionTile) {
						// Tile destruction if its a destructible
						if (tile[i].destructible)
						{
							if (tile[i].id == 1 || tile[i].id == 4) {
								if (!tile[i].startJarBreaking) {
									// Get ccenter of object's target
									float bmx = getCenter(tile[i].x, tile[i].w);
									float bmy = getCenter(tile[i].y, tile[i].h);

									// Get center of object
									float bmx2 = getCenter(object[j].x, object[j].w);
									float bmy2 = getCenter(object[j].y, object[j].h);

									// Distance of tile relative to attack-object
									float distance = getDistance(bmx, bmy, bmx2, bmy2);

									// If distance is less than 128 pixels
									if (distance < 128)
									{
										// Get angle of tile relative to attack-object
										float angle = atan2(bmy - bmy2,bmx - bmx2);
										angle = angle * (180 / 3.1416);
										if (angle < 0) {
											angle = 360 - (-angle);
										}

										// Handle radians, cos, sin
										float radians = (3.1415926536/180)*(angle);
										float Cos = floor(cos(radians)*10+0.5)/10;
										float Sin = floor(sin(radians)*10+0.5)/10;

										////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
										//--------------------------------------------------------------------------------------------------------------------------------//
										//----------------------------- Collision Detection based on player-attack hit-box and tile hit-box ------------------------------//
										// Check collision between object and tile
										if (checkCollision(object[j].x, object[j].y, object[j].w, object[j].h, tile[i].x, tile[i].y, tile[i].w, tile[i].h))
										{

											// Shake camera
											ShakeCamera();

											// Reduce Tile health
											tile[i].health -= player.getDamage();

											// Health reached 0
											if (tile[i].health <= 0)
											{
												// Spawn health or coin, or power up 1 or power up 2
												int randNum = rand() % 10;
												int tempID;

												if (randNum >= 0 && randNum <= 2) {
													tempID = 25;
													ite.SpawnAndThrowItem(item,
															tile[i].x+tile[i].w/2-ite.rSwords[tempID].w/2,
															tile[i].y+tile[i].h/2-ite.rSwords[tempID].h/2,
															tempID, 0.0, randDouble(1, 2));
												} else if (randNum >= 3 && randNum <= 5) {
													tempID = 28;
													ite.SpawnAndThrowItem(item,
															tile[i].x+tile[i].w/2-ite.rSwords[tempID].w/2,
															tile[i].y+tile[i].h/2-ite.rSwords[tempID].h/2,
															tempID, 0.0, randDouble(1, 2));
												} else if (randNum == 6) {
													tempID = 29;
													ite.SpawnAndThrowItem(item,
															tile[i].x+tile[i].w/2-ite.rSwords[tempID].w/2,
															tile[i].y+tile[i].h/2-ite.rSwords[tempID].h/2,
															tempID, 0.0, randDouble(1, 2));
												} else if (randNum == 7) {
													tempID = 30;
													ite.SpawnAndThrowItem(item,
															tile[i].x+tile[i].w/2-ite.rSwords[tempID].w/2,
															tile[i].y+tile[i].h/2-ite.rSwords[tempID].h/2,
															tempID, 0.0, randDouble(1, 2));
												}

												// If jar Tile
												if (tile[i].id == 1) {

													// Start breaking animation for Jar Tile
													tile[i].startJarBreaking = true;

													// Remove a jar from the vector
													jarsLeft.pop_back();
												}

												// If Barrel Tile
												else if (tile[i].id == 4) {

													// Remove Tile
													tile[i].alive = false;
													tl.tileCount--;
												}
											}

											// Particle effect
											//part.spawnTileHitVFX(particles, tile[i].x, tile[i].y, tile[i].w, tile[i].h);

											// Remove Object
											//object[j].alive = false;
											//obj.count--;

											// Play hit sound effect: Slash attack
											playHitTile = true;
											if (tile[i].id == 1) {
												playPotSFX = true;
									            Mix_PlayChannel(1, settings.sPotBreak, 0);
									            Mix_PlayChannel(2, settings.sValidation, 0);
											} else {
									            Mix_PlayChannel(3, settings.sValidation, 0);
											}
										}
										//----------------------------- Collision Detection based on player-attack hit-box and tile hit-box ------------------------------//
										//--------------------------------------------------------------------------------------------------------------------------------//
										////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
									}
								}
							}
						}
					}
				}
			}
		}
	}

	// Play it once so we dont play it X amount of timesin teh for loop of Tiles
	if (playHitTile) {

        // Play Pot SFX
        if (playPotSFX) {
            //Mix_PlayChannel(-1, settings.sPotBreak, 0);
            //Mix_PlayChannel(-1, settings.sValidation, 0);
        }

        // Play default SFX
        else {
           // Mix_PlayChannel(-1, settings.sSlashTilec, 0);
        }
	}

}

void PlayGame::checkPlayerAttacksBossParticleCollision()
{
	bool playHitSFX = false;

	for (int j = 0; j < obj.max; j++)
	{
		if (object[j].alive)
		{
			for (int i = 0; i < part.max; i++)
			{
				if (particles[i].alive)
				{
					if (particles[i].hurtType == 1)
					{
						// Get center of object's target
						float bmx = particles[i].x+particles[i].w/2;
						float bmy = particles[i].y+particles[i].h/2;

						// Get center of object
						float bmx2 = object[j].x+object[j].w/2;
						float bmy2 = object[j].y+object[j].h/2;

						// Distance of particles relative to attack-object
						float distance = sqrt((bmx - bmx2) * (bmx - bmx2)+
											  (bmy - bmy2) * (bmy - bmy2));

						// If it goes less than 0, the game crashes
						if (distance <= 1) {
							distance = 1;
						}

						// If distance is less than 50 pixels
						if (distance < 600)
						{
							// Get angle of particles relative to attack-object
							float angle = atan2(bmy - bmy2,bmx - bmx2);
							angle = angle * (180 / 3.1416);
							if (angle < 0) {
								angle = 360 - (-angle);
							}

							// Handle radians, cos, sin
							float radians = (3.1415926536/180)*(angle);
							float Cos = floor(cos(radians)*10+0.5)/10;
							float Sin = floor(sin(radians)*10+0.5)/10;

							////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//--------------------------------------------------------------------------------------------------------------------------------//
							//----------------------------- Collision Detection based on player-attack hit-box and particles hit-box ------------------------------//
							// Check collision between object and particles
							//if (checkCollision(object[j].x, object[j].y, object[j].w, object[j].h, particles[i].x, particles[i].y, particles[i].w, particles[i].h))
							//{


							// Circle Collision
							if (distance < object[j].h/2 + particles[i].w/2)
							{
								// Shake camera
								ShakeCamera();

								// Deflect projectile if we have fists on
								//if (player.getItemEqipped(0)) {

									/*// Change Enemy bullet into a Player bullet!
									particles[i].hurtType = -1;

									// Knock back enemy bullet particles
									{
										float distanceW = sqrt((bmx - bmx2) * (bmx - bmx2));
										float distanceH = sqrt((bmy - bmy2) * (bmy - bmy2));
										//float tempVX 	= 5 * (bmx - bmx2) / distanceW;
										//float tempVY 	= 5 * (bmy - bmy2) / distanceH;

										// Apply player's cast damage to the projectile we're deflecting back
										particles[i].dmgToParticles = player.getCastDamage();

										// Reverse particles velocity vX and vY
										particles[i].vX += particles[i].vX * -4;
										particles[i].vY += particles[i].vY * -4;
									}*/
								//}
								// Destroy projectile
								//else {

									// Reduce health of Enemey Particle
									particles[i].health -= player.getDamage();

									// Particle effect
									part.spawnBloodVFX(particles,
											getCenter(particles[i].x, particles[i].w),
											getCenter(particles[i].y, particles[i].h),
											particles[i].w, particles[i].h,
											{0, 200, 200});

									// Remove Object
									//object[j].alive = false;
									//obj.count--;
								//}

								// Play hit sound effect: Slash attack
								playHitSFX = true;
							}
							//----------------------------- Collision Detection based on player-attack hit-box and particles hit-box ------------------------------//
							//--------------------------------------------------------------------------------------------------------------------------------//
							////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
						}
					}
				}
			}
		}
	}

	// Play hit sound effect: Slash attack
	if (playHitSFX) {
	    Mix_PlayChannel(-1, settings.sSlashHitBoss, 0);
	}
}


void PlayGame::checkBossAttacksCollisionPlayer() {
	for (int j = 0; j < obj.max; j++)
	{
		if (object[j].alive)
		{
			if (object[j].id == 2)
			{
				// Get ccenter of object's target
				float bmx = player.getCenterX();
				float bmy = player.getCenterY();

				// Get center of object
				float bmx2 = object[j].x+object[j].w/2;
				float bmy2 = object[j].y+object[j].h/2;

				// Distance of tile relative to attack-object
				float distance = sqrt((bmx - bmx2) * (bmx - bmx2)+
									  (bmy - bmy2) * (bmy - bmy2));

				// If it goes less than 0, the game crashes
				if (distance <= 1) {
					distance = 1;
				}

				// If distance is less than 50 pixels
				if (distance < 50)
				{
					// If Player is currently invurnerable state on
					if (player.getInvurnerableStatus())
					{
						// Remove object
						object[j].alive = false;
						obj.count--;

						// Spawn blood VFX
						part.spawnParryVFX(particles, player.x, player.y, player.w, player.h);

						// Reference to exact boss (this was stored in the object when it was created
						int i = object[j].CreatorIndex;

						// If that boss is still alive
						if (boss[i].alive)
						{
							// If particle is coming from the right of Boss
							int xDir;

							// Determine which side the player is from the Boss
							if (player.getCenterX() > boss[i].x+boss[i].w/2) xDir = -1; else xDir = 1;

							// Knockback boss
							boss[i].vX += player.getKnockBackPower() * xDir;
							boss[i].vY = -4;

							// play sound effect
							Mix_PlayChannel(-1, settings.sParrySuccess, 0);
						}
					}

					// If player is currently parrying
					else if (player.getParryStatus())
					{
						// Remove object
						object[j].alive = false;
						obj.count--;

						// Immediately stop parrying, and spawn a counter attack
						{
							// Stop parry, and reset its cool down
							player.ResetParry();

							// Activate player shield
							player.ActivateInvurnerability();

							// Launch attack
							player.SlashAttack();
						}

						// Spawn blood VFX
						part.spawnBloodVFX(particles, player.x, player.y, player.w, player.h, {255,0,0});

						// play sound effect
						Mix_PlayChannel(-1, settings.sParrySuccess, 0);
					}

					////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					//--------------------------------------------------------------------------------------------------------------------------------//
					//----------------------------- Collision Detection based on Boss-attack hit-box and Player hit-box -----------------------------//
					// Check collision between object and boss
					else if (checkCollision(object[j].x, object[j].y, object[j].w, object[j].h, player.x, player.y, player.w, player.h))
					{
						// If player is on right side of object
						int xDir;
						if (player.getCenterX() > object[j].x+object[j].w/2) {
							xDir = 1;
						}
						// If player is on left side of object
						else {
							xDir = -1;
						}

						// Knockback player
						//player.stunned = true;
						player.setVx(player.getKnockBackPower() * xDir);
						player.setVy(-4);

						// Subtract player health
						player.IncreaseHealth(-20);

						// Spawn blood VFX
						part.spawnBloodVFX(particles, player.x, player.y, player.w, player.h, {255,0,0});

						// play sound effect
						Mix_PlayChannel(-1, settings.sSlashHitBoss, 0);

						// Show damage text (it will print how much damage the player did to the boss)
						std::stringstream tempss;
						tempss << 20;
						tex.spawn(gRenderer, text, player.x+player.w/2, player.y-15, 0.0, -0.5, 150, tempss.str().c_str(), 1,
								{255, 255, 0, 255});
					}
					//----------------------------- Collision Detection based on Boss-attack hit-box and Player hit-box -----------------------------//
					////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					//--------------------------------------------------------------------------------------------------------------------------------//

					// Shake camera
					ShakeCamera();
				}
			}
		}
	}
}

void PlayGame::checkCollisionBossPlayer() {

	// boss circle collision check with other bosss
	for (int i = 0; i < bos.max; i++) {
		if (boss[i].alive) {

			// Center of Boss target for collision
			float bmx = player.x+player.w/2;
			float bmy = player.y+player.h/2;

			// Center of Boss
			float bmx2 = boss[i].x+boss[i].w/2;
			float bmy2 = boss[i].y+boss[i].h/2;

			// Angel stuff
			float angle = atan2(bmy - bmy2,bmx - bmx2);
			angle = angle * (180 / 3.1416);
			if (angle < 0) {
				angle = 360 - (-angle);
			}

			// Radiands stuff
			float radians = (3.1415926536/180)*(angle);
			float Cos = floor(cos(radians)*10+0.5)/10;
			float Sin = floor(sin(radians)*10+0.5)/10;

			// Distance
			float distance = sqrt((bmx - bmx2) * (bmx - bmx2)+
								  (bmy - bmy2) * (bmy - bmy2));


			float distanceW = sqrt((bmx - bmx2) * (bmx - bmx2));
			float distanceH = sqrt((bmy - bmy2) * (bmy - bmy2));

			// Do this or you will crash
			if (distance <= 0.01) {
				distance = 0.01;
			}
			if (distanceW <= 0.01) {
				distanceW = 0.01;
			}
			if (distanceH <= 0.01) {
				distanceH = 0.01;
			}

			//float tempVX = (cos( (3.14159265/180)*(angle) ));;
			//float tempVY = (sin( (3.14159265/180)*(angle) ));
			//float tempVel = 5 * (bmx - bmx2) / distance;

			////////////////////////////////////////////////////////////////////////
			//--------------------------------------------------------------------//
			//-------------------- Circle Collision Detection --------------------//
			{
				// Get new direction for Player to go
				/*float tempVX 	= player.velMax * (bmx - bmx2) / distanceW;
				float tempVY 	= player.velMax * (bmy - bmy2) / distanceH;

				// If distance between both is lower than X, then have them go away from each other
				if (distance < boss[i].w/4 + player.w/2)
				{
					// Apply new direction to Player's x & y velocity
					player.vX = tempVX;
					//player.x += tempVX;

					player.vY = tempVY;
					//player.y += tempVY;
				}*/
			}
			//-------------------- Circle Collision Detection --------------------//
			//--------------------------------------------------------------------//
			////////////////////////////////////////////////////////////////////////
		}
	}
}

void PlayGame::checkCollisionParticlePlayer() {
	// Player
	if (player.alive && !player.getDashStatus()){
		// Particle
		for (int i = 0; i < part.max; i++)
		{
			if (particles[i].alive)
			{
				// enemy particle hitting Player
				if (particles[i].hurtType == 1)
				{
					// particle collision with player using a CIRCLE
					float bmx = player.x + player.w/2;
					float bmy = player.y + player.h/2;
					float bmx2 = particles[i].x + particles[i].w/2;
					float bmy2 = particles[i].y + particles[i].h/2;
					float distance = sqrt((bmx - bmx2) * (bmx - bmx2)+
										  (bmy - bmy2) * (bmy - bmy2));
					// collision occurred
					if (distance < player.getW()/2 + particles[i].w/2)
					{
						///////////////////////////////////////////////////////////////////
						//---------------------------------------------------------------//
						//------------- Invulnerability is up Successful ----------------//

						if (player.getInvurnerableStatus()) {

							// remove particle
							particles[i].alive = false;
							part.count--;

							// Spawn Parry VFX
							part.spawnParryVFX(particles, player.x, player.y, player.w, player.h);

			                // Show damage text (it will print how much damage the player did to the boss)
			    			std::stringstream tempss;
			    			tempss << "0";
			    			tex.spawn(gRenderer, text, player.x+player.w/2, player.y-24, 0.0, -0.5, 150,
			    					tempss.str().c_str(), 1, {144, 144, 144, 255});

							// play sound effect
							Mix_PlayChannel(-1, settings.sParrySuccess, 0);
						}
						//------------- Invulnerability is up Successful ----------------//
						//---------------------------------------------------------------//
						///////////////////////////////////////////////////////////////////




						///////////////////////////////////////////////////////////////////
						//---------------------------------------------------------------//
						//------------------------ Parry Successful ---------------------//

						// If player is currently parrying
						else if (player.getParryStatus())
						{
							// Spawn Counter Attack: Slash Attack Wave
							{
								// Change Enemy bullet into a Player bullet!
								particles[i].hurtType = -1;

								// Knock back enemy bullet particles
								{
									float distanceW = sqrt((bmx - bmx2) * (bmx - bmx2));
									float distanceH = sqrt((bmy - bmy2) * (bmy - bmy2));
									//float tempVX 	= 5 * (bmx - bmx2) / distanceW;
									//float tempVY 	= 5 * (bmy - bmy2) / distanceH;

									// Apply player's cast damage to the projectile we're deflecting back
									particles[i].dmgToParticles = player.getCastDamage();

									// Reverse particles velocity vX and vY
									particles[i].vX += particles[i].vX * -2;
									particles[i].vY += particles[i].vY * -2;
								}
							}

							// Extend Parry duration
							player.ExtendParryDuration();

							// Shorten Parry cool down
							player.ShortenParryCD(10.0f);

							// Increase score depending on size of bullet parried
							{
								// Small blue bullet from Boss
								if (particles[i].w == 20)
								{
									// Increase health
									player.IncreaseHealth(1);

					                // Increase player score
					                player.IncreaseScore(1);
								}

								// Small red bullet from Boss
								if (particles[i].w == 32)
								{
									// Increase health
									player.IncreaseHealth(2);

					                // Increase player score
					                player.IncreaseScore(4);
								}

								// Large red bullet from Boss
								if (particles[i].w == 96)
								{
									// Increase health
									player.IncreaseHealth(25);

					                // Increase player score
					                player.IncreaseScore(4);
								}
							}

							// Spawn Parry VFX
							part.spawnParryVFX(particles, player.x, player.y, player.w, player.h);

							// play sound effect
							Mix_PlayChannel(-1, settings.sParrySuccess, 0);
						}
						//------------------------ Parry Successful ---------------------//
						//---------------------------------------------------------------//
						///////////////////////////////////////////////////////////////////

						///////////////////////////////////////////////////////////////////
						//---------------------------------------------------------------//
						//---------------- NOT Parrying, so hurt the Player -------------//
						else {
							// Knock back player
							{
								float distanceW = sqrt((bmx - bmx2) * (bmx - bmx2));
								float distanceH = sqrt((bmy - bmy2) * (bmy - bmy2));
								float tempVX 	= 5 * (bmx - bmx2) / distanceW;
								float tempVY 	= 5 * (bmy - bmy2) / distanceH;

								player.setVx(tempVX);
								player.setVy(tempVY);
							}

							// Spawn blood VFX
							part.spawnBloodVFX(particles, player.x, player.y, player.w, player.h, {255,0,0});

							// play sound effect
							Mix_PlayChannel(-1, settings.sSlashHitBoss, 0);

							// Subtract player health
							player.IncreaseHealth(-particles[i].damage);

			                // Show damage text (it will print how much damage the player did to the boss)
			    			std::stringstream tempss;
			    			tempss << particles[i].damage;
			    			tex.spawn(gRenderer, text, player.x+player.w/2, player.y-15, 0.0, -0.5, 150,
			    					tempss.str().c_str(), 1, {255, 20, 20, 255});

							// remove particle
							particles[i].alive = false;
							part.count--;

						}

						// Shake camera
						ShakeCamera();
						//---------------- NOT Parrying, so hurt the Player -------------//
						//---------------------------------------------------------------//
						///////////////////////////////////////////////////////////////////
					}
				}
			}
		}	// end Particle
	}	// end Player
}


void PlayGame::checkCollisionParticleParticle() {
	// Player particle
	for (int i = 0; i < part.max; i++)
	{
		if (particles[i].alive)
		{
			if (particles[i].hurtType == 0)
			{
				for (int j = 0; j < part.max; j++)
				{
					// Make sure we are not comparing the same particle
					if (i != j)
					{
						if (particles[j].alive)
						{
							if (particles[j].hurtType == 1)
							{
								// Particle target center
								float bmx = particles[j].x + particles[j].w/2;
								float bmy = particles[j].y + particles[j].h/2;

								// Particle center
								float bmx2 = particles[i].x + particles[i].w/2;
								float bmy2 = particles[i].y + particles[i].h/2;
								float distance = sqrt((bmx - bmx2) * (bmx - bmx2)+
													  (bmy - bmy2) * (bmy - bmy2));

								// Circle Collision
								if (distance < particles[j].w/2 + particles[i].w/2)
								{
									// Reduce health of Enemey Particle
									particles[j].health -= particles[i].dmgToParticles;

									// If its a large Player particle, just reduce its health
									if (particles[i].w == 50) {

										// Reduce health of Player Particle
										particles[i].health -= particles[j].dmgToParticles;
									}
									// If regular sized particle from Player, just remove it
									else {

										// Remove particle
										particles[i].time = 0;
										particles[i].alive = false;
										part.count--;
									}

									// Spawn blood VFX
									part.spawnBloodVFX(particles, particles[j].x, particles[j].y, particles[j].w, particles[j].h, {0, 240, 240});

									// play sound effect
									Mix_PlayChannel(-1, settings.sParrySuccess, 0);
								}
							}
						}
					}
				}
			}
		}
	}
}

void PlayGame::checkBossOrMobDied() {

	for (int i = 0; i < bos.max; i++)
	{
		if (boss[i].alive)
		{
			// If boss health goes lower than 0, remove boss
			if (boss[i].health <= 0) {
				boss[i].alive = false;
				bos.count--;

				// Update levelsCompleted
		    	//SaveLevelsCompleted();
			}
		}
	}

	for (int i = 0; i < mb.max; i++)
	{
		if (mob[i].alive)
		{
			// If boss health goes lower than 0, remove boss
			if (mob[i].health <= 0) {

				// Spawn health or coin, or power up 1 or power up 2
				int randNum = rand() % 10;
				int tempID;

				if (randNum >= 0 && randNum <= 2) {
					tempID = 25;
					ite.SpawnAndThrowItem(item,
							mob[i].x+mob[i].w/2-ite.rSwords[tempID].w/2,
							mob[i].y+mob[i].h/2-ite.rSwords[tempID].h/2,
							tempID, 0.0, randDouble(1, 2));
				} else if (randNum >= 3 && randNum <= 5) {
					tempID = 28;
					ite.SpawnAndThrowItem(item,
							mob[i].x+mob[i].w/2-ite.rSwords[tempID].w/2,
							mob[i].y+mob[i].h/2-ite.rSwords[tempID].h/2,
							tempID, 0.0, randDouble(1, 2));
				} else if (randNum == 6) {
					tempID = 29;
					ite.SpawnAndThrowItem(item,
							mob[i].x+mob[i].w/2-ite.rSwords[tempID].w/2,
							mob[i].y+mob[i].h/2-ite.rSwords[tempID].h/2,
							tempID, 0.0, randDouble(1, 2));
				} else if (randNum == 7) {
					tempID = 30;
					ite.SpawnAndThrowItem(item,
							mob[i].x+mob[i].w/2-ite.rSwords[tempID].w/2,
							mob[i].y+mob[i].h/2-ite.rSwords[tempID].h/2,
							tempID, 0.0, randDouble(1, 2));
				}

				// Remove mob
				mob[i].alive = false;
				mb.count--;

				// Update levelsCompleted
		    	//SaveLevelsCompleted();
			}
		}
	}
}

void PlayGame::RenderTileBreakingBehind() {

	// Render Tile break
	{
		// Bottom of Player
		float bmy = player.getBottomSide();

		for (int i = 0; i < tl.max; i++) {
			if (tile[i].alive) {

				// Pot
				if (tile[i].id == 1) {
					if (tile[i].destructible) {

						// if player's y position is lower than Tile's y position
						if (bmy > tile[i].y + tile[i].h) {

							int sprite_index = 9;

							if (tile[i].health == 20) {
								sprite_index = -1;
							} else if (tile[i].health >= 0 && tile[i].health <= 19) {
								sprite_index = 1;
							}

							if (sprite_index != -1) {
								//gTileBreak.setAlpha(255);
								//gTileBreak.render(gRenderer, tile[i].x - camx, tile[i].y - camy, tile[i].w, tile[i].h, &rTileBreak[sprite_index]);
							}
						}
					}
				}

				// Barrel
				/*if (tile[i].id == 4) {
					// If destructible, render destructible Texture on Tiles
					{
						if (tile[i].destructible) {

							// if player's y position is lower than Tile's y position
							if (bmy > tile[i].y + tile[i].h) {

								int sprite_index = 9;

								if (tile[i].health == 20) {
									sprite_index = -1;
								} else if (tile[i].health >= 0 && tile[i].health <= 19) {
									sprite_index = 1;
								}

								if (sprite_index != -1) {
									gTileBreak.setAlpha(255);
									gTileBreak.render(gRenderer, tile[i].x - camx, tile[i].y - camy, tile[i].w, tile[i].h, &rTileBreak[sprite_index]);
								}
							}
						}
					}
				}*/
			}
		}
	}
}

void PlayGame::RenderTileBreakingFront() {

	// Render Tile break
	{
		// Bottom of Player
		float bmy = player.getBottomSide();

		for (int i = 0; i < tl.max; i++) {
			if (tile[i].alive) {

				// Pot
				if (tile[i].id == 1) {
					if (tile[i].destructible) {

						// if player's y position is lower than Tile's y position
						if (bmy < tile[i].y + tile[i].h) {

							int sprite_index = 9;

							if (tile[i].health == 20) {
								sprite_index = -1;
							} else if (tile[i].health >= 0 && tile[i].health <= 19) {
								sprite_index = 1;
							}

							if (sprite_index != -1) {
								//gTileBreak.setAlpha(255);
								//gTileBreak.render(gRenderer, tile[i].x - camx, tile[i].y - camy, tile[i].w, tile[i].h, &rTileBreak[sprite_index]);
							}
						}
					}
				}

				// Barrel
				/*if (tile[i].id == 4) {
					if (tile[i].destructible) {

						// if player's y position is lower than Tile's y position
						if (bmy < tile[i].y + tile[i].h) {

							int sprite_index = 9;

							if (tile[i].health == 20) {
								sprite_index = -1;
							} else if (tile[i].health >= 0 && tile[i].health <= 19) {
								sprite_index = 1;
							}

							if (sprite_index != -1) {
								gTileBreak.setAlpha(255);
								gTileBreak.render(gRenderer, tile[i].x - camx, tile[i].y - camy, tile[i].w, tile[i].h, &rTileBreak[sprite_index]);
							}
						}
					}
				}*/
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------ Save Functions ------------------------------------------------------//

// If boss died, update levelsUnlocked.mp
void PlayGame::SaveLevelsCompleted() {
	// Open highscore first to check value
	unsigned int PreviousLevelsUnlocked = -1;

	// Open file
	{
		// Set file path
		std::stringstream filePath;
		filePath << "data/LevelsUnlocked.mp";

		// Open file
		std::fstream file(filePath.str().c_str());

		// Part #1
		// We run this code first to see if there is a file that exist
		// If there is no file that exist then we create one, with the default of whatever level
		// we are currently on
		{
			if (!file.is_open()) {
				//std::cout<< "File does not exist for Levels Unlocked, creating new file.\n";
				{
					// Set file path
					std::stringstream filePath;
					filePath << "data/LevelsUnlocked.mp";

					// Create file
					std::ofstream fileSave;

					// Open newly made file
					fileSave.open(filePath.str().c_str());

					// Save to file
					fileSave << LevelToLoad;

					// Close file
					fileSave.close();
				}
			}
			// Close file
			file.close();
		}

		// Part #2
		// Next we attempt to open the newly created file OR
		// We attempt to load a previously saved file.
		{
			// Open file again
			std::fstream fileAgain(filePath.str().c_str());

			// If file opened
			if (fileAgain.is_open()) {
				// Store previous levels completed in this variable for checking
				fileAgain >> PreviousLevelsUnlocked;

				// Check if current level is the same previous completed, if so go to next level
				if (LevelToLoad == PreviousLevelsUnlocked) {

					std::stringstream filePath;
					filePath << "data/LevelsUnlocked.mp";

					// Save current level as number of levels completed
					std::ofstream fileSave;
					fileSave.open(filePath.str().c_str());
					fileSave << LevelToLoad+1;
					fileSave.close();
				}

				// If level completed is greater than previous levels completed, do nothing
				else if (LevelToLoad > PreviousLevelsUnlocked) {
					// do nothing
				}
			}
			// Close file
			fileAgain.close();
		}
	}
}

std::string PlayGame::saveSpawnPoint(){
	/*std::ofstream fileSpawnSave;
	fileSpawnSave.open( "data/maps/spawn.mp" );
		std::stringstream tempss;
		tempss << spawnX << " " << spawnY;
		fileSpawnSave << tempss.str().c_str();

		fileSpawnSave.close();*/

	std::stringstream tempss;
	tempss << spawnX << " " << spawnY;
	return tempss.str().c_str();
}

std::string PlayGame::saveMapSize(){

	std::stringstream tempss;
	tempss << map.getWidth() << " " << map.getHeight();
	return tempss.str().c_str();
}

void PlayGame::loadSpawnPoint(){

	// Set file path and name
	std::stringstream tempss;
	tempss << "data/maps/level";
	tempss << this->LevelToLoad;
	tempss << "/spawn.mp";

	// open config file
	std::fstream fileSpawn(tempss.str().c_str());
	if (fileSpawn.is_open()) {
		fileSpawn >>  spawnX >> spawnY;
		player.x		= spawnX;
		player.y		= spawnY;
	}
	// Error opening spawn.mp file
	else{
		// Set file path and name
		std::stringstream tempss;
		tempss << "data/maps/level";
		tempss << this->LevelToLoad;
		tempss << "/spawn.mp";

		// Creating default spawn.mp
		std::ofstream newSpawnFile(tempss.str().c_str());
		// If successfully created spawn.mp file
		if (newSpawnFile.is_open()) {
			newSpawnFile << "0 0";
		}
		// Opening and creating default config file failed, set manually
		else{
			spawnX			= 0;
			spawnY			= 0;
			player.x		= spawnX;
			player.y		= spawnY;
		}
		newSpawnFile.close();
	}
	fileSpawn.close();
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------- Controls ---------------------------------------------------------//

// Mouse Pressed
PlayGame::Result PlayGame::mousePressed(SDL_Event event){
	if (event.type == SDL_MOUSEBUTTONDOWN) {
		if (event.button.button == SDL_BUTTON_LEFT) {
			leftClick = true;
		}
		if (event.button.button == SDL_BUTTON_RIGHT) {
			rightClick = true;
			//zom.spawn(zombie, mx+camx-80/2, my+camy-80/2, 80, 80, 0.0, randDouble(3.6, 4.4), 0);
			//spaw.spawn(spawner, newMx+camx-48/2, newMy+camy-48/2, 48, 48);


			// If we are on editor mode
			if (editor) {
				// If not on Tile-bar, place other tiles
				if (!tb.touching) {

					// If bosss is our placement selection
					if (place_type == 2) {

						// Spawn bosss
						bos.Spawn(boss, mex+camx, mey+camy, 256, 256, 0.0, randDouble(3.6, 4.4), 0);
					}

					// If bosss is our placement selection
					if (place_type == 4) {

						// Spawn Mob
						mb.Spawn(mob, mex+camx, mey+camy, 14*2, 12*2, 0.0, randDouble(1.6, 1.4));
					}
				}
			}
		}
	}
	if (event.type == SDL_MOUSEBUTTONUP) {
		if (event.button.button == SDL_BUTTON_LEFT) {
			leftClick = false;
		}
		if (event.button.button == SDL_BUTTON_RIGHT) {
			rightClick = false;
		}
	}
	return Nothing;
}

// Mouse Released
PlayGame::Result PlayGame::mouseReleased(SDL_Event event){
	PlayGame::Result result = Nothing;
	if (event.type == SDL_MOUSEBUTTONUP) {
		if (event.button.button == SDL_BUTTON_LEFT) {
			leftClick = false;

			if (promptConfirm) {
				// Yes input
				if (mex < screenWidth/2) {
					promptConfirm = false;

					// Load room
					if (promptType == 0) {
						// This will hold all our data
						LoadLevel();

						// Print message to screen
						tex.spawn(gRenderer, text, 0, 0, 0.0, 0.0, 255, "Loading Data...", 0);

						// Set player to spawn
						//player.x = spawnX;
						//player.y = spawnY;

					}

					// Save room
					else {

						// This will hold all our data
						std::stringstream aVeryLongString;

						{
							// Save Tile data
							aVeryLongString << tl.SaveData(tile);

							// Save Tilec data
							aVeryLongString << tlc.SaveData(tilec);

							// Save Boss data
							aVeryLongString << bos.SaveData(boss);

							// Save Item data
							aVeryLongString << ite.SaveData(item);

							// Save Mob data
							aVeryLongString << mb.SaveData(mob);

							// Save Player spawn point
							aVeryLongString << saveSpawnPoint() << std::endl;

							// Save Size of Map.cpp
							aVeryLongString << saveMapSize();

							std::ofstream fileParrys2;
							std::stringstream tempNameParryNum2;
							//tempNameParryNum << "data/maps/" << tempLevelSaveName.str().c_str();
							//tempNameParryNum << saveFileName.str().c_str() << "TileNum.mp";
							tempNameParryNum2 << "data/maps/";
							tempNameParryNum2 << "Level";
							tempNameParryNum2 << this->LevelToLoad;
							tempNameParryNum2 << ".mp";

							// Open file
							fileParrys2.open(tempNameParryNum2.str().c_str());

							// Save all data
							fileParrys2 << aVeryLongString.str().c_str();

							// Close file
							fileParrys2.close();
						}

						// Print message to screen
						tex.spawn(gRenderer, text, 0, 0, 0.0, 0.0, 255, "Saving data...", 0);
					}
				}

				// No input
				else {
					promptConfirm = false;
				}
			}
		}
		if (event.button.button == SDL_BUTTON_RIGHT) {
			rightClick = false;
		}
	}
	return result;
}


// Editor controls
void PlayGame::editorOnKeyDown( SDL_Keycode sym )
{
	switch (sym) {
	case SDLK_w:
		if (shift) {
			tb.moveBarSelection(tilebar, "up");
		}else{
			camUp = true;
		}
		break;
	case SDLK_s:
		if (shift) {
			tb.moveBarSelection(tilebar, "down");
		}else{
			camDown = true;
		}
		break;
	case SDLK_a:
		if (shift) {
			tb.moveBarSelection(tilebar, "left");
		}else{
			camLeft = true;
		}
		break;
	case SDLK_d:
		if (shift) {
			tb.moveBarSelection(tilebar, "right");
		}else{
			camRight = true;
		}
		break;
	case SDLK_UP:				// camera up
		if (shift) {
			tl.MoveTiles(tile, "up");
			tlc.MoveTilecs(tilec, "up");
			mb.Move(mob, "up");
			spawnY -= 64;
		} else {
			yOffetDebug -= 64;
		}
		break;
	case SDLK_DOWN:				// camera down
		if (shift) {
			tl.MoveTiles(tile, "down");
			tlc.MoveTilecs(tilec, "down");
			mb.Move(mob, "down");
			spawnY += 64;
		} else {
			yOffetDebug += 64;
		}
		break;
	case SDLK_LEFT:				// camera left
		if (shift) {
			tl.MoveTiles(tile, "left");
			tlc.MoveTilecs(tilec, "left");
			mb.Move(mob, "left");
			spawnX -= 64;
		} else {
			xOffetDebug -= 64;
		}
		break;
	case SDLK_RIGHT:			// camera right
		if (shift) {
			tl.MoveTiles(tile, "right");
			tlc.MoveTilecs(tilec, "right");
			mb.Move(mob, "right");
			spawnX += 64;
		} else {
			xOffetDebug += 64;
		}
		break;
	case SDLK_y:				// camera lock
		camlock = (!camlock);
		break;
	case SDLK_x:				// Save spawn point
		spawnX = newMx+camx;
		spawnY = newMy+camy;
		if (shift) {
			//loadSpawnPoint();
		}else{
			//saveSpawnPoint();
		}
		break;
	case SDLK_q:								// Change place type (i.e. Tiles or Collision Tiles)
		place_type++;
		break;
	case SDLK_TAB:								// Toggle hide other layers
		tl.hideOtherLayers = (!tl.hideOtherLayers);
		break;
	case SDLK_k:{								// Change collisionTile, if we should place a Tile with Collision

			tl.collisionTile = (!tl.collisionTile);
			break;
		}
	case SDLK_l:{								// Change layer, if shoudl place a Tile at this certain layer
			if (shift){
				if (place_type==0) {
					tl.layer--;
				}else if (place_type==1) {
					tlc.newPos--;
				}
			}else{
				if (place_type==0) {
					tl.layer++;
				}else if (place_type==1) {
					tlc.newPos++;
				}
			}
			break;
		}
	case SDLK_i:								// Change tile id to place
		// TODO [ ] make change types for Mobs
		if (shift) {
			// Tiles
			if (place_type==0) {
				tl.id--;
			}

			// Tilec
			else if (place_type==1) {
				tlc.LevelToLoad--;
			}

			// Items
			else if (place_type==3) {
				ite.id--;
				if (ite.id < 0) {
					ite.id = ite.ITEMS_UNIQUE-1;
				}
			}

			// Mobs
			else if (place_type==4) {
				mb.type--;
				if (mb.type < 0) {
					mb.type = 1;
				}
			}
		}else{
			// Tiles
			if (place_type==0) {
				tl.id++;
			}

			// Tilec
			else if (place_type==1) {
				tlc.LevelToLoad++;
			}

			// Items
			else if (place_type==3) {
				ite.id++;
				if (ite.id > ite.ITEMS_UNIQUE-1) {
					ite.id = 0;
				}
			}

			// Mobs
			else if (place_type==4) {
				mb.type++;
				if (mb.type > 1) {
					mb.type = 0;
				}
			}
		}
		break;
	case SDLK_COMMA:							// Toggle if the Tile should be a collision tile
		if (place_type == 0 ) {
			tl.changeTileCollision(tile);
		}
		break;
	case SDLK_PERIOD:							// Change layer of the Tile our mouse is hovering over
		if (place_type == 0 ) {
			if (shift)
				tl.changeTileLayer(tile, -1);
			else
				tl.changeTileLayer(tile, 1);
		}else if (place_type == 1 ) {
			tlc.ChangePos(tilec);
		}
		break;
	case SDLK_c:								// Tile, copy Tile
		if (editor) {
			if (place_type == 0 ) {
				tl.copyTile(tile);
			}else if (place_type == 1) {
				tlc.Copy(tilec);
			}else if (place_type == 3) {
				ite.Copy(item);
			}
		}
		break;
	case SDLK_SPACE:							// Tile, remove all Tiles (and Collision Tiles)
		if (editor)
		{
			// If holding shift, only delete the currently selected place_type
			if (shift)
			{
				// Currently selected: Textured tiles
				if (place_type == 0 )
				{
					tl.RemoveAll(tile);
				}

				// Currently selected: Collision tiles
				else if (place_type == 1)
				{
					tlc.RemoveAll(tilec);
				}

				// Currently selected: Boss Objects
				else if (place_type == 2)
				{
					bos.RemoveAll(boss);
				}

				// Currently selected: Items
				else if (place_type == 3)
				{
					ite.RemoveAll(item);
				}

				// Currently selected: Mobs
				else if (place_type == 4)
				{
					mb.RemoveAll(mob);
				}
			}
			// Remove all tiles
			else {
				// Remove textured tiles
				tl.RemoveAll(tile);

				// Remove collision tiles
				tlc.RemoveAll(tilec);

				// Remove bosses
				bos.RemoveAll(boss);

				// Remove items
				ite.RemoveAll(item);

				// Remove mobs
				mb.RemoveAll(mob);
			}
		}
		break;
	case SDLK_LEFTBRACKET:						// Tile, subtract parry in x-axis
		if (shift) {
			map.IncreaseWidth(-tb.tileSizeInWorldW*2);
		} else {
			if (place_type == 0) {
				if (tl.multiW > 1) {
					tl.multiW -= 1;
				}
			}else if (place_type == 1) {
				if (tlc.multiW > 1) {
					tlc.multiW -= 1;
				}
			}else if (place_type == 3) {
				if (ite.multiW > 1) {
					ite.multiW -= 1;
				}
			}
		}
		break;
	case SDLK_RIGHTBRACKET:						// Tile, add parry in x-axis
		if (shift) {
			map.IncreaseWidth(tb.tileSizeInWorldW*2);
		} else {
			if (place_type == 0) {
				tl.multiW += 1;
			}else if (place_type == 1) {
				tlc.multiW += 1;
			}else if (place_type == 3) {
				ite.multiW += 1;
			}
		}
		break;
	case SDLK_MINUS:							// Tile, subtract parry in y-axis
		if (shift) {
			map.IncreaseHeight(-tb.tileSizeInWorldW*2);
		} else {
			if (place_type == 0) {
				if (tl.multiH> 1) {
					tl.multiH -= 1;
				}
			}else if (place_type == 1) {
				if (tlc.multiH> 1) {
					tlc.multiH -= 1;
				}
			}else if (place_type == 3) {
				if (ite.multiH> 1) {
					ite.multiH -= 1;
				}
			}
		}
		break;
	case SDLK_EQUALS:							// Tile, add parry in y-axis
		if (shift) {
			map.IncreaseHeight(tb.tileSizeInWorldW*2);
		} else {
			if (place_type == 0) {
				tl.multiH += 1;
			}else if (place_type == 1) {
				tlc.multiH += 1;
			}else if (place_type == 3) {
				ite.multiH += 1;
			}
		}
		break;
	case SDLK_0:
		{
			promptConfirm = true;
			if (!shift) {
				promptType = 0;
			} else {
				promptType = 1;
			}
			break;
		}
	case SDLK_u:	// Save player data to file
    	if (shift) {
    		player.SaveData();

			// Print message to screen
			tex.spawn(gRenderer, text, 0, 0, 0.0, 0.0, 255, "Saving Player data...", 0);
    	} else {
    		player.LoadData();

			// Print message to screen
			tex.spawn(gRenderer, text, 0, 0, 0.0, 0.0, 255, "Loading Player data...", 0);
    	}
		break;
	case SDLK_1:							// increase or decrease width of tile being placed
		if (shift) {
			if (place_type == 0) {
				tl.tilew -= 8;
			}else if (place_type == 1) {
				tlc.tilew -= 8;
			}
		}else{
			if (place_type == 0) {
				tl.tilew += 8;
			}else if (place_type == 1) {
				tlc.tilew += 8;
			}
		}
		break;
	case SDLK_2:							// increase or decrease height of tile being placed
		if (shift) {
			if (place_type == 0) {
				tl.tileh -= 8;
			}else if (place_type == 1) {
				tlc.tileh -= 8;
			}
		}else{
			if (place_type == 0) {
				tl.tileh += 8;
			}else if (place_type == 1) {
				tlc.tileh += 8;
			}
		}
		break;
		// Set render size
	case SDLK_F5:							// Spawn player on spawner
		player.x = spawnX;
		player.y = spawnY;
		break;
	}
}
void PlayGame::editorOnKeyUp( SDL_Keycode sym ) {
	switch (sym) {

	case SDLK_w:
		camUp = false;
		break;
	case SDLK_s:
		camDown = false;
		break;
	case SDLK_a:
		camLeft = false;
		break;
	case SDLK_d:
		camRight = false;
		break;
	}
}


/* Global functions used in World */


bool PlayGame::checkCollisionRect( SDL_Rect a, SDL_Rect b )
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


// Load saved configurations
void PlayGame::LoadCFG() {

	// Load Video settings from file
	settings.LoadVideoCFG();

	// Load Audio settings from file
	//settings.LoadAudioCFG();
}

void PlayGame::LoadHighScore() {
	std::stringstream filePath;
	filePath << "data/maps/highscore";
	filePath << LevelToLoad;
	filePath << ".txt";

	int previousHighScore = -1;
	std::fstream fileTileDataL(filePath.str().c_str());

	// File exists, load last high score
	if (fileTileDataL.is_open()) {
		fileTileDataL >> previousHighScore;
		fileTileDataL.close();

		// Apply that high score to current level for Render
		player.ApplyHighScore(previousHighScore);
	}

	// File does NOT exist, create file with default of 0 high score
	else {
		//std::cout<< "File does not exist on Loading Highscore, creating new\n";
		{
			std::stringstream filePath;
			filePath << "data/maps/highscore";
			filePath << LevelToLoad;
			filePath << ".txt";

			std::ofstream fileSave;
			fileSave.open(filePath.str().c_str());
			fileSave << 0;
			fileSave.close();

			// Apply that high score to current level for Render
			player.ApplyHighScore(0);
		}
	}
}

void PlayGame::LoadLevel()
{
	// Remove everything
	{
		// Remove particles tiles
		part.RemoveAll(particles);

		// Remove textured tiles
		tl.RemoveAll(tile);

		// Remove collision tiles
		tlc.RemoveAll(tilec);

		// Remove bosss
		bos.RemoveAll(boss);

		// Remove items
		ite.RemoveAll(item);

		// Remove mobs
		mb.RemoveAll(mob);
	}

	// Set file path and name
	std::stringstream tempss;
	tempss << "data/maps/";
	tempss << "Level";
	tempss << LevelToLoad;
	tempss << ".mp";
	std::fstream fileTileDataL(tempss.str().c_str());

	// While file is open
	while( fileTileDataL.good() )
	{
		// Load Tile data
		tl.LoadData(tile, fileTileDataL);

		// Load Tilec data
		tlc.LoadData(tilec, fileTileDataL);

		// Load Boss data
		bos.LoadData(boss, fileTileDataL);

		// Load Item data
		ite.LoadData(item, fileTileDataL);

		// Load Mob data
		mb.LoadData(mob, fileTileDataL);

		// Load Player spawn point
		fileTileDataL >>  this->spawnX >> this->spawnY;

		// Load map size
		fileTileDataL >>  map.w >> map.h;

		// Break out of file
		break;
	}
	fileTileDataL.close();

	// Load spawn on level load
	player.x		= this->spawnX;
	player.y		= this->spawnY;

	// Center camera right way so it doesnt look like its panning to that location
	camx  = player.x+player.w/2-screenWidth/2;
	camy  = player.y+player.h/2-screenHeight/2;

	// Update number of jars left
	UpdateJarsLeft();
}

void PlayGame::LoadAudioSettings() {
	settings.LoadAudio();
	settings.LoadAudioCFG();
	settings.ApplyAudioCfgToSFX();
}
