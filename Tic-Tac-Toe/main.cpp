#include <iostream>

// SDL Installation and Game Loop Tutorials
//https://www.youtube.com/watch?v=tmGBhM8AEj8
//https://www.youtube.com/watch?v=XfZ6WrV5Z7Y

// Add Audio SDL
//https://www.youtube.com/watch?v=MAeqfcLy64w

// Add resource.h
//https://www.youtube.com/watch?v=5MSYn_J0OAQ

#include "GameWindow.h"


int main(int argc, char* args[]) {

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		//fprintf(stderr, "Error initializing SDL.\n");
		return false;
	}

	SDL_Window* window = SDL_CreateWindow(
		NULL,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		SDL_WINDOW_BORDERLESS
	);

	if (!window) {
		//fprintf(stderr, "Error creating SDL Window\n");
		return false;
	}

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
	if (!renderer) {
		//fprintf(stderr, "Error creating SDL Renderer\n");
		return false;
	}

	SDL_RenderSetLogicalSize(renderer, WINDOW_WIDTH, WINDOW_HEIGHT);
	SDL_SetWindowFullscreen(window, true);

	//return true;

	//Mix_OpenAudio(441000, MIX_DEFAULT_FORMAT, 2, 1024);
	Mix_OpenAudio(48000, AUDIO_S16, 2, 4096);

	//GameWindowBattle myActiveWindow;
	//myActiveWindow.initializeGame();

	//// Wait load window
	//SDL_Delay(3000);

	//myActiveWindow.setup();
	
	GameStartMenu myActiveWindow;
	//myActiveWindow.initializeGame();
	myActiveWindow.window = window;
	myActiveWindow.renderer = renderer;

	myActiveWindow.setup();

	while (1) {
		myActiveWindow.inputs();
		myActiveWindow.update();
		myActiveWindow.render();
	}

	myActiveWindow.destroyWindow();

 	return 0;
}