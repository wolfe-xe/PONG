#include <stdio.h>
#include <SDL.h>
#include <random>

#include "Constants.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

//int game_is_running = FALSE;
int last_frame_time = 0;

struct controls {
	int game_is_running = FALSE;
	int game_is_restarted = FALSE;
	int up = FALSE;
	int down = FALSE;
} controls;

struct Box {
	float x;
	float y;
	float width;
	float height;
	float x_velocity;
	float y_velocity;
} box;

struct player{
	float x;
	float y;
	float width;
	float height;
	float y_vel;
} player;

struct opponent{
	float x;
	float y;
	float width;
	float height;
	float y_vel;
} opponent;

struct guiSeparator {
	float x;
	float y;
	float width;
	float height;
} guiSeparator;

struct p_score {
	float x;
	float y;
	float w;
	float h;
}p_score;

struct p_score_max {
	float x;
	float y;
	float w;
	float h;
} p_score_max;

struct o_score {
	float x;
	float y;
	float w;
	float h;
}o_score;

struct o_score_max {
	float x;
	float y;
	float w;
	float h;
} o_score_max;


int Initialize_Window(void) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		fprintf(stderr, "Error Initializing SDL. \n");
		return FALSE;
	}
	window = SDL_CreateWindow(
		"GameLoop", 
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		SDL_WINDOW_SHOWN
	);
	if (!window) {
		fprintf(stderr, "Error creating SDL window \n");
		return FALSE;
	}
	renderer = SDL_CreateRenderer(window, -1, 0);
	if (!renderer) {
		fprintf(stderr, "Error creating SDL Renderer \n");
	}
	return TRUE;
}

void Process_Input() {
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type) {
	case SDL_QUIT:
		controls.game_is_running = FALSE;
		break;

	case SDL_KEYDOWN:
		if (event.key.keysym.sym == SDLK_ESCAPE) {
			controls.game_is_running = FALSE;
		}
		//player controls
		if (event.key.keysym.sym == SDLK_w) {
			controls.up = TRUE;
			player.y_vel = -SPEED;
		}
		if (event.key.keysym.sym == SDLK_s) {
			controls.down = TRUE;
			player.y_vel = SPEED;
		}
		//opponent controls
		if (event.key.keysym.sym == SDLK_UP) {
			controls.up = TRUE;
			opponent.y_vel = -SPEED;
		}
		if (event.key.keysym.sym == SDLK_DOWN) {
			controls.down = TRUE;
			opponent.y_vel = SPEED;

		}

	case SDL_KEYUP:
		//player
		if (event.key.keysym.sym == SDLK_w) {
			controls.up = FALSE;
		}
		if (event.key.keysym.sym == SDLK_s) {
			controls.down = FALSE;
		}
		//opponent
		if (event.key.keysym.sym == SDLK_UP) {
			controls.up = FALSE;
		}
		if (event.key.keysym.sym == SDLK_DOWN) {
			controls.down = FALSE;
		}
	}
}

int RandomDir(int x, int y) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<>	dis(0, 1);

	if (dis(gen) == 0) {
		return x;
	}
	else {
		return y;
	}
}

void Start() {
	//setting up ball's initial pos
	box.x = (WINDOW_WIDTH - box.width) / 2;
	box.y =  (WINDOW_HEIGHT - box.height) / 2;
	box.width = 10;
	box.height = 10;
	box.x_velocity = RandomDir(SPEED, -SPEED);
	box.y_velocity = RandomDir(SPEED, -SPEED);
	
	//setting up players initial pos
	player.x = 15;
	player.y = (WINDOW_HEIGHT - player.height) / 2;
	player.width = 15;
	player.height = 50;
	player.y_vel = 0;

	//setting up opponents inital pos
	opponent.x = WINDOW_WIDTH - 35;
	opponent.y = (WINDOW_HEIGHT - opponent.height) / 2;
	opponent.width = 15;
	opponent.height = 50;
	opponent.y_vel = 0;

	//setting up guiSeparator pos
	guiSeparator.x = (WINDOW_WIDTH - guiSeparator.width) / 2;
	guiSeparator.y = 0;
	guiSeparator.width = 5;
	guiSeparator.height = 10;

	p_score.x = 130;
	p_score.y = 15;
	p_score.h = 100;
	p_score.w = 10;

	p_score_max.x = 130;
	p_score_max.y = 15;
	p_score_max.h = 100;
	p_score_max.w = 10;

	o_score.x = WINDOW_WIDTH - 230;
	o_score.y = 15;
	o_score.h = 100;
	o_score.w = 10;

	o_score_max.x = WINDOW_WIDTH - 230;
	o_score_max.y = 15;
	o_score_max.h = 100;
	o_score_max.w = 10;

}

