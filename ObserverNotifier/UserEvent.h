/*
UserEvent.h
Captures events from the user then tells any observers about them.
*/

#pragma once

#include "ObserverNotifier.h"

class UserEvent: public ObserverNotifier
{
private:
	SDL_Event e;
	
protected:
public:
	UserEvent();
	~UserEvent();
	
	void run();
};
