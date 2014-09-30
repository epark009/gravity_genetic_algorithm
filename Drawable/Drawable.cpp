#include "Drawable.h"

Drawable::Drawable(SDL_Window* window, SDL_Renderer* renderer)
{
	this->window = window;
	this->renderer = renderer;
}

Drawable::~Drawable()
{
}