#pragma once

#include "Game.h"
#include "SDL.h"

class TextureManager
{
public:
	static SDL_Texture* LoadTexture(const char* fileName);

	// Crtanje dela teksture
	static void Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest);

	// Crtanje cele teksture (overload)
	static void Draw(SDL_Texture* tex, SDL_Rect dest);
};
