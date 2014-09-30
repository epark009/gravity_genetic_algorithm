/*
Circle.h
Draws a circle using SDL, pixel by pixel. Anti-aliased. Can be any size, color, transparency, and position.
*/

#pragma once

#include "Drawable.h"

class Circle: public Drawable
{
private:
	SDL_Point position;
	int radius;
	SDL_Color color;

protected:
public:
	Circle(SDL_Window* window, SDL_Renderer* renderer, int x, int y, int radius, int r, int g, int b, int a);
	Circle(SDL_Window* window, SDL_Renderer* renderer, SDL_Point position, int radius, SDL_Color color);
	~Circle();
	
	void draw();
};
