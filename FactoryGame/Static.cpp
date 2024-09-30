#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

#include "Static.h"
using namespace std;

bool init(SDL_Window*& window, SDL_Renderer*& renderer, int width, int height) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
        return false;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << endl;
        return false;
    }

    window = SDL_CreateWindow("FactoryGame",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        width, height, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << endl;
        return false;
    }
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    return true;
}

SDL_Texture* cropTexture(SDL_Texture* texture, SDL_Rect rect, SDL_Renderer* renderer)
{
    SDL_Texture* newTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, rect.w, rect.h);
    SDL_SetTextureBlendMode(newTexture, SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(renderer, newTexture);
    SDL_RenderCopy(renderer, texture, &rect, NULL);
    SDL_SetRenderTarget(renderer, NULL);

    return newTexture;
}

SDL_Texture** loadTexture(const string& path, SDL_Renderer* renderer, int frame_number=1)
{
    SDL_Texture** textureList = new SDL_Texture * [frame_number];

    SDL_Surface* surface = IMG_Load(path.c_str());
    if (surface == nullptr) {
        cerr << "Unable to load image! SDL_image Error: " << IMG_GetError() << endl;
        return nullptr;
    }
    SDL_Texture* baseTexture = SDL_CreateTextureFromSurface(renderer, surface);
    if (baseTexture == nullptr) {
        cerr << "Unable to create texture from surface! SDL_Error: " << SDL_GetError() << endl;
        return nullptr;
    }

    SDL_FreeSurface(surface);

    int width, height;
    SDL_QueryTexture(baseTexture, NULL, NULL, &width, &height);
    int widthUnit = (int)(width / frame_number);

    for (int i = 0; i < frame_number; i++) {
        textureList[i] = cropTexture(baseTexture, { widthUnit * i, 0, widthUnit, height - 1 }, renderer);
        if (textureList[i] == nullptr) {
            cerr << "Unable to crop the texture! SDL_Error: " << SDL_GetError() << endl;
            return nullptr;
        }
    }

    SDL_DestroyTexture(baseTexture);

    return textureList;
}