#pragma once
#include "SFML/Graphics.hpp"

using namespace sf;
class Flappy {
private:
	// bird's location & movement
	Vector2f Flappy_position;
	float x_speed;

	// visuals
	Sprite Flappy_sprite;
	Texture Flappy_texture;

	// keyboard flags
	bool is_left_pressed;
	bool is_right_pressed;

public:
	// constructor
	Flappy();

	// visual func
	Sprite getSprite();

	// movement func
	void moveLeft();
	void moveRight();
	void stopLeft();
	void stopRight();

	// screen func
	void update(float time_elapsed);
};
