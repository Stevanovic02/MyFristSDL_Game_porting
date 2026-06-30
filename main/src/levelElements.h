#pragma once
#include "SDL.h"

class LevelElements
{
public:
    LevelElements(const char* texturesheet, int x, int y);
    ~LevelElements();
    void Update();
    void Render();
    SDL_Rect getHitbox() const { return hitbox; }

    void setSize(int w, int h);   // 🔹 Dodato

private:
    int xPos, yPos;
    int width, height;            // 🔹 Čuvamo originalne dimenzije

    SDL_Texture* levelElementsTexture;
    SDL_Rect srcRect, destRect;
    SDL_Rect hitbox;
};
