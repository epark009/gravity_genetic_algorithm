#include "Isosceles.h"

Isosceles::Isosceles(SDL_Window* window, SDL_Renderer* renderer, int x, int y, int width, int height, double angle, int r, int g, int b, int a): Drawable(window, renderer)
{
	position.x = x;
	position.y = y;
	this->width = width;
	this->height = height;
	this->angle = angle;
	color.r = r;
	color.g = g;
	color.b = b;
	color.a = a;
}

Isosceles::Isosceles(SDL_Window* window, SDL_Renderer* renderer, SDL_Point position, int width, int height, double angle, SDL_Color color): Drawable(window, renderer)
{
	this->position = position;
	this->width = width;
	this->height = height;
	this->angle = angle;
	this->color = color;
}

Isosceles::~Isosceles()
{
}

void Isosceles::draw()
{
	int window_w, window_h;
	SDL_GetWindowSize(window, &window_w, &window_h);

	std::vector<SDL_Point> draw_points;

	double x_u = cos(angle);
	double y_u = sin(angle);
	double px_u = cos(angle + global::PI/2);
	double py_u = sin(angle + global::PI/2);

	// i want pi/2 to point up
	// since sdl uses 0,0 as top left, i gotta reverse y direction to make the angle the way i want
	SDL_Point tip, left, right;
	tip.x = position.x + global::round(x_u * double(height));
	tip.y = position.y - global::round(y_u * double(height));
	left.x = position.x - global::round(px_u * (double(width)/2));
	left.y = position.y + global::round(py_u * (double(width)/2));
	right.x = position.x + global::round(px_u * (double(width)/2));
	right.y = position.y - global::round(py_u * (double(width)/2));

	// try to get minimal drawing area, so i don't have to draw across the entire window
	int tlx = window_w;
	int tly = window_h;
	int brx = 0;
	int bry = 0;

	if(tip.x < tlx) tlx = tip.x;
	if(position.x < tlx) tlx = position.x;
	if(left.x < tlx) tlx = left.x;
	if(right.x < tlx) tlx = right.x;
	if(tip.y < tly) tly = tip.y;
	if(position.y < tly) tly = position.y;
	if(left.y < tly) tly = left.y;
	if(right.y < tly) tly = right.y;
	if(tip.x > brx) brx = tip.x;
	if(position.x > brx) brx = position.x;
	if(left.x > brx) brx = left.x;
	if(right.x > brx) brx = right.x;
	if(tip.y > bry) bry = tip.y;
	if(position.y > bry) bry = position.y;
	if(left.y > bry) bry = left.y;
	if(right.y > bry) bry = right.y;

	// don't draw if it goes off screen either
	if(tlx < 0) tlx = 0;
	if(tly < 0) tly = 0;
	if(brx > window_w) brx = window_w;
	if(bry > window_h) bry = window_h;

	double slope_tl = tip.x - left.x != 0? double(tip.y - left.y) / double(tip.x - left.x) : global::VERTICAL_SLOPE;
	double slope_lr = left.x - right.x != 0? double(left.y - right.y) / double(left.x - right.x) : global::VERTICAL_SLOPE;
	double slope_rt = right.x - tip.x != 0? double(right.y - tip.y) / double(right.x - tip.x) : global::VERTICAL_SLOPE;
	int tl_b = tip.y - slope_tl * tip.x;
	int lr_b = left.y - slope_lr * left.x;
	int rt_b = right.y - slope_rt * right.x;

	// for each pixel in drawing area
	for(int x = tlx; x <= brx; x++)
	{
		for(int y = tly; y <= bry; y++)
		{
			double dist_tl = double(y - slope_tl * x - tl_b) / sqrt(slope_tl*slope_tl + 1);
			double dist_lr = double(y - slope_lr * x - lr_b) / sqrt(slope_lr*slope_lr + 1);
			double dist_rt = double(y - slope_rt * x - rt_b) / sqrt(slope_rt*slope_rt + 1);

			double distance = abs(dist_tl) < abs(dist_lr)? dist_tl : dist_lr;
			distance = abs(distance) < abs(dist_rt)? distance : dist_rt;
			if(distance < 0) distance *= -1;
			
			// if distance is within 1 pixel, it's an edge
			if(distance < 1)
			{
				int anti_alias = 255 * distance;

				SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a - anti_alias);
				SDL_RenderDrawPoint(renderer, x, y);
			}
		}
	}
}
