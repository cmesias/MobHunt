/*
 * Item.cpp
 *
 *  Created on: May 31, 2017
 *      Author: Carl
 */


#include "Item.h"

void Item::Init(Item item[]){
	count = 0;
	for (int i = 0; i < max; i++) {
		item[i].hoverAmount 	= 9;
		item[i].hoverDir 		= 1;
		item[i].alive 			= false;
		item[i].PlayerBehindItem = false;
		item[i].promptSelf = false;
		item[i].damage 			= 0;
		item[i].slashAtkSpe 	= 0.25;
	}
}

void Item::Load(SDL_Renderer* gRenderer){
	gSwords.loadFromFile(gRenderer, "resource/gfx/author_0x72/0x72_16x16DungeonTileset_swords.png");

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

		// Golden key
		rSwords[28] = {96,48,8,7};
	}

	// Other fonts
	LoadFonts();
}

void Item::Free(){
	gSwords.free();

	// Other classes
	FreeFonts();
}

void Item::Copy(Item item[]) {
	for (int i = 0; i < max; i++) {
		if (item[i].alive){
			if (item[i].mouse){
				id = item[i].id;
			}
		}
	}
}

void Item::Remove(Item item[], int click) {
	for (int i = 0; i < max; i++) {
		if (item[i].alive){
			// Left click
			if (click == 0) {
				if (item[i].mouseBox){
					item[i].alive = false;
					count--;
				}
			// Right click
			}else{
				if (item[i].mouse){
					item[i].alive = false;
					count--;
				}
			}
		}
	}
}

void Item::RemoveAll(Item item[]) {
	count = 0;
	multiW = 1;
	multiH = 1;
	id = 0;
	for (int i = 0; i < max; i++) {
		if (item[i].alive){
			item[i].alive = false;
		}
	}
}

void Item::Spawn(Item item[], float x, float y, int id) {
	Remove(item, 0);
	for (int i = 0; i < max; i++) {
		if (!item[i].alive) {
			item[i].w 				= getItemSizeW();
			item[i].h 				= getItemSizeH();
			item[i].x 				= x;
			item[i].y 				= y;
			item[i].vX 				= 0.0;
			item[i].vY 				= 0.0;
			item[i].id 				= id;
			item[i].collision 		= false;
			item[i].mouse 			= false;
			item[i].promptSelf 		= false;
			item[i].mouseBox 		= false;
			item[i].onScreen 		= false;
			item[i].alive 			= true;
			setStatsBasedOnType(item, i);
			count++;
			break;
		}
	}
}

void Item::SpawnAndThrowItem(Item item[], float x, float y, int id, float vX, float vY) {
	Remove(item, 0);
	for (int i = 0; i < max; i++) {
		if (!item[i].alive) {
			item[i].w 				= getItemSizeW();
			item[i].h 				= getItemSizeH();
			item[i].x 				= x;
			item[i].y 				= y;
			item[i].id 				= id;
			item[i].collision 		= false;
			item[i].mouse 			= false;
			item[i].promptSelf 		= false;
			item[i].mouseBox 		= false;
			item[i].onScreen 		= false;
			item[i].alive 			= true;
			// Applies atk spe, damage for spawning object
			setStatsBasedOnType(item, i);
			// Apply speed the item was spawned at
			item[i].vX 				= vX;
			item[i].vY 				= vY;
			count++;
			break;
		}
	}
}

void Item::SpawnAll(Item item[], int newMx, int newMy, int camx, int camy) {
	Remove(item, 0);
	for (int j = 0; j < multiW; j++) {
		for (int h = 0; h < multiH; h++) {
			int x = int(newMx + j * getItemSizeW() + camx);
			int y = int(newMy + h * getItemSizeH() + camy);
			Spawn(item, x, y,  this->id);
		}
	}
}

