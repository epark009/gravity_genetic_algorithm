#include "World.h"

// ABSTRACT STATE
WorldState::WorldState(SDL_Point window_size): ObserverNotifier()
{
	this->window_size = window_size;
}

WorldState::WorldState(SDL_Point window_size, std::list<ObserverNotifier*> observers, std::list<CIRCLE> planets, std::list<SHIP> ships, SDL_Point start, double start_angle, double gravity_k_modifier, double max_initial_velocity): ObserverNotifier()
{
	this->window_size = window_size;
	this->observers = observers;
	this->planets = planets;
	this->ships = ships;
	this->start = start;
	this->start_angle = start_angle;
	this->gravity_k_modifier = gravity_k_modifier;
	this->max_initial_velocity = max_initial_velocity;
}

WorldState::~WorldState()
{
}

void WorldState::draw_planets()
{
	for(std::list<CIRCLE>::iterator i = planets.begin(); i != planets.end(); i++)
	{
		std::vector<void*> args;
		msg::SHAPE shape = msg::CIRCLE;
		args.push_back(&shape);
		args.push_back(&(i->pos));
		args.push_back(&(i->r));
		args.push_back(&(i->color));
		notify(msg::DRAW, args);
	}
}

void WorldState::draw_start_point()
{
	std::vector<void*> args;
	int tr = 2;
	SDL_Color tc;
	tc.r = 255;
	tc.g = 0;
	tc.b = 0;
	tc.a = 255;
	msg::SHAPE shape = msg::CIRCLE;
	args.push_back(&shape);
	args.push_back(&start);
	args.push_back(&tr);
	args.push_back(&tc);
	notify(msg::DRAW, args);
}

void WorldState::draw_ships()
{
	for(std::list<SHIP>::iterator i = ships.begin(); i != ships.end(); i++)
	{
		std::vector<void*> args;
		int tw = 10;
		int th = i->length;
		msg::SHAPE shape = msg::ISOSCELES;
		SDL_Point p;
		p.x = global::round(i->x);
		p.y = global::round(i->y);
		args.push_back(&shape);
		args.push_back(&p);
		args.push_back(&tw);
		args.push_back(&th);
		args.push_back(&(i->angle));
		args.push_back(&(i->color));
		notify(msg::DRAW_TEMP, args);
	}
}
	
void WorldState::draw_line(int start_x, int start_y, int end_x, int end_y, int r, int g, int b, int a)
{
	std::vector<void*> args;
	msg::SHAPE shape = msg::LINE;
	SDL_Point start_point;
	start_point.x = start_x;
	start_point.y = start_y;
	SDL_Point end_point;
	end_point.x = end_x;
	end_point.y = end_y;
	SDL_Color color;
	color.r = r;
	color.g = g;
	color.b = b;
	color.a = a;
	args.push_back(&shape);
	args.push_back(&start_point);
	args.push_back(&end_point);
	args.push_back(&color);
	notify(msg::DRAW_TEMP, args);
}

// INITIAL STATE
InitialWorldState::InitialWorldState(SDL_Point window_size): WorldState(window_size)
{
	num_planets = 3;
	num_ships = 100;
	gravity_k_modifier = 50; // gravity constant determined by size of planet, this will divide that number
	max_initial_velocity = 10;
}

InitialWorldState::InitialWorldState(SDL_Point window_size, std::list<ObserverNotifier*> observers): WorldState(window_size)
{
	num_planets = 3;
	num_ships = 100;
	this->observers = observers;
	gravity_k_modifier = 50; // gravity constant determined by size of planet, this will divide that number
	max_initial_velocity = 6;
}

InitialWorldState::InitialWorldState(SDL_Point window_size, std::list<ObserverNotifier*> observers, std::list<CIRCLE> planets, std::list<SHIP> ships, SDL_Point start, double start_angle, double gravity_k_modifier, double max_initial_velocity): WorldState(window_size, observers, planets, ships, start, start_angle, gravity_k_modifier, max_initial_velocity)
{
}

InitialWorldState::~InitialWorldState()
{
}

