/*
ObserverNotifier.h
This is the abstract base class of the Observer/Notifier pattern.
notify() sends a message to all observers with arguments if any are necessary.
notification() receives the message that was sent by a notifier.
*/

#pragma once

#include "../all.h"

class ObserverNotifier
{
private:
protected:
	std::list<ObserverNotifier*> observers;

public:
	ObserverNotifier();
	virtual ~ObserverNotifier();

	// these aren't pure virtual because some ObserverNotifiers might not need to notify or receive notifications
	virtual void notify(msg::MESSAGE message);
	virtual void notify(msg::MESSAGE message, std::vector<void*> args);
	virtual void notification(msg::MESSAGE message);
	virtual void notification(msg::MESSAGE message, std::vector<void*> args);
	
	void add_observer(ObserverNotifier* observer);
};