void Item::Update(Item item[], int newMx, int newMy, int mex, int mey, int camx, int camy,
		  float targetX, float targetY, float targetW, float targetH) {
	for (int i = 0; i < max; i++) {
		if (item[i].alive) {
			// center of item
			item[i].x2 = item[i].x+item[i].w/2;
			item[i].y2 = item[i].y+item[i].h/2;

			// Check if player is in front of Tile or not
			if (targetY+targetH < item[i].y+item[i].h){
				item[i].PlayerBehindItem = true;
			} else {
				item[i].PlayerBehindItem = false;
			}

			// Move item if it has velocity
			item[i].x += item[i].vX;
			item[i].y += item[i].vY;

			// Slow down item every frame
			item[i].vX = item[i].vX - item[i].vX * 0.7;
			item[i].vY = item[i].vY - item[i].vY * 0.7;

			// item target
			/*float bmx2 = item[i].x+item[i].w/2;
			float bmy2 = item[i].y+item[i].h/2;
			float bmx  = player.x2+player.radius;
			float bmy  = player.y2+player.radius;
			// item distance from target
			float distance = sqrt((bmx - bmx2) * (bmx - bmx2) + (bmy - bmy2) * (bmy - bmy2));
			if (distance <= 0.1) {
				distance = 0.1;
			}
			item[i].angle = atan2(bmy - bmy2,bmx - bmx2);
			item[i].angle = item[i].angle * (180 / 3.1416);

			if (item[i].angle < 0) {
				item[i].angle = 360 - (-item[i].angle);
			}*/


			//-----------------------------------------------------------------------------------//

			/* NOTE: we create a different constant variable
			 * for the Item for it to follow the Player.
			 * We create another variable specifically for knocking back the Item.
			 */

			// item movement
			//item[i].x += item[i].vX;
			//item[i].y += item[i].vY;

			// velocity decay
			//item[i].vX = item[i].vX - item[i].vX * 0.08;
			//item[i].vY = item[i].vY - item[i].vY * 0.08;

			// item circle collision check with other items
			/*for (int j = 0; j < max; j++) {
				if (i !=j) {
					if (item[j].alive) {
						float bmx = item[j].x+item[j].w/2;
						float bmy = item[j].y+item[j].h/2;
						float bmx2 = item[i].x+item[i].w/2;
						float bmy2 = item[i].y+item[i].h/2;
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
						if (distance <= 1) {
							distance = 1;
						}
						if (distance < item[i].w/2 + item[j].w/2) {
							item[i].x -= 1 * Cos;
							item[i].y -= 1 * Sin;
						}
					}
				}
			}*/
		}
	}
}

void Item::UpdateEditor(Item item[], int newMx, int newMy, int mex, int mey, int camx, int camy) {
	int tileW = getItemSizeW()*multiW;
	int tileH = getItemSizeH()*multiH;
	for (int i = 0; i < max; i++) {
		if (item[i].alive) {

			// item screen check
			if (item[i].x + item[i].w >= camx && item[i].x <= camx + screenWidth &&
				item[i].y + item[i].h >= camy && item[i].y <= camy + screenHeight) {
				item[i].onScreen = true;
			}else{
				item[i].onScreen = false;
			}
			//If the mouse+size is on the tile
			if (newMx+tileW-4 > item[i].x && newMx+2 < item[i].x + item[i].w &&
				newMy+tileH-4 > item[i].y && newMy+2 < item[i].y + item[i].h) {
				item[i].mouseBox = true;
			} else {
				item[i].mouseBox = false;
			}
			//If the mouse is on the tile
			if (mex > item[i].x && mex < item[i].x + item[i].w &&
				mey > item[i].y && mey < item[i].y + item[i].h) {
				item[i].mouse = true;
			} else {
				item[i].mouse = false;
			}
		}
	}
}

void Item::RenderBehindPlayer(SDL_Renderer *gRenderer, Item item[], int camx, int camy) {
	for (int i = 0; i < max; i++) {
		if (item[i].alive) {
			// If player is below Item
			if (item[i].PlayerBehindItem)
			{
				// Do nothing
			}
			// If player is above Item
			if (!item[i].PlayerBehindItem)
			{
				// Give visual effect that sword is hovering
				float newY = item[i].y;
				{
					item[i].hoverAmount += 0.1 * item[i].hoverDir;
					if (item[i].hoverAmount > 4) {
						item[i].hoverDir = -1;
					}
					else if (item[i].hoverAmount < 1) {
						item[i].hoverDir = 1;
					}

					newY = item[i].y - item[i].hoverAmount + yOffset;
				}
				// Render items
				gSwords.setAlpha(255);
				gSwords.render(gRenderer, item[i].x - camx, newY - camy, item[i].w, item[i].h, &rSwords[item[i].id]);
			}
		}
	}
}

void Item::RenderOnTopOfPlayer(SDL_Renderer *gRenderer, Item item[], int camx, int camy) {
	for (int i = 0; i < max; i++) {
		if (item[i].alive) {

			// If player is front of Item
			if (item[i].PlayerBehindItem)
			{
				// Give visual effect that sword is hovering
				float newY = item[i].y;
				{
					item[i].hoverAmount += 0.1 * item[i].hoverDir;

					// If hover amount greater than 4
					if (item[i].hoverAmount > 4) {

						// Change direction
						item[i].hoverDir = -1;
					}

					// If hover amount less than 1
					else if (item[i].hoverAmount < 1) {

						// Change direction
						item[i].hoverDir = 1;
					}

					newY = item[i].y - item[i].hoverAmount + yOffset;
				}
				// Render items
				gSwords.setAlpha(255);
				gSwords.render(gRenderer, item[i].x - camx, newY - camy, item[i].w, item[i].h, &rSwords[item[i].id]);
			}


			// If player is above Item
			if (!item[i].PlayerBehindItem)
			{
				// Do nothing
			}
		}
	}
}

