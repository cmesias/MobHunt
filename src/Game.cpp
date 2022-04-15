/* Game Engine by Carl Mesias using 'SDL2 Graphics Library'
 *
 * 4-19-2017
 * Finish Player.h class by also updating input events
 * Add inheritance of LTexture.h into Player.h
 *
 *
 *
 *
 */

#include "Game.h"
#include "SplashScreen.h"
#include "MainMenu.h"
#include "CustomizeCharacter.h"
#include "ActSelection.h"
#include "Credits.h"
#include "Game/Item.h"
#include "PlayGame.h"

#include "stdlib.h"
#include "stdio.h"
#include "time.h"

// Initialize
void Game::Init() {
	if (_gameState != Uninitialized)
		return;

	// Default, load level 1
	LevelToLoad = 1;

	// Load Video CFG
	settings.LoadVideoCFG();

	// Load Audio CFG
	settings.LoadAudioCFG();

	// Initialize Video
	SDL_Init( SDL_INIT_VIDEO);

	// Create window
	//gWindow.create("CEngine V2", RESOLUTION, ANTI_ALIAS, FULLSCREEN, VSYNC | SDL_WINDOW_RESIZABLE );
	gWindow.create(GameName.c_str(),
			settings.getResolutionValue(),
			settings.getAntiAliasValue(),
			settings.getFullscreenValue(),
			settings.getVsyncValue());

	// Determine if we should have vsync on
	Uint32 gRendererFlags = SDL_RENDERER_ACCELERATED;
	if (settings.getVsyncValue()) {
		gRendererFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
	} else {
		gRendererFlags = SDL_RENDERER_ACCELERATED;
	}

	// Create renderer for window
	gRenderer = gWindow.createRenderer(gRendererFlags);
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

	// Set render size
	SDL_RenderSetLogicalSize(gRenderer,screenWidth,screenHeight);

	//Initialize PNG loading
	int imgFlags = IMG_INIT_PNG;
	IMG_Init(imgFlags);
	TTF_Init();

	//SDL_Init(SDL_INIT_EVERYTHING);

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) < 0) {
		printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n",
		Mix_GetError());
	}

	// Set Game-State to ShowSplash()
	_gameState = Game::ShowingSplash;

	//SDL_MaximizeWindow(gWindow.getWindow());
	//SDL_SetWindowFullscreen(gWindow.getWindow(), SDL_TRUE);


}

// Load
void Game::Load() {

	// Load Video CFG
	settings.LoadVideoCFG();

	// Load Audio CFG
	settings.LoadAudioCFG();

	// Other classes Fonts
	LoadFonts();
}

// Free
void Game::Free() {
	// Other classes Fonts
	FreeFonts();
}