// clear the view, then generate new planets, and set the next state
void InitialWorldState::run(World* world)
{
	// clear screen
	planets.clear();
	notify(msg::FULL_CLEAR_VIEW);

	// generate random planets
	// make sure they don't overlap
	for(unsigned int i = 0; i < num_planets; i++)
	{
		CIRCLE p;
		do
		{
			p.pos.x = global::randrange(window_size.x);
			p.pos.y = global::randrange(window_size.y);
			p.r = global::randrange(10, 100);
		}while(!valid_planet(p));
		p.color.r = 0;
		p.color.g = 255;
		p.color.b = 0;
		p.color.a = 255;
		planets.push_back(p);
	}
	draw_planets();

	// set a random start point on a planet
	CIRCLE start_planet = planets.front();
	do
	{
		random_planet_surface(start_planet, start, start_angle);
	}while(start.x < 0 || start.x >= window_size.x || start.y < 0 || start.y >= window_size.y);
	draw_start_point();

	// generate random ships, start them at the starting point
	for(unsigned int i = 0; i < num_ships; i++)
	{
		double rand_angle = (global::PI/2) * double(global::randrange(1000)) / 1000;
		if(global::randrange(1)) rand_angle *= -1;
		
		SHIP ship;
		ship.x = start.x;
		ship.y = start.y;
		ship.start_angle = start_angle + rand_angle;
		ship.angle = ship.start_angle;
		SDL_Color tc;
		tc.r = global::randrange(255);
		tc.g = global::randrange(255);
		tc.b = global::randrange(255);
		tc.a = 255;
		ship.color = tc;
		ship.start_magnitude = (double(global::randrange(1, 100)) / 100) * max_initial_velocity;
		ship.magnitude = ship.start_magnitude;
		ship.length = 20;
		ship.crashed = false;
		ship.score = 0;
		ships.push_back(ship);
	}
	draw_ships();

	world->set_state(new SimulatingWorldState(window_size, observers, planets, ships, start, start_angle, gravity_k_modifier, max_initial_velocity));
}

// test to make sure current planet isn't overlapping an existing planet
bool InitialWorldState::valid_planet(CIRCLE p)
{
	bool valid = false;

	if(planets.size() == 0)
	{
		valid = true;
	}
	else
	{
		for(std::list<CIRCLE>::iterator i = planets.begin(); i != planets.end(); i++)
		{
			SDL_Point xy_distance;
			xy_distance.x = i->pos.x - p.pos.x;
			xy_distance.y = i->pos.y - p.pos.y;
			double distance = sqrt(xy_distance.x*xy_distance.x + xy_distance.y*xy_distance.y);
			distance -= i->r + p.r;
			if(distance >= 0)
			{
				valid = true;
			}
			else
			{
				valid = false;
				break;
			}
		}
	}

	return valid;
}

// get a random point on a planet's surface
void InitialWorldState::random_planet_surface(CIRCLE p, SDL_Point& pos, double& angle)
{
	// some random angle between 0 and 2pi
	// 1/2000th of a circle should be a small enough angle increment
	angle = (double(global::randrange(2000)) / 1000) * global::PI;
	double x_u = cos(angle);
	double y_u = -sin(angle);
	pos.x = p.pos.x + global::round(p.r * x_u);
	pos.y = p.pos.y + global::round(p.r * y_u);
}

// RUNNING STATE
SimulatingWorldState::SimulatingWorldState(SDL_Point window_size): WorldState(window_size)
{
	restarting = false;
	last_time = SDL_GetTicks();
	simulation_time = 300000;
}

SimulatingWorldState::SimulatingWorldState(SDL_Point window_size, std::list<ObserverNotifier*> observers, std::list<CIRCLE> planets, std::list<SHIP> ships, SDL_Point start, double start_angle, double gravity_k_modifier, double max_initial_velocity): WorldState(window_size, observers, planets, ships, start, start_angle, gravity_k_modifier, max_initial_velocity)
{
	restarting = false;
	last_time = SDL_GetTicks();
	simulation_time = 300000;
}

SimulatingWorldState::~SimulatingWorldState()
{
}

