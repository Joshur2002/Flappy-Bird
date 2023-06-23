#include "Engine.h"

Engine::Engine() {
	// window
	Vector2f resolution;
	resolution.x = VideoMode::getDesktopMode().width;	// 2880
	resolution.y = VideoMode::getDesktopMode().height;	// 1920

	gameWindow.create(VideoMode(resolution.x, resolution.y), "Flappy Bird");

	// visuals
	// background
	bg_texture.loadFromFile("Sprites/flappy_bg3.png");
	bg_sprite.setTexture(bg_texture);
	bg_sprite.setScale(20.f, 20.f);

	// ground
	gnd_texture.loadFromFile("Sprites/ground.png");
	gnd_sprite1.setTexture(gnd_texture);
	gnd_sprite2.setTexture(gnd_texture);
	gnd_sprite1.setScale(20.f, 20.f);
	gnd_sprite2.setScale(20.f, 20.f);
	gnd_sprite2.setPosition(SCREEN_RIGHT_BOUNDARY, 0);

	// pipes
	pipe_up_texture.loadFromFile("Sprites/pipe_up.png");
	pipe_down_texture.loadFromFile("Sprites/pipe_down.png");
	
	float j = 0;
	for (int i = 0; i < NUM_PIPES; i++) {
		pipes[i].pipe_up_sprite.setTexture(pipe_up_texture);
		pipes[i].pipe_down_sprite.setTexture(pipe_down_texture);
		pipes[i].pipe_up_sprite.setPosition(PIPE_STARTING_POS_X + j, (rand() % PIPE_UP_FLEXIBILITY) + PIPE_UP_OFFSET);
		
		// adjust pipe_down's prop.
		Vector2u pipe_down_dim = pipe_down_texture.getSize();
		pipes[i].pipe_down_sprite.setOrigin(0, pipe_down_dim.y);
		pipes[i].pipe_down_sprite.setPosition(pipes[i].pipe_up_sprite.getPosition().x, pipes[i].pipe_up_sprite.getPosition().y - PIPE_OPENING);
		
		// scale
		pipes[i].pipe_up_sprite.setScale(13.f, 13.f);
		pipes[i].pipe_down_sprite.setScale(13.f, 13.f);
		j += PIPE_SPACING;

		pipes[i].is_visible = false;
	}

	start_pressed = false;
	is_space_pressed = false;
	is_collision = false;

	X_Speed = X_SCROLL_SPEED;
	score = 0;
}

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