void Item::RenderUI(SDL_Renderer *gRenderer, Item item[], int camx, int camy) {
	for (int i = 0; i < max; i++) {
		if (item[i].alive) {
			if (item[i].promptSelf) {
				std::stringstream tempsi;
				tempsi << "E";
				gText.loadFromRenderedText(gRenderer, tempsi.str().c_str(), {255, 255, 255}, gFont20);
				gText.render(gRenderer, item[i].x+item[i].w/2-gText.getWidth()/2-camx,
									    item[i].y+item[i].hoverAmount*0.6-gText.getHeight()-camy,
										gText.getWidth(), gText.getHeight());
			}
		}
	}
}

void Item::RenderDebug(SDL_Renderer* gRenderer, Item item[], int camx, int camy) {
	for (int i = 0; i < max; i++) {
		if (item[i].alive) {
			if (item[i].mouse) {
				SDL_Rect tempr = {item[i].x - camx, item[i].y - camy, item[i].w, item[i].h};
				SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
				SDL_RenderDrawRect(gRenderer, &tempr);
			}
			else {
				SDL_Rect tempr = {item[i].x - camx, item[i].y+1 - camy, item[i].w, item[i].h};
				SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
				SDL_RenderDrawRect(gRenderer, &tempr);
			}
			if (item[i].mouseBox) {
				SDL_Rect tempr = {item[i].x - camx, item[i].y - camy, item[i].w-4, item[i].h-4};
				SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
				SDL_RenderDrawRect(gRenderer, &tempr);
			}
		}
	}
}

void Item::RenderHand(SDL_Renderer *gRenderer, Item item[], int newMx, int newMy, int mex, int mey)
{
	for (int j = 0; j < multiW; j++) {
		for (int h = 0; h < multiH; h++) {
			gSwords.render(gRenderer, newMx+j*getItemSizeW(), newMy+h*getItemSizeH(), getItemSizeW(),
										getItemSizeH(), &rSwords[id]);

			SDL_Rect tempr = {newMx, newMy, getItemSizeW()*multiW, getItemSizeH()*multiH};
			SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
			SDL_RenderDrawRect(gRenderer, &tempr);
		}
	}
}

void Item::setStatsBasedOnType(Item item[], int i) {

	// Sword damage and attack speed
	{
		// TODO do damage for all swords
		if (item[i].id == 0) {
			item[i].damage 			= 10;
			item[i].slashAtkSpe 	= 1;
		}
		// Wood sword
		if (item[i].id == 1) {
			item[i].slashAtkSpe 	= 20;
			item[i].damage 			= 10;
		}
		// Rusty sword
		if (item[i].id == 2) {
			item[i].slashAtkSpe 	= 5;
			item[i].damage 			= 15;
		}
		// Iron sword
		if (item[i].id >= 3 && item[i].id <= 10) {
			item[i].slashAtkSpe 	= 2.5;
			item[i].damage 			= 20;
		}
		// Mini rapier
		if (item[i].id == 11) {
			item[i].slashAtkSpe 	= 5;
			item[i].damage 			= 5;
		}
		// Hammer
		if (item[i].id == 12) {
			item[i].slashAtkSpe 	= 5;
			item[i].damage 			= 50;
		}
		// Fat sword
		if (item[i].id == 13) {
			item[i].slashAtkSpe 	= 5;
			item[i].damage 			= 100;
		}
		// Long rapier
		if (item[i].id == 14) {
			item[i].slashAtkSpe 	= 5;
			item[i].damage 			= 10;
		}
		// Gold sword
		if (item[i].id == 15) {
			item[i].slashAtkSpe 	= 5;
			item[i].damage 			= 200;
		}
		// Sivler swords
		if (item[i].id >= 16 && item[i].id <= 18) {
			item[i].slashAtkSpe 	= 5;
			item[i].damage 			= 75;
		}
		// Broken swords
		if (item[i].id >= 19 && item[i].id <= 22) {
			item[i].slashAtkSpe 	= 10;
			item[i].damage 			= 5;
		}
		// Bomb
		if (item[i].id == 23) {
			item[i].slashAtkSpe 	= 5;
			item[i].damage 			= 100;
		}
	}

	// Sizes
	{
		// Set size for Swords when loaded
		if (item[i].id >= 0 && item[i].id <= 10) {
			item[i].w = 10*2;
			item[i].h = 21*2;
		}

		// Set size for wide sword
		if (item[i].id == 13) {
			item[i].w = 10*2;
			item[i].h = 25*2;
		}

		// Long rapier
		if (item[i].id == 14) {
			item[i].w = 8*2;
			item[i].h = 27*2;
		}

		// Gold long sword
		if (item[i].id == 15) {
			item[i].w = 10*2;
			item[i].h = 29*2;
		}

		// Silver sword rapier
		if (item[i].id == 16) {
			item[i].w = 6*2;
			item[i].h = 27*2;
		}

		// Silver sword
		if (item[i].id == 17) {
			item[i].w = 10*2;
			item[i].h = 24*2;
		}

		// Silver sword w/ gold
		if (item[i].id == 18) {
			item[i].w = 10*2;
			item[i].h = 24*2;
		}

		// Broken sword 1
		if (item[i].id == 19) {
			item[i].w = 6*2;
			item[i].h = 9*2;
		}

		// Broken sword 2
		if (item[i].id == 20) {
			item[i].w = 6*2;
			item[i].h = 12*2;
		}

		// Broken sword 3
		if (item[i].id == 21) {
			item[i].w = 6*2;
			item[i].h = 12*2;
		}

		// Broken sword 4
		if (item[i].id == 22) {
			item[i].w = 6*2;
			item[i].h = 12*2;
		}

		// Bomb
		if (item[i].id == 23) {
			item[i].w = 14*2;
			item[i].h = 13*2;
		}

		// Heart
		if (item[i].id == 24) {
			item[i].w = 10*4;
			item[i].h = 10*4;
		}

		// Coin
		if (item[i].id == 25) {
			item[i].w = 10*2;
			item[i].h = 10*2;
		}

		// Silver key
		if (item[i].id == 26) {
			item[i].w = 5*2;
			item[i].h = 10*2;
		}

		// Gold key
		if (item[i].id == 27) {
			item[i].w = 5*2;
			item[i].h = 10*2;
		}

		// Green health key
		if (item[i].id == 28) {
			item[i].w = 8*2;
			item[i].h = 7*2;
		}

		// Set size for Keys when loaded
		if (item[i].id == 26 || item[i].id == 27) {
			item[i].w = 7*2;
			item[i].h = 15*2;
		}
	}

	// Defaults
	item[i].vX 				= 0.0;
	item[i].vY 				= 0.0;
}

