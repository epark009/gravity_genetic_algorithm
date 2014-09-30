#pragma once

#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

namespace msg
{
	enum MESSAGE
	{
		QUIT,
		DRAW,
		DRAW_TEMP,
		CLEAR_VIEW,
		FULL_CLEAR_VIEW,
		RESTART,
		PAUSE,
		NONE
	};
	
	enum SHAPE
	{
		CIRCLE,
		ISOSCELES,
		LINE
	};
}

namespace global
{
	const double PI = 3.1415926535;
	const double VERTICAL_SLOPE = 999999;
	
	inline void sdl_error(std::ostream &os, std::string msg)
	{
		os << msg << " Error: " << SDL_GetError() << std::endl;
	}

	inline int randrange(int end)
	{
		return rand() % (end + 1);
	}

	inline int randrange(int start, int end)
	{
		return rand() % (end - start + 1) + start;
	}

	inline int round(double value)
	{
		return floor(value + 0.5);
	}
}
