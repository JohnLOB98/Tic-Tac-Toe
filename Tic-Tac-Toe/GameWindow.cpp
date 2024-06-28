#include "GameWindow.h"

// BEGIN GAME APP -----------------------------------------------------------------------------------------------------------
//void GameApp::destroyWindow() const {
//
//	SDL_DestroyRenderer(renderer);
//	SDL_DestroyWindow(window);
//	SDL_Quit();
//}

SDL_Texture* GameApp::loadTexture(const char* filePath) {

	SDL_Texture* texture = NULL;
	//string h = "hgola";
	//char* c = const_cast<char*>(filePath.c_str());
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

void GameApp::DestroyListTextures(std::vector<SDL_Texture*> animation) {

	for (auto& texture : animation) SDL_DestroyTexture(texture);
}
// END GAME APP ------------------------------------------------------------------------------------------------------------


// BEGIN BATTLE MENU --------------------------------------------------------------------------------------------------------
GameBattleMenu::~GameBattleMenu() {

	for (int i = 0; i < 9; ++i) DestroyListTextures(Squares.textures[i]);

	SDL_DestroyTexture(textureBackground);
	SDL_DestroyTexture(textureOnLeaveEmptySpace);
	SDL_DestroyTexture(textureTransitionBattle);
	SDL_DestroyTexture(animationSquares);
	SDL_DestroyTexture(textureOnEnterEmptySpace);

	DestroyListTextures(texturesCharmander);
	DestroyListTextures(texturesCharmanderWin);
	DestroyListTextures(texturesSquirtle);
	DestroyListTextures(texturesSquirtleWin);

	Mix_FreeMusic(battleMusic);
	Mix_FreeMusic(winBattleMusic);
	Mix_FreeChunk(charmanderSound);
	Mix_FreeChunk(squirtleSound);

	backgroundTexture.Destroy();
}

void GameBattleMenu::setup(){
	
 	textureOnEnterEmptySpace = loadTexture("Assets/onEmptySpace.png");
	textureBackground = loadTexture("Assets/backgroundPlaying.png");
	textureOnLeaveEmptySpace = loadTexture("Assets/emptyspace.png");
	texturesCharmander.push_back(loadTexture("Assets/Charmander0.png"));
	texturesCharmander.push_back(loadTexture("Assets/Charmander1.png"));
	texturesSquirtle.push_back(loadTexture("Assets/Squirtle0.png"));
	texturesSquirtle.push_back(loadTexture("Assets/Squirtle1.png"));
	texturesCharmanderWin.push_back(loadTexture("Assets/charmanderWin0.png"));
	texturesCharmanderWin.push_back(loadTexture("Assets/charmanderWin1.png"));
	texturesSquirtleWin.push_back(loadTexture("Assets/squirtleWin0.png"));
	texturesSquirtleWin.push_back(loadTexture("Assets/squirtleWin1.png"));

	textureTransitionBattle = loadTexture("Assets/transitionBattle.png");
	animationSquares = loadTexture("Assets/animationSquares.png");

	backgroundTexture.texture = loadTexture("Assets/startMenuBackground.png");
	backgroundTexture.srcrect = { 0, 0, 340, 340 };
	backgroundTexture.dstrect = { 0, 0, 340, 340 };
	
	winBattleMusic = loadMusic("Assets/Audio/winBattle.mp3");
	battleMusic = loadMusic("Assets/Audio/pokemonWildBattle.mp3");

	squirtleSound = loadSound("Assets/Audio/squirtle.mp3");
	charmanderSound = loadSound("Assets/Audio/charmander.mp3");

	Mix_PlayMusic(battleMusic, -1);

	Mix_PlayMusic(battleMusic, -1);

	int size = sizeof(table) / sizeof(table[0]);
	for (int i = 0; i < size; ++i) {

		table[i] = 'e';

		Squares.srcrect[i] = { 0, 0, 100, 100 };
		Squares.dstrect[i] = {
			4 + i % 3 * (SquareWidth + 16), // x
			4 + i / 3 * (SquareWidth + 16), // y
			SquareWidth,				 // width
			SquareWidth				 // height
		};
		Squares.textures.push_back(std::vector<SDL_Texture*>());
		for (int j = 0; j < FPS; ++j) Squares.textures[i].push_back(textureOnLeaveEmptySpace);
	}
}

void GameBattleMenu::inputs() {

	if (flagAnimation2) {
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

				if (!isSpaceAvailable()) {
					activeReturnAnimation = true;
					break;
				}

				for (int i = 0; i < 9; ++i) {

					bool flagX = MouseX >= Squares.dstrect[i].x && MouseX <= Squares.dstrect[i].x + Squares.dstrect[i].w;
					bool flagY = MouseY >= Squares.dstrect[i].y && MouseY <= Squares.dstrect[i].y + Squares.dstrect[i].h;
					if (table[i] == 'e' && flagX && flagY) {
						selectedSquare = i;
						break;
					}
				}

				if (selectedSquare > -1) {
					
					setSquare(selectedSquare, players[turnPlayer - 1]);

					if (turnPlayer == 1) {
						for (int j = 0; j < 6; ++j) Squares.textures[selectedSquare][j] = texturesCharmander[0];
						for (int j = 6; j < 12; ++j) Squares.textures[selectedSquare][j] = texturesCharmander[1];
						for (int j = 12; j < 18; ++j) Squares.textures[selectedSquare][j] = texturesCharmander[0];
						for (int j = 18; j < FPS; ++j) Squares.textures[selectedSquare][j] = texturesCharmander[1];
					}
					else {
						for (int j = 0; j < 6; ++j) Squares.textures[selectedSquare][j] = texturesSquirtle[0];
						for (int j = 6; j < 12; ++j) Squares.textures[selectedSquare][j] = texturesSquirtle[1];
						for (int j = 12; j < 18; ++j) Squares.textures[selectedSquare][j] = texturesSquirtle[0];
						for (int j = 18; j < FPS; ++j) Squares.textures[selectedSquare][j] = texturesSquirtle[1];
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
			
			case SDL_MOUSEMOTION: {

				int MouseX = event.button.x;
				int MouseY = event.button.y;

				for (int i = 0; i < 9; ++i) {

					bool flagX = MouseX >= Squares.dstrect[i].x && MouseX <= Squares.dstrect[i].x + Squares.dstrect[i].w;
					bool flagY = MouseY >= Squares.dstrect[i].y && MouseY <= Squares.dstrect[i].y + Squares.dstrect[i].h;
					
					// OnMouseEnter_emptySpace | OnMouseLeave_emptySpace
					if (table[i] == 'e') {
						SDL_Texture* texture = flagX && flagY ? textureOnEnterEmptySpace : textureOnLeaveEmptySpace;
						for (int j = 0; j < FPS; ++j) Squares.textures[i][j] = texture;
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

					activeReturnAnimation = true;


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

	//float deltaTime = (actualFrameTime - lastFrameTime) / 1000.0f;

	lastFrameTime = SDL_GetTicks();
	if (++actualFrame > FPS) actualFrame = 0;
}

void GameBattleMenu::render() {

	SDL_Rect dstrect = { 0, 0, 340, 340 };

	if (activeReturnAnimation) {
		
		if (actualFramesAnimation > totalFramesAnimation1 / 2)
			SDL_RenderCopy(renderer, backgroundTexture.texture, &backgroundTexture.srcrect, &backgroundTexture.dstrect);

		int x = actualFramesAnimation % 6 * 34; // 34 is texture width in px
		int y = actualFramesAnimation / 6 * 34;
		SDL_Rect srcrect = { x, y, 34, 34 };

		SDL_RenderCopy(renderer, textureTransitionBattle, &srcrect, &dstrect);

		if (++actualFramesAnimation > totalFramesAnimation1) {
			isMenuRunning = false;
			nextMenu = 1;
		}
	}
	else if (flagAnimation2) {

		SDL_Rect srcrect = { 0, 0, 340, 340 };
		SDL_RenderCopy(renderer, textureBackground, &srcrect, &dstrect);

		for (int i = 0; i < 9; ++i) {

			Squares.frames[i] = ++Squares.frames[i] < FPS ? Squares.frames[i] : 0;
			SDL_RenderCopy(renderer, Squares.textures[i][Squares.frames[i]], &Squares.srcrect[i], &Squares.dstrect[i]);
		}
	}
	else if (flagAnimation1) {
		
		SDL_RenderCopy(renderer, textureBackground, &dstrect, &dstrect);

		int x = actualFramesAnimation % 6 * 85; // 85 is texture width in px an 6 the columns of texture
		int y = actualFramesAnimation / 6 * 85;
		SDL_Rect srcrect = { x, y, 85, 85 };

		SDL_RenderCopy(renderer, animationSquares, &srcrect, &dstrect);

		if (++actualFramesAnimation > totalFramesAnimation2) {
			flagAnimation2 = true;
			actualFramesAnimation = 0;
		}
	}
	else {
		if (actualFramesAnimation > totalFramesAnimation1 / 2) SDL_RenderCopy(renderer, textureBackground, &dstrect, &dstrect);

		int x = actualFramesAnimation % 6 * 34; // 34 is texture width in px
		int y = actualFramesAnimation / 6 * 34;
		SDL_Rect srcrect = { x, y, 34, 34 };

		SDL_RenderCopy(renderer, textureTransitionBattle, &srcrect, &dstrect);

		if (++actualFramesAnimation > totalFramesAnimation1) {
			actualFramesAnimation = 0;
			flagAnimation1 = true;
		}

	}

	SDL_RenderPresent(renderer);
}

void GameBattleMenu::setWinnerSpaces(string lineWinner, char playerWinner) {

	SDL_Texture* texture0 = nullptr;
	SDL_Texture* texture1 = nullptr;

	if (playerWinner == 'o') {
		texture0 = texturesCharmanderWin[0];
		texture1 = texturesCharmanderWin[1];
	}
	else {
		texture0 = texturesSquirtleWin[0];
		texture1 = texturesSquirtleWin[1];
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
		
		for (int j = 0; j < 6; ++j) Squares.textures[ spaces[i] ][j] = texture0;
		for (int j = 6; j < 12; ++j) Squares.textures[ spaces[i] ][j] = texture1;
		for (int j = 12; j < 18; ++j) Squares.textures[spaces[i]][j] = texture0;
		for (int j = 18; j < FPS; ++j) Squares.textures[spaces[i]][j] = texture1;
	}

}

void GameBattleMenu::setSquare(int selectedSquare, char player) {

	table[selectedSquare] = player;
	isLineComplete(player);

	bool isAllFull = true;
	for (int i = 0; i < 9; ++i) {
		if (table[i] == 'e') isAllFull = false;
	}
}

bool GameBattleMenu::isLineComplete(char player) {

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

bool GameBattleMenu::isSpaceAvailable() {

	for (int i = 0; i < 9; ++i)
		if (table[i] == 'e') return true;

	return false;
}
// END BATTLE MENU ----------------------------------------------------------------------------------------------------------


// BEGIN START MENU --------------------------------------------------------------------------------------------------------
GameStartMenu::~GameStartMenu() {

	SDL_DestroyTexture(btnBattleTexture);
	SDL_DestroyTexture(textureOnEnterBtnBattle);
	SDL_DestroyTexture(textureOnLeaveBtnBattle);
	SDL_DestroyTexture(textureOnEnterBtnRun);
	SDL_DestroyTexture(textureOnLeaveBtnRun);

	Mix_FreeMusic(backgroundMusic);

	backgroundTexture.Destroy();
	battleTexture.Destroy();
	runTexture.Destroy();
}

void GameStartMenu::setup(){
	
	backgroundTexture.texture = loadTexture("Assets/startMenuBackground.png");
	backgroundTexture.srcrect = { 0, 0, 340, 340 };
	backgroundTexture.dstrect = { 0, 0, 340, 340 };
	battleTexture.texture = loadTexture("Assets/battle.png");
	battleTexture.srcrect = { 0, 0, 13, 13 };
	battleTexture.dstrect = { 50, 265, 52, 52 };
	runTexture.texture = loadTexture("Assets/run.png");
	runTexture.srcrect = { 0, 0, 13, 13 };
	runTexture.dstrect = { 122, 265, 52, 52 };

	textureOnEnterBtnBattle = loadTexture("Assets/onBtnBattle.png");
	textureOnLeaveBtnBattle = loadTexture("Assets/battle.png");
	textureOnEnterBtnRun = loadTexture("Assets/onBtnRun.png");
	textureOnLeaveBtnRun = loadTexture("Assets/run.png");

	backgroundMusic = loadMusic("Assets/Audio/palletTown.mp3");
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
				//Mix_PlayMusic(battleMusic, -1);
			}

			vRect = runTexture.dstrect;
			flagWidth = MouseX >= vRect.x && MouseX <= vRect.x + vRect.w;
			flagHeight = MouseY >= vRect.y && MouseY <= vRect.y + vRect.h;
			if (flagWidth && flagHeight) isMenuRunning = false;

		} break;

		case SDL_MOUSEMOTION: {

			int MouseX = event.button.x;
			int MouseY = event.button.y;

			bool flagX = MouseX >= battleTexture.dstrect.x && MouseX <= battleTexture.dstrect.x + battleTexture.dstrect.w;
			bool flagY = MouseY >= battleTexture.dstrect.y && MouseY <= battleTexture.dstrect.y + battleTexture.dstrect.h;
			
			// OnMouseEnter_btnBattle | OnMouseLeave_btnBattle
			battleTexture.texture = flagX && flagY ? textureOnEnterBtnBattle : textureOnLeaveBtnBattle;


			flagX = MouseX >= runTexture.dstrect.x && MouseX <= runTexture.dstrect.x + runTexture.dstrect.w;
			flagY = MouseY >= runTexture.dstrect.y && MouseY <= runTexture.dstrect.y + runTexture.dstrect.h;
			// OnMouseEnter_btnRun | OnMouseEnter_btnRun 
			runTexture.texture = flagX && flagY ? textureOnEnterBtnRun : textureOnLeaveBtnRun;


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
	int timeWait = FRAME_TARGET_TIME - (actualFrameTime - lastFrameTime);

	if (timeWait > 0 && timeWait <= FRAME_TARGET_TIME) SDL_Delay(timeWait);

	lastFrameTime = SDL_GetTicks();
	float deltaTime = (SDL_GetTicks() - lastFrameTime) / 1000.0f;
}
// END START MENU -------------------------------------------------------------------------------------------------------
