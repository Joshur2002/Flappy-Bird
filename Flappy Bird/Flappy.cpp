#include "Flappy.h"

Flappy::Flappy() {
	// initialize position & velocity
	Flappy_position.x = 500;
	Flappy_position.y = 500;
	Flappy_velocity.x = 350;
	Flappy_velocity.y = 450;

	// visuals
	Flappy_texture.loadFromFile("flappy.png");
	Flappy_sprite.setTexture(Flappy_texture);
	Flappy_sprite.setScale(.5, .5);

	// flags
	was_prev_space = false;
	//start_key_pressed = false;
}

Sprite Flappy::getSprite() {
	return Flappy_sprite;
}

void Flappy::update(bool start_key_pressed, bool is_space_pressed, float time_elapsed) {
// start key was pressed
  	if (start_key_pressed) {
		// x-axis 
		Flappy_position.x += time_elapsed * Flappy_velocity.x;
		if (Flappy_position.x >= 2770)
			Flappy_position.x = 0;
		

		// y-axis
		if (is_space_pressed) {
			if (was_prev_space) {
				Flappy_position.y += time_elapsed * Flappy_velocity.y;
			}
			else {
				Flappy_position.y -= time_elapsed * Flappy_velocity.y + 200;
				was_prev_space = true;
			}
		}
		else {
			Flappy_position.y += time_elapsed * Flappy_velocity.y;
			was_prev_space = false;
		}
		if (Flappy_position.y >= 1700)
			Flappy_position.y = 1700;
	}

	Flappy_sprite.setPosition(Flappy_position);
}