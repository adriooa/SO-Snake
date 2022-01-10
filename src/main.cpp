
#include <SDL2/SDL.h>
#include "game.h"
#include "gameObject.h"
using namespace std;

//Essa funcao apenas chamara as funcoes da classe game que é responsavel por gerenciar o jogo em si
int main(int argc, char* argv[])
{
	//Cria o objeto do jogo
	Game* game = new Game();
	game->init();

	//Display da tela do menu
	game->menu();
	//Aguarda entrada do usuario para o jogo iniciar
	SDL_Event event;
	while (1)
	{
		SDL_PollEvent(&event);
		if (event.type == SDL_KEYDOWN)
			break;
	}

	//Inicia o jogo
	game->start();
	//Inicia o game loop
	game->threadLoop();

	//Nesse ponto o jogo ja acabou (game over)
	//Loop da tela do game over
	while (1)
	{

		//Display da tela do game over
		game->gameOver();

		//Limpa a entrada de eventos para esperar o evento de pressionar uma tecla
		SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
		while (!(SDL_PollEvent(&event) && (event.type == SDL_KEYDOWN || event.type == SDL_QUIT))) {}

		//Free nos objetos do jogo
		game->free();

		//Checa se a tecla apertada foi o 'x'
		if (event.type == SDL_QUIT || event.key.keysym.sym == SDLK_x)
		{
			//Se o evento foi fechar o jogo ou pressionar 'x' fecha o jogo
			break;
		}

		//Inicia outro jogo
		game->start();
		game->threadLoop();
	}

	//Fecha o jogo e as janelas
	game->exit();

	delete game;

	return 0;
}