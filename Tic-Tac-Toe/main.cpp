#include <iostream>
//#include <windows.h>
#include <SDL.h>

//https://www.youtube.com/watch?v=tmGBhM8AEj8
//https://www.youtube.com/watch?v=XfZ6WrV5Z7Y

#define WINDOW_HEIGHT 600
#define WINDOW_WIDTH 800

#define SquareWidth 100

#define FPS 30
#define FRAME_TARGET_TIME (1000 / FPS)

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

bool gameRunning;
bool isGameEnd = false;

int MouseX;
int MouseY;
int selectedSquare;
int lastFrameTime;
int turnPlayer;
const int maxPlayers = 2;

char table[9];
char winner;
const char players[2] = { 'o', 'x' };


SDL_Rect Squares[9];

bool isLineComplete(char* table, char player) {

	bool flag = false;

	// HORIZONTAL EVALUATION
	if (table[0] == player && table[1] == player && table[2] == player) flag = true;
	else if (table[3] == player && table[4] == player && table[5] == player) flag = true;
	else if (table[6] == player && table[7] == player && table[8] == player) flag = true;
	// VERTICAL EVALUAITON
	else if (table[0] == player && table[3] == player && table[6] == player) flag = true;
	else if (table[1] == player && table[4] == player && table[7] == player) flag = true;
	else if (table[2] == player && table[5] == player && table[8] == player) flag = true;
	// DIAGONAL EVALUATION
	else if (table[0] == player && table[4] == player && table[8] == player) flag = true;
	else if (table[2] == player && table[4] == player && table[6] == player) flag = true;

	if (flag) {
		isGameEnd = true;
		winner = player;
	}

	return flag;
}

char* setSquare(char* table, int selectedSquare, char player) {

	table[selectedSquare] = player;
	isLineComplete(table, player);

	return table;
}


bool initialize_window(void) {

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		fprintf(stderr, "Error initializing SDL.\n");
		return false;
	}

	window = SDL_CreateWindow(
							NULL, 
							SDL_WINDOWPOS_CENTERED, 
							SDL_WINDOWPOS_CENTERED, 
							WINDOW_WIDTH, 
							WINDOW_HEIGHT, 
							SDL_WINDOW_BORDERLESS
	);

	if (!window) {
		fprintf(stderr, "Error creating SDL Window\n");
		return false;
	}

	renderer = SDL_CreateRenderer(window, -1, 0);
	if (!renderer) {
		fprintf(stderr, "Error creating SDL Renderer\n");
		return false;
	}

	return true;
}

void setup() {
	
	int size = sizeof(table) / sizeof(table[0]);
	for (int i = 0; i < size; ++i) {
		table[i] = 'e';
		Squares[i] = {
			10 + i % 3 * (SquareWidth + 10), // x
			10 + i / 3 * (SquareWidth + 10), // y
			SquareWidth,				 // width
			SquareWidth				 // height
		};
	}

	turnPlayer = 1;

}

void inputs() {

	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type) {

		case SDL_QUIT:
			gameRunning = false;
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE) gameRunning = false;
			break;
		case SDL_MOUSEBUTTONDOWN:
			MouseX = event.button.x;
			MouseY = event.button.y;

			selectedSquare = -1;

			for (int i = 0; i < 9; ++i) {
				
				bool flagX = MouseX >= Squares[i].x && MouseX <= Squares[i].x + Squares[i].w;
				bool flagY = MouseY >= Squares[i].y && MouseY <= Squares[i].y + Squares[i].h;
				if (table[i] == 'e' && flagX && flagY) {
					selectedSquare = i;
					break;
				}
			}

			if (selectedSquare > -1) {
				setSquare(table, selectedSquare, players[turnPlayer - 1]);
				if (++turnPlayer > maxPlayers) turnPlayer = 1;
			}

			if (isGameEnd) {
				std::cout << "The winnner is player " << winner << '\n';
				gameRunning = false;
			}

			break;

		case NULL:
		default:
			break;
	}
}

void update() {

	// Calculate how much we have to wait until reach the target frame time
	Uint32 actualFrameTime = SDL_GetTicks();
	int timeWait = FRAME_TARGET_TIME - (actualFrameTime - lastFrameTime);

	if (timeWait > 0 && timeWait <= FRAME_TARGET_TIME) {
		SDL_Delay(timeWait);
	}

	float deltaTime = (actualFrameTime - lastFrameTime) / 1000.0f;

	lastFrameTime = actualFrameTime;
}

void render() {

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	// Draw Rectangle
	for (int i = 0; i < 9; ++i) {
		
		if (table[i] == 'e') SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		else if (table[i] == 'x') SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		else SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

		SDL_RenderFillRect(renderer, &Squares[i]);
	}

	SDL_RenderPresent(renderer);
}


void destroyWindow() {
	
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

int main(int argc, char* args[]) {

	//HWND windowHandle = GetConsoleWindow();
	//ShowWindow(windowHandle, SW_HIDE);

	gameRunning = initialize_window();

	setup();

	while (gameRunning) {
		inputs();
		update();
		render();
	}

	destroyWindow();

 	return 0;
}