/*
Line.h
Draws an anti-aliased line, pixel by pixel
*/

#pragma once

#include "Drawable.h"

class Line: public Drawable
{
private:
	SDL_Point start_point;
	SDL_Point end_point;
	SDL_Color color;

protected:
public:
	Line(SDL_Window* window, SDL_Renderer* renderer, int x0, int y0, int x1, int y1, int r, int g, int b, int a);
	Line(SDL_Window* window, SDL_Renderer* renderer, SDL_Point start_point, SDL_Point end_point, SDL_Color color);
	~Line();
	
	void draw();
};
