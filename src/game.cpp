
#include "game.h"
#include "gameObject.h"
#include <thread>
#include <SDL2/SDL_image.h>

GameObject* player;


Game::Game() {}
Game::~Game() {}


bool Game::running() {
	return isrunning;
}

void Game::setRunning(bool v)
{
	isrunning = v;
}

//Configuracao inicial
void Game::init()
{
	//Inicializar SDL
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG);

	//Inicia a janela e o renderizador
	mywindow = SDL_CreateWindow("SO_Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 640, SDL_WINDOW_SHOWN);
	myrenderer = SDL_CreateRenderer(mywindow, -1, 0);
}

//Display da imagem do menu na janela
void Game::menu()
{
	SDL_Texture* texture = IMG_LoadTexture(myrenderer, "res/snake.png");

	SDL_RenderCopy(myrenderer, texture, NULL, NULL);
	SDL_RenderPresent(myrenderer);

	SDL_DestroyTexture(texture);
}

//Display da tela de game over na janela
void Game::gameOver()
{
	SDL_Texture* texture = IMG_LoadTexture(myrenderer, "res/snake game over.png");

	SDL_RenderCopy(myrenderer, texture, NULL, NULL);
	SDL_RenderPresent(myrenderer);

	SDL_DestroyTexture(texture);

}

//Criar objetos das classes necessarias
void Game::start()
{
	player = new GameObject();
	player->init(this);
	this->setRunning(true);
}

//Deleta o player (free)
void Game::free()
{
	this->setRunning(false);
	delete(player);
}

//Fecha janelas e SDL
void Game::exit()
{
	SDL_DestroyWindow(mywindow);
	SDL_DestroyRenderer(myrenderer);
	SDL_Quit();
}


//Funcao usada na thread main para ler os comandos do usuario
void Game::handleInput()
{
	while (this->running())
	{
		player->handleInput();
	}
}

//Funcao usada na thread update para processar os comandos
void Game::newUpdate()
{
	const int FPS = 30;
	const int FRAME_DELAY = 1000 / FPS;
	int frameTime;
	Uint32 frameStart;

	while (this->running())
	{
		frameStart = SDL_GetTicks();

		player->newUpdate();

		frameTime = SDL_GetTicks() - frameStart;

		if (FRAME_DELAY > frameTime)
		{
			SDL_Delay(FRAME_DELAY - frameTime);
		}
	}
}

//Funcao usada na thread render para printar as alteracoes na janela
void Game::newRender()
{
	while (this->running())
	{
		player->render(myrenderer);
	}
}

//Game loop
void Game::threadLoop()
{
	//Inializam-se as threads
	thread* macaThread = new thread(&GameObject::macaUpdate, player);
	thread* updateThread = new thread(&Game::newUpdate, this);
	thread* renderThread = new thread(&Game::newRender, this);
	thread* tamanhoCobraThread = new thread(&GameObject::gerenciarTamanhoCauda, player);

	this->handleInput();

	//Fechando as threads

	if (updateThread->joinable())
		updateThread->join();
	delete updateThread;

	if (renderThread->joinable())
		renderThread->join();
	delete renderThread;


	if (tamanhoCobraThread->joinable())
		tamanhoCobraThread->join();
	delete tamanhoCobraThread;

	if (macaThread->joinable())
		macaThread->join();
	delete macaThread;
}
