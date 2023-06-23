#pragma once
#include "Flappy.h"

#define X_SCROLL_SPEED					350
#define NUM_PIPES						5
#define PIPE_LEFT_BOUNDARY				-1100
#define PIPE_RIGHT_BOUNDARY				2900	// 3700 for 6 pipes w/ 800, 2100 for 4 pipes w/ 800
#define PIPE_STARTING_POS_X				3000
#define PIPE_OPENING					250
#define PIPE_UP_OFFSET					340
#define PIPE_UP_FLEXIBILITY				1200
#define	PIPE_SPACING					800
#define SCREEN_RIGHT_BOUNDARY			2880
#define PIPE_LEFT_COLLISION_OFFSET	630
#define PIPE_RIGHT_COLLISION_OFFSET	990
#define PIPE_OPENING_COLLISION_OFFSET	40

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

	vector<string> textures_list;
	vector<Texture> score_textures;
	vector<Sprite> score_sprites;
	vector<int> which_textures;

	// copy of Flappy constructor
	Flappy Flappy;

	// func only for Engine
	void input();
	void update(float dt_as_sec);
	void draw();
	void checkCollision();
	void scoreboard();
	bool closeEnough(float a, float b);
	void reset();

	// flag
	bool start_pressed;
	bool is_space_pressed;
	bool is_collision;
	bool updated_score;

	// vars
	float X_Speed;
	int score;

public:
	// Constructor
	Engine();

	// func for others too
	void start();
};
