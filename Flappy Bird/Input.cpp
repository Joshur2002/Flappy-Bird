
#include "Engine.h"

/*
input()
	DESCRIPTION: Takes input from the kyeboard and raises the approriate flags in response.
	INPUTS: n/a
	RETURN: n/a
	EFFECT: Sends signals that affect many functions, especially the update function for Flappy.
*/
void Engine::input() {
	// exit
	if (Keyboard::isKeyPressed(Keyboard::Escape))
		gameWindow.close();

	// jump
	if (Keyboard::isKeyPressed(Keyboard::Space)) {
		start_pressed = true;
		is_space_pressed = true;
	}
	else
		is_space_pressed = false;
}