void SimulatingWorldState::run(World* world)
{
	unsigned int current_time = SDL_GetTicks();

	if(restarting)
	{
		world->set_state(new InitialWorldState(window_size, observers));
		return;
	}

	if(last_time + simulation_time < current_time)
	{
		for(std::list<SHIP>::iterator i = ships.begin(); i != ships.end(); i++)
		{
			if(i->score == 0)
			{
				i->score = current_time - last_time;
			}
			else if(i->crashed == true)
			{
				i->score /= 2;
			}
		}

		world->set_state(new GeneticWorldState(window_size, observers, planets, ships, start, start_angle, gravity_k_modifier, max_initial_velocity));
		return;
	}

	notify(msg::CLEAR_VIEW);
	
	bool all_crashed = true;
	// go through each ship
	for(std::list<SHIP>::iterator i = ships.begin(); i != ships.end(); i++)
	{
		all_crashed = all_crashed && i->crashed;

		double gravitation_x = 0;
		double gravitation_y = 0;
		// go through each planet
		for(std::list<CIRCLE>::iterator j = planets.begin(); j != planets.end(); j++)
		{
			// distance between the base of ship and this planet
			double dist_x = j->pos.x - i->x;
			double dist_y = j->pos.y - i->y;
			double distance = sqrt(dist_x*dist_x + dist_y*dist_y);

			// distance between tip of ship and this planet
			double tip_x = double(j->pos.x) - (i->x + i->length * cos(i->angle));
			double tip_y = double(j->pos.y) - (i->y + (-i->length * sin(i->angle)));
			double distance_tip = sqrt(tip_x*tip_x + tip_y*tip_y);

			// allow for some time in the beginning before checking for collisions
			if(last_time + 100 < current_time)
			{
				// set as crashed if any collisions
				if(distance <= j->r || distance_tip <= j->r)
				{
					i->crashed = true;
					i->score = i->score == 0? current_time - last_time : i->score;
				}

				// set as crashed if too far
				if(i->x < -100 || i->x > window_size.x + 100 || i->y < -100 || i->y > window_size.y + 100)
				{
					i->crashed = true;
					i->score = i->score == 0? current_time - last_time : i->score;
				}
			}
			
			if(!i->crashed)
			{
				double gravity_k = double(j->r) / gravity_k_modifier;
				double unit_x = dist_x / distance;
				double unit_y = dist_y / distance;
				// add vectors together for total gravitation vector
				gravitation_x += (unit_x * gravity_k) / distance;
				gravitation_y += (unit_y * gravity_k) / distance;

				// each planetary vector
				//draw_line(i->x, i->y, i->x + ((unit_x * gravity_k) / distance) * 100 * gravity_k_modifier, i->y + ((unit_y * gravity_k) / distance) * 100 * gravity_k_modifier, 255, 255, 255, 255);
			}
		}

		if(!i->crashed)
		{
			// total gravitation vector
			draw_line(i->x, i->y, i->x + gravitation_x * 100 * gravity_k_modifier, i->y + gravitation_y * 100 * gravity_k_modifier, 255, 0, 0, 255);

			// get new magnitude and angle for this ship
			double v_x = i->magnitude * cos(i->angle);
			double v_y = -i->magnitude * sin(i->angle);

			// current direction vector
			//draw_line(i->x, i->y, i->x + (v_x * 100 * gravity_k_modifier), i->y + (v_y * 100 * gravity_k_modifier), 0, 255, 0, 255);

			double new_v_x = v_x + gravitation_x;
			double new_v_y = v_y + gravitation_y;
			double new_h = sqrt(new_v_x*new_v_x + new_v_y*new_v_y);

			double angle_diff = atan2(-new_v_y, new_v_x) - atan2(-v_y, v_x);

			i->magnitude = new_h;
			i->angle += angle_diff;

			// new direction vector
			draw_line(i->x, i->y, i->x + (new_v_x * 100), i->y + (new_v_y * 100), 255, 255, 0, 255);

			i->x += new_v_x;
			i->y += new_v_y;
		}
	}
	if(all_crashed)
	{
		world->set_state(new GeneticWorldState(window_size, observers, planets, ships, start, start_angle, gravity_k_modifier, max_initial_velocity));
		return;
	}
	draw_ships();
}

void SimulatingWorldState::notification(msg::MESSAGE message)
{
	switch(message)
	{
	case msg::RESTART:
		do_restart();
		break;
	default:
		break;
	}
}

void SimulatingWorldState::do_restart()
{
	restarting = true;
}

// RUNNING STATE
GeneticWorldState::GeneticWorldState(SDL_Point window_size): WorldState(window_size)
{
	crossover_chance = 0.8;
	mutation_chance = 0.2;
}

GeneticWorldState::GeneticWorldState(SDL_Point window_size, std::list<ObserverNotifier*> observers, std::list<CIRCLE> planets, std::list<SHIP> ships, SDL_Point start, double start_angle, double gravity_k_modifier, double max_initial_velocity): WorldState(window_size, observers, planets, ships, start, start_angle, gravity_k_modifier, max_initial_velocity)
{
	crossover_chance = 0.8;
	mutation_chance = 0.2;
	std::cout << "========================" << std::endl;
	std::cout << "Genetic Algorithm Report" << std::endl;
	std::cout << "========================" << std::endl;
	std::cout << "Current ships state" << std::endl;
	for(std::list<SHIP>::iterator i = ships.begin(); i != ships.end(); i++)
	{
		std::cout << "Score: " << i->score << std::endl;
	}
	std::cout << "--------------" << std::endl;
}

GeneticWorldState::~GeneticWorldState()
{
}

