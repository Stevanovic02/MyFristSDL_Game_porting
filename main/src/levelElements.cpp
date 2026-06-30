#include "levelElements.h"
#include "TextureManager.h"
#include "Game.h"  // zbog Game::renderer

LevelElements::LevelElements(const char* texturesheet, int x, int y)
{
    levelElementsTexture = TextureManager::LoadTexture(texturesheet);
    if (!levelElementsTexture) {
        SDL_Log("Failed to load texture for LevelElements.cpp: %s", texturesheet);
    }

    xPos = x;
    yPos = y;

    // Učitaj originalne dimenzije slike
    SDL_QueryTexture(levelElementsTexture, NULL, NULL, &width, &height);

    // Podesi početne rect vrednosti
    srcRect = { 0, 0, width, height };
    destRect = { xPos, yPos, width, height };
    hitbox = destRect;
}

LevelElements::~LevelElements()
{
    if (levelElementsTexture) {
        SDL_DestroyTexture(levelElementsTexture);
    }
}

void LevelElements::Update()
{
    destRect.x = xPos;
    destRect.y = yPos;

    // hitbox uvek prati destRect
    hitbox = destRect;
}

void LevelElements::Render()
{
    SDL_RenderCopy(Game::renderer, levelElementsTexture, &srcRect, &destRect);
}

void LevelElements::setSize(int w, int h)
{
    destRect.w = w;
    destRect.h = h;
    hitbox.w = w;
    hitbox.h = h;
}
