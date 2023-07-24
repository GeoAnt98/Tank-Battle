#include <GameDev2D.h>
#include "Game.h"


//Function prototypes
void Init();
void Shutdown();

void Update(double delta);
void Draw();

void HandleKeyPress(GameDev2D::Keyboard::Key key);

//The only GLOBAL variable ALLOWED
GameDev2D::Game* game = nullptr;

//Entry point to the application
int WINAPI WinMain(HINSTANCE aCurrentInstance, HINSTANCE aPreviousInstance, LPSTR aCommandLine, int aCommandShow)
{
    //Run GameDev2D, pass in the Init, Shutdown, Update and Draw methods
    GameDev2D::Run(Init, Shutdown, Update, Draw);
    return 0;
}

void Init()
{
	// Register functions for input
	GameDev2D::RegisterKeyPressedCallback(HandleKeyPress);
	// Create the game instance
	game = new GameDev2D::Game();
}

void Shutdown()
{
	if (game != nullptr)
	{
		delete game;
		game = nullptr;
	}
}

void Update(double aDelta)
{
	if (game != nullptr)
	{
		game->Update(aDelta);
	}
}

void Draw()
{
	if (game != nullptr)
	{
		game->Draw();
	}
}

void HandleKeyPress(GameDev2D::Keyboard::Key key)
{
}
