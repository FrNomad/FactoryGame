#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include "static.h"
using namespace std;

class GameMap
{
private:
	Struct::Coordinate centerPos;
	double zoomLevel;
	Struct::Coordinate _absoluteCoords(Struct::Coordinate*);
	void _renderVisibleMesh(SDL_Renderer*, Struct::Coordinate*, Struct::Coordinate*);
	void _renderVisibleObjects(SDL_Renderer*, Struct::Coordinate*, Struct::Coordinate*);
public:
	GameMap() {
		this->centerPos = { 0.0, 0.0 };
		this->zoomLevel = Config::PRERENDER_ZOOMLEVEL;
	}
	void renderWholeMap(SDL_Renderer*);
	void moveWholeMap(Struct::Coordinate*);
	void changeZoomLevel(double, int, int);
};

