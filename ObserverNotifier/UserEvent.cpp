#include "UserEvent.h"

UserEvent::UserEvent(): ObserverNotifier()
{
}

UserEvent::~UserEvent()
{
}

void UserEvent::run()
{
	while(SDL_PollEvent(&e))
	{
		switch(e.type)
		{
		case SDL_QUIT:
			notify(msg::QUIT);
			break;
		case SDL_KEYDOWN:
			SDL_Keycode key_pressed = e.key.keysym.sym;

			switch(key_pressed)
			{
			case SDLK_SPACE:
				notify(msg::RESTART);
				break;
			case SDLK_ESCAPE:
				notify(msg::QUIT);
				break;
			case SDLK_p:
				notify(msg::PAUSE);
				break;
			}
			break;
		}
	}
}
