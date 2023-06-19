#pragma once
#include "Flappy.h"

#define X_SCROLL_SPEED 350
#define NUM_PIPES 5

class Engine {
private:
	// Render window
	RenderWindow gameWindow;

	// visual dimensions
	Vector2f gnd1_position;
	Vector2f gnd2_position;

	// visuals
	Sprite bg_sprite;
	Sprite gnd_sprite1;
	Sprite gnd_sprite2;
	

	Sprite pipe_body_sprite;
	Sprite pipe_tip_sprite;
	/*struct pipe_t {
		Sprite pipe_body_sprite;
		Sprite pipe_tip_sprite;
		Vector2f pipe_position;
	};
	vector<pipe_t> pipe_sprites;*/

	Texture bg_texture;
	Texture gnd_texture;
	Texture pipe_body_texture;
	Texture pipe_tip_texture;

	// copy of Flappy constructor
	Flappy Flappy;

	// func only for Engine
	void input();
	void update(float dt_as_sec);
	void draw();

	// flag
	bool start_pressed;
	bool is_space_pressed;

public:
	// Constructor
	Engine();

	// func for others too
	void start();
};
