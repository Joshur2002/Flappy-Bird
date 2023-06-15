#pragma once
#include "SFML/Graphics.hpp"

using namespace sf;

// 9.8 was ok
#define GRAVITY 9.8

class Flappy {
private:
	// bird's position & velocity
	Vector2f Flappy_position;
	Vector2f Flappy_velocity;

	// visuals
	Sprite Flappy_sprite;
	Texture Flappy_texture;

	// flags
	bool was_prev_space;

public:
	// constructor
	Flappy();

	// visual func
	Sprite getSprite();

	// screen func
	void update(bool start_key_pressed, bool is_space_pressed, float time_elapsed);
};
