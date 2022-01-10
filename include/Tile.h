#ifndef Tile_h_
#define Tile_h_

#include <SDL2/SDL.h>
#include <list>

using namespace std;


#define DIM 21

class Map
{
public:
    Map();
    ~Map();

    short tilemap[DIM][DIM];

    void init();
    void update(pair<int, int>, pair<int, int>*, pair<int, int>*, pair<int, int>*);
    void render(SDL_Renderer* renderer);
    short getField(int, int);


};

#endif