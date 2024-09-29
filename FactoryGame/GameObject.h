#pragma once

#include <SDL2/SDL.h>
#include "Static.h"

class GameObject
{
private:
	Struct::Coordinate leftBelowPoint;
	Struct::Direction rotate;
	int width, height;
	int frame = 0;
public:
	GameObject(Struct::Coordinate coord, int width, int height, Struct::Direction rotate=Struct::DIR_SOUTH) {
		this->leftBelowPoint = coord;
		this->width = width;
		this->height = height;
		this->rotate = rotate;
	}
	virtual void render(SDL_Renderer*) = 0;
};