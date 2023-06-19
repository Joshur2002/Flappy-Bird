#include "Flappy.h"

Flappy::Flappy() {
	// initialize position & velocity
	Flappy_position.x = 500;
	Flappy_position.y = 500;
	Flappy_velocity.x = 350;
	Flappy_velocity.y = 0;
	deg = 0;

	// visuals
	Flappy_texture.loadFromFile("flappy.png");
	Flappy_sprite.setTexture(Flappy_texture);
	Flappy_sprite.setScale(1.25, 1.25);
	Flappy_sprite.setOrigin((Vector2f)Flappy_texture.getSize() / 2.f);	// centers origin of Flappy

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
		// velocity cap
		if (Flappy_velocity.y >= 1100)
			Flappy_velocity.y = 1100;
		else
			Flappy_velocity.y = Flappy_velocity.y + (GRAVITY * time_elapsed) * 500;

		// deg cap
		if (deg >= 30)
			deg = 30;
		else
			deg = deg + (100 * time_elapsed);

		// x-axis 
		//Flappy_position.x += time_elapsed * Flappy_velocity.x;
		if (Flappy_position.x >= 2770)
			Flappy_position.x = 0;
		
		// y-axis
		if (is_space_pressed) {
			// go down
			if (was_prev_space) {
				Flappy_position.y = Flappy_position.y + (time_elapsed * Flappy_velocity.y) + (.5 * GRAVITY * time_elapsed * time_elapsed);
				deg = deg + (.5 * GRAVITY * time_elapsed * time_elapsed);
			}
			// go up
			else {
				Flappy_velocity.y = -1450 + GRAVITY * time_elapsed;
				Flappy_position.y = Flappy_position.y + (time_elapsed * Flappy_velocity.y) + (.5 * GRAVITY * time_elapsed * time_elapsed);
				deg = -40;
				was_prev_space = true;
			}
		}
		else {
			Flappy_position.y = Flappy_position.y + (time_elapsed * Flappy_velocity.y) + (.5 * GRAVITY * time_elapsed * time_elapsed);
			deg = deg + (.5 * GRAVITY * time_elapsed * time_elapsed);
			was_prev_space = false;
		}
		// floor cap
		if (Flappy_position.y >= 1440)
			Flappy_position.y = 1440;
	}
	Flappy_sprite.setPosition(Flappy_position);
	Flappy_sprite.setRotation(deg);
}