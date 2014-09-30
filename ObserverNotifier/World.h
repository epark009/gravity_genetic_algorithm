/*
World.h
This contains all the source for the state pattern
*/

#pragma once

#include "ObserverNotifier.h"

// STATES
// state design pattern
// these determines what goes in the view and how the objects in the view interact with each other
class World;
class WorldState: public ObserverNotifier
{
private:
protected:
	struct CIRCLE
	{
		int r;
		SDL_Point pos;
		SDL_Color color;
	};
	
	struct SHIP
	{
		double x;
		double y;
		SDL_Color color;
		double start_angle;
		double angle;
		double start_magnitude;
		double magnitude;
		double length;
		bool crashed;
		double score;
	};

	SDL_Point window_size;
	std::list<CIRCLE> planets;
	unsigned int num_planets;
	std::list<SHIP> ships;
	unsigned int num_ships;
	SDL_Point start;
	double start_angle;
	double gravity_k_modifier;
	double max_initial_velocity;

public:
	WorldState(SDL_Point window_size);
	WorldState(SDL_Point window_size, std::list<ObserverNotifier*> observers, std::list<CIRCLE> planets, std::list<SHIP> ships, SDL_Point start, double start_angle, double gravity_k_modifier, double max_initial_velocity);
	virtual ~WorldState();

	virtual void run(World* world) = 0;
	void draw_planets();
	void draw_start_point();
	void draw_ships();
	void draw_line(int start_x, int start_y, int end_x, int end_y, int r, int g, int b, int a);
};

// the initial state
// return here whenever the simulation restarts
class InitialWorldState: public WorldState
{
private:
	bool valid_planet(CIRCLE p);
	void random_planet_surface(CIRCLE p, SDL_Point& pos, double& angle);

protected:
public:
	InitialWorldState(SDL_Point window_size);
	InitialWorldState(SDL_Point window_size, std::list<ObserverNotifier*> observers);
	InitialWorldState(SDL_Point window_size, std::list<ObserverNotifier*> observers, std::list<CIRCLE> planets, std::list<SHIP> ships, SDL_Point start, double start_angle, double gravity_k_modifier, double max_initial_velocity);
	~InitialWorldState();

	void run(World* world);
};

class SimulatingWorldState: public WorldState
{
private:
	bool restarting;
	unsigned int last_time;
	unsigned int simulation_time;
	int border_limit;

	void do_restart();

protected:
public:
	SimulatingWorldState(SDL_Point window_size);
	SimulatingWorldState(SDL_Point window_size, std::list<ObserverNotifier*> observers, std::list<CIRCLE> planets, std::list<SHIP> ships, SDL_Point start, double start_angle, double gravity_k_modifier, double max_initial_velocity);
	~SimulatingWorldState();

	void run(World* world);
	void notification(msg::MESSAGE message);
};

class GeneticWorldState: public WorldState
{
private:
	double crossover_chance;
	double mutation_chance;

protected:
public:
	GeneticWorldState(SDL_Point window_size);
	GeneticWorldState(SDL_Point window_size, std::list<ObserverNotifier*> observers, std::list<CIRCLE> planets, std::list<SHIP> ships, SDL_Point start, double start_angle, double gravity_k_modifier, double max_initial_velocity);
	~GeneticWorldState();

	void run(World* world);
};

// CLIENT
// the client for the state design pattern
// delegates stuff to whatever the current state is
class World: public ObserverNotifier
{
private:
	WorldState* current_state;
	SDL_Point window_size;

	void do_restart();

protected:
public:
	World(SDL_Point window_size);
	~World();

	void run();
	void set_state(WorldState* new_state);
	void add_observer(ObserverNotifier* observer);
	void notification(msg::MESSAGE message);
};
