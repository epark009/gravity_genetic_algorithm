#include "Line.h"

Line::Line(SDL_Window* window, SDL_Renderer* renderer, int x0, int y0, int x1, int y1, int r, int g, int b, int a): Drawable(window, renderer)
{
	start_point.x = x0;
	start_point.y = y0;
	end_point.x = x1;
	end_point.y = y1;
	color.r = r;
	color.g = g;
	color.b = b;
	color.a = a;
}

Line::Line(SDL_Window* window, SDL_Renderer* renderer, SDL_Point start_point, SDL_Point end_point, SDL_Color color): Drawable(window, renderer)
{
	this->start_point = start_point;
	this->end_point = end_point;
	this->color = color;
}

Line::~Line()
{
}

void Line::draw()
{
	int window_w, window_h;
	SDL_GetWindowSize(window, &window_w, &window_h);

	std::vector<SDL_Point> draw_points;

	// try to get minimal drawing area, so i don't have to draw across the entire window
	int tlx = window_w;
	int tly = window_h;
	int brx = 0;
	int bry = 0;

	if(start_point.x < tlx) tlx = start_point.x;
	if(end_point.x < tlx) tlx = end_point.x;
	if(start_point.y < tly) tly = start_point.y;
	if(end_point.y < tly) tly = end_point.y;
	if(start_point.x > brx) brx = start_point.x;
	if(end_point.x > brx) brx = end_point.x;
	if(start_point.y > bry) bry = start_point.y;
	if(end_point.y > bry) bry = end_point.y;

	// don't draw if it goes off screen either
	if(tlx < 0) tlx = 0;
	if(tly < 0) tly = 0;
	if(brx > window_w) brx = window_w;
	if(bry > window_h) bry = window_h;

	double slope = end_point.x - start_point.x != 0? double(end_point.y - start_point.y) / double(end_point.x - start_point.x) : global::VERTICAL_SLOPE;
	int b = start_point.y - slope * start_point.x;

	// for each pixel in drawing area
	for(int x = tlx; x <= brx; x++)
	{
		for(int y = tly; y <= bry; y++)
		{
			double dist = double(y - slope * x - b) / sqrt(slope*slope + 1);

			if(dist < 0) dist *= -1;
			
			// if distance is within 1 pixel, it's an edge
			if(dist <= 1)
			{
				int anti_alias = 255 * dist;

				SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a - anti_alias);
				SDL_RenderDrawPoint(renderer, x, y);
			}
		}
	}
}
