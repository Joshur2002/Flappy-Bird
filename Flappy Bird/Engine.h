#pragma once
#include "Flappy.h"

#define X_SCROLL_SPEED 350
#define NUM_PIPES 5
#define PIPE_LEFT_BOUNDARY -1100
#define PIPE_RESET_BOUNDARY 2900	// 3700 for 6 pipes w/ 800, 2100 for 4 pipes w/ 800

class Engine {
private:
	// Render window
	RenderWindow gameWindow;

	// visual dimensions
	/*Vector2f gnd1_position;
	Vector2f gnd2_position;*/

	// visuals
	Sprite bg_sprite;
	Sprite gnd_sprite1;
	Sprite gnd_sprite2;
	/*Sprite pipe_up_sprite;
	Sprite pipe_down_sprite;*/
	
	struct pipe_t {
		Sprite pipe_up_sprite;		// refer to dir they face; up = thick part up; down = thick part down
		Sprite pipe_down_sprite;
		/*Vector2f pipe_up_position;
		Vector2f pipe_down_position;*/
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

	// flag
	bool start_pressed;
	bool is_space_pressed;
	bool is_collision;

public:
	// Constructor
	Engine();

	// func for others too
	void start();
};
