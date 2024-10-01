#include <SDL2/SDL.h>
#include <iostream>
#include <cmath>
#include "GameMap.h"
#include "Static.h"

Struct::Coordinate GameMap::_absoluteCoords(Struct::Coordinate coord)
{
	double ratio = Config::SCREEN_WIDTH / (2.0 * this->zoomLevel);

	double absoluteX = (coord.x - this->centerPos.x) * ratio + Config::SCREEN_WIDTH * 0.5;
	double absoluteY = (this->centerPos.y - coord.y) * ratio + Config::SCREEN_HEIGHT * 0.5;

	return { absoluteX, absoluteY };
}

void GameMap::_setZoomUnit(void)
{
	if (this->zoomLevel < 30) this->zoomUnit = 1;
	else if (this->zoomLevel < 60) this->zoomUnit = 2;
	else this->zoomUnit = 4;
}

void GameMap::_renderVisibleMesh(SDL_Renderer* renderer, Struct::Coordinate* min, Struct::Coordinate* max)
{

	Struct::Coordinate absoluteMin = this->_absoluteCoords(*min);
	Struct::Coordinate absoluteMax = this->_absoluteCoords(*max);
	
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

void GameMap::_renderObject(SDL_Renderer* renderer, GameObject* object, Struct::Coordinate* min, Struct::Coordinate* max)
{
	double width = object->getWidth();
	double height = object->getHeight();

	double x1 = object->getPosition().x;
	double x2 = x1 + width;
	double y1 = object->getPosition().y;
	double y2 = y1 + height;

	if (((min->x > x1 || max->x < x1) && (min->x > x2 || max->x < x2)) || (min->y > y1 || max->y < y1) && (min->y > y2 || max->y < y2)) {
		return;
	}

	SequentialGameObject* sequential = dynamic_cast<SequentialGameObject*>(object);
	if (sequential != nullptr) {
		sequential->updateFrame(this->getInnerClock());
	}

	Struct::Coordinate pos1 = this->_absoluteCoords({ x1, y2 });
	Struct::Coordinate pos2 = this->_absoluteCoords({ x2, y1 });

	SDL_Rect renderBox = { pos1.x, pos1.y, pos2.x - pos1.x, pos2.y - pos1.y };

	SDL_RenderCopyEx(renderer, object->getCurrentTexture(), NULL, &renderBox, object->getDirection(), NULL, SDL_FLIP_NONE);
}

void GameMap::_renderVisibleObjects(SDL_Renderer* renderer, Struct::Coordinate* min, Struct::Coordinate* max)
{
	for (GameObject*& object : this->innerObjects) {
		this->_renderObject(renderer, object, min, max);
	}
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

unsigned long long GameMap::getInnerClock(void)
{
	auto currentTime = std::chrono::system_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - this->innerClockZero);
	return static_cast<unsigned long long>(elapsed.count());
}

void GameMap::attachObject(GameObject* object)
{
	this->innerObjects.push_back(object);
}