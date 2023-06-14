#include "Flappy.h"

/*
Flappy()
	DESCRIPTION: Flappy's constructor
*/
Flappy::Flappy() {
	// set location & movement
	Flappy_position.x = 100;
	Flappy_position.y = 200;
	x_speed = 200;

	// set visuals
	Flappy_texture.loadFromFile("flappy.png");
	Flappy_sprite.setTexture(Flappy_texture);
	Flappy_sprite.setScale(.5, .5);

	// set flag defaults
	is_left_pressed = false;
	is_right_pressed = false;
}

/* 
getSprite()
	DESCRIPTION: Allows other func to use Flappy's private sprite
*/
Sprite Flappy::getSprite() {
	return Flappy_sprite;
}

/* 
moveLeft
	DESCRIPTION: Sets the moving left flag for Flappy
*/
void Flappy::moveLeft() {
	is_left_pressed = true;
}

/*
moveRight
	DESCRIPTION: Sets the moving right flag for Flappy
*/
void Flappy::moveRight() {
	is_right_pressed = true;
}

/*
stopLeft
	DESCRIPTION: Sets the moving left flag for Flappy
*/
void Flappy::stopLeft() {
	is_left_pressed = false;
}

/*
stopRight
	DESCRIPTION: Sets the moving right flag for Flappy
*/
void Flappy::stopRight() {
	is_right_pressed = false;
}

void Flappy::update(float time_elapsed) {
	if (is_left_pressed)
		Flappy_position.x -= x_speed * time_elapsed;
	if (is_right_pressed)
		Flappy_position.x += x_speed * time_elapsed;

	Flappy_sprite.setPosition(Flappy_position);
}
