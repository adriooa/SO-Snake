#ifndef game_object_h
#define game_object_h

#include <SDL2/SDL.h>
#include "game.h"
#include "Tile.h"

#include <list>
#include <mutex>
#include <thread>
#include <semaphore.h>
#include <atomic>
using namespace std;


class GameObject
{
public:
	GameObject();
	~GameObject();

	void init(Game* g);

	void handleInput();
	void newUpdate();
	void render(SDL_Renderer* renderer);

	void gerenciarTamanhoCauda();
	void macaUpdate();
	bool testeColisao();

private:
	Game* game;
	Map* mapObj;
	int x, y;
	float xx, yy;
	float y_vel, x_vel, real_x_vel, real_y_vel;
	pair<int, int>* cauda;
	pair<int, int>* maca, * muro = nullptr;
	bool keepCauda;

	sem_t semaforoMaca, semaforoCauda;

	//mutex mutexCauda;
	atomic_bool mutexCauda;

	list<pair<int, int>> cobra;

	void muroUpdate();
	void movePlayer();
	void doKeyDown(SDL_KeyboardEvent* event);

};

#endif