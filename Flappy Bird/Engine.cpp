#include "Engine.h"

Engine::Engine() {
	// window
	Vector2f resolution;
	resolution.x = VideoMode::getDesktopMode().width;	// 2880
	resolution.y = VideoMode::getDesktopMode().height;	// 1920

	gameWindow.create(VideoMode(resolution.x, resolution.y), "Flappy Bird");
	//gameWindow.create(VideoMode(800, 600), "Flappy Bird", Style::Resize);


	// visuals
	// background
	bg_texture.loadFromFile("flappy_bg3.png");
	bg_sprite.setTexture(bg_texture);
	bg_sprite.setScale(20.f, 20.f);

	// ground
	gnd_texture.loadFromFile("ground.png");
	gnd_sprite1.setTexture(gnd_texture);
	gnd_sprite2.setTexture(gnd_texture);
	gnd_sprite1.setScale(20.f, 20.f);
	gnd_sprite2.setScale(20.f, 20.f);
	gnd_sprite2.setPosition(2880, 0);

	// pipes
	pipe_up_texture.loadFromFile("pipe_up.png");
	pipe_down_texture.loadFromFile("pipe_down.png");
	
	float j = 0;
	for (int i = 0; i < NUM_PIPES; i++) {
		pipes[i].pipe_up_sprite.setTexture(pipe_up_texture);
		pipes[i].pipe_down_sprite.setTexture(pipe_down_texture);
		pipes[i].pipe_up_sprite.setPosition(3000 + j, (rand() % 1500) + 320);
		
		// adjust pipe_down's prop.
		Vector2u pipe_down_dim = pipe_down_texture.getSize();
		pipes[i].pipe_down_sprite.setOrigin(0, pipe_down_dim.y);
		pipes[i].pipe_down_sprite.setPosition(pipes[i].pipe_up_sprite.getPosition().x, pipes[i].pipe_up_sprite.getPosition().y - 250);
		
		// scale
		pipes[i].pipe_up_sprite.setScale(13.f, 13.f);
		pipes[i].pipe_down_sprite.setScale(13.f, 13.f);
		j += 800;

		/*if (pipes[i].pipe_up_sprite.getPosition().x <= 2880)
			pipes[i].is_visible = true;
		else
			pipes[i].is_visible = false;*/
		pipes[i].is_visible = false;
	}

	start_pressed = false;
	is_space_pressed = false;
	is_collision = false;
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
	// update where they are
	Vector2f gnd1_position = gnd_sprite1.getPosition();
	Vector2f gnd2_position = gnd_sprite2.getPosition();

	// ground scrolling logic
	if (gnd1_position.x <= -2880)
		gnd_sprite1.setPosition(2880, 0);

	if (gnd2_position.x <= -2880)
		gnd_sprite2.setPosition(2880, 0);

	gnd_sprite1.move(-X_SCROLL_SPEED * dt_as_sec, 0);
	gnd_sprite2.move(-X_SCROLL_SPEED * dt_as_sec, 0);

	// pipe scrolling logic
	for (int i = 0; i < NUM_PIPES; i++) {
		Vector2f pipe_up_position = pipes[i].pipe_up_sprite.getPosition();
		Vector2f pipe_down_position = pipes[i].pipe_down_sprite.getPosition();
		// left of screen
		if (pipes[i].pipe_up_sprite.getPosition().x < PIPE_LEFT_BOUNDARY) {
			pipes[i].pipe_up_sprite.setPosition(PIPE_RESET_BOUNDARY, pipes[i].pipe_up_sprite.getPosition().y);
			pipes[i].pipe_down_sprite.setPosition(PIPE_RESET_BOUNDARY, pipes[i].pipe_down_sprite.getPosition().y);
			pipes[i].is_visible = false;
		}
		// on screen
		else if (pipes[i].pipe_up_sprite.getPosition().x >= PIPE_LEFT_BOUNDARY && pipes[i].pipe_up_sprite.getPosition().x < 2880) {
			pipes[i].pipe_up_sprite.setPosition(pipe_up_position.x - X_SCROLL_SPEED * dt_as_sec, pipe_up_position.y);
			pipes[i].pipe_down_sprite.setPosition(pipe_down_position.x - X_SCROLL_SPEED * dt_as_sec, pipe_down_position.y);
			pipes[i].is_visible = true;
		}
		// right of screen
		else {
			Vector2f pipe_position = pipes[i].pipe_up_sprite.getPosition();
			pipes[i].pipe_up_sprite.setPosition(pipe_up_position.x - X_SCROLL_SPEED * dt_as_sec, pipe_up_position.y);
			pipes[i].pipe_down_sprite.setPosition(pipe_down_position.x - X_SCROLL_SPEED * dt_as_sec, pipe_down_position.y);
			pipes[i].is_visible = false;
		}
	}

	// update Flappy logic
	Flappy.update(start_pressed, is_space_pressed, dt_as_sec);
}

void Engine::draw() {
	// draw from back to front
	gameWindow.clear(Color::White);
	gameWindow.draw(bg_sprite);
	
	for (int i = 0; i < 6; i++) {
		if (pipes[i].is_visible == true) {
			gameWindow.draw(pipes[i].pipe_up_sprite);
			gameWindow.draw(pipes[i].pipe_down_sprite);
		}
	}
	gameWindow.draw(gnd_sprite1);
	gameWindow.draw(gnd_sprite2);

	gameWindow.draw(Flappy.getSprite());
	gameWindow.display();
}

void Engine::checkCollision() {
	

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
