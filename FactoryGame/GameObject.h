#pragma once

#include <SDL2/SDL.h>
#include <string>
#include "Static.h"

class GameObject {
protected:
    const std::string& textureDir;
    Struct::Coordinate leftBelowPoint;
    Struct::Direction direction;
    int frame = 0;
    int width, height;
    int frameNumber;
    SDL_Texture** textureList = nullptr;

public:
    GameObject(SDL_Renderer* renderer, int width, int height, const std::string& filename, Struct::Coordinate coord = { 0, 0 }, Struct::Direction direction = Struct::DIR_SOUTH, int frame_number = 1)
        : width(width), height(height), textureDir(filename), leftBelowPoint(coord), direction(direction), frameNumber(frame_number) {
        this->textureList = loadTexture(filename, renderer, frame_number);
    }
    ~GameObject() {
        for (int i = 0; i < frameNumber; i++) {
            SDL_DestroyTexture(this->textureList[i]);
        }
    }
    virtual SDL_Texture* getCurrentTexture(void);
    virtual void updatePosition(Struct::Coordinate);
    virtual void moveBy(Struct::Coordinate);
    virtual void rotate(Struct::RotateDir);
    virtual Struct::Coordinate getPosition(void);
    virtual int getWidth(void);
    virtual int getHeight(void);
    virtual Struct::Direction getDirection(void);
};


class Dulgi : public GameObject
{
public:
    Dulgi(SDL_Renderer* renderer) : GameObject(renderer, 7, 5, "textures/dulgi.png", { 1, 1 }, Struct::DIR_SOUTH, 4) {
	}
    void changeFrame(void);
};