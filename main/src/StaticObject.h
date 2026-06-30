#pragma once

#include "Game.h"


class StaticObject
{
public:
	StaticObject(const char* texturesheet, int x, int y);
	~StaticObject();
	void Update();
	void Render();


private:

	int xPos, yPos;

	SDL_Texture* staticObjectTexture;
	SDL_Rect srcRect, destRect;

};