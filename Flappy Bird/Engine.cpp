#include "Engine.h"

Engine::Engine() {
	// god mode
	god_mode = false;

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
		pipes[i].passed = false;
	}

	textures_list = {	"Sprites/ascii_0.png",
						"Sprites/ascii_1.png",
						"Sprites/ascii_2.png", 
						"Sprites/ascii_3.png", 
						"Sprites/ascii_4.png", 
						"Sprites/ascii_5.png", 
						"Sprites/ascii_6.png", 
						"Sprites/ascii_7.png", 
						"Sprites/ascii_8.png",
						"Sprites/ascii_9.png"	};

	// flags/vars
	start_pressed = false;
	is_space_pressed = false;
	is_collision = false;
	updated_score = false;

	X_Speed = X_SCROLL_SPEED;
	score = 0;
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

	// update closest pipes
	getPipes();

	// ground scrolling logic
	if (gnd1_position.x <= -SCREEN_RIGHT_BOUNDARY)
		gnd_sprite1.setPosition(SCREEN_RIGHT_BOUNDARY, 0);

	if (gnd2_position.x <= -SCREEN_RIGHT_BOUNDARY)
		gnd_sprite2.setPosition(SCREEN_RIGHT_BOUNDARY, 0);

	gnd_sprite1.move(-X_Speed * dt_as_sec, 0);
	gnd_sprite2.move(-X_Speed * dt_as_sec, 0);

	updated_score = false;

	// pipe scrolling logic
	for (int i = 0; i < NUM_PIPES; i++) {
		Vector2f pipe_up_position = pipes[i].pipe_up_sprite.getPosition();
		Vector2f pipe_down_position = pipes[i].pipe_down_sprite.getPosition();
		// left of screen
		if (pipes[i].pipe_up_sprite.getPosition().x < PIPE_LEFT_BOUNDARY) {
			pipes[i].pipe_up_sprite.setPosition(PIPE_RIGHT_BOUNDARY, (rand() % PIPE_UP_FLEXIBILITY) + PIPE_UP_OFFSET);
			pipes[i].pipe_down_sprite.setPosition(PIPE_RIGHT_BOUNDARY, pipes[i].pipe_up_sprite.getPosition().y - PIPE_OPENING);
			pipes[i].is_visible = false;
			pipes[i].passed = false;
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
		if (pipes[i].pipe_up_sprite.getPosition().x < PIPE_LEFT_COLLISION_OFFSET && !pipes[i].passed) {
			score++;
			pipes[i].passed = true;
		}
	}
	// update Flappy logic
	Flappy.update(start_pressed, is_space_pressed, is_collision, dt_as_sec);

	// check collisions
	checkCollision();
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
	if (!god_mode) {
		if (Flappy_position.y >= GROUND) {
			is_collision = true;
			reset();
		}

		// pipes
		for (int i = 0; i < NUM_PIPES; i++) {
			if (Flappy_position.x >= pipes[i].pipe_up_sprite.getPosition().x + PIPE_LEFT_COLLISION_OFFSET && Flappy_position.x <= pipes[i].pipe_up_sprite.getPosition().x + PIPE_RIGHT_COLLISION_OFFSET && Flappy_position.y >= pipes[i].pipe_up_sprite.getPosition().y + PIPE_OPENING_COLLISION_OFFSET ||
				Flappy_position.x >= pipes[i].pipe_down_sprite.getPosition().x + PIPE_LEFT_COLLISION_OFFSET && Flappy_position.x <= pipes[i].pipe_down_sprite.getPosition().x + PIPE_RIGHT_COLLISION_OFFSET && Flappy_position.y <= pipes[i].pipe_down_sprite.getPosition().y - PIPE_OPENING_COLLISION_OFFSET) {
				is_collision = true;
				reset();
			}
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
	// initialize
	int num_digits = 0;
	int divisor = 1;
	int temp = 1;
	int score_temp = 0;
	which_textures.clear();
	score_textures.clear();
	score_sprites.clear();

	// calc sprites need
	if (score == 0) {
		num_digits = 1;
		which_textures.push_back(0);
	}
	else {
		while (temp > 0 || score >= divisor) {
			temp = (score / divisor) % 10;
			// break early
			if (score_temp >= score)
				break;
			num_digits++;
			score_temp += temp * divisor;
			divisor *= 10;
			which_textures.push_back(temp);
		}
	}
	// flip texture vector
	reverse(which_textures.begin(), which_textures.end());

	// set up sprites
	score_textures.resize(num_digits);
	score_sprites.resize(num_digits);
	float score_position = 100;
	for (int i = 0; i < score_sprites.size(); i++) {
		score_textures[i].loadFromFile(textures_list[which_textures[i]]);
		score_sprites[i].setTexture(score_textures[i]);
		score_sprites[i].setScale(10.f, 10.f);
		score_sprites[i].setPosition(score_position, 100);
		score_position += 100;
	}
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

vector<float> Engine::getPipes() {
	// initialize
	int closest_pipe_index = closestPipe();
	vector<float> pipes_vector;

	pipes_vector.push_back(pipes[closest_pipe_index].pipe_down_sprite.getPosition().y);
	pipes_vector.push_back(pipes[closest_pipe_index].pipe_down_sprite.getPosition().x);
		
	pipes_vector.push_back(pipes[closest_pipe_index].pipe_up_sprite.getPosition().y);
	pipes_vector.push_back(pipes[closest_pipe_index].pipe_up_sprite.getPosition().x);

	return pipes_vector;
}

int Engine::closestPipe() {
	// initialize
	int closest_pipe_index = 0;
	float smallest_dist = BIG_NUMBER;
	float cur_dist;
	Vector2f Flappy_position = Flappy.getFlappy_Position();

	for (int i = 0; i < NUM_PIPES; i++) {
		cur_dist = pipes[i].pipe_up_sprite.getPosition().x + 990 - Flappy_position.x;

		// seen closest pipe
		if (cur_dist < smallest_dist && cur_dist >= 0) {
			// update index and smallest dist
			closest_pipe_index = i;
			smallest_dist = cur_dist;
		}
	}
	return closest_pipe_index;
}

/*
decide()
	DESCRIPTION: Decide what action to do based off state, Q-table, and N-table.
	INPUTS:
	RETURN:
	EFFECT:
*/
void Engine::decide() {
	// grab the current state of game
	//tuple<int, int, int, int> state = getState();



}

void Engine::reward() {

}

void Engine::update() {

}




/*
discreteization(a)
	DESCRIPTION: Takes in a float value and converts it into something discrete.
	INPUTS: a -- A float that will be converted through the function.
	RETURN: An integer that resulted from the input's transformation.
	EFFECT: Changed float to int.
*/
void Engine::epsilonGreedy() {

}
int Engine::discretization(float a) {
	/*
		Think of Riemann Sums for binSize. (might be wrong analogy).
			The lower the value, the more divisions there are (a more accurate representation).
			The higher the value, the more crude divisions there are (a less accurate representation).
	*/
	float binSize = 1.0f;
	return static_cast<int>(floor(a / binSize));
}

/*
getActions()
	DESCRIPTION:
	INPUTS:
	RETURN:
	EFFECT:
*/
tuple<int, int, vector<int>> Engine::getState() {
	// initialize
	Vector2f Flappy_position = Flappy.getFlappy_Position();
	vector<float> Pipe_position = getPipes();

	Vector2i Flappy_position_updated;
	vector<int> Pipe_position_updated;
	//Pipe_position_updated.resize(4);

	// convert Flappy
	Flappy_position_updated.x = discretization(Flappy_position.x);
	Flappy_position_updated.y = discretization(Flappy_position.y);

	// convert pipes
	for (int i = 0; i < NUM_PIPES * 4; i++) {
		Pipe_position_updated[i] = discretization(Pipe_position[i]);
	}

	return make_tuple(Flappy_position_updated.x, Flappy_position_updated.y, Pipe_position_updated);
}

//void Engine::reinforcementLearning() {
//	decide();
//	reward();
//	update();
//
//}






















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
		draw();
	}
}


