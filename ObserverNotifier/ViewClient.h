/*
ViewClient.h
Contains any drawable objects and draws all of them onto the screen.
*/

#pragma once

#include "ObserverNotifier.h"
#include "../Drawable/Drawable.h"
#include "../Drawable/Circle.h"
#include "../Drawable/Isosceles.h"
#include "../Drawable/Line.h"

class ViewClient: public ObserverNotifier
{
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	std::list<Drawable*> drawables;
	std::list<Drawable*> temporary;

	void do_draw(std::vector<void*> args, bool temporary);
	void do_clear_view();
	void do_full_clear_view();

protected:
public:
	ViewClient(SDL_Window* window, SDL_Renderer* renderer);
	~ViewClient();
	
	void run();
	void notification(msg::MESSAGE message);
	void notification(msg::MESSAGE message, std::vector<void*> args);
};
