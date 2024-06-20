#include "GameWindow.h"

void GameApp::destroyWindow() const {

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

SDL_Texture* GameApp::loadTexture(const char* filePath) {

	SDL_Texture* texture = NULL;
	texture = IMG_LoadTexture(renderer, filePath);

	if (!texture) {
		std::cout << "Texture couldn't load from file path: " << filePath << "\nError: " << SDL_GetError() << '\n';
	}

	return texture;
}
Mix_Music* GameApp::loadMusic(const char* filePath) {

	Mix_Music* music = NULL;
	music = Mix_LoadMUS(filePath);
	if (!music) std::cout << "Music Error: " << Mix_GetError() << '\n';

	return music;
}
Mix_Chunk* GameApp::loadSound(const char* filePath) {

	Mix_Chunk* sound = NULL;
	sound = Mix_LoadWAV(filePath);
	if (!sound) std::cout << "Sound Error: " << Mix_GetError() << '\n';

	return sound;
}

// GAME APP END ------------------------------------------------------------------------------------------------------------


// GAME BATTLE MENU BEGIN --------------------------------------------------------------------------------------------------
void GameBattleMenu::setup(){

	assetBackground = loadTexture("C:/Users/Admin/Desktop/Assets/backgroundPlaying.png");
	assetEmptySpace = loadTexture("C:/Users/Admin/Desktop/Assets/emptyspace.png");
	assetPlayer1[0] = loadTexture("C:/Users/Admin/Desktop/Assets/Charmander0.png");
	assetPlayer1[1] = loadTexture("C:/Users/Admin/Desktop/Assets/Charmander1.png");
	assetPlayer2[0] = loadTexture("C:/Users/Admin/Desktop/Assets/Squirtle0.png");
	assetPlayer2[1] = loadTexture("C:/Users/Admin/Desktop/Assets/Squirtle1.png");
	assetCharmanderWin[0] = loadTexture("C:/Users/Admin/Desktop/Assets/charmanderWin0.png");
	assetCharmanderWin[1] = loadTexture("C:/Users/Admin/Desktop/Assets/charmanderWin1.png");
	assetSquirtleWin[0] = loadTexture("C:/Users/Admin/Desktop/Assets/squirtleWin0.png");
	assetSquirtleWin[1] = loadTexture("C:/Users/Admin/Desktop/Assets/squirtleWin1.png");

	assetsTransitionBattle = loadTexture("C:/Users/Admin/Desktop/Assets/transitionBattle.png");
	winBattleMusic = loadMusic("C:/Users/Admin/Desktop/Assets/Audio/winBattle.mp3");

	squirtleSound = loadSound("C:/Users/Admin/Desktop/Assets/Audio/squirtle.mp3");
	charmanderSound = loadSound("C:/Users/Admin/Desktop/Assets/Audio/charmander.mp3");


	int size = sizeof(table) / sizeof(table[0]);
	for (int i = 0; i < size; ++i) {

		table[i] = 'e';

		Squares.srcrect[i] = { 0, 0, 100, 100 };
		Squares.dstrect[i] = {
			10 + i % 3 * (SquareWidth + 10), // x
			10 + i / 3 * (SquareWidth + 10), // y
			SquareWidth,				 // width
			SquareWidth				 // height
		};

		for (int j = 0; j < FPS; ++j) {
			Squares.assets[i][j] = assetEmptySpace;
		}
	}
}

void GameBattleMenu::inputs() {

	if (flagAnimation) {
		SDL_Event event;
		SDL_PollEvent(&event);

		if (!isBattleEnd) {
			switch (event.type) {

			case SDL_QUIT:
				isMenuRunning = false;
				break;

			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE) isMenuRunning = false;
				break;

			case SDL_MOUSEBUTTONDOWN: {

				int MouseX = event.button.x;
				int MouseY = event.button.y;
				int selectedSquare = -1;

				for (int i = 0; i < 9; ++i) {

					bool flagX = MouseX >= Squares.dstrect[i].x && MouseX <= Squares.dstrect[i].x + Squares.dstrect[i].w;
					bool flagY = MouseY >= Squares.dstrect[i].y && MouseY <= Squares.dstrect[i].y + Squares.dstrect[i].h;
					if (table[i] == 'e' && flagX && flagY) {
						selectedSquare = i;
						break;
					}
				}

				if (selectedSquare > -1) {
					
					setSquare(table, selectedSquare, players[turnPlayer - 1]);

					if (turnPlayer == 1) {
						for (int j = 0; j < 12; ++j) Squares.assets[selectedSquare][j] = assetPlayer1[0];
						for (int j = 12; j < FPS; ++j) Squares.assets[selectedSquare][j] = assetPlayer1[1];
					}
					else {
						for (int j = 0; j < 12; ++j) Squares.assets[selectedSquare][j] = assetPlayer2[0];
						for (int j = 12; j < FPS; ++j) Squares.assets[selectedSquare][j] = assetPlayer2[1];
					}

					turnPlayer == 1 ? Mix_PlayChannel(-1, charmanderSound, 0) : Mix_PlayChannel(-1, squirtleSound, 0); 
					if (++turnPlayer > maxPlayers) turnPlayer = 1;

				}

				if (isBattleEnd) {

					if (winner != 'e') {
						setWinnerSpaces(lineWin, winner);
						std::cout << "The winnner is player " << winner << '\n';
						Mix_PlayMusic(winBattleMusic, -1);
					} 
				}

			} break;

			case NULL:
			default:
				break;
			}
		}
		else {

			switch (event.type) {

				case SDL_MOUSEBUTTONDOWN:
					isMenuRunning = false;
					nextMenu = 1;

				default:
					break;
			}
		}
	}
}

