
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
		std::cout << "Error initializing SDL.\n";
		return false;
	}

	SDL_Window* window = SDL_CreateWindow(
		NULL,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		SDL_WINDOW_SHOWN
		//SDL_WINDOW_BORDERLESS
	);

	if (!window) {
		std::cout << "Error creating SDL Window.\n";
		return false;
	}

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
	if (!renderer) {
		std::cout << "Error creating SDL Renderer.\n";
		return false;
	}

	SDL_RenderSetLogicalSize(renderer, WINDOW_WIDTH, WINDOW_HEIGHT);
	SDL_SetWindowTitle(window, "Tic-Tac-Toe");
	SDL_Surface* icon = IMG_Load("C:/Users/Admin/Desktop/Assets/icon.png");
	SDL_SetWindowIcon(window, icon);
	//SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	//SDL_SetWindowFullscreen(window, true);
	
	Mix_OpenAudio(48000, AUDIO_S16, 2, 1024);

	//pathAssets = SDL_GetBasePath();

	int GameMenu = 1;
	bool isGameRunning = true;

	while (isGameRunning) {

		switch (GameMenu) {

			case 1: {
				GameStartMenu* myActiveWindow = new GameStartMenu();
				myActiveWindow->window = window;
				myActiveWindow->renderer = renderer;

				myActiveWindow->setup();

				while (myActiveWindow->isMenuRunning) {
					myActiveWindow->inputs();
					myActiveWindow->update();
					myActiveWindow->render();
				}

				GameMenu = myActiveWindow->nextMenu;
				delete myActiveWindow;
			} break;
			
			case 2: {
				GameBattleMenu* myActiveWindow = new GameBattleMenu();
				myActiveWindow->window = window;
				myActiveWindow->renderer = renderer;

				myActiveWindow->setup();

				while (myActiveWindow->isMenuRunning) {
					myActiveWindow->inputs();
					myActiveWindow->update();
					myActiveWindow->render();
				}

				GameMenu = myActiveWindow->nextMenu;
				delete myActiveWindow;
			} break;

			default:
				isGameRunning = false;
				break;
		}
	}

 	return 0;
}