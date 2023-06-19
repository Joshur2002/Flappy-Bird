#include "Engine.h"

Engine::Engine() {
	// window
	Vector2f resolution;
	resolution.x = VideoMode::getDesktopMode().width;	// 2880
	resolution.y = VideoMode::getDesktopMode().height;	// 1920

	gameWindow.create(VideoMode(resolution.x, resolution.y), "Flappy Bird");
	//gameWindow.create(VideoMode(800, 600), "Flappy Bird", Style::Resize);


	// visuals
	bg_texture.loadFromFile("flappy_bg3.png");
	bg_sprite.setTexture(bg_texture);
	bg_sprite.setScale(20.f, 20.f);

	gnd_texture.loadFromFile("ground.png");
	gnd_sprite1.setTexture(gnd_texture);
	gnd_sprite2.setTexture(gnd_texture);
	gnd_sprite1.setScale(20.f, 20.f);
	gnd_sprite2.setScale(20.f, 20.f);
	gnd_sprite2.setPosition(2880, 0);

	pipe_body_texture.loadFromFile("pipe_body.png");
	pipe_tip_texture.loadFromFile("pipe_tip.png");

	/*pipe_sprites.reserve(NUM_PIPES);	// set vector of fixed size
	for (vector<pipe_t>::iterator iter = pipe_sprites.begin(); iter != pipe_sprites.end(); ++iter) {
		iter->pipe_body_sprite.setTexture(pipe_body_texture);
		iter->pipe_tip_sprite.setTexture(pipe_tip_texture);
		iter->pipe_position.x = 2500;
		iter->pipe_position.y = 500;
	}*/

	pipe_body_texture.setRepeated(true);
	pipe_body_sprite.setTexture(pipe_body_texture);

	float repeatY = static_cast<float>(resolution.y) / pipe_body_texture.getSize().y;
	
	//pipe_body_sprite.setTextureRect(IntRect(0, 0, pipe_body_texture.getSize().x, repeatY));
	pipe_body_sprite.setTextureRect({ 0, 0, 100, 3000 });
	//pipe_body_sprite.setTextureRect(IntRect(0, 0, 100, resolution.y));

	pipe_body_sprite.setScale(10.0, repeatY);
	pipe_body_sprite.setPosition(500, 0);

	

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
	gnd1_position = gnd_sprite1.getPosition();
	gnd2_position = gnd_sprite2.getPosition();
	

	// scrolling logic
	if (gnd1_position.x <= -2880)
		gnd_sprite1.setPosition(2880, 0);

	if (gnd2_position.x <= -2880)
		gnd_sprite2.setPosition(2880, 0);

	gnd_sprite1.move(-X_SCROLL_SPEED * dt_as_sec, 0);
	gnd_sprite2.move(-X_SCROLL_SPEED * dt_as_sec, 0);

	// update Flappy logic
	Flappy.update(start_pressed, is_space_pressed, dt_as_sec);
}

void Engine::draw() {
	gameWindow.clear(Color::White);
	gameWindow.draw(bg_sprite);
	gameWindow.draw(gnd_sprite1);
	gameWindow.draw(gnd_sprite2);

	gameWindow.draw(pipe_body_sprite);


	gameWindow.draw(Flappy.getSprite());
	/*Vector2f resolution;
	resolution.x = VideoMode::getDesktopMode().width;
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