void Update() {
	//logic for fixed timeStep
	///<summary>
	/// wait until we reach the frame target time;
	/// while (!SDL_TICKS_PASSED(SDL_GetTicks(), last_frame_time + FRAME_TARGET_TIME));
	/// a while loop can be dangerous for the processor as it takes all the attention for the time being making it heavy on the cpu
	/// therefore using a SDL_Delay() would be a much better option as its not heavy on the CPU
	///</summary>
	int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - last_frame_time);
	if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
		SDL_Delay(time_to_wait);
	}

	//get a delta time factor, convert it to seconds to be used to update my game objects;
	float delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0f;
	//store the ms of the current frame to be used in the next one;
	last_frame_time = SDL_GetTicks();


	//control update for player
	player.y += player.y_vel * delta_time;
	//checking for player bounds 
	if (player.y <= 0) {
		player.y = 0;
		player.y_vel = -player.y_vel;
	}	
	//bottom bounds
	if (player.y >= WINDOW_HEIGHT - player.height) {
		player.y = WINDOW_HEIGHT - player.height;
		player.y_vel = -player.y_vel;
	}

	//updating opponent
	opponent.y += opponent.y_vel * delta_time;
	//checking opponents bounds
	if (opponent.y <= 0) {
		opponent.y = 0;
		opponent.y_vel = -opponent.y_vel;
	}
	//bottom bounds
	if (opponent.y >= WINDOW_HEIGHT - opponent.height) {
		opponent.y = WINDOW_HEIGHT - opponent.height;
		opponent.y_vel = -opponent.y_vel;
	}

	//ball controls 
	box.x += box.x_velocity * delta_time;
	box.y += box.y_velocity * delta_time;

	//collision detection with bounds
	if (box.x <= 0) {
		box.x = (WINDOW_WIDTH - box.width) / 2;
		box.x_velocity = -box.x_velocity;
		p_score.h -= 10;
		if (p_score.h == 0) {
			printf("gameOver\n");
		}
		printf("left side\n");
	}
	if (box.x >= WINDOW_WIDTH - box.width) {
		box.x = (WINDOW_WIDTH - box.width) / 2;
		box.x_velocity = -box.x_velocity;
		o_score.h -= 10;
		if (o_score.h == 0) {
			printf("gameOver\n");
			controls.game_is_running = FALSE;
		}
		printf("right side\n");
	}
	if (box.y <= 0) {
		box.y = 0;
		box.y_velocity = -box.y_velocity;
	}
	//bottom bounds
	if (box.y >= WINDOW_HEIGHT - box.height) {
		box.y = WINDOW_HEIGHT - box.height;
		box.y_velocity = -box.y_velocity;
	}

}

