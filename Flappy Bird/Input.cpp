#include "Engine.h"

void Engine::input() {
	// stop game
	if (Keyboard::isKeyPressed(Keyboard::Escape))
		window.close();
	// moving left and right
	if (Keyboard::isKeyPressed(Keyboard::A))
		Flappy.moveLeft();
	else
		Flappy.stopLeft();
	if (Keyboard::isKeyPressed(Keyboard::D))
		Flappy.moveRight();
	else
		Flappy.stopRight();
}
