#include "Game.h"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_main.h"
#include "GameObject.h"
#include <iostream>

Game* game = nullptr;

int main(int argc, char* argv[])
{
    const int FPS = 60;
    const int frameDelay = 1000 / FPS;

    Uint32 frameStart;
    int frameTime;

    bool restart = false;

    do
    {
        game = new Game();
        game->init("Rambo 2D expedition", 1600, 960, false);

        while (game->running())
        {
            frameStart = SDL_GetTicks();

            game->handleEvents();

            if (!game->isPaused)
                game->update();

            game->render();

            frameTime = SDL_GetTicks() - frameStart;

            if (frameDelay > frameTime)
                SDL_Delay(frameDelay - frameTime);
        }

        restart = game->shouldRestart();

        game->clean();
        delete game;
        game = nullptr;

    } while (restart);

    SDL_Quit();
    return 0;
}
