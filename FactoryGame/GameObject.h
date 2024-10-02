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
    GameObject(SDL_Renderer* renderer, int width, int height, const std::string& filename, Struct::Coordinate coord = { 0, 0 }, Struct::Direction direction = Struct::DIR_SOUTH, int frame_number_width = 1, int frame_number_height = 1)
        : width(width), height(height), textureDir(filename), leftBelowPoint(coord), direction(direction), frameNumber(frame_number_width * frame_number_height) {
        this->textureList = loadTexture(filename, renderer, frame_number_width, frame_number_height);
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
    virtual void gotoNextFrame(int);
    virtual Struct::Coordinate getPosition(void);
    virtual int getWidth(void);
    virtual int getHeight(void);
    virtual Struct::Direction getDirection(void);
};

class SequentialGameObject : public GameObject
{
protected:
    double frameSpeed;
    unsigned long long preRenderPoint=0;
public:
    SequentialGameObject(SDL_Renderer* renderer, int width, int height, double initial_framespeed, const std::string& filename, Struct::Coordinate coord = { 0, 0 }, Struct::Direction direction = Struct::DIR_SOUTH, int frame_number_width = 1, int frame_number_height = 1)
        : GameObject(renderer, width, height, filename, coord, direction, frame_number_width, frame_number_height) {
        this->frameSpeed = initial_framespeed;
    }
    virtual void setFrameSpeed(double);
    virtual void updateFrame(long long);
};