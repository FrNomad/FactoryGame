#pragma once

#include <SDL2/SDL.h>
#include <iostream>
using namespace std;

namespace Config {

	const int SCREEN_HEIGHT = (int)1080 * 0.75;
	const int SCREEN_WIDTH = (int)(16.0 / 9.0 * SCREEN_HEIGHT);

	const double PRERENDER_ZOOMLEVEL = 15.0;
	const double MIN_ZOOMLEVEL = 3.0;
	const double MAX_ZOOMLEVEL = 100.0;
	const double ZOOM_SENSITIVITY = 1.1;

	const Uint8 MESH_OPACITY_LV1 = 100;
	const Uint8 MESH_OPACITY_LV2 = 200;

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

	typedef enum {
		DIR_SOUTH = 0,
		DIR_EAST = 90,
		DIR_NORTH = 180,
		DIR_WEST = 270
	} Direction;

	typedef enum {
		ROTATE_CW = 1,
		ROTATE_CCW = -1
	} RotateDir;

}

bool init(SDL_Window*&, SDL_Renderer*&, int, int);
SDL_Texture* cropTexture(SDL_Texture*, SDL_Rect, SDL_Renderer*);
SDL_Texture** loadTexture(const string&, SDL_Renderer*, int);