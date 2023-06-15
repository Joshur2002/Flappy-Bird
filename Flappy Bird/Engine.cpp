#include "Engine.h"

Engine::Engine() {
	// window
	Vector2f resolution;
	resolution.x = VideoMode::getDesktopMode().width;	// 2880
	resolution.y = VideoMode::getDesktopMode().height;	// 1920

	gameWindow.create(VideoMode(resolution.x, resolution.y), "Flappy Bird");
	//gameWindow.create(VideoMode(800, 600), "Flappy Bird", Style::Resize);


	// visuals
	bg_texture.loadFromFile("flappy_bg2.png");
	bg_sprite.setTexture(bg_texture);
	bg_sprite.setScale(20.0, 20.0);

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
	Vector2f resolution;
	/*resolution.x = VideoMode::getDesktopMode().width;
	resolution.y = VideoMode::getDesktopMode().height;
	printf("Game window size %f is while %f is y\n", resolution.x, resolution.y);*/
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
