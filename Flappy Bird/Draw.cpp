#include "Engine.h"

void Engine::draw() {
	// clear screen
	window.clear(Color::White);

	// replace screen with new contents
	window.draw(bg_sprite);
	window.draw(Flappy.getSprite());

	// draw new content onto screen
	window.display();
}
