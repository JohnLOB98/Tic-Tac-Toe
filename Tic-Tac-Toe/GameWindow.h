#pragma once
#include <iostream>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#define WINDOW_HEIGHT 340
#define WINDOW_WIDTH 340

#define FPS 24
#define FRAME_TARGET_TIME (1000 / FPS)

#define SquareWidth 100


class GameApp {
public:

	SDL_Window* window;
	SDL_Renderer* renderer;
	int actualFrame = 0; // min 0, max 24 | FPS Macro
	int nextMenu = -1;

	bool isMenuRunning = true;

	void destroyWindow() const;

	//bool initializeGame();

	virtual void setup() {};
	virtual void inputs() {};
	virtual void update() {};
	virtual void render() {};

	SDL_Texture* loadTexture(const char* filePath);
	Mix_Music* loadMusic(const char* filePath);


};

class GameBattleMenu : public GameApp {
public:

	// COOMON VARIABLES
	bool flagAnimation = false;

	int lastFrameTime;
	int selectedSquare = 0;
	int turnPlayer = 1;
	int actualFramesAnimation = 0;

	char table[9];
	char winner;

	const int totalFramesAnimation = 64;
	const int maxPlayers = 2;
	const char players[2] = { 'o', 'x' };

	bool isBattleEnd = false;

	// ASSETS VAIRABLES
	SDL_Rect Squares[9] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
	SDL_Texture* assetBackground = NULL;
	SDL_Texture* assetEmptySpace = NULL;
	SDL_Texture* assetPlayer1[2] = { NULL, NULL };
	SDL_Texture* assetPlayer2[2] = { NULL, NULL };
	SDL_Texture* assetsTransitionBattle = NULL;

	// MUSIC VARIABLES
	Mix_Music* winBattleMusic = NULL;

	
	// 
	virtual void setup() override;
	virtual void inputs() override;
	virtual void update() override;
	virtual void render() override;

	
	char* setSquare(char* table, int selectedSquare, char player);
	bool isLineComplete(char* table, char player);
};



class GameStartMenu : public GameApp {
public:
	Uint32 lastFrameTime = 0;

	const int widthButton = 44;
	const int heightButton = 44;

	SDL_Rect Buttons[3] = { NULL, NULL, NULL };
	SDL_Texture* btnBattleTexture = NULL;

	Mix_Music* battleMusic = NULL;
	Mix_Music* backgroundMusic = NULL;

	virtual void setup() override;
	virtual void inputs() override;
	virtual void update() override;
	virtual void render() override;
};