void Render() {
	SDL_SetRenderDrawColor(renderer, 2, 1, 0, 255);
	SDL_RenderClear(renderer);

	//here is where we can start drawing gameObjects
	SDL_Rect box_rect = {
		(int)box.x,
		(int)box.y,
		(int)box.width,
		(int)box.height
	};

	SDL_Rect player_rect = {
		(int)player.x,
		(int)player.y,
		(int)player.width,
		(int)player.height
	};

	SDL_Rect opponent_rect = {
		(int)opponent.x,
		(int)opponent.y,
		(int)opponent.width,
		(int)opponent.height
	};

	SDL_Rect guiSeparator_rect = {
		(int)guiSeparator.x,
		(int)guiSeparator.y,
		(int)guiSeparator.width,
		(int)guiSeparator.height,
	};

	SDL_Rect p_score_rect = {
		(int)p_score.x,
		(int)p_score.y,
		(int)p_score.h,
		(int)p_score.w
	};

	SDL_Rect p_score_max_rect = {
		(int)p_score_max.x,
		(int)p_score_max.y,
		(int)p_score_max.h,
		(int)p_score_max.w,
	};

	SDL_Rect o_score_rect = {
		(int)o_score.x,
		(int)o_score.y,
		(int)o_score.h,
		(int)o_score.w
	};

	SDL_Rect o_score_max_rect = {
		(int)o_score_max.x,
		(int)o_score_max.y,
		(int)o_score_max.h,
		(int)o_score_max.w,
	};

	//checks for player collision
	SDL_bool playerCollision = SDL_HasIntersection(&player_rect, &box_rect);
	if (playerCollision) {
		printf("collided with player\n");
		box.x_velocity = -box.x_velocity;
		box.y_velocity = -box.y_velocity;
	}
	//checks for opponents collision
	SDL_bool opponentCollision = SDL_HasIntersection(&opponent_rect, &box_rect);
	if (opponentCollision) {
		printf("collided with opponent\n");
		box.x_velocity = -box.x_velocity;
		box.y_velocity = -box.y_velocity;
	}

	SDL_SetRenderDrawColor(renderer, 115, 110, 110, 255);
	SDL_RenderFillRect(renderer, &p_score_max_rect);
	SDL_RenderFillRect(renderer, &o_score_max_rect);

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(renderer, &box_rect);

	SDL_SetRenderDrawColor(renderer, 115, 110, 110, 255);
	//SDL_RenderFillRect(renderer, &guiSeparator_rect);
	for (guiSeparator_rect.y = 0; guiSeparator_rect.y < WINDOW_HEIGHT; guiSeparator_rect.y += 28)
	{
		SDL_RenderFillRect(renderer, &guiSeparator_rect);
	}

	//SDL_SetRenderDrawColor(renderer, 5, 255, 5, 255);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(renderer, &p_score_rect);
	SDL_RenderFillRect(renderer, &o_score_rect);

	//SDL_SetRenderDrawColor(renderer, 65, 94, 54, 255);
	SDL_SetRenderDrawColor(renderer, 146, 153, 144, 255);
	SDL_RenderFillRect(renderer, &player_rect);

	//SDL_SetRenderDrawColor(renderer, 97, 54, 54, 255);
	SDL_SetRenderDrawColor(renderer, 153, 144, 144, 255);
	SDL_RenderFillRect(renderer, &opponent_rect);

	SDL_RenderPresent(renderer);
	
}

void Destroy_Window() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit;
}

/// <summary>
/// argc (argument count) - Non - negative value representing the number of arguments passed to the program from the environment in which the program is run.
/// argv (argument vector) - Pointer to the first element of an array of argc + 1 pointers, of which the last one is null and the previous ones, if any, point to strings that represent the arguments passed to the program from the host environment. If argv[0] is not a null pointer (or, equivalently, if argc > 0), it points to a string that represents the program name, which is empty if the program name is not available from the host environment.
/// </summary>
int main(int argc, char* argv[]) {
	controls.game_is_running = Initialize_Window();

	Start();

	while (controls.game_is_running) {
		Process_Input();
		Update();
		Render();
	}

	Destroy_Window();

	return FALSE;
}

/// TODO:
/// []better game mechanics
/// []better collision detection
/// [x]random value at start
///		[]work on randomness
/// []Graphical elements
///		[-]middle line separating both halfs
///		[]score ui
///	[]possibly a start menu
///		[]restart
///		[]pause and play

