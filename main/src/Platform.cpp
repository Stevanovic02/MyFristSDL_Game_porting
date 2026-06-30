#include "Platform.h"
#include "Game.h"
#include "TextureManager.h"

Platform::Platform(const char* texturePath, int x, int y, int w, int h)
    : xPos(x), yPos(y), width(w), height(h)
{
    objTexture = TextureManager::LoadTexture(texturePath);
    srcRect = { 0, 0, w, h };
    destRect = { x, y, w, h };
}

Platform::~Platform() {
    if (objTexture) {
        SDL_DestroyTexture(objTexture);
        objTexture = nullptr;
    }
}

void Platform::Update() {
    destRect.x = xPos;
    destRect.y = yPos;
    destRect.w = width;
    destRect.h = height;
}

void Platform::Render() {
    if (objTexture) {
        SDL_RenderCopy(Game::renderer, objTexture, &srcRect, &destRect);
    }
}

SDL_Rect Platform::getHitbox() const {
    return destRect;
}

void Platform::setPosition(int x, int y) {
    xPos = x;
    yPos = y;
    destRect.x = x;
    destRect.y = y;
}
