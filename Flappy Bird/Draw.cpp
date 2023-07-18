
#include "Engine.h"

/*
draw()
	DESCRIPTION: Displays the various game entities onto the game screen.
	INPUTS: n/a
	RETURN: n/a
	EFFECT: Shows objects on the game window.
*/
void Engine::draw() {
	// draw from back to front
	gameWindow.clear(Color::White);

	//background
	gameWindow.draw(bg_sprite);

	// pipes
	for (int i = 0; i < NUM_PIPES; i++) {
		if (pipes[i].is_visible == true) {
			gameWindow.draw(pipes[i].pipe_up_sprite);
			gameWindow.draw(pipes[i].pipe_down_sprite);
		}
	}
	// ground
	gameWindow.draw(gnd_sprite1);
	gameWindow.draw(gnd_sprite2);

	// scoreboard
	scoreboard();
	for (int i = 0; i < score_sprites.size(); i++) {
		gameWindow.draw(score_sprites[i]);
	}

	// Flappy
	gameWindow.draw(Flappy.getSprite());
	gameWindow.display();
}
