#pragma once

#include <SDL2/SDL.h>
#include<iostream>
using namespace std;

namespace Config {

	const int SCREEN_HEIGHT = (int)1080 * 0.75;
	const int SCREEN_WIDTH = (int)(16.0 / 9.0 * SCREEN_HEIGHT);

	const double PRERENDER_ZOOMLEVEL = 15.0;
	const double MIN_ZOOMLEVEL = 3.0;
	const double MAX_ZOOMLEVEL = 100.0;

	const Uint8 MESH_OPACITY = 100;

}

namespace Color {

	const SDL_Color BG_COLOR = { 0, 31, 63 };
	const SDL_Color WHITE = { 255, 255, 255 };

}

namespace Struct {

	typedef struct {
		double x;
		double y;
	} Coordinate;

}

bool init(SDL_Window*&, SDL_Renderer*&, int, int);
SDL_Texture* loadTexture(const string&, SDL_Renderer*);