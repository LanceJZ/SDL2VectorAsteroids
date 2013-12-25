#include "Window.h"

//Initialize the unique_ptr's deleters here
std::unique_ptr<SDL_Window, void(*)(SDL_Window*)> Window::mWindow
= std::unique_ptr<SDL_Window, void(*)(SDL_Window*)>(nullptr, SDL_DestroyWindow);

std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer*)> Window::mRenderer
= std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer*)>(nullptr, SDL_DestroyRenderer);
//Other static members
SDL_Rect Window::mBox;

//Random Number Generator
std::mt19937 m_Random;

void Window::Init(int width, int height, std::string title)
{
	// Intiate Random Number Generator	
	m_Random.seed(time(0));

	//initialize all SDL subsystems
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
		throw std::runtime_error("SDL Init Failed");
	//if (TTF_Init() == -1)
		//throw std::runtime_error("TTF Init Failed");

	// -- For Music --
	/* We're going to be requesting certain things from our audio
	device, so we set them up beforehand */
	int audio_rate = 22050;
	Uint16 audio_format = AUDIO_S16; /* 16-bit stereo */
	int audio_channels = 2;
	int audio_buffers = 4096;

	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

	/* This is where we open up our audio device.  Mix_OpenAudio takes
	as its parameters the audio format we'd /like/ to have. */
	if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers))
		throw std::runtime_error("Unable to open audio!\n");

	/* If we actually care about what we got, we can ask here.  In this
	program we don't, but I'm showing the function call here anyway
	in case we'd want to know later. */
	Mix_QuerySpec(&audio_rate, &audio_format, &audio_channels);
	/* Every sound that gets played is assigned to a channel.  Note that
	this is different from the number of channels you request when you
	open the audio device; a channel in SDL_mixer holds information
	about a sound sample that is playing, while the number of channels
	you request when opening the device is dependant on what sort of
	sound you want (1 channel = mono, 2 = stereo, etc) */

	//-- For Sounds --
	/* Mix_Chunk is like Mix_Music, only it's for ordinary sounds. */

	//Setup our window size
	mBox.x = 0;
	mBox.y = 0;
	mBox.w = width;
	mBox.h = height;

	/* Create the windows and initialize the renderers */
	//Create our window
	mWindow.reset(SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, mBox.w, mBox.h, SDL_WINDOW_SHOWN));

	//Make sure it created ok
	if (mWindow == nullptr)
		throw std::runtime_error("Failed to create window");

	mRenderer.reset(SDL_CreateRenderer(mWindow.get(), -1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));

	//Make sure it created ok
	if (mRenderer == nullptr)
		throw std::runtime_error("Failed to create renderer");

	Clear();
	SDL_SetRenderDrawBlendMode(mRenderer.get(), SDL_BLENDMODE_NONE);
	SDL_RenderFillRect(mRenderer.get(), NULL);
}

void Window::Quit()
{
	//TTF_Quit();
	Mix_CloseAudio();
	SDL_Quit();
}

//void Window::Draw(SDL_Texture *texture, SDL_Rect &destination, SDL_Rect *clip,
//	float angle, int xPivot, int yPivot, SDL_RendererFlip flip)
//{
//	//Convert pivot pos from relative to object's center to screen space
//	xPivot += destination.w / 2;
//	yPivot += destination.h / 2;
//	//SDL expects an SDL_Point as the pivot location
//	SDL_Point pivot = { xPivot, yPivot };
//	//Draw the texture
//	SDL_RenderCopyEx(mRenderer.get(), texture, clip, &destination, angle, &pivot, flip);
//}

void Window::logSDLError(std::ostream &os, const std::string &msg)
{
	os << msg << " error: " << std::endl << SDL_GetError() << std::endl;
	SDL_Delay(666);
	SDL_Delay(666);
	SDL_Delay(666);
}

//SDL_Texture* Window::LoadImage(const std::string &file)
//{
//	SDL_Texture *texture = nullptr;
//	//Load the image
//	texture = IMG_LoadTexture(mRenderer.get(), file.c_str());
//	//Make sure file loaded.
//	if (texture == nullptr)
//	{
//		logSDLError(std::cout, "LoadTexture");
//	}
//
//	return texture;
//}

//SDL_Texture* Window::RenderText(const std::string &message, TTF_Font *font, SDL_Color color)
//{
//	//Render the message to an SDL_Surface, as that's what TTF_RenderText_X returns
//	SDL_Surface *surf = TTF_RenderText_Blended(font, message.c_str(), color);
//	SDL_Texture *texture = SDL_CreateTextureFromSurface(mRenderer.get(), surf);
//	//Clean up unneeded stuff
//	SDL_FreeSurface(surf);	
//
//	return texture;
//}
//
//TTF_Font* Window::LoadFont(const std::string &fontFile, int fontSize)
//{
//	//Open the font
//	TTF_Font *font = nullptr;
//	font = TTF_OpenFont(fontFile.c_str(), fontSize);
//
//	if (font == nullptr)
//	{
//		throw std::runtime_error("Failed to load font: " + fontFile + TTF_GetError());
//		logSDLError(std::cout, "Failed to load font: " + fontFile + TTF_GetError());
//	}
//
//	return font;
//}

void Window::Clear()
{
	SDL_SetRenderDrawColor(mRenderer.get(), 0, 0, 0, 255);
	SDL_RenderClear(mRenderer.get());
}

void Window::Present()
{
	SDL_RenderPresent(mRenderer.get());
}

SDL_Rect Window::Box()
{
	//Update mBox to match the current window size
	SDL_GetWindowSize(mWindow.get(), &mBox.w, &mBox.h);
	return mBox;
}

void Window::DrawLine(Line *LineLocation, Color *LineColor)
{	
	SDL_SetRenderDrawColor(mRenderer.get(), LineColor->Red, LineColor->Green, LineColor->Blue, LineColor->Alpha);
	SDL_RenderDrawLine(mRenderer.get(), LineLocation->LineStart.x, LineLocation->LineStart.y,
		LineLocation->LineEnd.x, LineLocation->LineEnd.y);
}

void Window::DrawPoint(Vector2i *Location, Color *PointColor)
{
	SDL_SetRenderDrawColor(mRenderer.get(), PointColor->Red, PointColor->Green, PointColor->Blue, PointColor->Alpha);
	SDL_RenderDrawPoint(mRenderer.get(), Location->x, Location->y);
}

void Window::DrawRect(const SDL_Rect *RectangleLocation, Color *RectangleColor)
{
	SDL_SetRenderDrawColor(mRenderer.get(), RectangleColor->Red, RectangleColor->Green, RectangleColor->Blue, RectangleColor->Alpha);
	SDL_RenderDrawRect(mRenderer.get(), RectangleLocation);
}

Vector2i Window::GetWindowSize()
{
	Vector2i WindowSize;

	SDL_GetWindowSize(mWindow.get(), &WindowSize.x, &WindowSize.y);
	
	return WindowSize;
}

int Window::Random(int Min, int Max)
{
	std::uniform_int_distribution<uint32_t> roll(Min, Max);
	return roll(m_Random);
}
