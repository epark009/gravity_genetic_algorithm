/*
Isosceles.h
Draws an isosceles triangle using SDL, pixel by pixel. Anti-aliased. Can be any size, color, transparency, and position.
Centered around the base of the triangle.
*/

#pragma once

#include "Drawable.h"

class Isosceles: public Drawable
{
private:
	SDL_Point position;
	int height;
	int width;
	double angle;
	SDL_Color color;

protected:
public:
	Isosceles(SDL_Window* window, SDL_Renderer* renderer, int x, int y, int width, int height, double angle, int r, int g, int b, int a);
	Isosceles(SDL_Window* window, SDL_Renderer* renderer, SDL_Point position, int width, int height, double angle, SDL_Color color);
	~Isosceles();
	
	void draw();
};