int Item::getItemSizeW() {
	return this->rSwords[this->id].w*2;
}

int Item::getItemSizeH() {
	return this->rSwords[this->id].h*2;
}

int Item::getW(Item item[], int i) {

	return item[i].w;
}

int Item::getH(Item item[], int i) {
	return item[i].h;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------------------------------//
//--------------------------------------------- Load Item Data ------------------------------------------------//
// Load data
void Item::LoadData(Item item[], std::fstream &fileTileDataL)
{
	//std::cout << "Attempting to load... Item" << std::endl;
	// Load Item count data
	fileTileDataL >> this->count;
	//std::cout << this->count << std::endl;

	// Load rest of Tilec data
	for (int i = 0; i < this->count; i++) {
		if (!item[i].alive) {
			fileTileDataL >>
			item[i].x 		>>
			item[i].y 		>>
			item[i].w 		>>
			item[i].h 		>>
			item[i].id 		>>
			item[i].alive;

			//std::cout << "Item j: " << j << ", x: " << item[j].x << ", y: " << item[j].y << std::endl;
		}
		//break;
	}

	// Set collision rects for specific Tiles
	for (int i = 0; i < this->count; i++) {
		if (item[i].alive) {

			// Set default parameters
			setStatsBasedOnType(item, i);
		}
	}
}
//--------------------------------------------- Load Item Data ------------------------------------------------//
//-------------------------------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------------------------------//
//--------------------------------------------- Save Item Data ------------------------------------------------//
std::string Item::SaveData(Item item[])
{
	// Save all tile data in a string first
	std::stringstream tempsstb;
	for (int i = 0; i < max; i++) {
		if (item[i].alive) {
			tempsstb << item[i].x 			<< " "
					 << item[i].y 			<< " "
					 << item[i].w  			<< " "
					 << item[i].h  			<< " "
					 << item[i].id  		<< " "
			   	   	 << item[i].alive 		<< "\n";
		}
	}
	std::string temps;
	temps = tempsstb.str().c_str();

	// Commented out because this takes out the extra line thats empty at the end
	//temps = temps.substr(0, temps.size()-1);

	// Save number of tiles in World
	std::stringstream aVeryLongString;
	aVeryLongString << count << "\n";

	// Save the data of all Items
	aVeryLongString << temps.c_str();
	//----------------------------- Save Everything in One File -------------------//
	//-----------------------------------------------------------------------------//
	/////////////////////////////////////////////////////////////////////////////////

	return aVeryLongString.str().c_str();
}
//--------------------------------------------- Save Item Data ------------------------------------------------//
//-------------------------------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