void GameBattleMenu::update() {
	
	Uint32 actualFrameTime = SDL_GetTicks();
	int timeWait = FRAME_TARGET_TIME - (actualFrameTime - lastFrameTime);

	if (timeWait > 0 && timeWait <= FRAME_TARGET_TIME) SDL_Delay(timeWait);

	float deltaTime = (actualFrameTime - lastFrameTime) / 1000.0f;

	lastFrameTime = actualFrameTime;
	if (++actualFrame > FPS) actualFrame = 0;
}

void GameBattleMenu::render() {

	if (flagAnimation) {

		SDL_Rect srcrect = { 0, 0, 340, 340 };
		SDL_Rect dstrect = { 0, 0, 340, 340 };

		SDL_RenderCopy(renderer, assetBackground, &srcrect, &dstrect);

		for (int i = 0; i < 9; ++i) {

			Squares.frames[i] = ++Squares.frames[i] < FPS ? Squares.frames[i] : 0;
			SDL_RenderCopy(renderer, Squares.assets[i][Squares.frames[i]], &Squares.srcrect[i], &Squares.dstrect[i]);
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

void GameBattleMenu::setWinnerSpaces(string lineWinner, char playerWinner) {

	SDL_Texture* texture0 = NULL;
	SDL_Texture* texture1 = NULL;

	if (playerWinner == 'o') {
		texture0 = assetCharmanderWin[0];
		texture1 = assetCharmanderWin[1];
	}
	else {
		texture0 = assetSquirtleWin[0];
		texture1 = assetSquirtleWin[1];
	}

	int spaces[3] = { 0, 0, 0 };

	if (lineWinner == "horizontal1") {
		spaces[0] = 0; spaces[1] = 1; spaces[2] = 2;
	}
	else if (lineWinner == "horizontal2") {
		spaces[0] = 3; spaces[1] = 4; spaces[2] = 5;
	}
	else if (lineWinner == "horizontal3") {
		spaces[0] = 6; spaces[1] = 7; spaces[2] = 8;
	}
	else if (lineWinner == "vertical1") {
		spaces[0] = 0; spaces[1] = 3; spaces[2] = 6;
	}
	else if (lineWinner == "vertical2") {
		spaces[0] = 1; spaces[1] = 4; spaces[2] = 7;
	}
	else if (lineWinner == "vertical3") {
		spaces[0] = 2; spaces[1] = 5; spaces[2] = 8;
	}
	else if (lineWinner == "diagonal1") {
		spaces[0] = 0; spaces[1] = 4; spaces[2] = 8;
	}
	else if (lineWinner == "diagonal2") {
		spaces[0] = 2; spaces[1] = 4; spaces[2] = 6;
	}

	for (int i = 0; i < 3; ++i) {
		
		for (int j = 0; j < 12; ++j) Squares.assets[ spaces[i] ][j] = texture0;
		for (int j = 12; j < FPS; ++j) Squares.assets[ spaces[i] ][j] = texture1;
	}

}

char* GameBattleMenu::setSquare(char* table, int selectedSquare, char player) {

	table[selectedSquare] = player;
	isLineComplete(table, player);

	bool isAllFull = true;
	for (int i = 0; i < 9; ++i) {
		if (table[i] == 'e') isAllFull = false;
	}

	//isBattleEnd = isAllFull;
	return table;
}

bool GameBattleMenu::isLineComplete(char* table, char player) {

	bool flag = false;

	// HORIZONTAL EVALUATION
	if (table[0] == player && table[1] == player && table[2] == player) {
		flag = true;
		lineWin = "horizontal1";
	}
	else if (table[3] == player && table[4] == player && table[5] == player) {
		flag = true;
		lineWin = "horizontal2";
	}
	else if (table[6] == player && table[7] == player && table[8] == player) {
		flag = true;
		lineWin = "horizontal3";
	}
	// VERTICAL EVALUAITON
	else if (table[0] == player && table[3] == player && table[6] == player) {
		flag = true;
		lineWin = "vertical1";
	}
	else if (table[1] == player && table[4] == player && table[7] == player) {
		flag = true;
		lineWin = "vertical2";
	}
	else if (table[2] == player && table[5] == player && table[8] == player) {
		flag = true;
		lineWin = "vertical3";
	}
	// DIAGONAL EVALUATION
	else if (table[0] == player && table[4] == player && table[8] == player) {
		flag = true;
		lineWin = "diagonal1";
	}
	else if (table[2] == player && table[4] == player && table[6] == player) {
		flag = true;
		lineWin = "diagonal2";
	}

	if (flag) {
		isBattleEnd = true;
		winner = player;
	}

	return flag;
}
// END GAME APP BATTLE ---------------------------------------------------------------------------------------------------


void GameStartMenu::setup(){
	
	backgroundTexture.texture = loadTexture("C:/Users/Admin/Desktop/Assets/startMenuBackground.png");
	backgroundTexture.srcrect = { 0, 0, 340, 340 };
	backgroundTexture.dstrect = { 0, 0, 340, 340 };
	battleTexture.texture = loadTexture("C:/Users/Admin/Desktop/Assets/battle.png");
	battleTexture.srcrect = { 0, 0, 13, 13 };
	battleTexture.dstrect = { 50, 265, 52, 52 };
	runTexture.texture = loadTexture("C:/Users/Admin/Desktop/Assets/run.png");
	runTexture.srcrect = { 0, 0, 13, 13 };
	runTexture.dstrect = { 122, 265, 52, 52 };


	battleMusic = loadMusic("C:/Users/Admin/Desktop/Assets/Audio/pokemonWildBattle.mp3");
	backgroundMusic = loadMusic("C:/Users/Admin/Desktop/Assets/Audio/palletTown.mp3");
	Mix_PlayMusic(backgroundMusic, -1);
}

void GameStartMenu::inputs() {

	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type) {

		case SDL_QUIT:
			break;

		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE) isMenuRunning = false;
			break;
		
		case SDL_MOUSEBUTTONDOWN: {
			int MouseX = event.button.x;
			int MouseY = event.button.y;

			SDL_Rect vRect = battleTexture.dstrect;
			bool flagWidth = MouseX >= vRect.x && MouseX <= vRect.x + vRect.w;
			bool flagHeight = MouseY >= vRect.y && MouseY <= vRect.y + vRect.h;
			
			if (flagWidth && flagHeight) {
				isMenuRunning = false;
				nextMenu = 2;
				Mix_PlayMusic(battleMusic, -1);
			}

			vRect = runTexture.dstrect;
			flagWidth = MouseX >= vRect.x && MouseX <= vRect.x + vRect.w;
			flagHeight = MouseY >= vRect.y && MouseY <= vRect.y + vRect.h;
			if (flagWidth && flagHeight) isMenuRunning = false;

		} break;

		default:
			break;
	}
}

void GameStartMenu::render() {

	SDL_RenderCopy(renderer, backgroundTexture.texture, &backgroundTexture.srcrect, &backgroundTexture.dstrect);

	SDL_RenderCopy(renderer, battleTexture.texture, &battleTexture.srcrect, &battleTexture.dstrect);
	SDL_RenderCopy(renderer, runTexture.texture, &runTexture.srcrect, &runTexture.dstrect);

	SDL_RenderPresent(renderer);
}

void GameStartMenu::update() {
		
	Uint32 actualFrameTime = SDL_GetTicks();
	int timeWait = FRAME_TARGET_TIME - (actualFrame - lastFrameTime);

	if (timeWait > 0 && timeWait <= FRAME_TARGET_TIME) SDL_Delay(timeWait);

	float deltaTime = (SDL_GetTicks() - lastFrameTime) / 1000.0f;
	lastFrameTime = SDL_GetTicks();
}
