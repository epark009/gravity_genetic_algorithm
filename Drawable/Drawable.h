/*
Drawable.h
An abstract base class for any SDL item that can be drawn on the screen.
*/

#pragma once

#include "../all.h"

class Drawable
{
private:
protected:
	SDL_Window* window;
	SDL_Renderer* renderer;

public:
	Drawable(SDL_Window* window, SDL_Renderer* renderer);
	virtual ~Drawable();

	virtual void draw() = 0;
};
