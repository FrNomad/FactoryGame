#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include "Static.h"
#include "GameMap.h"
using namespace std;

int mainLoop(void* param) {

    Struct::ThreadParameter* tParam = (Struct::ThreadParameter*)(param);

    while (!(*tParam->quit)) {

        tParam->gameMap->renderWholeMap(tParam->renderer);

        SDL_RenderPresent(tParam->renderer);
    }
    return 0;
}

int eventLoop(void* param) {

    Struct::ThreadParameter* tParam = (Struct::ThreadParameter*)(param);
    SDL_Event e;

    int mouseX, mouseY;
    int offsetX, offsetY;
    bool isDragging = false;

    while (!(*tParam->quit)) {

        SDL_GetMouseState(&mouseX, &mouseY);

        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_MOUSEWHEEL) {
                tParam->gameMap->changeZoomLevel(-e.wheel.y, mouseX, mouseY);
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN) {
                if (e.button.button == SDL_BUTTON_MIDDLE) {
                    isDragging = true;
                    offsetX = mouseX;
                    offsetY = mouseY;
                }
            }
            else if (e.type == SDL_MOUSEBUTTONUP) {
                if (e.button.button == SDL_BUTTON_MIDDLE) {
                    isDragging = false;
                }
            }
            else if (e.type == SDL_MOUSEMOTION) {
                if (isDragging) {
                    tParam->gameMap->dragByMouse(offsetX - mouseX, offsetY - mouseY);
                    offsetX = mouseX;
                    offsetY = mouseY;
                }
            }
            else if (e.type == SDL_QUIT) {
                *(tParam->quit) = true;
            }
            if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                case SDLK_ESCAPE:
                    *(tParam->quit) = true;
                    break;
                case SDLK_UP:
                    tParam->object->moveBy({ 0, 1 });
                    break;
                case SDLK_DOWN:
                    tParam->object->moveBy({ 0, -1 });
                    break;
                case SDLK_LEFT:
                    tParam->object->moveBy({ -1, 0 });
                    break;
                case SDLK_RIGHT:
                    tParam->object->moveBy({ 1, 0 });
                    break;
                case SDLK_r:
                    tParam->object->rotate(Struct::ROTATE_CW);
                    break;
                }
            }
        }
    }
    return 0;
}

int main(int argc, char* args[]) {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    if (!init(window, renderer, Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT)) {
        cerr << "Failed to initialize!" << endl;
        return -1;
    }

    bool quit = false;

    GameMap* gameMap = new GameMap();
    SequentialGameObject* sample = new SequentialGameObject(renderer, 7, 5, 15, "textures/factorio_biter.png", { -3, -3 }, Struct::DIR_SOUTH, 8, 2);
    gameMap->attachObject(sample);

    Struct::ThreadParameter tParam = { renderer, gameMap, sample, &quit };

    SDL_Thread* mainThread = SDL_CreateThread(mainLoop, "mainLoop", (void*)&tParam);
    SDL_Thread* eventThread = SDL_CreateThread(eventLoop, "eventLoop", (void*)&tParam);

    int mainResult, eventResult;
    SDL_WaitThread(mainThread, &mainResult);
    SDL_WaitThread(eventThread, &eventResult);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}
