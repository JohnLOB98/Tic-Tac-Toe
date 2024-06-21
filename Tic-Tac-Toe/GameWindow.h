#pragma once
#include <iostream>
#include <vector>
#include <string>
//#include <functional>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#define WINDOW_HEIGHT 340
#define WINDOW_WIDTH 340

#define FPS 24
#define FRAME_TARGET_TIME (1000 / FPS)

#define SquareWidth 100

typedef std::string string;

//char* pathAssets;
//
//string onEmptySpace = "onEmptySpace";


class GameApp {
public:

	SDL_Window* window;
	SDL_Renderer* renderer; 
	int actualFrame = 0; // min 0, max 24 | FPS Macro
	int nextMenu = -1;

	bool isMenuRunning = true;

	//void destroyWindow() const;

	virtual void setup() {};
	virtual void inputs() {};
	virtual void update() {};
	virtual void render() {};

	SDL_Texture* loadTexture(const char* filePath);
	Mix_Music* loadMusic(const char* filePath);
	Mix_Chunk* loadSound(const char* filePath);

	void DestroyListTextures(std::vector<SDL_Texture*> animation);


	struct constantTexture {
		
		SDL_Rect srcrect;
		SDL_Rect dstrect;
		SDL_Texture* texture;

		void Destroy() {

			SDL_DestroyTexture(texture);
		}
	};
};

class GameBattleMenu : public GameApp {
public:

	// CONSTRUCTORS
	~GameBattleMenu();

	// COMMON VARIABLES
	bool flagAnimation1 = false;
	bool flagAnimation2 = false;
	bool activeReturnAnimation = false;
	bool isBattleEnd = false;

	int lastFrameTime;
	int turnPlayer = 1;
	int actualFramesAnimation = 0;

	char table[9];
	char winner = 'e';

	const int totalFramesAnimation1 = 32;
	const int totalFramesAnimation2 = 25;
	const int maxPlayers = 2;
	const char players[2] = { 'o', 'x' };
	
	string lineWin = "";

	// TEXTURES
	struct Squares {

		SDL_Rect srcrect[9] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
		SDL_Rect dstrect[9] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
		std::vector<std::vector<SDL_Texture*>> textures;
		int frames[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	} Squares;

	SDL_Texture* textureBackground = nullptr;
	SDL_Texture* textureOnLeaveEmptySpace = nullptr;
	SDL_Texture* textureOnEnterEmptySpace = nullptr;
	SDL_Texture* textureTransitionBattle = nullptr;
	SDL_Texture* animationSquares = nullptr;

	std::vector<SDL_Texture*> texturesCharmander;
	std::vector<SDL_Texture*> texturesCharmanderWin;
	std::vector<SDL_Texture*> texturesSquirtle;
	std::vector<SDL_Texture*> texturesSquirtleWin;

	constantTexture backgroundTexture;

	// MUSIC 
	Mix_Music* winBattleMusic = nullptr;
	Mix_Music* battleMusic = nullptr;

	// SOUNDS
	Mix_Chunk* charmanderSound = nullptr;
	Mix_Chunk* squirtleSound = nullptr;

	virtual void setup() override;
	virtual void inputs() override;
	virtual void update() override;
	virtual void render() override;

	// METHODS
	void setWinnerSpaces(string lineWinner, char playerWinner);
	void setSquare(int selectedSquare, char player);
	bool isLineComplete(char player);
	bool isSpaceAvailable();
};

class GameStartMenu : public GameApp {
public:
	Uint32 lastFrameTime = 0;

	const int widthButton = 44;
	const int heightButton = 44;

	SDL_Texture* btnBattleTexture = nullptr;
	SDL_Texture* textureOnEnterBtnBattle = nullptr;
	SDL_Texture* textureOnLeaveBtnBattle = nullptr;
	SDL_Texture* textureOnEnterBtnRun = nullptr;
	SDL_Texture* textureOnLeaveBtnRun = nullptr;
	constantTexture backgroundTexture;
	constantTexture battleTexture;
	constantTexture runTexture;

	Mix_Music* backgroundMusic = nullptr;

	GameStartMenu() {};
	~GameStartMenu();

	virtual void setup() override;
	virtual void inputs() override;
	virtual void update() override;
	virtual void render() override;
};