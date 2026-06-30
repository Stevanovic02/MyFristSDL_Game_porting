#include "StaticObject.h"
#include "TextureManager.h"

StaticObject::StaticObject(const char* texturesheet, int x, int y)
{
	staticObjectTexture = TextureManager::LoadTexture(texturesheet);
	if (!staticObjectTexture) {
		SDL_Log("Failed to load texture for StaticObject: %s", texturesheet);
	}
	

	xPos = x;
	yPos = y;
}


void StaticObject::Update()
{

	srcRect.h = 64;
	srcRect.w = 64;
	srcRect.x = 0;
	srcRect.y = 0;

	destRect.x = xPos;
	destRect.y = yPos;
	destRect.h = srcRect.h * 2;
	destRect.w = srcRect.w * 2;
}

void StaticObject::Render()
{
	SDL_RenderCopy(Game::renderer, staticObjectTexture, &srcRect, &destRect);
}