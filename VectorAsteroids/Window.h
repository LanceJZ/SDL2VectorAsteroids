#ifndef WINDOW_H
#define WINDOW_H
#include <SDL.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdexcept>
#include <string>
#include <sstream>
#include <iostream>
#include <memory>
#include <random>
#include <vmath.h>
#include <time.h>
#include "Common.h"

struct Line
{
	Vector2i LineStart;
	Vector2i LineEnd;
};

struct Color
{
	int Red;
	int Green;
	int Blue;
	int Alpha;
};

class Window
{
public:
	/**
	* Initialize SDL, setup the window and renderer
	* @param title The window title
	*/
	//static void Init(int width, int height, int argc, char **argv, std::string title = "SDL2 Window");
	static void Init(int width, int height,
		std::string title = "SDL2 Window");
	//Quit SDL and destroy the window and renderer
	static void Quit();
	/**
	* Draw a SDL_Texture to the screen at dstRect with various other options
	* @param texture The SDL_Texture to draw
	* @param destination The destination position and width/height to draw the texture with
	* @param clip The clip to apply to the image, if desired
	* @param angle The rotation angle to apply to the texture, default is 0
	* @param xPivot The x coordinate of the pivot, relative to (0, 0) being center of dstRect
	* @param yPivot The y coordinate of the pivot, relative to (0, 0) being center of dstRect
	* @param flip The flip to apply to the image, default is none
	*/
	//static void Draw(SDL_Texture *texture, SDL_Rect &destination, SDL_Rect *clip = nullptr,
		//float angle = 0.0, int xPivot = 0, int yPivot = 0, SDL_RendererFlip flip = SDL_FLIP_NONE);
	/**
	* Loads an image directly to texture using SDL_image's
	* built in function IMG_LoadTexture
	* @param file The image file to load
	* @return SDL_Texture* to the loaded texture
	*/
	//static SDL_Texture* LoadImage(const std::string &file);
	/**
	* Generate a texture containing the message we want to display
	* @param message The message we want to display
	* @param font The font we want to use to render the text
	* @param color The color we want the text to be
	* @return An SDL_Texture* to the rendered message
	*/
	//static SDL_Texture* RenderText(const std::string &message, TTF_Font *font, SDL_Color color);
	/**
	* Generate a texture containing the message we want to display
	* @param fontFile The font we want to use to render the text
	* @param fontSize The size we want the font to be
	* @return An TTF_Font* to the rendered message
	*/
	//static TTF_Font* LoadFont(const std::string &fontFile, int fontSize);
	///Clear the renderer
	static void Clear(void);
	///Present the renderer, ie. update screen
	static void Present(void);
	///Get the window's box
	static SDL_Rect Box(void);
	//Log SDL Error to ostream.
	static void logSDLError(std::ostream &os, const std::string &msg);
	//Draw Line from one point to another of color.
	static void DrawLine(Line *LineLocation, Color *LineColor);
	//Draw a point on the screen of color.
	static void DrawPoint(Vector2i *PointLocation, Color *PointColor);
	//Draw a Rectangle on the screen of color.
	static void DrawRect(const SDL_Rect *RectangleLocation, Color *RectColor);
	//Return Window Size.
	static Vector2i GetWindowSize(void);
	//Random number generator
	static int Random(int Min, int Max);

private:
	static std::unique_ptr<SDL_Window, void(*)(SDL_Window*)> mWindow;
	static std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer*)> mRenderer;
	static SDL_Rect mBox;
};

#endif