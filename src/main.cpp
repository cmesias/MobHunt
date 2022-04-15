#include "Game.h"

int main(int argc, char *argv[]) {
	Game game;

	game.Start();

	return 0;
}

// Notes

/*
 * Common problems:
 * loadFromRenderedText -> need "#include <SDL2/SDL_ttf.h>"
 *
 * ShowSplashScreen() -> ShowMenu()
 * 						 -> mainMenu.Show(gWindow, gRenderer, result);
 *						 -> mainMenu.Render() - its crashing here, why?
 *						 -> [x] - Fixed: it was rendering some stuff from Joystick variables that were
 *						 	      not initialized so we just commented it out.
 *
 * Other Notes:
 * - the screen is being render at 1920x1080, but we shrinking it to 1280x720
 *
 *	How Saving Works**** (3-11-2022)
 *	(Do not delete)
 *	- Part I: Save 		Tile count.
 *	- Part II save: 	Save Tile data.
 *	- Part III save: 	Save Tilec count.
 *	- Part IV save: 	Save Tilec data.
 *	- Part V save: 		Save Slime data.
 *	- Part VI save: 	Save Player spawn point
 *
 *
 */

/*	Stuff to do :)
 *
 *
 * [Game Features]
 *
 * // PlayGame.cpp
 * TODO [x] (3-8-2022) - Make hit box on player's attack wider, but less height
 * TODO [x] (3-8-2022) - Check collision for player's attack hit-box and Slime's
 * 						hit-box with a square check collision rather than distance collision
 * // Player.cpp
 * TODO [x] - fix players position when they Attack facing the Left side
 * TODO [x] - fix player size so that they may be able to go through a tight squeeze of 48x48
 * TODO [x] (3-7-2022) - Save and load Boss data to files for loading
 * TODO [x] (3-7-2022) - Replace Title Menu screen from our previous Dungeon Game,
 *						 and also get pause-menu from previous Dungeon Game
 * TODO [x] - Code an Options Menu inside options to be used in both MainMenu and during GamePlay
 *			  After coding of Options Menu, the goal is to remove the Pause.cpp class.
 * TODO [x] - Create shadows under Player's texture
 *
 * //Customize Character
 * TODO [ ] - how to save Texture as a PNG file format? possible solution:: find another library for loading png
 * TODO [ ] - replace in-game-text-buttons with drawn buttons from photoshop
 *
 * // No specific class
 * TODO [x] - Go left and right, up and down
 * TODO [x] - Create monsters class (i.e. slime, goblin)
 * TODO [x] (3-8-2022) - Create TextNFont.cpp class - This class can be included by any other classes that want to render text
 * TODO [x] - Create sound effect for down stab hitting the ground
 * TODO [x] - Change sound effect for hitting a slime with player slash attack
 * TODO [x] - Down stab hit-box is not aligned with players sword
 * TODO [x] (3-10-2022) - Slime.cpp -> Add code to play sound effect when slime attacks
 * TODO [x] (3-10-2022) - Create level selection Scene
 * TODO [x] (3-10-2022) - Dash ability
 * TODO [x] (3-12-2022) - Add objective to game (defeat the boss)
 * TODO [x] (3-15-2022) - Make walk VFX nicer
 * TODO [x] (3-15-2022) - Give dash a sound effect
 * TODO [x] (3-15-2022) - Fix collision check with Slimer and Tiles
 * TODO [x] (3-15-2022) - Create sprite in photoshop for Player that animates when the Player uses teh Dash ability
 * TODO [x] (3-16-2022) - Make player invulnerable when dashing
 * TODO [x] (3-18-2022) - Change Player bullet texture to Slash Textures instead of Bullet textures
 * TODO [x] Create damage multiplier for player when " " happens
 * TODO [x] Render text for damage multiplier above player
 * TODO [x] When player loads a new level, and returns to the old level it loads in the rong spot. Maybe create a "lastKnownPosition"
 * 			variable so when the player loads a previous area, it will save where they used to. Maybe we can create some kind of grid system
 * 			Where we can make a Level.mp that is already in the right position on the x and y position if it were a huge map.
 * 			For example:
 * 			[][][]
 * 			[][][]
 * 			[][][]
 * 			- Each grid is an area (level) we can load. When we create
 * 			  a level, the positions of the next areas will be going off the x and y positions of the previous areas.
 * 			- For example if Area (0,0) start on (0,0) and ends in (500,500) pixels,
 * 			  then Area (0,1) will start on on (500,0) and ends in (1000,500);
 * TODO [ ] - Mob.cpp, when getting hit by the player, set animState for Mob to 1 (this will give a "stunned" effect to the Mob)
 *
 *
 * TODO [x] - PlayGame.cpp: fix this: The audio just stops playing for the audio that we load in this class.
 *  		  After coming back Settings, even if we don't change
 * 		 	  anything and just come back, the bug still happens. Fixed, for some reason,
 * 		 	  after coming back from the Settings menu, we have to load the audio files again.

// TODO [x]  - get rid of old options thing, and put this in PlayGame
 *
 * TODO [x] (3-25-2022) - Change player texture with a character form the dungeon tileset
 * TODO [x] (3-25-2022) - Create simple pathfinding for Mobs to follow Player
 * TODO [ ] (3-25-2022) - Integrate Enemies.cpp into PlayGame.cpp to spawn different types of monsters
 * TODO [ ] (3-25-2022) - Add coins variable to -> Player.cpp
 * TODO [ ] (3-25-2022) - When Mobs die, have them drop coins -> Mob.cpp
 * TODO [ ] (3-27-2022) - When picking up an item from an area, permanently remove that item from that area
 * TODO [x] (3-27-2022) - No inventory slot, just one slot per run
 * TODO [x] (3-27-2022) - Crashes when returning to menu or exiting game, fixed some how?
 * TODO [x] (3-28-2022) - Create prompt for items pick-up
 * TODO [x] (3-28-2022) - Create prompt for leaving-area door
 * TODO [x] (3-28-2022) - Create prompt to unlock doors when the player has a key to unlock
 * TODO [x] (3-28-2022) - Create prompt to unlock Chests with appropriate keys, then Spawn a random Sword out of the chests
 * TODO [ ] (3-28-2022) - When picking up a new weapon, drop the old one near by
 * TODO [ ] (3-28-2022) - Create some kind of stun for the enemies if tehy are getting hit by the player?
 * 						  But then again this can be a difficulty? If its on easy then the enemies will get "stunned" when hit,
 * 						  but if the Player wants to play on hard then enemies will not get stunned when hit regularly.
 *
 *
 * TODO [ ] *** create a cast damage for each sword, create this new variable in Itms.cpp
 * TODO [ ] (3-28-2022) - Create an autosave when the player goes to a new area autosave.
 * 						  [ ] Create autosave tiles that will save a checkpoint.
 * 						  [ ] Figure out if we should spawn back monsters or keep them dead.
 * 						  [ ] Autosave when the player takes a key from an area, and keep it gone.
 *
 *
 *  TODO [ ] fix memory leak in Settings.cpp
 *
 *
 * TODO [x] (3-29-2022) - Create shadows for particles/projectiles
 *
 *
 * //---------- Item.cpp
 * TODO [x] (3-26-2022) - Create Item.cpp to create items that the player can pick up such as: Swords, Tools, Bombs and Hearts.
 * TODO [x] (3-26-2022) - Create collision check with Player.cpp
 *
 * //---------- Player.cpp
 * TODO [x] (3-26-2022) - Create inventory for Player.cpp?
 *
 *
 * //------------------- Future
 * TODO [ ] - Create End Screen after defeating Boss, and add a transition to go into next Level.
 * TODO [ ] - Also after defeating the boss, make sure to save into the save file "levelsUnlocked.mp" to save how many levels the Player has completed
 * TODO [ ] - When we have 2 or more Boss's spawned, they used up the max number of particles we have started with, making it look like the Boss is not shooting its full attack
 *
 * TODO [ ] - Fix this checkBossDied() at PlayGame.cpp
 * 			  not really considered a bug, but if we have 5 bosses spawned,
 * 			  if the player defeats 1 boss, it will automatically save to the levelsUnlocked.mp file
 * 			  A way around this is creating a variables in PlayGame.cpp like RequiredBossesToDefeat and set it to 5.
 * 			  if the player defeats one boss at a time, the RequiredBossesToDefeat variable will reduce by 1.
 * 			  if the RequiredBossesToDefeat var reaches 0 or lower, then we save to the levelsCompleted.mp file.
 *
 * TODO [ ] Players.cpp (2) - Create summary page after defeating the boss and add things like:
 * TODO [ ] Players.cpp (2) - Play SFX when Player has no more mana, but is still trying to shoot out SlashAttack particles
 * 					 1. How many bullets the Player shot
 * 					 2. How many bullets the Player took
 * 					 3. How many bullets the Player destroyed
 * 					 4. How many bullets the Player blocked/parried
 * 					 5. How many times the Player died
 * TODO [ ] - Create re-bind keys section in Settings
 *
 *
 * [Game Features (2)]
 *
 * TODO [ ] (3-12-2022) - Add more monsters
 * TODO [ ] (3-20-2022) - Get input from player so they can input their names
 *
 * [Bugs]
 * TODO [x] - SaveHighScore in Player.cpp not working
 * TODO [ ] - Line 1288 PlayGame.cpp, show congrats screen when Boss dies, and do for Slash attack also
 * TODO [ ] - Fix when text is displayed formthe text file Credits.txt
 *
 * [Re-Enable if Editing]
 * TODO [ ] - Re-enable Keyboard key P and H to show Debug and Enable Editor mode
 * TODO [ ] - Re-enable Editor, Debug, and camlock back to true upon Init() of PlayGame.cpp
 * TODO [ ] - Re-enable saving Tile Map and loading Tile Map SDLK_0 and _1
 *
 *
 * [Contact these people!]
 * TODO [ ] - Contact resource authors and notify them of the game.
 *
 *
 *
 * TODO [ ]	- Resource Name: 		Knight set
 *			- Author: 				MASTER484
 *			- Author Website: 		http://m484games.ucoz.com/

 * TODO [ ]	- Resource Name:		16x16-dungeon-tileset
 *			- Author: 				0x72
 *			- Author Website: 		https://0x72.itch.io/16x16-dungeon-tileset
 *
 *
 * TODO [ ]	- Resource Name: 		Neon starlight by Necrophageon
 *			  Author: 				Necrophageon
 *			  Author Website: 		http://famitracker.com/forum/profile.php?id=2752
 *			  Author Website #2: 	https://soundcloud.com/necrophageon
 *
 *
 *
 *
 *
 *



 */