/*
update()
	DESCRIPTION: Updates variables and their properites based off time elapsed.
	INPUTS: dt_as_sec -- A float that acts as change in time (dt).
	RETURN: n/a
	EFFECT: Updates the variables' properties, such as position. movement, and visibility.
*/
void Engine::update(float dt_as_sec) {
	// update where they are
	Vector2f gnd1_position = gnd_sprite1.getPosition();
	Vector2f gnd2_position = gnd_sprite2.getPosition();
	Vector2f Flappy_position = Flappy.getFlappy_Position();

	// ground scrolling logic
	if (gnd1_position.x <= -SCREEN_RIGHT_BOUNDARY)
		gnd_sprite1.setPosition(SCREEN_RIGHT_BOUNDARY, 0);

	if (gnd2_position.x <= -SCREEN_RIGHT_BOUNDARY)
		gnd_sprite2.setPosition(SCREEN_RIGHT_BOUNDARY, 0);

	gnd_sprite1.move(-X_Speed * dt_as_sec, 0);
	gnd_sprite2.move(-X_Speed * dt_as_sec, 0);

	// pipe scrolling logic
	for (int i = 0; i < NUM_PIPES; i++) {
		Vector2f pipe_up_position = pipes[i].pipe_up_sprite.getPosition();
		Vector2f pipe_down_position = pipes[i].pipe_down_sprite.getPosition();
		// left of screen
		if (pipes[i].pipe_up_sprite.getPosition().x < PIPE_LEFT_BOUNDARY) {
			pipes[i].pipe_up_sprite.setPosition(PIPE_RIGHT_BOUNDARY, (rand() % PIPE_UP_FLEXIBILITY) + PIPE_UP_OFFSET);
			pipes[i].pipe_down_sprite.setPosition(PIPE_RIGHT_BOUNDARY, pipes[i].pipe_up_sprite.getPosition().y - PIPE_OPENING);
			pipes[i].is_visible = false;
		}
		// on screen
		else if (pipes[i].pipe_up_sprite.getPosition().x >= PIPE_LEFT_BOUNDARY && pipes[i].pipe_up_sprite.getPosition().x < SCREEN_RIGHT_BOUNDARY) {
			pipes[i].pipe_up_sprite.setPosition(pipe_up_position.x - X_Speed * dt_as_sec, pipe_up_position.y);
			pipes[i].pipe_down_sprite.setPosition(pipe_down_position.x - X_Speed * dt_as_sec, pipe_down_position.y);
			pipes[i].is_visible = true;
		}
		// right of screen
		else {
			Vector2f pipe_position = pipes[i].pipe_up_sprite.getPosition();
			pipes[i].pipe_up_sprite.setPosition(pipe_up_position.x - X_Speed * dt_as_sec, pipe_up_position.y);
			pipes[i].pipe_down_sprite.setPosition(pipe_down_position.x - X_Speed * dt_as_sec, pipe_down_position.y);
			pipes[i].is_visible = false;
		}
		// score pipe logic
		/*if (Flappy_position.x == pipes[i].pipe_up_sprite.getPosition().x && Flappy_position.y < pipes[i].pipe_up_sprite.getPosition().y) {
			score++;
			reset();
		}*/
	}



	// update Flappy logic
	Flappy.update(start_pressed, is_space_pressed, is_collision, dt_as_sec);
}

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
	gameWindow.draw(bg_sprite);
	
	for (int i = 0; i < NUM_PIPES; i++) {
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

/*
checkCollision()
	DESCRIPTION:
	INPUTS:
	RETURN:
	EFFECT:
*/
void Engine::checkCollision() {
	// initialize
	Vector2f Flappy_position = Flappy.getFlappy_Position();
	
	// ground
	if (Flappy_position.y >= GROUND) {
		is_collision = true;
		reset();
	}

	// pipes
	for (int i = 0; i < NUM_PIPES; i++) {
		if (Flappy_position.x >= pipes[i].pipe_up_sprite.getPosition().x + PIPE_LEFT_COLLISION_OFFSET && Flappy_position.x <= pipes[i].pipe_up_sprite.getPosition().x + PIPE_RIGHT_COLLISION_OFFSET && Flappy_position.y >= pipes[i].pipe_up_sprite.getPosition().y + PIPE_OPENING_COLLISION_OFFSET ||
			Flappy_position.x >= pipes[i].pipe_down_sprite.getPosition().x + PIPE_LEFT_COLLISION_OFFSET && Flappy_position.x <= pipes[i].pipe_down_sprite.getPosition().x + PIPE_RIGHT_COLLISION_OFFSET && Flappy_position.y <= pipes[i].pipe_down_sprite.getPosition().y - PIPE_OPENING_COLLISION_OFFSET){
			is_collision = true;
			reset();
		}
	}

}

/*
score()
	DESCRIPTION:
	INPUTS:
	RETURN:
	EFFECT:
*/
void Engine::scoreboard() {
	//1234567++1234567;

}

/*
reset()
	DESCRIPTION:
	INPUTS:
	RETURN:
	EFFECT:
*/
void Engine::reset() {
	if (is_collision = true) {
		// reset pipes
		float j = 0;
		for (int i = 0; i < NUM_PIPES; i++) {
			pipes[i].pipe_up_sprite.setPosition(PIPE_STARTING_POS_X + j, (rand() % PIPE_UP_FLEXIBILITY) + PIPE_UP_OFFSET);
			pipes[i].pipe_down_sprite.setPosition(pipes[i].pipe_up_sprite.getPosition().x, pipes[i].pipe_up_sprite.getPosition().y - PIPE_OPENING);
			j += PIPE_SPACING;

			pipes[i].is_visible = false;
		}
		// reset Flappy
		Flappy.update(start_pressed, is_space_pressed, is_collision, .001);

		// reset score
		score = 0;

		// reset flags
		is_collision = false;
	}
}

/*
start()
	DESCRIPTION:
	INPUTS:
	RETURN:
	EFFECT:
*/
void Engine::start() {
	Clock clock;
	float dt_in_sec;
	// game running
	while (gameWindow.isOpen()) {
		Time dt = clock.restart();
		dt_in_sec = dt.asSeconds();
		input();
		update(dt_in_sec);
		checkCollision();
		draw();
	}
}
