#pragma once
#include "Game.h"


class GameObject
{
public:
	GameObject(const char* texturesheet, int x, int y, int a, float speed = 1.0f);
	~GameObject();
	void Update();
	void Render();
	void StopMovement();


	bool isMoving = false;

	SDL_Rect getDestRect() const { return destRect; }

	void setPosition(int x, int y);

private:

	int xPos, yPos;
	int a = 1;
	int directionX;
	float speedMultiplier = 1.0f;

	SDL_Texture* objectTexture;
	SDL_Rect srcRect, destRect;

	SDL_Texture* textureEnemyRun;
	SDL_Texture* textureEnemyIdle;
	SDL_Texture* currentTexture;
	SDL_Rect hitbox;


	int runFrameWidth, runFrameHeight, runCurrentFrame, runFrameCount, runAnimationSpeed;
	Uint32 lastFrameTime;
	int idleFrameWidth, idleFrameHeight, idleFrameCount, idleCurrentFrame, idleAnimationSpeed;
	Uint32 idleLastFrameTime;
	bool facingLeft;
};



//=======================================================================================================================================================================================



class MainCharacter
{
public:
	MainCharacter(const char* runTextureSheet, int x, int y);
	~MainCharacter();

	void Update();

	void handleEvent(SDL_Event& e,
#ifdef __ANDROID__
					 SDL_Rect leftButtonRect, SDL_Rect rightButtonRect,
					 SDL_Rect upButtonRect, SDL_Rect downButtonRect,
					 int windowWidth, int windowHeight
#endif
	);


	void move(Uint32 deltaTime);
	void Render();
	void StopMovement();

	bool isJumping = false;
	bool isMoving = false;
	float jumpVelocity = 0.0f;
	float gravity = 1.0f;
	float jumpStrength = -23.0f;
	void updateGroundLevel(SDL_Window* window);
	int groundLevel = 0;

	SDL_Rect hitbox;

	int xPos, yPos;
	SDL_Rect destRect;

	SDL_Rect getDestRect() const { return destRect; }

	bool onPlatform = false;
	bool droppingThrough = false;
	Uint32 dropTimer = 0;

	void setPosition(int x, int y);



private:
	static constexpr double CharVelocity = 2.5;
	int mVelX, mVelY;
	bool mMovingUp, mMovingDown, mMovingLeft, mMovingRight;
	int directionX;

	SDL_Texture* textureRun;
	SDL_Texture* textureJump;
	SDL_Texture* textureIdle;
	SDL_Texture* currentTexture; 

	SDL_Rect srcRect;

	int frameWidth, frameHeight, currentFrame, frameCount, animationSpeed;
	Uint32 lastFrameTime;

	int jumpFrameWidth, jumpFrameHeight, jumpFrameCount, jumpAnimationSpeed;

	int idleFrameWidth, idleFrameHeight, idleFrameCount, idleCurrentFrame, idleAnimationSpeed;
	Uint32 idleLastFrameTime;

	bool facingRight;

};


//=======================================================================================================================================================================================


class CloudObjects
{
public:
	CloudObjects(const char* texturesheet, int x, int y);
	~CloudObjects();
	void Update();
	void Render();

	void setPosition(int x, int y);
private:

	int xPosCloud, yPosCloud;

	SDL_Texture* cloudTexture;
	SDL_Rect srcRect, destRect;

};



//=======================================================================================================================================================================================




class Particles
{
	public:
		Particles(const char* texturesheet, int x, int y);
		~Particles();
		void Update();
		void Render();
		void EnableMovement();

		SDL_Texture* textureParticleExplosion;
		SDL_Texture* currentTexture;

		SDL_Rect getDestRect() const { return destRect; }
		bool isPlaying = false;

		void setPosition(int x, int y);

	private:
		int xPos, yPos;
		SDL_Texture* particleTexture;
		SDL_Rect srcRect, destRect;
		int particleFrameWidth, particleFrameHeight, particleCurrentFrame, particleFrameCount, particleAnimationSpeed;
		Uint32 lastFrameTime;

		Uint32 lastSpawnTime;
		Uint32 nextSpawnDelay;
		bool isAnimating;

		void ResetSpawnDelay();
};
