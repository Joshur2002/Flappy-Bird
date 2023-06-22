#pragma once
#include "Flappy.h"

#define X_SCROLL_SPEED			350
#define NUM_PIPES				5
#define PIPE_LEFT_BOUNDARY		-1100
#define PIPE_RIGHT_BOUNDARY		2900	// 3700 for 6 pipes w/ 800, 2100 for 4 pipes w/ 800
#define PIPE_STARTING_POS_X		3000
#define PIPE_OPENING			250
#define PIPE_UP_OFFSET			340
#define PIPE_UP_FLEXIBILITY		1200
#define	PIPE_SPACING			800
#define SCREEN_RIGHT_BOUNDARY	2880

class Engine {
private:
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
	};

	pipe_t pipes[6];

	Texture bg_texture;
	Texture gnd_texture;
	Texture pipe_up_texture;
	Texture pipe_down_texture;

	// copy of Flappy constructor
	Flappy Flappy;

	// func only for Engine
	void input();
	void update(float dt_as_sec);
	void draw();
	void checkCollision();
	void score();
	void reset();

	// flag
	bool start_pressed;
	bool is_space_pressed;
	bool is_collision;

	// vars
	float X_Speed;

public:
	// Constructor
	Engine();

	// func for others too
	void start();
};
