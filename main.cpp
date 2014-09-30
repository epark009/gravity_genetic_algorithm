#include "all.h"

#include "ObserverNotifier/ObserverNotifier.h"
#include "ObserverNotifier/World.h"
#include "ObserverNotifier/UserModel.h"
#include "ObserverNotifier/UserEvent.h"
#include "ObserverNotifier/ViewClient.h"

#include "Drawable/Drawable.h"

// SDL initialization boilerplate
bool sdl_init()
{
	if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		global::sdl_error(std::cout, "SDL_Init");
		return false;
	}

	if((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG)
	{
		global::sdl_error(std::cout, "IMG_Init");
		SDL_Quit();
		return false;
	}
	return true;
}

// SDL quit boilerplate
void sdl_quit()
{
	IMG_Quit();
	SDL_Quit();
}

int main()
{
	// setting up SDL
	if(!sdl_init())
	{
		return 1;
	}

	SDL_Point window_size;
	window_size.x = 800;
	window_size.y = 600;

	SDL_Window* window = SDL_CreateWindow("Genetic Algorithm", 100, 100, window_size.x, window_size.y, SDL_WINDOW_SHOWN);
	if(window == nullptr)
	{
		global::sdl_error(std::cout, "SDL_CreateWindow");
		sdl_quit();
		return 1;
	}

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if(renderer == nullptr)
	{
		SDL_DestroyWindow(window);
		global::sdl_error(std::cout, "SDL_CreateRenderer");
		sdl_quit();
		return 1;
	}
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	// set up random seed
	srand(time(0));
	
	// tying MVC classes together
	// model
	World* world = new World(window_size);
	UserModel* user_model = new UserModel();
	
	// view
	ViewClient* view_client = new ViewClient(window, renderer);
	
	// controller
	UserEvent* user_event = new UserEvent();
	
	// tying observernotifiers together
	world->add_observer(view_client);
	user_event->add_observer(user_model);
	user_event->add_observer(world);

	// main program loop
	while(!user_model->has_quit())
	{
		user_event->run();
		view_client->run();
		if(!user_model->has_paused())
		{
			world->run();
		}
	}
	
	sdl_quit();
	return 0;
}
