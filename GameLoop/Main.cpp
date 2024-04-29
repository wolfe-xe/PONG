#include <stdio.h>
#include <SDL.h>

/// <summary>
/// argc (argument count) - Non - negative value representing the number of arguments passed to the program from the environment in which the program is run.
/// argv (argument vector) -	Pointer to the first element of an array of argc + 1 pointers, of which the last one is null and the previous ones, if any, point to strings that represent the arguments passed to the program from the host environment. If argv[0] is not a null pointer (or, equivalently, if argc > 0), it points to a string that represents the program name, which is empty if the program name is not available from the host environment.
/// </summary>

int main(int argc, char* argv[]) {
	SDL_Init(SDL_INIT_EVERYTHING);
	printf("Hello, World!\n");
	return 0;
}