#include "Engine.h"

Engine::Engine() : gen(rd()) {
	// debug/player modes
	god_mode = false;
	ai_mode = true;

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
		int random_d = getRandomInt(0, 5000);
		pipes[i].pipe_up_sprite.setTexture(pipe_up_texture);
		pipes[i].pipe_down_sprite.setTexture(pipe_down_texture);
		pipes[i].pipe_up_sprite.setPosition(PIPE_STARTING_POS_X + j, (random_d % PIPE_UP_FLEXIBILITY) + PIPE_UP_OFFSET);
		
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
	score_old = 0;

	epsilon = 0.80;				//	.80
	decay_factor = .90;			//	.95
	discount_factor = .80;		//	.90
	learning_rate = .20;		//	.10
	n_first_times = 1;			//	10
	algorithm_freq = .25;
	
	biggest_Q_value = 0.f;

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
		int random_d = getRandomInt(0, 5000);

		// left of screen
		if (pipes[i].pipe_up_sprite.getPosition().x < PIPE_LEFT_BOUNDARY) {
			pipes[i].pipe_up_sprite.setPosition(PIPE_RIGHT_BOUNDARY, (random_d % PIPE_UP_FLEXIBILITY) + PIPE_UP_OFFSET);
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

	// check reset
	if (is_collision)
		reset();
}

/*
checkCollision()
	DESCRIPTION: If god_mode is off, it will check if Flappy has collided with any of the pipes. If it did, it will switch a flag to high.
	INPUTS: n/a
	RETURN: n/a
	EFFECT: Switches a collision flag to high that will affect update and reset.
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
				//reset();
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
			int random_d = getRandomInt(0, 5000);
			pipes[i].pipe_up_sprite.setPosition(PIPE_STARTING_POS_X + j, (random_d % PIPE_UP_FLEXIBILITY) + PIPE_UP_OFFSET);
			pipes[i].pipe_down_sprite.setPosition(pipes[i].pipe_up_sprite.getPosition().x, pipes[i].pipe_up_sprite.getPosition().y - PIPE_OPENING);
			j += PIPE_SPACING;

			pipes[i].is_visible = false;
			pipes[i].passed = false;
		}
		// reset Flappy
		Flappy.update(start_pressed, is_space_pressed, is_collision, .001);

		// reset score
		score = 0;
		score_old = 0;

		// reset flags
		is_collision = false;
	}
}

/*
getPipes()
	DESCRIPTION: Grabs information about the pipe closest to the front of Flappy.
	INPUTS:	n/a
	RETURN:	Vector<float> that contains the pipes' x and y values.
	EFFECT: Makes new vector to be used.
*/
vector<float> Engine::getPipes() {
	// initialize
	int closest_pipe_index = closestPipe();
	vector<float> pipes_vector;

	pipes_vector.push_back(pipes[closest_pipe_index].pipe_up_sprite.getPosition().x);
	pipes_vector.push_back(pipes[closest_pipe_index].pipe_up_sprite.getPosition().y);

	pipes_vector.push_back(pipes[closest_pipe_index].pipe_down_sprite.getPosition().x);
	pipes_vector.push_back(pipes[closest_pipe_index].pipe_down_sprite.getPosition().y);

	return pipes_vector;
}

/*
closestPipe()
	DESCRIPTION: Iterates through all pipes and finds the index of the closest pipe to the front of Flappy.
	INPUTS:	n/a
	RETURN:	Index value of the pipe.
	EFFECT:	Finds the index of closest pipe.
*/
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
getRandomInt()
	DESCRIPTION: Seeds and calculates a random int.
	INPUTS:	min -- Lower bound of rng.
			max -- Higher bound of rng.
	RETURN: Random int.
	EFFECT: Is rng.
*/
int Engine::getRandomInt(int min, int max) {
	/*static std::random_device rd;
	static std::mt19937 gen(rd());*/
	uniform_int_distribution<int> dist(min, max);
	return dist(gen);
}

/*
getRandomFloat()
	DESCRIPTION: Seeds and calculates a random float.
	INPUTS:	min -- Lower bound of rng.
			max -- Higher bound of rng.
	RETURN: Random float.
	EFFECT: Is rng.
*/
float Engine::getRandomFloat(float min, float max) {
	/*static std::random_device rd;
	static std::mt19937 gen(rd());*/
	std::uniform_real_distribution<float> dist(min, max);
	return dist(gen);
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
	Clock clock2;
	float dt_in_sec;
	float dt2_in_sec = 0.f;
	Time dt2 = clock2.restart();

	// game running
	while (gameWindow.isOpen()) {
		Time dt = clock.restart();
		dt_in_sec = dt.asSeconds();
		Time dt2_elapsed = clock2.getElapsedTime();
		dt2_in_sec = dt2_elapsed.asSeconds();
		/*
			TIME INTERVALS:
				.25 -- ok
				.50 -- ok
				.10 -- might be ok
				n/a -- NOT OK
				.2 -- ok
		*/
		if (dt2_in_sec >= algorithm_freq) {
			initialize();
			action(dt_in_sec);
			learn(dt_in_sec);
			dt2 = clock2.restart();
			dt2_in_sec = 0.f;
		}
		if (!ai_mode)
 			input(0);
		update(dt_in_sec);
		draw();
	}
}

// ---------------------------------------------------------------- AI ------------------------------------------------------------

/*
initialize()
	DESCRIPTION: Checks if the current game's state/action pair is in the tables. If not, it'll add it initialized to zero.
	INPUTS: n/a
	RETURN: n/a
	EFFECT: Could insert new key:value pair into Q_table and N_table.
*/
void Engine::initialize() {
	// add the current state/action into the tables if not done so already
	tuple<int, int, vector<int>> state = getState();

	// since both unordered maps are uninitialized (empty), only one is needed to check whether it's been "seen" before
	tuple<int, int, vector<int>, int> state_no_flap = tuple_cat(state, make_tuple(0));
	tuple<int, int, vector<int>, int> state_flap = tuple_cat(state, make_tuple(1));

	// check if either are already in the map
	unordered_map<tuple<int, int, vector<int>, int>, float, TupleHash>::iterator state_action = Q_table.find(state_no_flap);

	// not in Q_table/N-table
	if (state_action == Q_table.end()) {
		Q_table.emplace(state_no_flap, 0.f);
		Q_table.emplace(state_flap, 0.f);
		N_table.emplace(state_no_flap, 0);
		N_table.emplace(state_flap, 0);
	}

	// make the saved_state_action not empty
	if (saved_state_action.empty()) {
		saved_state_action.push_back(state_flap);
		saved_state_action.push_back(state_flap);
	}
}

/*
action()
	DESCRIPTION: Decides whether the model should explore, exploit, or choose at random, and then chooses an action.
	INPUTS:	n/a
	RETURN: An int that decides the action.
	EFFECT: Decides an action.
*/
int Engine::action(float time_elapsed) {
	// initialize
	tuple<int, int, vector<int>, int> state_action;
	int action;

	//	get random prob between 0 and 1
	float random_prob = getRandomFloat(0, 1);

	// explore
	if (random_prob < epsilon) {
		action = explore(time_elapsed);
	}
	// exploit
	else {
		action = exploit();
	}
	// decay
	if (Q_table.size() > 2500 && epsilon > .05)		// adjust rhis JOSH delete after changing
		epsilon *= decay_factor;

	input(action);

	return action;
}

/*
explore()
	DESCRIPTION: Explores new state/actions to potentially expand its "view".
	INPUTS:	n/a
	RETURN: 0 -- No flap.
			1 -- Flap.
	EFFECT:	Sends an action signal back to action().
*/
int Engine::explore(float time_elapsed) {
	// initialize
	int action;
	vector<tuple<int, int, vector<int>, int>> explore_list;
	unordered_map<tuple<int, int, vector<int>, int>, int, TupleHash>::iterator state_action = N_table.begin();
	unordered_map<tuple<int, int, vector<int>, int>, float, TupleHash>::iterator state_action_Q = Q_table.begin();

	// get current game's state
	tuple<int, int, vector<int>> state = getState();

	// reset to allow new generation of biggest Q values
	biggest_Q_value = 0;

	// search for state/action pairs that have the 
	for (state_action; state_action != N_table.end(); ++state_action, ++state_action_Q) {
		//int flappy_y = get<1>(state);
		int table_flappy_y = get<1>(*state_action);
		/*
			for some reason 
				int table_pipe_up_y = get<2>(*state_action)[1]; <- doesnt work

			Below is the jank work around
		*/
		tuple<int, int, vector<int>, int> temp = state_action->first;
		vector<int> temp2 = get<2>(temp);
		int table_pipe_up_y = temp2[1];

		// explore if its been explored less than n_first_times && biased decision (based off table var. not actual)
		if (state_action->second < n_first_times/* && table_flappy_y <= table_pipe_up_y + PIPE_OPENING_COLLISION_OFFSET*/) {
			explore_list.push_back(state_action->first);
		}

		// check if state/action value is >= biggest seen Q-value
		checkBiggestQValue(state_action_Q->first);

	}
	// empty because biased
	if (explore_list.empty()) {
		tuple<int, int, vector<int>> next_state = calculateNextState(tuple_cat(state, make_tuple(getRandomInt(0, 1))), time_elapsed);
		explore_list.push_back(tuple_cat(next_state, make_tuple(0)));
		explore_list.push_back(tuple_cat(next_state, make_tuple(1)));
		saved_state_action[0] = explore_list[getRandomInt(0, 1)];	// these two lines and the else are technically the same
		action = get<3>(saved_state_action[0]);						// maybe clean up later
	}
	// not empty
	else {
		saved_state_action[0] = explore_list[getRandomInt(0, explore_list.size() - 1)];
		action = get<3>(saved_state_action[0]);
	}

	return action;
}

/* 
exploit()
	DESCRIPTION: Finds the state/action with the highest Q-value and returns the action.
	INPUTS: n/a
	RETURN: 0 -- No flap.
			1 -- Flap.
	EFFECT:	Sends an action signal back to action().
*/
int Engine::exploit() {
	// initialize
	int action;

	// choose from exploit section
	saved_state_action[0] = saved_state_action[getRandomInt(1, saved_state_action.size() - 1)];
	action = get<3>(saved_state_action[0]);

	return action;
}

/*
checkBiggestQValue()
	DESCRIPTION: Checks the state/action pair from explore and compares its value to the record holder for 
	INPUTS:	state_action -- Key from Q_table.
	RETURN: n/a
	EFFECT: Adds to existing vector if a tie value, clear and add if greater than, or does nothing.

*/
void Engine::checkBiggestQValue(tuple<int, int, vector<int>, int> state_action) {
	// Q value from explore is bigger or equal
	if (Q_table[state_action] >= biggest_Q_value) {
		// tie
		saved_state_action.push_back(state_action);

		// biggest
		if (Q_table[state_action] > biggest_Q_value) {
			saved_state_action.erase(saved_state_action.begin() + 1, saved_state_action.end());
			saved_state_action.push_back(state_action);
			biggest_Q_value = Q_table[state_action];
		}
	}
}

tuple<int, int, vector<int>> Engine::calculateNextState(tuple<int, int, vector<int>, int> state_action, float time_elapsed) {
	// initialize
	tuple<int, int, vector<int>> next_state = make_tuple(get<0>(state_action), get<1>(state_action), get<2>(state_action));
	Vector2f Flappy_position = Flappy.getFlappy_Position();
	Vector2f Flappy_velocity = Flappy.getFlappy_Velocity();
	Vector2i pipe_up_position = { get<2>(next_state)[0], get<2>(next_state)[1] };
	Vector2i pipe_down_position = { get<2>(next_state)[2], get<2>(next_state)[3] };

	// update flappy's y
	if (get<3>(state_action) != 1) {
		get<1>(next_state) = Flappy_position.y + (time_elapsed * Flappy_velocity.y) + (.5 * GRAVITY * time_elapsed * time_elapsed);
	}
	else {
		Flappy_velocity.y = -1450 + GRAVITY * time_elapsed;
		get<1>(next_state) = Flappy_position.y + (time_elapsed * Flappy_velocity.y) + (.5 * GRAVITY * time_elapsed * time_elapsed);
	}
	// update pipes' x
	get<2>(next_state)[0] = pipe_up_position.x - X_Speed * time_elapsed;
	get<2>(next_state)[2] = pipe_down_position.x - X_Speed * time_elapsed;

	return next_state;
}


/*
learn()
	DESCRIPTION: Updates the Q-value for the state/action pair with Q-learning algorithm.
	INPUTS:	n/a
	RETURN: n/a
	EFFECT: Updates a Q-value in the Q-table.

*/
void Engine::learn(float dt_in_sec) {
	// initialize
	float Q_new;
	int action = get<3>(saved_state_action[0]);
	tuple<int, int, vector<int>, int> state_action = saved_state_action[0];

	// calculate Q_local
	float Q_local = calculateQLocal(state_action, dt_in_sec);

	// calculate Q
	Q_new = (1 - learning_rate) * Q_table[state_action] + learning_rate * Q_local;

	// update Q_table & N_table
	updateQTable(state_action, Q_new);
	updateNTable(state_action);

	// check if the updated value is better than existing
	checkBiggestQValue(state_action);
}

float Engine::calculateQLocal(tuple<int, int, vector<int>, int> state_action, float time_elapsed) {
	// initialize
	float Q_local;
	
	tuple<int, int, vector<int>> next_state = calculateNextState(state_action, time_elapsed);
	
	// max Q(s(t+1), a)
	float max = 0.f;
	tuple<int, int, vector<int>, int> next_state_no_flap = tuple_cat(next_state, make_tuple(0));
	tuple<int, int, vector<int>, int> next_state_flap = tuple_cat(next_state, make_tuple(1));
	
	// add to Q-table if not already in
	unordered_map<tuple<int, int, vector<int>, int>, float, TupleHash>::iterator it1 = Q_table.find(next_state_no_flap);
	if (it1 == Q_table.end()) {
		Q_table.emplace(next_state_no_flap, 0.f);
	}
	max = Q_table[next_state_no_flap];	
	
	unordered_map<tuple<int, int, vector<int>, int>, float, TupleHash>::iterator it2 = Q_table.find(next_state_flap);
	if (it2 == Q_table.end()) {
		Q_table.emplace(next_state_flap, 0.f);
	}
	if (max <= Q_table[next_state_flap]) {		// if they're equal, always flap
		max = Q_table[next_state_flap];	
	}

	// reward * discount * max Q_local
	Q_local = calculateReward() + discount_factor * max;

	return Q_local;
}

float Engine::calculateReward() {
	// initialize
	float reward = 0.f;
	tuple<int, int, vector<int>, int> state_action = saved_state_action[0];

	// collision
	checkCollision();
	if (is_collision) {
		reward = -1.f;
	}
	// passed a pipe
	else if (score > score_old) {
		reward = 1.f;
		score_old = score;
	}
	// survived
	else {
		reward = 0.1;

		// encourage flapping away from ground
		if (GROUND - get<1>(state_action) <= 10 && get<3>(state_action) == 1) {
			reward = .25;
		}
		// encourage being within pipe opening
		if (get<1>(state_action) < get<2>(state_action)[1] + PIPE_OPENING_COLLISION_OFFSET) {
			reward = .50;
		}

	}

	return reward;
}

void Engine::updateQTable(tuple<int, int, vector<int>, int> state_action, float Q_new) {
	Q_table[state_action] = Q_new;
}

/*
updateNTable()
	DESCRIPTION:
	INPUTS:
	RETURN:
	EFFECT:
*/
void Engine::updateNTable(tuple<int, int, vector<int>, int> state_action) {
	// initialize
	unordered_map<tuple<int, int, vector<int>, int>, int, TupleHash>::iterator it = N_table.find(state_action);

	// update N-table value
	if (it != N_table.end()) {
		N_table[state_action] = it->second++;
	}
}














/*
discretization()
	DESCRIPTION: Takes in a float value and converts it into a discretable value.
	INPUTS: a -- Float that will be converted into a discrete value.
	RETURN: An int that was the result of a conversion of the input.
	EFFECT: Converts float to int.
*/
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
getState()
	DESCRIPTION: Gets the game's current state.
	INPUTS: n/a
	RETURN: Tuple in the form of (int, int, vector<int>).
	EFFECT: Makes a tuple.
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
	for (vector<float>::iterator it = Pipe_position.begin(); it != Pipe_position.end(); ++it) {
		Pipe_position_updated.push_back(discretization(*it));
	}
	
	return make_tuple(Flappy_position_updated.x, Flappy_position_updated.y, Pipe_position_updated);
}




