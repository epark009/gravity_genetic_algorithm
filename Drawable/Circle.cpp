#include "Circle.h"

Circle::Circle(SDL_Window* window, SDL_Renderer* renderer, int x, int y, int radius, int r, int g, int b, int a): Drawable(window, renderer)
{
	position.x = x;
	position.y = y;
	this->radius = radius;
	color.r = r;
	color.g = g;
	color.b = b;
	color.a = a;
}

Circle::Circle(SDL_Window* window, SDL_Renderer* renderer, SDL_Point position, int radius, SDL_Color color): Drawable(window, renderer)
{
	this->position = position;
	this->radius = radius;
	this->color = color;
}

Circle::~Circle()
{
}

void Circle::draw()
{
	int window_w, window_h;
	SDL_GetWindowSize(window, &window_w, &window_h);

	std::vector<SDL_Point> draw_points;

	// get drawing area of circle and don't draw outside the window
	int tlx = position.x - radius;
	tlx = tlx >= 0? tlx : 0;
	int tly = position.y - radius;
	tly = tly >= 0? tly : 0;
	int brx = position.x + radius;
	brx = brx <= window_w? brx : window_w;
	int bry = position.y + radius;
	bry = bry <= window_h? bry : window_h;

	// for each pixel in this area
	for(int x = tlx; x <= brx; x++)
	{
		for(int y = tly; y <= bry; y++)
		{
			// get distance between this pixel and the center of the circle
			int x_length = abs(position.x - x);
			int y_length = abs(position.y - y);
			double r = sqrt(x_length*x_length + y_length*y_length);

			// if this circle is in range (ceilinged)
			if(ceil(r) <= radius+1)
			{
				// draw it, with anti aliasing if i'm at the very edge of the circle and the radius doesn't fully reach this pixel
				int anti_alias = 0;
				if(r > radius)
				{
					anti_alias = floor(double(255) * (r - double(radius)));
				}

				// draw each anti-aliased pixel separately
				if(anti_alias > 0)
				{
					SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a - anti_alias > 0? color.a - anti_alias : 0);
					SDL_RenderDrawPoint(renderer, x, y);
				}
				// non anti-aliased pixels will be drawn all at once using RenderDrawPoints
				else
				{
					SDL_Point p;
					p.x = x;
					p.y = y;
					draw_points.push_back(p);
				}
			}
		}
	}

	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_RenderDrawPoints(renderer, &draw_points[0], draw_points.size());
}
