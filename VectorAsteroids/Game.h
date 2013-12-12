// ============================================================================
// [Include Section]
// ============================================================================
#ifndef Game_H
#define Game_H
#include <thread>
#include <random/mersenne_twister.hpp>
#include <random/uniform_int_distribution.hpp>
#include "Window.h"
#include "Timer.h"
#include "Player.h"
#include "RockController.h"
#include "UFOController.h"
#include "Collision.h"
// ============================================================================
// [Game]
// ============================================================================
class Game
{
public:

	// Application state
	enum APP_STATE
	{
		APP_OK = 0,
		APP_FAILED = 1,
		APP_FAILEDTTF = 2,
		APP_FAILEDTEXTURE = 3
	};

	Game();
	~Game();
	
	//Random Number Generator
	boost::random::mt19937 *random;	
	// Game objects here ---------------------
	Player *pThePlayer = nullptr;
	RockController *pRocks = nullptr;
	UFOController *pUFOs = nullptr;
	Collision *pCollision = nullptr;
	// ---------------------------------------

	// Run game, called by main().
	int OnExecute();

private:
	
	//Our Timer
	Timer *pTimer;

	// Whether the application is running.
	bool 			running;

	// Ticks last cycle/frame
	int m_LastTickTime;

	// Fonts
	//TTF_Font *font;

	// Colors
	SDL_Color fontColor;

	// Initialize application
	int 			OnInit();
	
	// Clean up the application
	void 			OnCleanup();

	// Called to process SDL event.
	void 			OnEvent(SDL_Event* event);

	// Called to update game logic
	void			OnUpdate();
	
	// Called to render the app.
	void 			OnRender();

	//Error logger
	void logSDLError(std::ostream &os, const std::string &msg);
};
#endif
