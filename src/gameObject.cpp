#include "gameObject.h"
#include <stdio.h>
#include <algorithm>
#include <thread>
#include <time.h>
#include <iostream>
using namespace std;


GameObject::GameObject() {}
GameObject::~GameObject() {
	delete(mapObj);
	mapObj = nullptr;
}


//Inicializa o player
void GameObject::init(Game* g)
{
	game = g;

	//Iniciliza variavies
	//Posicao
	x = 10;
	y = 10;
	xx = x;
	yy = y;

	//Velocidade
	x_vel = 1;
	y_vel = 0;
	real_x_vel = x_vel;
	real_y_vel = y_vel;

	//Vetor das posicoes da cobra
	cobra.push_back({ 9,10 });
	cobra.push_back({ 10,10 });

	//Tile map
	mapObj = new Map();
	mapObj->init();

	//Outras
	maca = nullptr;
	muro = nullptr;
	cauda = nullptr;
	keepCauda = false;

	sem_init(&semaforoMaca,0,0);
	sem_init(&semaforoCauda,0,0);
	sem_post(&semaforoMaca);

	mutexCauda.store(true);

	srand(time(NULL) + 10);
}

//Funcao usada na thread macaUpdate para criar uma nova maca
void GameObject::macaUpdate()
{
	//Inicializacao
	srand(time(NULL));
	int maca_x = 0;
	int maca_y = 0;
	short field;

	//Loop enquanto o jogo rodar
	while (game->running())
	{
		// Down no semaforo, isso vai tentar gerar uma ma�a nova, mas s� ser� possivel quando o semaforo for diferente de 0
		sem_wait(&semaforoMaca);

		do
		{
			//Gera uma posicao aleatoria para ma�a
			maca_x = (rand() % 18) + 1;
			maca_y = (rand() % 18) + 1;

			field = mapObj->getField(maca_x, maca_y);
			//Checa se a posicao n�o � o corpo da cobra, mesmo assim ela pode nascer em um muro
		} while (field == 2 || field == 3);

		maca = new pair<int, int>(maca_x, maca_y);
	}
}

//Funcao para criar um novo obstaculo
void GameObject::muroUpdate()
{
	//Inicializacao
	if (muro != nullptr)
	{
		free(muro);
		muro = nullptr;
	}
	//Gera uma posicao aleatoria para muro
	int muro_x = 0;
	int muro_y = 0;
	short field;

	if (cobra.size() < 14)
	{
		//Checa se a posicao n�o � o corpo da cobra nem uma maca
		do
		{
			muro_x = (rand() % 13) + 1;
			muro_y = (rand() % 13) + 1;

			//Impedindo que o muro apareca em um raio de 2 pixels de distancia da cabe�a da cobra
			if (muro_x >= x - 2)
				muro_x += 5;
			if (muro_y >= y - 2)
				muro_y += 5;

			field = mapObj->getField(muro_x, muro_y);
		} while (field == 2 || field == 3 || (x == muro_x && y == muro_y));

		muro = new pair<int, int>(muro_x, muro_y);
	}
	else
		muro = nullptr;
}

//Funcao usada na thread gerenciarTamanhoCauda para 'deletar' a cauda da cobra que andou
void GameObject::gerenciarTamanhoCauda()
{
	while (game->running())
	{
		//down
		sem_wait(&semaforoCauda);

		if (cauda != nullptr)
		{
			free(cauda);
		}

		cauda = new pair<int, int>(cobra.front());
		//Deleta cauda antiga
		cobra.pop_front();

		//mutex up
		mutexCauda.store(true);
		mutexCauda.notify_all();
	}
}

//Testa em que a cobra bateu
bool GameObject::testeColisao()
{
	//Obtem o valor do campo da nova posicao da cabeca da cobra
	short field = mapObj->getField(x, y);

	//caso maca (comeu uma maca
	if (field == 3)
	{
		//Deleta a maca atual e gera outra
		free(maca);
		maca = nullptr;
		//Up no semaforo da maca
		sem_post(&semaforoMaca);

		//Deleta a cauda, ja que a cauda nao sera deletada
		free(cauda);
		cauda = nullptr;

		//Cria um novo muro
		this->muroUpdate();
	}

	//Caso obstaculo e corpo da cobra
	else if (field == 1 || field == 2 || field == -1)
	{
		//A cobra bateu, fechar o jogo
		return true;
	}

	//Caso nada (a cobra nao bateu)
	else if (field == 0) {
		//mutex down
		mutexCauda.store(false);

		//up
		sem_post(&semaforoCauda);
	}

	return false;
}

//Atualiza a posicao da cobra
void GameObject::movePlayer() {
	//Checa velocidades
	if (x_vel != 0)
	{
		//Muda a posicao da cabeca da cobra
		xx += 0.25 * x_vel;
		yy = static_cast<float>(y);
	}
	else if (y_vel)
	{
		xx = static_cast<float>(x);
		yy += 0.25 * y_vel;
	}

	//Posicao real
	x = static_cast<int>(xx);
	y = static_cast<int>(yy);

	pair<int, int>cabeca = cobra.back();

	//Checa se a cobra andou pra frente
	if (x != cabeca.first || y != cabeca.second)
	{
		//Incrementa no vetor de posicoes
		// A cabeca esta no fim do vetor
		cobra.push_back({ x , y });

		//Testa se a cobra bateu
		if (testeColisao() == true)
		{
			game->setRunning(false);

			// D� up no semaforo da maca, para que a thread nao fique em loop infinito, possibilitando o fechamento da thread
			sem_post(&semaforoMaca);
			sem_post(&semaforoCauda);
		}

		real_x_vel = x_vel;
		real_y_vel = y_vel;
	}
}

//Le as entradas do usuario para movimentacao da cobra
void GameObject::doKeyDown(SDL_KeyboardEvent* event)
{
	//Checa qual tecla foi pressionada e atualiza as velocidades
	if (event->keysym.sym == SDLK_w && real_y_vel != 1)
	{
		y_vel = -1;
		x_vel = 0;
	}

	else if (event->keysym.sym == SDLK_s && real_y_vel != -1)
	{
		y_vel = 1;
		x_vel = 0;
	}

	else if (event->keysym.sym == SDLK_a && real_x_vel != 1)
	{
		x_vel = -1;
		y_vel = 0;
	}

	else if (event->keysym.sym == SDLK_d && real_x_vel != -1)
	{
		x_vel = 1;
		y_vel = 0;
	}
}

//Funcao para o render
void GameObject::render(SDL_Renderer* renderer)
{
	mapObj->render(renderer);
}

//Trata a entrada do jogador
void GameObject::handleInput()
{
	SDL_Event event;

	if (SDL_PollEvent(&event))
	{
		switch (event.type)
		{

			//Checa se fechou a janela
		case SDL_QUIT:
			game->setRunning(false);
			break;

			//Checa se pressionou tecla
		case SDL_KEYDOWN:
			doKeyDown(&event.key);
			break;

		default:
			break;
		}
	}

}

//Funcao usada para gerenciar o processamento do update
void GameObject::newUpdate()
{
	movePlayer();

	//Apenas passa para a proxima etapa quando o semaforo � igual a 0
	while (mutexCauda == false) {}

	//O update ja foi feito no processamento, agora faremos no mapa
	mapObj->update(cobra.back(), cauda, maca, muro);
}
