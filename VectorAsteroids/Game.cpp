#include "Game.h"

Game::Game() :
	running(false){}

Game::~Game()
{
	OnCleanup();
}

int Game::OnInit()
{
	//Screen Size
	int screenWidth = 800;
	int screenHeight = 600;

	try
	{
		Window::Init(screenWidth, screenHeight, "Vector Asteroids in SDL2 Alpha 03.83");
	}
	catch (const std::runtime_error &error)
	{
		Window::logSDLError(std::cout, "Window OnInit error code: " + std::string(error.what()));
		Window::Quit();
		return APP_FAILED;
	}

	// Success
	return APP_OK;
}

void Game::OnCleanup()
{		
	delete pThePlayer;
	delete pTimer;
	delete pCollision;
	delete pRocks;
	delete pUFOs;
}

int Game::OnExecute()
{
	// Initialize application.
	int status = OnInit();

	if (status != APP_OK)
	{
		return status;
	}
	// Intiate Timer.
	pTimer = new Timer();
	// Intiate game objects here -----------------------
	pThePlayer = new Player();
	pRocks = new RockController();	
	pUFOs = new UFOController();
	pCollision = new Collision(pThePlayer, pRocks, pUFOs);
	//--------------------------------------------------
	
	// Start the Timer.
	pTimer->Start();

	// Enter the SDL event loop.
	SDL_Event event;

	//Get it running.
	running = true;

	//Record ticks this first frame.
	m_LastTickTime = pTimer->Ticks();

	while (running)
	{
		while (SDL_PollEvent(&event))
		{
			OnEvent(&event);
		}

		OnUpdate();
		std::this_thread::yield();
		OnRender();
		std::this_thread::yield();
	}

	return status;
}

void Game::OnUpdate()
{
	// Update your game logic here
	double frame;
	int ticksThisFrame = pTimer->Ticks() - m_LastTickTime;
	frame = (double)ticksThisFrame * 0.001;

	// Game object updates here.
	if (!pTimer->Paused())
	{
		pCollision->Update();
		pThePlayer->Update(&frame);
		pRocks->Update(&frame);
		pUFOs->Update(&frame, &Vector2i(pThePlayer->GetLocation()));
	}
	//Record ticks this frame.
	m_LastTickTime = pTimer->Ticks();
}

void Game::OnRender()
{	
	Window::Clear();
	// Do your drawing here ----------------------------------------------

	// Render Lines here ------------------------------------------------

	pThePlayer->Draw();
	pRocks->Draw();
	pUFOs->Draw();
	pCollision->Draw();

	// Render textures here ----------------------------------------------
	// Render text here -------------------------------------------------
	
	// End your drawing --------------------------------------------------
	Window::Present();
}

void Game::OnEvent(SDL_Event* event)
{
	switch (event->type)
	{
	case SDL_QUIT:
		running = false;
		break;

	case SDL_KEYUP:
		if (event->key.keysym.sym == SDLK_a || event->key.keysym.sym == SDLK_LEFT)
		{
			pThePlayer->SetTurnLeft(false);
		}

		if (event->key.keysym.sym == SDLK_d || event->key.keysym.sym == SDLK_RIGHT)
		{
			pThePlayer->SetTurnRight(false);
		}

		if (event->key.keysym.sym == SDLK_w || event->key.keysym.sym == SDLK_UP)
		{
			pThePlayer->SetThrust(false);
		}
		break;

	case SDL_KEYDOWN:
		if (event->key.keysym.sym == SDLK_ESCAPE)
		{
			running = false;
		}

		if (event->key.keysym.sym == SDLK_a || event->key.keysym.sym == SDLK_LEFT)
		{
			pThePlayer->SetTurnLeft(true);
			pCollision->LeftArrowKey(true);
		}

		if (event->key.keysym.sym == SDLK_d || event->key.keysym.sym == SDLK_RIGHT)
		{
			pThePlayer->SetTurnRight(true);
			pCollision->RightArrowKey(true);
		}

		if (event->key.keysym.sym == SDLK_w || event->key.keysym.sym == SDLK_UP)
		{
			pThePlayer->SetThrust(true);
			pCollision->UpArrowKey(true);
		}

		if (event->key.keysym.sym == SDLK_DOWN)
		{
			pCollision->DownArrowKey(true);
		}

		if (event->key.keysym.sym == SDLK_s || event->key.keysym.sym == SDLK_SPACE || event->key.keysym.sym == SDLK_LCTRL)
		{
			pThePlayer->FireButtonPressed();
			pCollision->FireKey(true);
		}

		if (event->key.keysym.sym == SDLK_RCTRL)
		{
			pThePlayer->HyperSpaceButtonPressed();
		}

		if (event->key.keysym.sym == SDLK_n)
		{
			pCollision->NewGame();
		}

		if (event->key.keysym.sym == SDLK_p)
		{
			if (pTimer->Paused())
			{
				pTimer->Unpause();
				pUFOs->UnpauseTimer();
				pThePlayer->TimerUnpause();
			}
			else
			{
				pTimer->Pause();
				pUFOs->PauseTimer();
				pThePlayer->TimerPause();
			}
		}

	default:
		break;
	}
}
