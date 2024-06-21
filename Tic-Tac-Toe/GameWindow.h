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

class GameApp {
public:

	SDL_Window* window;
	SDL_Renderer* renderer;
	int actualFrame = 0; // min 0, max 24 | FPS Macro
	int nextMenu = -1;

	bool isMenuRunning = true;

	void destroyWindow() const;

	virtual void setup() {};
	virtual void inputs() {};
	virtual void update() {};
	virtual void render() {};

	SDL_Texture* loadTexture(const char* filePath);
	Mix_Music* loadMusic(const char* filePath);
	Mix_Chunk* loadSound(const char* filePath);


	struct constantTexture {
		
		SDL_Rect srcrect;
		SDL_Rect dstrect;
		SDL_Texture* texture;

		void Destroy() {

			SDL_DestroyTexture(texture);
		}
		//void onMouseEnter(SDL_Texture* a, std::function<void(SDL_Texture*)> func) { return func(a); }
		//void onMouseLeave(SDL_Texture* a, std::function<void(SDL_Texture*)> func) { return func(a); }

		//constantTexture(SDL_Rect& vsrcrect, SDL_Rect& vdstrect, string filePath) :
		//	srcrect(vsrcrect), dstrect(vdstrect) {
		//	
		//	texture = loadTexture("hi");
		//};
	};
};

class GameBattleMenu : public GameApp {
public:

	// COOMON VARIABLES
	bool flagAnimation = false;

	int lastFrameTime;
	int turnPlayer = 1;
	int actualFramesAnimation = 0;

	char table[9];
	char winner = 'e';

	const int totalFramesAnimation = 64;
	const int maxPlayers = 2;
	const char players[2] = { 'o', 'x' };
	string lineWin = "";

	bool isBattleEnd = false;

	// ASSETS
	struct Squares {

		SDL_Rect srcrect[9] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
		SDL_Rect dstrect[9] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
		SDL_Texture* assets[9][24];
		int frames[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	} Squares;
	SDL_Texture* assetBackground = NULL;
	SDL_Texture* assetEmptySpace = NULL;
	SDL_Texture* assetPlayer1[2] = { NULL, NULL };
	SDL_Texture* assetPlayer2[2] = { NULL, NULL };
	SDL_Texture* assetCharmanderWin[2] = { NULL, NULL };
	SDL_Texture* assetSquirtleWin[2] = { NULL, NULL };
	SDL_Texture* assetsTransitionBattle = NULL;

	// MUSIC 
	Mix_Music* winBattleMusic = nullptr;
	Mix_Music* battleMusic = nullptr;

	// SOUNDS
	Mix_Chunk* charmanderSound = nullptr;
	Mix_Chunk* squirtleSound = nullptr;

	
	// 
	virtual void setup() override;
	virtual void inputs() override;
	virtual void update() override;
	virtual void render() override;

	void setWinnerSpaces(string lineWinner, char playerWinner);
	char* setSquare(char* table, int selectedSquare, char player);
	bool isLineComplete(char* table, char player);
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
	~GameStartMenu() { std::cout << "The window has been destroyed."; };


	virtual void setup() override;
	virtual void inputs() override;
	virtual void update() override;
	virtual void render() override;
};