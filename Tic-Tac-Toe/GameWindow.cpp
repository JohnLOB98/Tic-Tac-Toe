#include "GameWindow.h"

bool GameApp::initializeGame() {

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		//fprintf(stderr, "Error initializing SDL.\n");
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
		//fprintf(stderr, "Error creating SDL Window\n");
		return false;
	}

	renderer = SDL_CreateRenderer(window, -1, 0);
	if (!renderer) {
		//fprintf(stderr, "Error creating SDL Renderer\n");
		return false;
	}

	SDL_RenderSetLogicalSize(renderer, WINDOW_WIDTH, WINDOW_HEIGHT);
	SDL_SetWindowFullscreen(window, true);

	return true;
}

void GameApp::destroyWindow() const {

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}


void GameWindowBattle::setup(){

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

	assetBackground = loadTexture("C:/Users/Admin/Desktop/Assets/backgroundPlaying.png");
	assetEmptySpace = loadTexture("C:/Users/Admin/Desktop/Assets/emptyspace.png");
	assetPlayer1[0] = loadTexture("C:/Users/Admin/Desktop/Assets/Charmander0.png");
	assetPlayer1[1] = loadTexture("C:/Users/Admin/Desktop/Assets/Charmander1.png");
	assetPlayer2[0] = loadTexture("C:/Users/Admin/Desktop/Assets/Squirtle0.png");
	assetPlayer2[1] = loadTexture("C:/Users/Admin/Desktop/Assets/Squirtle1.png");
	assetsTransitionBattle = loadTexture("C:/Users/Admin/Desktop/Assets/transitionBattle.png");

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");



	battleMusic = loadMusic("C:/Users/Admin/Desktop/Assets/Audio/pokemonWildBattle.mp3");
	winBattleMusic = loadMusic("C:/Users/Admin/Desktop/Assets/Audio/winBattle.mp3");

	Mix_PlayMusic(battleMusic, -1);

	int pastSeconds = SDL_GetTicks();
	int actualSeconds = pastSeconds;
	while (actualSeconds - pastSeconds < 1000) {
		actualSeconds = SDL_GetTicks();
	}
}

void GameWindowBattle::inputs() {

	if (flagAnimation) {
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

			if (isBattleEnd) {
				//std::cout << "The winnner is player " << winner << '\n';
				Mix_PlayMusic(winBattleMusic, -1);
				//Mix_PausedMusic(backgroundMusic);
				//gameRunning = false;
			}

			break;

		case NULL:
		default:
			break;
		}
	}
}

void GameWindowBattle::update() {
	
	Uint32 actualFrameTime = SDL_GetTicks();
	int timeWait = FRAME_TARGET_TIME - (actualFrameTime - lastFrameTime);

	if (timeWait > 0 && timeWait <= FRAME_TARGET_TIME) {
		SDL_Delay(timeWait);
	}

	float deltaTime = (actualFrameTime - lastFrameTime) / 1000.0f;

	lastFrameTime = actualFrameTime;
	if (++actualFrame > FPS) actualFrame = 0;
}

void GameWindowBattle::render() {

	//SDL_Rect srcrect;
	//SDL_Rect dstrect;

	if (flagAnimation) {
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		// Draw Rectangle
		SDL_Rect srcrect = { 0, 0, 340, 340 };
		SDL_Rect dstrect = { 0, 0, 340, 340 };

		SDL_RenderCopy(renderer, assetBackground, &srcrect, &dstrect);

		for (int i = 0; i < 9; ++i) {

			srcrect = { 0, 0, 100, 100 };
			dstrect = { 10 + i % 3 * (SquareWidth + 10), 10 + i / 3 * (SquareWidth + 10), 100, 100 };

			if (actualFrame < 12) {
				if (table[i] == 'e') SDL_RenderCopy(renderer, assetEmptySpace, &srcrect, &dstrect);
				else if (table[i] == 'o') SDL_RenderCopy(renderer, assetPlayer1[0], &srcrect, &dstrect);
				else SDL_RenderCopy(renderer, assetPlayer2[0], &srcrect, &dstrect);
			}
			else {
				if (table[i] == 'e') SDL_RenderCopy(renderer, assetEmptySpace, &srcrect, &dstrect);
				else if (table[i] == 'o') SDL_RenderCopy(renderer, assetPlayer1[1], &srcrect, &dstrect);
				else SDL_RenderCopy(renderer, assetPlayer2[1], &srcrect, &dstrect);
			}
		}
	}
	else {

		bool flag = actualFramesAnimation < 32;
		int x = flag ? actualFramesAnimation % 6 * 34 : (totalFramesAnimation - actualFramesAnimation) % 6 * 34;
		int y = flag ? actualFramesAnimation / 6 * 34 : (totalFramesAnimation - actualFramesAnimation) / 6 * 34;
		SDL_Rect srcrect = { x , y, 34, 34 };
		SDL_Rect dstrect = { 0 , 0, 340, 340 };

		SDL_RenderCopy(renderer, assetsTransitionBattle, &srcrect, &dstrect);

		if (++actualFramesAnimation > totalFramesAnimation) flagAnimation = true;

	}


	SDL_RenderPresent(renderer);
}

char* GameWindowBattle::setSquare(char* table, int selectedSquare, char player) {

	table[selectedSquare] = player;
	isLineComplete(table, player);

	return table;
}

bool GameWindowBattle::isLineComplete(char* table, char player) {

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
		isBattleEnd = true;
		winner = player;
	}

	return flag;
}

SDL_Texture* GameWindowBattle::loadTexture(const char* filePath) {

	SDL_Texture* texture = NULL;
	texture = IMG_LoadTexture(renderer, filePath);

	if (!texture) {
		//std::cout << "Texture couldn't load from file path: " << filePath << "\nError: " << SDL_GetError() << '\n';
	}

	return texture;
}

Mix_Music* GameWindowBattle::loadMusic(const char* filePath) {

	Mix_Music* music = NULL;
	music = Mix_LoadMUS(filePath);
	if (!music) {
		std::cout << "Music Error: " << Mix_GetError() << '\n';
	}

	return music;
}
// END GAME APP BATTLE ---------------------------------------------------------------------------------------------------


void GameStartMenu::setup(){
	
	int size = sizeof(Buttons) / sizeof(Buttons[0]);
	for (int i = 0; i < size; ++i) {
		Buttons[i] = {
			(WINDOW_WIDTH - widthButton) / 2, // x
			85 + heightButton * i + 10 * i, // y
			widthButton,				 // width
			heightButton				 // height
		};
	}

}

void GameStartMenu::inputs() {

}

void GameStartMenu::render() {

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	for (int i = 0; i < 3; ++i) SDL_RenderFillRect(renderer, &Buttons[i]);
	

	SDL_RenderPresent(renderer);
}

void GameStartMenu::update() {
		
	Uint32 actualFrameTime = SDL_GetTicks();
	int timeWait = FRAME_TARGET_TIME - (actualFrame - lastFrameTime);

	if (timeWait > 0 && timeWait <= FRAME_TARGET_TIME) {
		SDL_Delay(timeWait);
	}

	float deltaTime = (SDL_GetTicks() - lastFrameTime) / 1000.0f;
	lastFrameTime = SDL_GetTicks();

}


