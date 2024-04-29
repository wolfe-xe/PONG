#include <stdio.h>
#include <SDL.h>

#include "Constants.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
int game_is_running = FALSE;

struct Box
{
	float x;
	float y;
	float width;
	float height;
} box;

int Initialize_Window(void) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		fprintf(stderr, "Error Initializing SDL. \n");
		return FALSE;
	}

	window = SDL_CreateWindow(
		"GameLoop", 
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WINDOW_WIDHT,
		WINDOW_HEIGHT,
		SDL_WINDOW_BORDERLESS
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
		game_is_running = FALSE;
		break;

	case SDL_KEYDOWN:
		if (event.key.keysym.sym == SDLK_ESCAPE) {
			game_is_running = FALSE;
		}
		break;
	}
}
void Start() {
	box.x = 20;
	box.y = 20;
	box.width = 10;
	box.height = 10;
}

void Update() {
	box.x += 1;
	box.y += 1;
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

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(renderer, &box_rect);

	SDL_RenderPresent(renderer);
	
}

void Destroy_Window() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit;
}

/// <summary>
/// argc (argument count) - Non - negative value representing the number of arguments passed to the program from the environment in which the program is run.
/// argv (argument vector) -	Pointer to the first element of an array of argc + 1 pointers, of which the last one is null and the previous ones, if any, point to strings that represent the arguments passed to the program from the host environment. If argv[0] is not a null pointer (or, equivalently, if argc > 0), it points to a string that represents the program name, which is empty if the program name is not available from the host environment.
/// </summary>
int main(int argc, char* argv[]) {
	game_is_running = Initialize_Window();

	Start();

	while (game_is_running) {
		Process_Input();
		Update();
		Render();
	}

	Destroy_Window();

	return FALSE;
}