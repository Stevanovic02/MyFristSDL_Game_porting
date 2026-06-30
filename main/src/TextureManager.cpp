#include "TextureManager.h"
#include "Game.h"

SDL_Texture* TextureManager::LoadTexture(const char* fileName)
{
    SDL_Surface* tempSurface = IMG_Load(fileName);
    if (!tempSurface) {
        SDL_Log("Failed to load image %s: %s\n", fileName, IMG_GetError());
        return nullptr;
    }

    SDL_Texture* tex = SDL_CreateTextureFromSurface(Game::renderer, tempSurface);
    SDL_FreeSurface(tempSurface);

    if (!tex) {
        SDL_Log("Failed to create texture from surface for %s: %s\n", fileName, SDL_GetError());
    }

    return tex;
}

// Crtanje dela teksture
void TextureManager::Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest)
{
    if (!tex) {
        SDL_Log("Attempted to draw a null texture!");
        return;
    }
    if (!Game::renderer) {
        SDL_Log("Renderer is null! Cannot draw texture.");
        return;
    }
    SDL_RenderCopy(Game::renderer, tex, &src, &dest);
}

// Crtanje cele teksture (overload)
void TextureManager::Draw(SDL_Texture* tex, SDL_Rect dest)
{
    if (!tex) {
        SDL_Log("Attempted to draw a null texture!");
        return;
    }
    if (!Game::renderer) {
        SDL_Log("Renderer is null! Cannot draw texture.");
        return;
    }
    SDL_RenderCopy(Game::renderer, tex, nullptr, &dest); // nullptr = cela tekstura
}