// Start Game
void Game::Start() {

	srand(time(0));

	// We set the gameScene to the SplashScreenScene in here
	Init();

	Load();

	// Lock cursor
	//SDL_SetRelativeMouseMode(SDL_TRUE);

	// Hide cursor
	SDL_ShowCursor(false);

	// While loop
	while (!IsExiting()) {
		GameLoop();
	}

	Free();

	// Close upon exit
	SDL_DestroyRenderer(gRenderer);
	gWindow.free();

	//Quit SDL subsystems
	Mix_Quit();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

// Exit Game
bool Game::IsExiting() {
	if (_gameState == Game::Exiting)
		return true;
	else
		return false;
}


// Splash Screen
void Game::ShowSplashScreen() {
	// Create Splash Screen
	SplashScreen splashScreen;

	// Show Main Menu
	SplashScreen::SplashResult result;
	splashScreen.Show(gWindow, gRenderer, result);

	// Do something on Main Menu return
	switch(result)
	{
	// If SplashScreen closes with a "Play" result
	case SplashScreen::Play:					// Play (Next scene is Main Menu)

		// Set _gameState to show main menu
		_gameState = Game::ShowingMenu;
		break;

	// If SplashScreen closes with a "" result
	case SplashScreen::Nothing:

		// Do nothing
		break;

	// If SplashScreen closes with a "Exit" result, close program
	case SplashScreen::Exit:					// Quit
		_gameState = Game::Exiting;
		break;
	}
}

// Splash Screen
void Game::ShowMenu()
{
	// Create Main Menu
	MainMenu mainMenu;

	// Show Main Menu
	MainMenu::MenuResult result;
	mainMenu.Show(gWindow, gRenderer, result);

	// Do something on Main Menu return
	switch(result)
	{

	case MainMenu::NewGame:					// Menu returned 'Play', NewGame()

		// Open customize character scene
		//_gameState = Game::ShowingCustomizeCharacter;

		// Open game play scene (loads level 1 directly)
		//_gameState = Game::ShowingPlayGame;

		// Open Act Selection scene so we can choose which level to play
		_gameState = Game::ShowingActSelection;
		break;

	case MainMenu::HowToPlay:				// Menu returned 'Load', LoadGame()
		//std::cout<< "HowToPlay" << std::endl;
		//_gameState = Game::LoadGameScene;
		break;

	case MainMenu::Options:					// Menu returned 'Options', Options()
		//std::cout<< "Options" << std::endl;
		_gameState = Game::OptionScene;
		break;

	case MainMenu::Credits:					// Menu returned 'Credits', Credits()
		//std::cout<< "Credits" << std::endl;
		_gameState = Game::CreditScene;
		break;

	case MainMenu::Nothing:
		//std::cout<< "Nothing" << std::endl;
		//
		break;

	case MainMenu::Exit:					// Menu returned 'Exit', Quit()
		//std::cout<< "Exit" << std::endl;
		_gameState = Game::Exiting;
		break;
	}
}

/* Customize Character */
void Game::ShowCustomizeCharacter(LWindow &gWindow, SDL_Renderer *gRenderer) {
	// Create Main Menu
	CustomizeCharacter customizeCharacter;
	// Show Main Menu
	CustomizeCharacter::Result result;
	customizeCharacter.Show(gWindow, gRenderer, result);
	// Do something on Main Menu return
	switch(result)
	{
	case CustomizeCharacter::Back:				// Back (to Main Menu)
		_gameState = Game::ShowingMenu;
		break;
	case CustomizeCharacter::StartGame:			// Start Game
		//_gameState = Game::ShowingActSelection;
		_gameState = Game::ShowingPlayGame;
		break;
	case CustomizeCharacter::Nothing:			// Nothing
		//
		break;
	case CustomizeCharacter::Exit:				// Exit Game
		_gameState = Game::Exiting;
		break;
	}
}

/* New Game: Choose Act # to Play */
void Game::ShowActSelectionScreen(LWindow &gWindow, SDL_Renderer *gRenderer) {
	// Create Main Menu
	ActSelection actSelection;

	// Show Main Menu
	ActSelection::Result result;
	actSelection.Show(gWindow, gRenderer, result, LevelToLoad);

	// Do something on Main Menu return
	switch(result)
	{

	// If ActSelection quits and the result is "Back"
	case ActSelection::Back:				// Back (to Main Menu)
		_gameState = Game::ShowingMenu;
		break;

	// If ActSelection quits and the result is "StartGame"
	case ActSelection::StartGame:			// Start Game
		// Open game play scene (loads level 1 directly)
		_gameState = Game::ShowingPlayGame;
		break;

	// If ActSelection quits and the result is "Nothing"
	case ActSelection::Nothing:				// Nothing
		//
		break;

	// If ActSelection quits and the result is "Exit"
	case ActSelection::Exit:				// Exit Game
		_gameState = Game::Exiting;
		break;
	}
}

/* Play Game */
void Game::ShowPlayGame() {
	// Create Main Menu
	PlayGame game1;
	// Show Main Menu
	PlayGame::Result result;
	game1.Show(gWindow, gRenderer, result, LevelToLoad);
	// Do something on Main Menu return
	switch(result)
	{
	case PlayGame::Back:				// Back (to Main Menu)
		_gameState = Game::ShowingMenu;
		break;
	case PlayGame::StartGame:			// Start Game
		//_gameState = Game::PlayGameScene;
		break;
	case PlayGame::Nothing:				// Nothing
		//
		break;
	case PlayGame::Exit:				// Exit Game
		_gameState = Game::Exiting;
		break;
	}
}

void Game::ShowCredits() {
	// Play Room Music
	//Mix_PlayMusic( sRelaxingInterlude, -1);
	// Create Main Menu
	Credits credits;
	// Show Main Menu
	Credits::CreditsResult result;
	credits.Show(gWindow, gRenderer, result);
	// Do something on Main Menu return
	switch(result)
	{
	case Credits::Back:						// Back
		//Mix_HaltMusic();
		_gameState = Game::ShowingMenu;
		break;
	case Credits::StartGame:				// Start a certain scene/level
		//
		break;
	case Credits::Nothing:					// Nothing
		//
		break;
	case Credits::Exit:						// Exit Game
		//Mix_HaltMusic();
		_gameState = Game::Exiting;
		break;
	}
}

// Loop Game
void Game::GameLoop()
{
	switch (_gameState)
	{
		case Game::ShowingSplash:							// Splash Screen
			{
				ShowSplashScreen();
				break;
			}
		case Game::ShowingMenu:								// Menu
			{
				ShowMenu();
				break;
			}
		case Game::ShowingCustomizeCharacter:				// Customize Character
			{
				ShowCustomizeCharacter(gWindow, gRenderer);
				break;
			}
		case Game::ShowingActSelection:						// Play Game
			{
				ShowActSelectionScreen(gWindow, gRenderer);
				break;
			}
		case Game::ShowingPlayGame:							// Play Game
			{
				ShowPlayGame();
				break;
			}
		case Game::CreditScene:							// Play Game
			{
				ShowCredits();
				break;
			}
		case Game::Exiting:									// Quit
			{
				IsExiting();
				break;
			}
	}
}

Game::GameState Game::_gameState = Uninitialized;



