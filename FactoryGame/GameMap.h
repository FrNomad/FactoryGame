#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include <chrono>
#include "Static.h"
#include "GameObject.h"
using namespace std;

class GameMap
{
private:
	Struct::Coordinate centerPos;
	double zoomLevel;
	int zoomUnit;
	vector<GameObject*> innerObjects;
	chrono::system_clock::time_point innerClockZero;
	Struct::Coordinate _absoluteCoords(Struct::Coordinate);
	void _setZoomUnit(void);
	void _renderVisibleMesh(SDL_Renderer*, Struct::Coordinate*, Struct::Coordinate*);
	void _renderObject(SDL_Renderer*, GameObject*, Struct::Coordinate*, Struct::Coordinate*);
	void _renderVisibleObjects(SDL_Renderer*, Struct::Coordinate*, Struct::Coordinate*);
public:
	GameMap() {
		this->centerPos = { 0.0, 0.0 };
		this->zoomLevel = Config::PRERENDER_ZOOMLEVEL;
		this->zoomUnit = 1;
		this->innerClockZero = chrono::system_clock::now();
	}
	void renderWholeMap(SDL_Renderer*);
	void dragByMouse(int, int);
	void changeZoomLevel(double, int, int);
	void getCurrentCenter(Struct::Coordinate*);
	unsigned long long getInnerClock(void);
	void attachObject(GameObject*);
};

