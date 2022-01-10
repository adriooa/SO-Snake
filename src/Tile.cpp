#include "Tile.h"
#include <algorithm>
#include <cstring>

Map::Map() {}
Map::~Map() {}

//Iicia o mapa
void Map::init()
{
	//Definicao da matriz o mapa
	short aux[21][21] = {
			{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,2,2,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1} };

	memcpy(tilemap, aux, sizeof(aux));
}

//Obtem o valor da matriz na cordenada dada
short Map::getField(int x, int y)
{
	if (x < 0 || x>20 || y < 0 || y>20)
		return -1;

	//Retorna o valor
	return tilemap[y][x];
}

//Update na matriz do mapa dada as novas posicoes de cabeca da cobra, cauda antiga, maca e muro
/*
 Espaco vazio - 0
 Muro - 1
 Corpo da cobra - 2
 Maca - 3
*/
void Map::update(pair<int, int> cabecaCobra, pair<int, int>* cauda, pair<int, int>* maca, pair<int, int>* muro)
{
	//seta a coordenada da cauda antiga para 0 (vazio)
	if (cauda != nullptr)
		tilemap[cauda->second][cauda->first] = 0;

	//seta a posicao da maca para 3
	if (maca != nullptr)
		tilemap[maca->second][maca->first] = 3;

	//seta muro para 1
	if (muro != nullptr)
	{
		tilemap[muro->second][muro->first] = 1;
	}

	//seta cabeca da cobra para 2
	tilemap[cabecaCobra.second][cabecaCobra.first] = 2;
}

//Display da janela apos alteracoes do update
void Map::render(SDL_Renderer* renderer)
{
	char R = 0, G = 0, B = 0;

	SDL_Rect r;
	r.w = 32;
	r.h = 32;

	int i, j;

	//Loop para cada posicao da matriz
	for (i = 0; i < DIM; i++)
	{
		for (j = 0; j < DIM; j++)
		{
			//Retangulo em questao
			r.x = j * 32;
			r.y = i * 32;
			r.w = 32;
			r.h = 32;

			//Seleciona a cor conforme o valor da coordenada na matriz
			switch (tilemap[i][j])
			{
			case 0:
				R = 1;
				G = 1;
				B = 1;
				break;

			case 1:
				R = 100;
				G = 200;
				B = 100;
				break;

			case 2:
				R = 255;
				G = 255;
				B = 255;

				r.x += 1;
				r.y += 1;
				r.w = 30;
				r.h = 30;

				break;

			case 3:
				R = 254;
				G = 0;
				B = 0;
				break;

			default:
				break;
			}

			//Printa o retangulo cobrido pela coordenada da matriz em questao
			SDL_SetRenderDrawColor(renderer, R, G, B, 255);
			SDL_RenderFillRect(renderer, &r);
		}
	}

	//Display da janela
	SDL_RenderPresent(renderer);
}
