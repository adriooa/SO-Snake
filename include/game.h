#ifndef game_h
#define game_h

#include <SDL2/SDL.h>
#include "game.h"
#include "Tile.h"

class Game
{
public:
	Game();
	~Game();

	bool running();
	void setRunning(bool v);

	void init();
	void exit();

	void menu();
	void gameOver();

	void start();
	void free();

	void handleInput();
	void newUpdate();
	void newRender();
	void threadLoop();


private:
	SDL_Window* mywindow;
	SDL_Renderer* myrenderer;
	bool isrunning = true;
};


#endif
