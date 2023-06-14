#pragma once
#include "Flappy.h"

class Engine {
private:
	// Render window
	RenderWindow window;

	// visuals
	Sprite bg_sprite;
	Texture bg_texture;

	// copy of Flappy constructor
	Flappy Flappy;

	// func only for Engine
	void input();
	void update(float dt_as_sec);
	void draw();

public:
	// Constructor
	Engine();

	// func for others too
	void start();
};
