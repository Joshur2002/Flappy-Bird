#pragma once
#include "SFML/Graphics.hpp"

using namespace sf;
using namespace std;

// 9.8 was ok
#define GRAVITY 9.8
#define GROUND 1580

class Flappy {
private:
	// bird's position & velocity
	Vector2f Flappy_position;
	Vector2f Flappy_velocity;
	float deg;

	// visuals
	Sprite Flappy_sprite;
	Texture Flappy_texture;

	// flags
	bool was_prev_space;

public:
	// constructor
	Flappy();

	// FLappy func
	Vector2f getFlappy();

	// visual func
	Sprite getSprite();

	// screen func
	void update(bool start_key_pressed, bool is_space_pressed, float time_elapsed);
};
