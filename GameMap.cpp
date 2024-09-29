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

void GameMap::_setZoomUnit(void)
{
	if (this->zoomLevel < 20) this->zoomUnit = 1;
	else if (this->zoomLevel < 50) this->zoomUnit = 2;
	else this->zoomUnit = 4;
}

void GameMap::_renderVisibleMesh(SDL_Renderer* renderer, Struct::Coordinate* min, Struct::Coordinate* max)
{

	Struct::Coordinate absoluteMin = this->_absoluteCoords(min);
	Struct::Coordinate absoluteMax = this->_absoluteCoords(max);
	
	double ratio = Config::SCREEN_WIDTH / (2.0 * this->zoomLevel);

	for (int x = min->x; x <= max->x; x++) {
		if (x % this->zoomUnit != 0) continue;

		double absoluteX = (x - this->centerPos.x) * ratio + Config::SCREEN_WIDTH * 0.5;

		if (x % (5 * this->zoomUnit) == 0) SDL_SetRenderDrawColor(renderer, Config::MESH_OPACITY_LV2, Config::MESH_OPACITY_LV2, Config::MESH_OPACITY_LV2, 255);
		else SDL_SetRenderDrawColor(renderer, Config::MESH_OPACITY_LV1, Config::MESH_OPACITY_LV1, Config::MESH_OPACITY_LV1, 255);
		
		SDL_RenderDrawLine(renderer, absoluteX, absoluteMin.y, absoluteX, absoluteMax.y);
	}

	for (int y = min->y; y <= max->y; y++) {
		if (y % this->zoomUnit != 0) continue;

		double absoluteY = (this->centerPos.y - y) * ratio + Config::SCREEN_HEIGHT * 0.5;

		if (y % (5 * this->zoomUnit) == 0) SDL_SetRenderDrawColor(renderer, Config::MESH_OPACITY_LV2, Config::MESH_OPACITY_LV2, Config::MESH_OPACITY_LV2, 255);
		else SDL_SetRenderDrawColor(renderer, Config::MESH_OPACITY_LV1, Config::MESH_OPACITY_LV1, Config::MESH_OPACITY_LV1, 255);

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

void GameMap::dragByMouse(int dx, int dy)
{
	double ratio = (2.0 * this->zoomLevel) / Config::SCREEN_WIDTH;

	this->centerPos.x += dx * ratio;
	this->centerPos.y -= dy * ratio;
}

void GameMap::changeZoomLevel(double level, int mouseX, int mouseY)
{
	double zoomRatio = pow(Config::ZOOM_SENSITIVITY, level);
	double newZoomLevel = this->zoomLevel * zoomRatio;
	if (newZoomLevel < Config::MIN_ZOOMLEVEL || newZoomLevel > Config::MAX_ZOOMLEVEL) {
		return;
	}

	double absoluteRatio = Config::SCREEN_WIDTH / (2.0 * this->zoomLevel);
	double newX = (1.0 - zoomRatio) * (this->centerPos.x + (mouseX - Config::SCREEN_WIDTH * 0.5)/absoluteRatio) + zoomRatio * this->centerPos.x;
	double newY = (1.0 - zoomRatio) * (this->centerPos.y - (mouseY - Config::SCREEN_HEIGHT * 0.5) / absoluteRatio) + zoomRatio * this->centerPos.y;

	this->centerPos = { newX, newY };
	this->zoomLevel = newZoomLevel;
	this->_setZoomUnit();
}

void GameMap::getCurrentCenter(Struct::Coordinate* coord)
{
	coord->x = this->centerPos.x;
	coord->y = this->centerPos.y;
}