void GeneticWorldState::run(World* world)
{
	// get max score
	double max_score = 0;
	for(std::list<SHIP>::iterator i = ships.begin(); i != ships.end(); i++)
	{
		if(i->score > max_score)
		{
			max_score = i->score;
		}
	}

	std::list<SHIP> new_ships;

	// crossing and mutating
	for(std::list<SHIP>::iterator i = ships.begin(); i != ships.end(); i++)
	{
		double dice = double(global::randrange(0, 1000)) / 1000;
		if(dice < mutation_chance)
		{
			std::cout << "Mutating... ";
			i->x = start.x;
			i->y = start.y;
			SDL_Color tc;
			tc.r = global::randrange(255);
			tc.g = global::randrange(255);
			tc.b = global::randrange(255);
			tc.a = 255;
			i->color = tc;
			i->length = 20;
			i->crashed = false;
			i->score = 0;
			if(global::randrange(1))
			{
				double rand_angle = (global::PI/2) * double(global::randrange(1000)) / 1000;
				if(global::randrange(1)) rand_angle *= -1;
				i->start_angle = start_angle + rand_angle;
				i->angle = i->start_angle;
				std::cout << "angle... ";
			}
			if(global::randrange(1))
			{
				i->start_magnitude = (double(global::randrange(1, 100)) / 100) * max_initial_velocity;
				i->magnitude = i->start_magnitude;
				std::cout << "magnitude...";
			}
			std::cout << std::endl;
			new_ships.push_back(*i);
			continue;
		}
		dice = double(global::randrange(1000)) / 1000;
		if(dice < crossover_chance)
		{
			std::cout << "Crossing... ";
			SHIP candidate = ships.back();
			for(std::list<SHIP>::iterator j = ships.begin(); j != ships.end(); j++)
			{
				if(j->score != i->score)
				{
					double fitness = j->score / max_score;
					dice = double(global::randrange(1000)) / 1000;
					if(dice < fitness)
					{
						double j_chance = double(j->score) / j->score + candidate.score;
						dice = double(global::randrange(1000)) / 1000;
						if(dice < j_chance) candidate = *j;
					}
				}
			}
			std::cout << "This ship score: " << i->score << " candidate score: " << candidate.score << std::endl;

			SHIP new_ship;
			new_ship.x = start.x;
			new_ship.y = start.y;
			SDL_Color tc;
			tc.r = (i->color.r + candidate.color.r) / 2;
			tc.g = (i->color.g + candidate.color.g) / 2;
			tc.b = (i->color.b + candidate.color.b) / 2;
			tc.a = 255;
			new_ship.color = tc;
			new_ship.length = 20;
			new_ship.crashed = false;
			new_ship.score = 0;
			
			double score_bias = 0.5;
			double candidate_bias = 0.5;
			if(i->score + candidate.score > 0)
			{
				score_bias = i->score / (i->score + candidate.score);
				candidate_bias = candidate.score / (i->score + candidate.score);
			}
			
			new_ship.start_angle = i->start_angle * score_bias + candidate.start_angle * candidate_bias;
			new_ship.angle = new_ship.start_angle;
			new_ship.start_magnitude = i->start_magnitude * score_bias + candidate.start_magnitude * candidate_bias;
			new_ship.magnitude = new_ship.start_magnitude;
			new_ships.push_back(new_ship);
			continue;
		}

		std::cout << "This ship continues to next generation." << std::endl;
		i->x = start.x;
		i->y = start.y;
		i->crashed = false;
		i->score = 0;
		i->angle = i->start_angle;
		i->magnitude = i->start_magnitude;
		new_ships.push_back(*i);
	}

	world->set_state(new SimulatingWorldState(window_size, observers, planets, new_ships, start, start_angle, gravity_k_modifier, max_initial_velocity));
}

// CLIENT
World::World(SDL_Point window_size)
{
	this->window_size = window_size;
	current_state = new InitialWorldState(window_size);
}

World::~World()
{
	delete current_state;
}

// this runs in the program loop
void World::run()
{
	current_state->run(this);
}

// only WorldState classes should call this function
void World::set_state(WorldState* new_state)
{
	delete current_state;
	current_state = new_state;
}

// adds observers to the state, client does not need to know about observers
void World::add_observer(ObserverNotifier* observer)
{
	current_state->add_observer(observer);
}

void World::notification(msg::MESSAGE message)
{
	switch(message)
	{
	case msg::RESTART:
		do_restart();
		break;
	default:
		break;
	}
}

void World::do_restart()
{
	current_state->notification(msg::RESTART);
}
