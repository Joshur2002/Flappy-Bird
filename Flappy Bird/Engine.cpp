#include "Engine.h"

Engine::Engine() {
	// window
	Vector2f resolution;
	resolution.x = VideoMode::getDesktopMode().width;
	resolution.y = VideoMode::getDesktopMode().height;

	gameWindow.create(VideoMode(resolution.x, resolution.y), "Flappy Bird", Style::Resize);

	// visuals
	bg_texture.loadFromFile("flappy_bg.png");
	bg_sprite.setTexture(bg_texture);
	bg_sprite.setScale(3.2, 3.2);

	start_pressed = false;
	is_space_pressed = false;
}

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

void Engine::update(float dt_as_sec) {
	Flappy.update(start_pressed, is_space_pressed, dt_as_sec);
}

void Engine::draw() {
	gameWindow.clear(Color::White);
	gameWindow.draw(bg_sprite);
	gameWindow.draw(Flappy.getSprite());

	gameWindow.display();
}

void Engine::start() {
	Clock clock;
	float dt_in_sec;
	// game running
	while (gameWindow.isOpen()) {
		Time dt = clock.restart();
		dt_in_sec = dt.asSeconds();
		input();
		update(dt_in_sec);
		draw();
	}
}
