#include "ObserverNotifier.h"

ObserverNotifier::ObserverNotifier()
{
}

ObserverNotifier::~ObserverNotifier()
{
	for(std::list<ObserverNotifier*>::iterator i = observers.begin(); i != observers.end(); i++)
	{
		delete *i;
	}
	observers.clear();
}

// sends messages to any observers without arguments
void ObserverNotifier::notify(msg::MESSAGE message)
{
	for(std::list<ObserverNotifier*>::iterator i = observers.begin(); i != observers.end(); i++)
	{
		(*i)->notification(message);
	}
}

// sends messages to any observers with arguments
// arguments are reinterpret casted on the other side
void ObserverNotifier::notify(msg::MESSAGE message, std::vector<void*> args)
{
	for(std::list<ObserverNotifier*>::iterator i = observers.begin(); i != observers.end(); i++)
	{
		(*i)->notification(message, args);
	}
}

// receive message from any notifier without arguments
void ObserverNotifier::notification(msg::MESSAGE message)
{
	switch(message)
	{
	default:
		break;
	}
}

// receive message from any notifier with arguments
// arguments need to be reinterpret casted from void* to its appropriate type
void ObserverNotifier::notification(msg::MESSAGE message, std::vector<void*> args)
{
	switch(message)
	{
	default:
		break;
	}
	args = args; // prevents warnings
}

// add an observer to list
void ObserverNotifier::add_observer(ObserverNotifier* observer)
{
	observers.push_back(observer);
}
