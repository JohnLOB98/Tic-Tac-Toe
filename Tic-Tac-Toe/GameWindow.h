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


//int init2 = Mix_Init(0);

//typedef enum {
//
//	MIX_INIT_FLAC = 0x000000001,
//	MIX_INIT_MOD  = 0x000000002,
//	MIX_INIT_MP3  = 0x000000008,
//	MIX_INIT_OGG  = 0x000000010,
//	MIX_INIT_MID  = 0x000000020,
//	MIX_INIT_OPUS =	0x000000040
//
//} MIX_InitFlags;


class GameApp {
public:

	SDL_Window* window;
	SDL_Renderer* renderer;
	int actualFrame = 0; // min 0, max 24 | FPS Macro

	//GameApp() : window(NULL), renderer(NULL) {};
	//GameApp(SDL_Window* vWindow, SDL_Renderer* vRenderer) : window(vWindow), renderer(vRenderer) {};
	//~GameApp() {};

	void destroyWindow() const;

	bool initializeGame();

	virtual void setup() {};
	virtual void inputs() {};
	virtual void update() {};
	virtual void render() {};

};

class GameWindowBattle : public GameApp {
public:

	// COOMON VARIABLES
	bool gameRunning = 1;
	bool flagAnimation = false;

	int lastFrameTime;
	int selectedSquare = 0;
	int turnPlayer = 1;
	int MouseX = 0;
	int MouseY = 0;
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
	Mix_Music* battleMusic = NULL;
	Mix_Music* winBattleMusic = NULL;

	
	// 
	virtual void setup() override;
	virtual void inputs() override;
	virtual void update() override;
	virtual void render() override;

	
	char* setSquare(char* table, int selectedSquare, char player);
	bool isLineComplete(char* table, char player);

	SDL_Texture* loadTexture(const char* filePath);
	//void loadAnimation(SDL_Texture* animation, const char* filePath, int frames);
	Mix_Music* loadMusic(const char* filePath);
};

class GameStartMenu : public GameApp {
public:
	int MouseX = 0;
	int MouseY = 0;
	Uint32 lastFrameTime = 0;

	const int widthButton = 100;
	const int heightButton = 50;

	SDL_Rect Buttons[3] = { NULL, NULL, NULL };


	virtual void setup() override;
	virtual void inputs() override;
	virtual void update() override;
	virtual void render() override;
};