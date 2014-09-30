/*
UserModel.h
Contains any data about what the user did.
*/

#pragma once

#include "ObserverNotifier.h"

class UserModel: public ObserverNotifier
{
private:
	bool quitting;
	bool pausing;

protected:
public:
	UserModel();
	~UserModel();
	
	void notification(msg::MESSAGE message);

	void quit();
	bool has_quit();
	void pause();
	bool has_paused();
};
