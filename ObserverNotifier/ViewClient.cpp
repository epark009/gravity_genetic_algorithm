#include "ViewClient.h"

ViewClient::ViewClient(SDL_Window* window, SDL_Renderer* renderer)
{
	this->window = window;
	this->renderer = renderer;
}

ViewClient::~ViewClient()
{
	for(std::list<Drawable*>::iterator i = drawables.begin(); i != drawables.end(); i++)
	{
		delete *i;
	}
	drawables.clear();
	for(std::list<Drawable*>::iterator i = temporary.begin(); i != temporary.end(); i++)
	{
		delete *i;
	}
	temporary.clear();
}

void ViewClient::run()
{
	// clear screen
	SDL_RenderClear(renderer);

	// draw all the stuff in order
	for(std::list<Drawable*>::iterator i = drawables.begin(); i != drawables.end(); i++)
	{
		(*i)->draw();
	}
	for(std::list<Drawable*>::iterator i = temporary.begin(); i != temporary.end(); i++)
	{
		(*i)->draw();
	}

	// set background color
	SDL_SetRenderDrawColor(renderer, 25, 25, 25, 255);
	SDL_RenderPresent(renderer);
}

void ViewClient::notification(msg::MESSAGE message)
{
	switch(message)
	{
	case msg::CLEAR_VIEW:
		do_clear_view();
		break;
	case msg::FULL_CLEAR_VIEW:
		do_full_clear_view();
		break;
	default:
		break;
	}
}

void ViewClient::notification(msg::MESSAGE message, std::vector<void*> args)
{
	switch(message)
	{
	case msg::DRAW:
		do_draw(args, false);
		break;
	case msg::DRAW_TEMP:
		do_draw(args, true);
		break;
	default:
		break;
	}
}

// create a drawable using args from message
void ViewClient::do_draw(std::vector<void*> args, bool temporary)
{
	Drawable* drawable;

	msg::SHAPE* shape = reinterpret_cast<msg::SHAPE*>(args[0]);
	if(*shape == msg::CIRCLE)
	{
		SDL_Point* pos = reinterpret_cast<SDL_Point*>(args[1]);
		int* r = reinterpret_cast<int*>(args[2]);
		SDL_Color* color = reinterpret_cast<SDL_Color*>(args[3]);

		drawable = new Circle(window, renderer, *pos, *r, *color);
	}
	else if(*shape == msg::ISOSCELES)
	{
		SDL_Point* pos = reinterpret_cast<SDL_Point*>(args[1]);
		int* width = reinterpret_cast<int*>(args[2]);
		int* height = reinterpret_cast<int*>(args[3]);
		double* angle = reinterpret_cast<double*>(args[4]);
		SDL_Color* color = reinterpret_cast<SDL_Color*>(args[5]);
		
		drawable = new Isosceles(window, renderer, *pos, *width, *height, *angle, *color);
	}
	else if(*shape == msg::LINE)
	{
		SDL_Point* start_point = reinterpret_cast<SDL_Point*>(args[1]);
		SDL_Point* end_point = reinterpret_cast<SDL_Point*>(args[2]);
		SDL_Color* color = reinterpret_cast<SDL_Color*>(args[3]);

		drawable = new Line(window, renderer, *start_point, *end_point, *color);
	}


	if(temporary)
	{
		this->temporary.push_back(drawable);
	}
	else
	{
		drawables.push_back(drawable);
	}
}

// just clear temporary items
void ViewClient::do_clear_view()
{
	for(std::list<Drawable*>::iterator i = temporary.begin(); i != temporary.end(); i++)
	{
		delete *i;
	}

	temporary.clear();
}

// clear drawable items and text
void ViewClient::do_full_clear_view()
{
	for(std::list<Drawable*>::iterator i = drawables.begin(); i != drawables.end(); i++)
	{
		delete *i;
	}
	for(std::list<Drawable*>::iterator i = temporary.begin(); i != temporary.end(); i++)
	{
		delete *i;
	}

	temporary.clear();
	drawables.clear();
}
