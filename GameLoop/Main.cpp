#include <stdio.h>
#include <SDL.h>

#include "Constants.h"

/// <summary>
/// argc (argument count) - Non - negative value representing the number of arguments passed to the program from the environment in which the program is run.
/// argv (argument vector) -	Pointer to the first element of an array of argc + 1 pointers, of which the last one is null and the previous ones, if any, point to strings that represent the arguments passed to the program from the host environment. If argv[0] is not a null pointer (or, equivalently, if argc > 0), it points to a string that represents the program name, which is empty if the program name is not available from the host environment.
/// </summary>
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
int game_is_running = FALSE;

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
	//TODO: 
}

void Update() {
	//TODO:
}

void Render() {
	//TODO:
}

void Destroy_Window() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit;
}

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