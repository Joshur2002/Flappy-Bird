
#ifndef ENGINE_H
#define ENGINE_H

#pragma once

#include "Flappy.h"
#include <vector>
#include <algorithm>

#include <unordered_map>
//#include <vector>
#include <tuple>
#include <cmath>
#include <functional>

#define X_SCROLL_SPEED					350		// 350 base, 9950 is good for bug testing scoring
#define NUM_PIPES						5
#define PIPE_LEFT_BOUNDARY				-1100
#define PIPE_RIGHT_BOUNDARY				2900	// 3700 for 6 pipes w/ 800, 2100 for 4 pipes w/ 800
#define PIPE_STARTING_POS_X				3000
#define PIPE_OPENING					250
#define PIPE_UP_OFFSET					340
#define PIPE_UP_FLEXIBILITY				1200
#define	PIPE_SPACING					800
#define SCREEN_RIGHT_BOUNDARY			2880
#define PIPE_LEFT_COLLISION_OFFSET		630
#define PIPE_RIGHT_COLLISION_OFFSET		990
#define PIPE_OPENING_COLLISION_OFFSET	40
#define BIG_NUMBER						2147483648 - 1

//class AI

// creates a unique hash for each key:value pair in the unordered map
struct TupleHash {
	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
	size_t operator()(const tuple<T1, T2, T3, T4, T5, T6, T7, T8>& tuple) const {
		size_t seed = 0;
		hash<T1> hasher1;
		hash<T2> hasher2;
		hash<T2> hasher3;
		hash<T2> hasher4;
		hash<T2> hasher5;
		hash<T2> hasher6;
		hash<T2> hasher7;
		hash<T2> hasher8;
		seed ^= hasher1(get<0>(tuple)) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		seed ^= hasher2(get<1>(tuple)) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		seed ^= hasher3(get<2>(tuple)) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		seed ^= hasher4(get<3>(tuple)) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		seed ^= hasher5(get<4>(tuple)) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		seed ^= hasher6(get<5>(tuple)) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		seed ^= hasher7(get<6>(tuple)) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		seed ^= hasher8(get<7>(tuple)) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		return seed;
	}
};

class Engine {
private:
	// copy of constructor
	Flappy Flappy;
	//AI* AI;

	// Render window
	RenderWindow gameWindow;

	// visuals
	Sprite bg_sprite;
	Sprite gnd_sprite1;
	Sprite gnd_sprite2;
	
	struct pipe_t {
		Sprite pipe_up_sprite;		// refer to dir they face; up = thick part up; down = thick part down
		Sprite pipe_down_sprite;
		bool is_visible;
		bool passed;
	};

	pipe_t pipes[6];

	Texture bg_texture;
	Texture gnd_texture;
	Texture pipe_up_texture;
	Texture pipe_down_texture;

	vector<string> textures_list;
	vector<Texture> score_textures;
	vector<Sprite> score_sprites;
	vector<int> which_textures;


	// func only for Engine
	void input();
	void update(float dt_as_sec);
	void draw();
	void checkCollision();
	void scoreboard();
	int closestPipe();
	void reset();

	// flag
	bool start_pressed;
	bool is_space_pressed;
	bool is_collision;
	bool updated_score;
	bool god_mode;

	// vars
	float X_Speed;
	int score;



	/*
		0 -- Flappy_position.x
		1 -- Flappy_position.y
		2 -- Flappy_velocity.y
		3 -- Pipe_Up_position.x
		4 -- Pipe_Up_position.y
		5 -- Pipe_Down_position.x
		6 -- Pipe_Down_position.y
		7 -- Action (flap/nothing)
	*/
	// Q value of that state/action pair
	unordered_map<tuple<int, int, int, int, int, int, int, int>, float, TupleHash> Q_table;
	// number of times that state/action pair has been explored
	unordered_map<tuple<int, int, int, int, int, int, int, int>, int, TupleHash> N_table;

	// main functions
	void decide();

	void reward();

	void action();

	void update();

	// helper functions
	tuple<int, int, vector<int>> getState();

	int discretization(float a);
	void epsilonGreedy();

	// variables
	float alpha;
	float epsilon;




public:
	// Constructor
	Engine();

	// func for others too
	void start();
	vector<float> getPipes();
};

#endif
