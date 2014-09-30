#include "UserModel.h"

UserModel::UserModel(): ObserverNotifier()
{
	quitting = false;
	pausing = false;
}

UserModel::~UserModel()
{
}

void UserModel::notification(msg::MESSAGE message)
{
	switch(message)
	{
	case msg::QUIT:
		quit();
		break;
	case msg::PAUSE:
		pause();
		break;
	default:
		break;
	}
}

void UserModel::quit()
{
	quitting = true;
}

bool UserModel::has_quit()
{
	return quitting;
}

void UserModel::pause()
{
	pausing = !pausing;
}

bool UserModel::has_paused()
{
	return pausing;
}