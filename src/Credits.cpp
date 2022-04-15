/*
 * Credits.cpp
 *
 *  Created on: Aug 22, 2017
 *      Author: Carl
 */

#include "Credits.h"


void Credits::Show(LWindow &gWindow, SDL_Renderer *gRenderer, Credits::CreditsResult &result) {

	// particle
	SDL_Rect room = {0, 0, 512, 512};
	int mex, mey;
	bool showDialogue;
	unsigned int dialogueIndex;

	// Upon entry
	showDialogue = true;
	dialogueIndex = 0;
	indicatorTimer = 0;
	indicatorFrame = 0;
	indicator = ">";
	holdUp = false;
	holdDown = false;
	holdTimer = 0.0;
	quit = false;
	gCursor.loadFromFile(gRenderer, "resource/gfx/cursor.png");
	gTexture.loadFromFile(gRenderer, "resource/gfx/test.png");
	gPoof.loadFromFile(gRenderer, "resource/gfx/poof.png");
	gBG.loadFromFile(gRenderer, "resource/gfx/credits-bg.png");
	gBG2.loadFromFile(gRenderer, "resource/gfx/credits-bg-2.png");
	gDialogueBox.loadFromFile(gRenderer, "resource/gfx/dialogue-box.png");
	gTargetTexture.createBlank( gRenderer, screenWidth, screenHeight, SDL_TEXTUREACCESS_TARGET );

	for (int i=0; i<7; i++) {
		rPoof[i] = {0+i*8, 0, 8, 8};
	}

	// Events
	SDL_Event event;

	// Dialogue
	std::vector<std::string> dialogue;
	dialogue.push_back("...");
	dialogue.push_back("Someone help please!");
	dialogue.push_back("...");
	dialogue.push_back("You there soldier!");
	dialogue.push_back("I know this may be too much to ask but you are the last of our soldiers...");
	dialogue.push_back("I beg of you bring this final message to the King.");
	dialogue.push_back("I don't have much time left...");
	dialogue.push_back("Tell him...");
	dialogue.push_back("\"Cough cough\"");
	dialogue.push_back("Tell him his daughter lives.");
	dialogue.push_back("That's it.");
	dialogue.push_back("...");
	dialogue.push_back("Me?");
	dialogue.push_back("I will try and fend off these wild beasts for as long as I stand.");
	dialogue.push_back("There are plenty monsters in the castle still but the majority are in front of the gates.");
	dialogue.push_back("Once you deliver this message to the King, if you would like to help, come find me at the gates.");
	dialogue.push_back("I will see you soldier.");
	dialogue.push_back("What was it, your name was?");

	// Around here, have soldier reply back and say something about their name
	std::string name = "Gridin";
	std::stringstream tempss;
	tempss << "Ah "<< name << ", ok, I will remember that.";
	dialogue.push_back(tempss.str().c_str());

	tempss.str(std::string());
	tempss << "After this is over I'll buy you a drink " << name << ".";
	dialogue.push_back(tempss.str().c_str());
	dialogue.push_back("Be seeing you.");

	float timer;
	float rate;
	unsigned int letters;
	std::stringstream dialogueToRender;

	timer = 0;
	rate = 20;
	letters = 0;


	// Load Credits.txt into a vector
	// this will make it so we can save CPU
	// while only rendering a few lines at a time
    unsigned int slideIndex;
	std::vector<std::string> wholeText;
	wholeText.clear();
	std::ifstream infile("CREDITS.txt");
	std::string line;
    while (std::getline(infile, line)) {
    	wholeText.push_back(line);
    }
    slideIndex = 0;


	static Particle part;
	static Particle particle[800];
	part.init(particle);
	part.load(gRenderer);
    LoadFonts();

	// While loop
	while (!quit) {

		// fps timer
		fps.start();
		// Get mouse position
		SDL_GetMouseState(&mex, &mey);

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
		mex = (screenWidth*mex)/gWindow.getWidth();			// New mouse coordinates, no relation to camera
		mey = (screenHeight*mey)/gWindow.getHeight();			// New mouse coordinates, no relation to camera

		// Handle events
		while (SDL_PollEvent(&event)) {

			// Global Events
			if (event.type == SDL_QUIT) {
				result = Exit;
				part.free();
				free();
				return;
			}

			// Handle window events
			gWindow.handleEvent(gRenderer, event);

			// Controller button down
			if (event.type == SDL_JOYBUTTONDOWN){

			}

			// Key Pressed
			if (event.type == SDL_KEYDOWN && event.key.repeat == 0) {
				switch (event.key.keysym.sym) {
				case SDLK_UP:						// reset test-room
					holdUp = true;
					if (slideIndex > 0) {
						slideIndex--;
					}
					break;
				case SDLK_DOWN:						// reset test-room
					holdDown = true;
					if (slideIndex < wholeText.size()-45) {
						slideIndex++;
					}
					break;
				case SDLK_w:						// reset test-room
					holdUp = true;
					if (slideIndex > 0) {
						slideIndex--;
					}
					break;
				case SDLK_s:						// reset test-room
					holdDown = true;
					if (slideIndex < wholeText.size()-45) {
						slideIndex++;
					}
					break;
				case SDLK_r:						// reset test-room

					break;
				case SDLK_q:
					//SDL_ShowCursor(SDL_TRUE);
					break;
				case SDLK_e:
					//SDL_ShowCursor(SDL_FALSE);
					break;
				case SDLK_RETURN:				// Cycle through dialogue
					break;
				case SDLK_ESCAPE:
					result = Back;
					break;
				}
			}
			// Key Released
			else if (event.type == SDL_KEYUP && event.key.repeat == 0) {
				switch (event.key.keysym.sym) {
				case SDLK_UP:
					holdUp = false;
					holdTimer = 0;
					break;
				case SDLK_DOWN:
					holdDown = false;
					holdTimer = 0;
					break;
				case SDLK_w:
					holdUp = false;
					holdTimer = 0;
					break;
				case SDLK_s:
					holdDown = false;
					holdTimer = 0;
					break;
				}
			}
			// Mouse Pressed
			if (event.type == SDL_MOUSEBUTTONDOWN) {
				poofs.push_back( Poof(mex-32/2, mey-32/2) );
				if (event.button.button == SDL_BUTTON_LEFT) {
					/*part.spawnParticleAngle(particle, "none", 3,
							mex - 4/2,
							mey - 4/2,
							4, 4,
							0.0, 1.2,
						   25,
						   {244,255,255}, 1,
						   0, 0,
						   255, 0,
						   60, 1,
						   false, 0.0,
						   false, 0.0);*/
				}
				if (event.button.button == SDL_BUTTON_RIGHT) {
				}
			}
			// Mouse Released
			else if (event.type == SDL_MOUSEBUTTONUP) {
				if (event.button.button == SDL_BUTTON_LEFT) {

				}
				if (event.button.button == SDL_BUTTON_RIGHT) {

				}
			}

		     if(event.wheel.y == 1) // scroll up
		     {
					if (slideIndex > 0) {
						slideIndex--;
					}
		     }
		     else if(event.wheel.y == -1) // scroll down
		     {
					if (slideIndex < wholeText.size()-45) {
						slideIndex++;
					}
		     }
		}

		// update joystick

		// Customize Character results
		switch (result)  {
			case Nothing:		// Nothing

				break;
			case Back:			// Back
				quit = true;
				break;
			case Exit:			// Exit
				quit = true;
				break;
		}

		if (holdUp) {
			holdTimer++;
			if (holdTimer > 21) {
				holdTimer = 21;
				if (slideIndex > 0) {
					slideIndex--;
				}
			}
		}
		if (holdDown) {
			holdTimer++;
			if (holdTimer > 21) {
				holdTimer = 21;
				if (slideIndex < wholeText.size()-45) {
					slideIndex++;
				}
			}
		}

		// Indicator animation
		indicatorTimer += 3;
		if (indicatorTimer > 60) {
			indicatorTimer = 0;
			indicatorFrame++;
			if (indicatorFrame > 1) {
				indicatorFrame = 0;
			}
		}
		if (indicatorFrame == 0) {
			indicator = "";
		}else{
			indicator = ">";
		}

		// Update particle
		part.Update(particle, 0, 0, room.w, room.h);
		part.UpdateVFX(particle, 0, 0, room.w, room.h);
		part.UpdateBullets(particle, 0, 0, room.w, room.h);


		if (poofs.size() < maxPoofs) {
			timer += 3;
			if (timer > 60) {
				timer = 0;
				float x =  randDouble(0, screenWidth-8);
				float y =  randDouble(0, screenHeight-8);
				poofs.push_back( Poof(x, y) );
			}
		}

		// update poofs
		for (unsigned int i=0; i<poofs.size(); i++) {
			// do timer for poofs
			poofs[i].timer += 11;
			if (poofs[i].timer > 60) {
				poofs[i].timer = 0;
				poofs[i].frame++;
				if (poofs[i].frame > 6) {
					poofs[i].frame = 0;
					// expired poofs
					poofs.erase(poofs.begin()+i);
				}
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////////////
		//------------------------------------ Draw on Scene here ------------------------------------//
		//Set our Texture as our render target
		gTargetTexture.setAsRenderTarget(gRenderer);
		// Clear render screen
		SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
		SDL_RenderClear(gRenderer);

			// Render foreground
			//gTexture.setAlpha(255);
			//gTexture.render(gRenderer, 0, 0, 128, 128);

			// Render particle
			part.RenderVFX(particle, 1, 1, 1, gRenderer);
			part.RenderBullets(particle, 1, 1, 1, gRenderer);

		//Reset render target
		SDL_SetRenderTarget( gRenderer, NULL );
		//------------------------------------ Draw on Scene here ------------------------------------//
		////////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////////////


		// Clear render screen
		SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
		SDL_RenderClear(gRenderer);


			// Render default lighting background (sets the darkness level
			/*gLightBG.setAlpha(50);
			gLightBG.setColor(255,255,255);
			gLightBG.setBlendMode(SDL_BLENDMODE_ADD);
			gLightBG.render( gRenderer, 0, 0, screenWidth, screenHeight);

			// Render Particle lights
			for (int i = 0; i < part.max; i++) {
				if (particle[i].alive) {
					if (particle[i].damage > 0) {
						gLight.setAlpha(180);
						gLight.setColor(particle[i].color.r, particle[i].color.g, particle[i].color.b);
						gLight.setBlendMode(SDL_BLENDMODE_ADD);
						gLight.render( gRenderer, particle[i].x2-108/2-0, particle[i].y2-108/2-0, 108, 108);
					}else{
						gLight.setAlpha(particle[i].alpha);
						gLight.setColor(particle[i].color.r, particle[i].color.g, particle[i].color.b);
						gLight.setBlendMode(SDL_BLENDMODE_ADD);
						SDL_Rect rparticle = {0,0,8,8};
						gLight.render( gRenderer, particle[i].x2-(particle[i].w/2)*7-0, particle[i].y2-(particle[i].h/2)*7-0, particle[i].w*7, particle[i].h*7, NULL, particle[i].angle);

					}
				}
			}*/

			// Render sceene
			//gTargetTexture.setBlendMode(SDL_BLENDMODE_MOD);
			gTargetTexture.render( gRenderer, 0, 0, screenWidth, screenHeight);
			gBG.render( gRenderer, 0, 0, screenWidth, screenHeight);

			// render "stars"
			gPoof.setAlpha(255);
			for (unsigned int i=0; i<poofs.size(); i++) {
				gPoof.render( gRenderer, poofs[i].x, poofs[i].y, 32, 32, &rPoof[poofs[i].frame] );
			}
			//gBG2.setAlpha(170);
			//gBG2.render( gRenderer, 0, 0, screenWidth, screenHeight);

			// Render credits text
			int CreditsTxtLines = 5;
			for (unsigned int i=0+slideIndex; i<45+slideIndex; i++) {
		    	gText.loadFromRenderedText(gRenderer, wholeText[i].c_str(), {255,255,255}, gFont12, 400);
				int newWidth = gText.getWidth();
				int newHeight = gText.getHeight();
		    	gText.render(gRenderer, 32,
		    							32+(       (i-slideIndex)   *   (newHeight+1)           ),
										newWidth,
										newHeight);
			}

			// RENDER BG IN FRONT OF TEXT
			gBG2.render( gRenderer, 0, 0, screenWidth, screenHeight);


			float x = 0+4;
			float y = screenHeight-24 - 8;

			/*renderDialogText(gRenderer, "Credits",
							 "Thank you for playing mey game, I hope you liked it. \nPress Escape to return to the Main Menu.", indicator.c_str(),
						     x, y, 21, 24,
						     x, y, screenWidth-8, 24,
						     {255,255,255}, {255,255,255},
						     {18,18,18}, {60,200,40},
						     {18,18,18}, {60,200,40},
						     gFont, gFont, gText,
							 1000,  true);*/




			/*if (showDialogue) {
				if ( letters < dialogue[dialogueIndex].size() ) {
					timer += rate;
					if (timer > 60) {
						dialogueToRender << dialogue[dialogueIndex][letters];
						timer = 0;
						letters++;
						// Play typing sound effect
						Mix_PlayChannel(-1, sTyping, 0);
					}
				}
				std::string temps;
				temps = dialogueToRender.str().c_str();
				float x = 0+4;
				float y = screenHeight-24 - 8;

				//gDialogueBox.render(gRenderer, x, y - 9 - 2, 50, 9);
				//gDialogueBox.render(gRenderer, x, y, 262, 24);

				renderDialogText(gRenderer, "Jacky The Jacker",
								 temps.c_str(), indicator.c_str(),
							     x, y, 21, 24,
							     x, y, screenWidth-8, 24,
							     {255,255,255}, {255,255,255},
							     {18,18,18}, {200,100,250},
							     {18,18,18}, {200,100,250},
							     gFont, gFont, gText,
								 1000,  true);

			}*/

			// Render mouse location
			gCursor.render(gRenderer, mex, mey, 20, 20);

		// Update screen
		SDL_RenderPresent(gRenderer);

		// fps
		frame++;
		if (cap && fps.get_ticks() < 1000 / FRAMES_PER_SECOND) {
			SDL_Delay((1000/FRAMES_PER_SECOND ) - fps.get_ticks());
		}
	}
	part.free();
	free();
}

void Credits::free() {
	// Free resources
	FreeFonts();
	gText.free();
	gBG.free();
	gBG2.free();
	gTexture.free();
	gTargetTexture.free();
	gDialogueBox.free();
	gPoof.free();
}
