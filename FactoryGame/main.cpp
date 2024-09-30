#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include "Static.h"
#include "GameMap.h"
using namespace std;

void mainLoop(SDL_Renderer* renderer) {
    bool quit = false;
    SDL_Event e;

    GameMap* gameMap = new GameMap();
    Dulgi* dulgi = new Dulgi(renderer);
    gameMap->attachObject(dulgi);

    int mouseX, mouseY;
    int offsetX, offsetY;
    bool isDragging = false;

    while (!quit) {

        SDL_GetMouseState(&mouseX, &mouseY);

        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_MOUSEWHEEL) {
                gameMap->changeZoomLevel(-e.wheel.y, mouseX, mouseY);
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN) {
                if (e.button.button == SDL_BUTTON_MIDDLE) {
                    isDragging = true;
                    offsetX = mouseX;
                    offsetY = mouseY;
                }
                else if (e.button.button == SDL_BUTTON_LEFT) {
                    dulgi->changeFrame();
                }
            }
            else if (e.type == SDL_MOUSEBUTTONUP) {
                if (e.button.button == SDL_BUTTON_MIDDLE) {
                    isDragging = false;
                }
            }
            else if (e.type == SDL_MOUSEMOTION) {
                if (isDragging) {
                    gameMap->dragByMouse(offsetX - mouseX, offsetY - mouseY);
                    offsetX = mouseX;
                    offsetY = mouseY;
                }
            }
            else if (e.type == SDL_QUIT) {
                quit = true;
            }
            if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                case SDLK_ESCAPE:
                    quit = true;
                    break;
                case SDLK_UP:
                    dulgi->moveBy({ 0, 1 });
                    break;
                case SDLK_DOWN:
                    dulgi->moveBy({ 0, -1 });
                    break;
                case SDLK_LEFT:
                    dulgi->moveBy({ -1, 0 });
                    break;
                case SDLK_RIGHT:
                    dulgi->moveBy({ 1, 0 });
                    break;
                case SDLK_r:
                    dulgi->rotate(Struct::ROTATE_CW);
                    break;
                }
            }
        }

        gameMap->renderWholeMap(renderer);

        SDL_RenderPresent(renderer);
    }
}

int main(int argc, char* args[]) {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    if (!init(window, renderer, Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT)) {
        cerr << "Failed to initialize!" << endl;
        return -1;
    }

    mainLoop(renderer);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}
