/* Global Variables */
/*
// Game Window Title
const std::string gameTitle = "Project Rem";

bool editor;
float camx, camy;
int mx, my;
int mouseWheelY;



// Our custom window
LWindow gWindow;

// Window dimensions
int SCREEN_WIDTH = 1280;
int SCREEN_HEIGHT = 720;

// The window renderer
SDL_Renderer* gRenderer = NULL;

// Fonts
TTF_Font *gFont12 = NULL;
TTF_Font *gFont21 = NULL;

// Textures
LTexture gText;






// Initialize SDL and creates window
bool init();

// Loads everything
bool loadMedia();

// Frees media and shuts down SDL
void close();

// Initialize everything
bool init() {
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init( SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	} else {
		//Set texture filtering to linear
		SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1");

		/////////////////////////////
		////////// DRAWING //////////
		//Create window
		gWindow.init(gameTitle.c_str(), SCREEN_WIDTH, SCREEN_HEIGHT);
		//Create vsynced renderer for window
		gRenderer = gWindow.createRenderer();
		//Initialize renderer color
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		////////// DRAWING //////////
		/////////////////////////////

		//Initialize PNG loading
		int imgFlags = IMG_INIT_PNG;
		IMG_Init(imgFlags);
		TTF_Init();

		//SDL_Init(SDL_INIT_EVERYTHING);

		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) < 0) {
			printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n",
			Mix_GetError());
			success = false;
		}
	}
	return success;
}

// Load everything
bool loadMedia() {
	bool success = true;
	gFont12 = TTF_OpenFont("resource/fonts/Viga-Regular.ttf", 12);
	gFont21 = TTF_OpenFont("resource/fonts/Viga-Regular.ttf", 21);
	return success;
}

void render() {

}

// Close everything
void close() {
	TTF_CloseFont(gFont12);
	TTF_CloseFont(gFont21);
	gFont12 = NULL;
	gFont21 = NULL;
	gText.free();

	SDL_DestroyRenderer(gRenderer);
	gWindow.free();

	//Quit SDL subsystems
	Mix_Quit();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}


// Set Colors
SDL_Color black = { 0, 0, 0, 255 };
SDL_Color blue = { 0, 0, 255, 255 };
SDL_Color green = { 0, 255, 0, 255 };
SDL_Color red = { 255, 0, 0, 255 };
SDL_Color white = { 255, 255, 255, 255 };

// Check collision between 2 objects
bool checkCollision(SDL_Rect a, SDL_Rect b){
	bool collide;
	if (a.x+a.w > b.x && a.x < b.x + b.w &&
		a.y+a.h > b.y && a.y < b.y + b.h){
		collide = true;
	}else{
		collide = false;
	}
	return collide;
}

// Object follows another object
void follow(SDL_Rect a, SDL_Rect b) {
	float bmx, bmy;
	bmx  = houseX+houseW-w2-35;
	bmy  = houseY+h2+125;
	float distance = sqrt((bmx - x2-w2/2) * (bmx - x2-w2/2)+
						  (bmy - y2-h2/2) * (bmy - y2-h2/2));

	// Player2
	float vX, vY;
	if (distance > 0.5){
		vX 	= 2 * (10*distance/600) * (bmx - x2-w2/2) / distance;
		vY 	= 2 * (10*distance/600) * (bmy - y2-h2/2) / distance;
	}
	x2 += vX;
	y2 += vY;
}

// Load Player data
void loadData(double &health, double &mana, double &coins){
	std::fstream fileData("data.sav");
	if (fileData.is_open()) {
		fileData >> health >> mana >> coins;
	}
	fileData.close();
}

// Save Player data
void saveData(double health, double mana, double coins){
	std::ofstream fileData;
	std::stringstream tempss;
	tempss << "data.sav";
	fileData.open(tempss.str().c_str());
	fileData << health << "\n"
			 << mana  << "\n"
			 << coins;
	fileData.close();
}

// Render dialog
void renderDialogText(std::string speakerName, std::string speakerText, int x, int y, int w, int h,
					  SDL_Color colorBG, SDL_Color colorBorder,
					  SDL_Color colorBGSpeaker, SDL_Color colorBorderSpeaker,
					  TTF_Font *gFontName, TTF_Font *gFontDialog, Uint32 wrapLength = 0xffffffff){

	// Dialogue rectangle
	SDL_Rect tempr = {x, y, w, h};
		// Dialogue Background
		SDL_SetRenderDrawColor(gRenderer, colorBG.r, colorBG.g, colorBG.b, 255);
		SDL_RenderFillRect(gRenderer, &tempr);
		// Dialogue Border
		SDL_SetRenderDrawColor(gRenderer, colorBorder.r, colorBorder.g, colorBorder.b, 255);
		SDL_RenderDrawRect(gRenderer, &tempr);

	std::stringstream tempss;
		// Dialogue speaker-name
		tempss.str(std::string());
		tempss << speakerName;
		gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), white, gFontName, wrapLength);

		// Dialogue speaker-name rectangle
		SDL_Rect tempRect = {x, y-gText.getHeight()-10, gText.getWidth()+15, gText.getHeight()+8};
			// Dialogue speaker-name Background
			SDL_SetRenderDrawColor(gRenderer, colorBGSpeaker.r, colorBGSpeaker.g, colorBGSpeaker.b, 255);
			SDL_RenderFillRect(gRenderer, &tempRect);
			// Dialogue speaker-name Border
			SDL_SetRenderDrawColor(gRenderer, colorBorderSpeaker.r, colorBorderSpeaker.g, colorBorderSpeaker.b, 255);
			SDL_RenderDrawRect(gRenderer, &tempRect);

		// Render speaker-name
		gText.render(gRenderer, x+5, y-gText.getHeight() - 5, gText.getWidth(), gText.getHeight());

		// Dialogue speaker Text
		tempss.str(std::string());
		tempss << speakerText;
		gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), white, gFontDialog, wrapLength);

		// Render speaker dialogue
		gText.render(gRenderer, x+10, y+10, gText.getWidth(), gText.getHeight());
}


// Render Text
void renderText(float x, float y, std::string text, SDL_Color color, TTF_Font *gFont, std::string align ="l", Uint32 wrapLength = 0xffffffff) {
	gText.loadFromRenderedText(gRenderer, text.c_str(), color, gFont, wrapLength);
	int w = 0;
	int h = 0;
	if (align=="l") {
		w = 0;
	}else if (align=="r") {
		w = gText.getWidth();
	}else if (align=="c") {
		w = gText.getWidth()/2;
		h = gText.getHeight()/2;
	}
	gText.render(gRenderer, x-w, y-h, gText.getWidth(), gText.getHeight());
}

// Render status bar
void renderStatusBar(int x, int y, int w, int h,
					 int health, int healthDecay, int maxHealth,
					 SDL_Color bgColor, SDL_Color decayColor,
					 SDL_Color healthColor, SDL_Color borderColor){

	SDL_Rect healthRect;
	healthRect.x = x;
	healthRect.y = y;
	healthRect.h = h;

	// Healthbar Background
	healthRect.w = ((w*maxHealth)/maxHealth);
	SDL_SetRenderDrawColor(gRenderer, bgColor.r, bgColor.g, bgColor.b, 255);
	SDL_RenderFillRect(gRenderer, &healthRect);

	// Healthbar Decay
	healthRect.w = (w*healthDecay)/maxHealth;
	SDL_SetRenderDrawColor(gRenderer, decayColor.r, decayColor.g, decayColor.b, 255);
	SDL_RenderFillRect(gRenderer, &healthRect);

	// Healthbar current Health
	healthRect.w = ((w*health)/maxHealth);
	SDL_SetRenderDrawColor(gRenderer, healthColor.r, healthColor.g, healthColor.b, 255);
	SDL_RenderFillRect(gRenderer, &healthRect);

	// Healthbar border
	healthRect.w = ((w*maxHealth)/maxHealth);
	SDL_SetRenderDrawColor(gRenderer, borderColor.r, borderColor.g, borderColor.b, 255);
	SDL_RenderDrawRect(gRenderer, &healthRect);
}*/
