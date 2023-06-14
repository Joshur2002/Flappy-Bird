#include "Engine.h"

Engine::Engine() {
	// display window
	Vector2f resolution;
	resolution.x = VideoMode::getDesktopMode().width;
	resolution.y = VideoMode::getDesktopMode().height;
	window.create(VideoMode(resolution.x, resolution.y), "Flappy Bird", Style::Resize);

	// visuals
	bg_texture.loadFromFile("flappy_bg.png");
	bg_sprite.setTexture(bg_texture);
	bg_sprite.setScale(3.5, 3.5);
}

void Engine::start() {
	// clock for time
	Clock clock;

	while (window.isOpen()) {
		// reset timer && convert
		Time dt = clock.restart();
		float dt_as_sec = dt.asSeconds();

		/*
			game logic sequence
				check for inputs->update the game->draw the updates onto screen
		*/
		input();
		update(dt_as_sec);
		draw();
	}
}

