#include <SDL2/SDL.h>
#include <cmath>
#include "GameMap.h"
#include "static.h"

Struct::Coordinate GameMap::_absoluteCoords(Struct::Coordinate* coord)
{
	double ratio = Config::SCREEN_WIDTH / (2.0 * this->zoomLevel);

	double absoluteX = (coord->x - this->centerPos.x) * ratio + Config::SCREEN_WIDTH * 0.5;
	double absoluteY = (this->centerPos.y - coord->y) * ratio + Config::SCREEN_HEIGHT * 0.5;

	return { absoluteX, absoluteY };
}

void GameMap::_renderVisibleMesh(SDL_Renderer* renderer, Struct::Coordinate* min, Struct::Coordinate* max)
{

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, Config::MESH_OPACITY);

	Struct::Coordinate absoluteMin = this->_absoluteCoords(min);
	Struct::Coordinate absoluteMax = this->_absoluteCoords(max);
	
	double ratio = Config::SCREEN_WIDTH / (2.0 * this->zoomLevel);

	for (int x = min->x; x <= max->x; x++) {
		double absoluteX = (x - this->centerPos.x) * ratio + Config::SCREEN_WIDTH * 0.5;
		SDL_RenderDrawLine(renderer, absoluteX, absoluteMin.y, absoluteX, absoluteMax.y);
	}

	for (int y = min->y; y <= max->y; y++) {
		double absoluteY = (this->centerPos.y - y) * ratio + Config::SCREEN_HEIGHT * 0.5;
		SDL_RenderDrawLine(renderer, absoluteMin.x, absoluteY, absoluteMax.x, absoluteY);
	}

}

void GameMap::_renderVisibleObjects(SDL_Renderer* renderer, Struct::Coordinate* min, Struct::Coordinate* max)
{
	return;
}

void GameMap::renderWholeMap(SDL_Renderer* renderer)
{

	SDL_SetRenderDrawColor(renderer, Color::BG_COLOR.r, Color::BG_COLOR.g, Color::BG_COLOR.b, 255);
	SDL_RenderClear(renderer); //draw bg

	double heightByWidth = ((double)Config::SCREEN_HEIGHT) / ((double)Config::SCREEN_WIDTH);
	int minX = (int)floor(this->centerPos.x - this->zoomLevel) - 1;
	int maxX = (int)ceil(this->centerPos.x + this->zoomLevel) + 1;
	int minY = (int)floor(this->centerPos.y - this->zoomLevel * heightByWidth) - 1;
	int maxY = (int)ceil(this->centerPos.y + this->zoomLevel * heightByWidth) + 1;

	Struct::Coordinate min = { minX, minY };
	Struct::Coordinate max = { maxX, maxY };

	this->_renderVisibleMesh(renderer, &min, &max);
	this->_renderVisibleObjects(renderer, &min, &max);

	return;

}

void GameMap::moveWholeMap(Struct::Coordinate* coord)
{
	this->centerPos = *coord;
}

void GameMap::changeZoomLevel(double level, int mouseX, int mouseY)
{
	this->zoomLevel += level;
	if (this->zoomLevel < Config::MIN_ZOOMLEVEL) {
		this->zoomLevel = Config::MIN_ZOOMLEVEL;
		return;
	}
	if (this->zoomLevel > Config::MAX_ZOOMLEVEL) {
		this->zoomLevel = Config::MAX_ZOOMLEVEL;
		return;
	}
}
