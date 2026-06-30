#include "SDL.h"
#include "GameObject.h"
#include "Game.h"
#include "TextureManager.h"
#include "iomanip"
#include <vector>
#include "Platform.h"


extern Game* game;

GameObject::GameObject(const char* texturesheet, int x, int y, int a = 1, float speed)
{
	objectTexture = TextureManager::LoadTexture(texturesheet);
	if (!objectTexture) {
		SDL_Log("Failed to load texture for GameObject: %s", texturesheet);
	}

	xPos = x;
	yPos = y;
	directionX = a;
	speedMultiplier = speed;

	runFrameWidth = 62;
	runFrameHeight = 62;
	runFrameCount = 8;
	runAnimationSpeed = 150;

	idleFrameWidth = 62;
	idleFrameHeight = 62;
	idleFrameCount = 8;
	idleAnimationSpeed = 150;

	textureEnemyRun = TextureManager::LoadTexture("EnemyRun1.png");
	textureEnemyIdle = TextureManager::LoadTexture("EnemyIdle1.png");

	currentTexture = textureEnemyRun;

}


void GameObject::Update()
{
	xPos += directionX * speedMultiplier;

	int winW = 0, winH = 0;
	SDL_GetWindowSize(Game::window, &winW, &winH);

    int offset = 50; // koliko može preko ivice da ide enemy

    if (xPos <= -offset || xPos >= winW - destRect.w + offset) {
        directionX *= -1;
    }


    directionX = (directionX > 0) ? 1 : -1; // directionX is always 1 or -1

	facingLeft = directionX > 0;

	isMoving = true;

	
	std::string directionStr = (directionX > 0) ? "RIGHT" : "LEFT";

	std::cout << "\033[31m\rZOMBIE xPos: " << std::setw(5) << xPos
		<< " | Enemy Direction: " << std::setw(2) << directionStr << "\033[0m" << std::flush;

	std::string state;
	if (isMoving) {
		state = facingLeft ? "RUNNING RIGHT" : "RUNNING LEFT";
	}
	else {
		state = facingLeft ? "IDLE RIGHT" : "IDLE LEFT";
	}

	if (!game->isPaused) {
		std::cout << "\033[31m\rZOMBIE xPos: " << std::setw(5) << xPos
			<< " | Enemy State: " << std::setw(13) << state << "\033[0m" << std::flush;
	}


	srcRect.h = 64;
	srcRect.w = 64;
	srcRect.x = 0;
	srcRect.y = 0;

	destRect.x = xPos;
	destRect.y = yPos;
	destRect.h = srcRect.h * 2;
	destRect.w = srcRect.w * 2;

	Uint32 now = SDL_GetTicks();

	if (isMoving)
	{
		if (now - lastFrameTime > runAnimationSpeed) {
			runCurrentFrame = (runCurrentFrame + 1) % runFrameCount;
			lastFrameTime = now;
		}

		srcRect.x = (runCurrentFrame % 3) * runFrameWidth;
		srcRect.y = (runCurrentFrame / 3) * runFrameHeight;
		srcRect.w = runFrameWidth;
		srcRect.h = runFrameHeight;

		destRect.w = runFrameWidth * 3;
		destRect.h = runFrameHeight * 3;

		currentTexture = textureEnemyRun;
	}
	else
	{
		if (now - idleLastFrameTime > idleAnimationSpeed) {
			idleCurrentFrame = (idleCurrentFrame + 1) % idleFrameCount;
			idleLastFrameTime = now;
		}

		srcRect.x = (idleCurrentFrame % 3) * idleFrameWidth;
		srcRect.y = (idleCurrentFrame / 3) * idleFrameHeight;
		srcRect.w = idleFrameWidth;
		srcRect.h = idleFrameHeight;

		destRect.w = idleFrameWidth * 3;
		destRect.h = idleFrameHeight * 3;

		currentTexture = textureEnemyRun;
	}

	destRect.x = xPos;
	destRect.y = yPos - ((destRect.h / 2) - 48);

	SDL_GetWindowSize(Game::window, &winW, &winH);

	if (xPos < -100)
		xPos = -100;
	if (xPos > winW - destRect.w + 100)
		xPos = winW - destRect.w + 100;

	if (yPos < 0)
		yPos = 0;
	if (yPos > winH - destRect.h)
		yPos = winH - destRect.h;

	destRect.x = xPos;
	destRect.y = yPos;


	hitbox = destRect;
}


void GameObject::Render()
{
	SDL_RendererFlip flip = facingLeft ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

	if (destRect.w == 0 || destRect.h == 0) {
		SDL_Log("WARNING: destRect.w or h is 0! w: %d, h: %d", destRect.w, destRect.h);
		SDL_Log("srcRect: x=%d y=%d w=%d h=%d", srcRect.x, srcRect.y, srcRect.w, srcRect.h);

	}

	SDL_RenderCopyEx(Game::renderer, currentTexture, &srcRect, &destRect, 0, NULL, flip);
}

void GameObject::StopMovement()
{
	directionX = 0;
	isMoving = false;
}





//=======================================================================================================================================================================================






MainCharacter::MainCharacter(const char* runTextureSheet, int x, int y)
{
	xPos = x;
	yPos = y;

	directionX = 1;
	facingRight = true;
	mVelX = CharVelocity;
	mVelY = CharVelocity;
	mMovingUp = mMovingDown = mMovingLeft = mMovingRight = false;
	currentFrame = 0;
	lastFrameTime = 0;

	frameWidth = 66;      
	frameHeight = 46;
	frameCount = 7;       
	animationSpeed = 100; 

	jumpFrameWidth = 66;
	jumpFrameHeight = 46;
	jumpFrameCount = 3;
	jumpAnimationSpeed = 50;

	idleFrameWidth = 66;
	idleFrameHeight = 46;
	idleFrameCount = 6;
	idleCurrentFrame = 0;
	idleAnimationSpeed = 150;
	idleLastFrameTime = 0;

	textureRun = TextureManager::LoadTexture(runTextureSheet);
	textureJump = TextureManager::LoadTexture("WarriorJumpSpriteSheet.png");
	textureIdle = TextureManager::LoadTexture("WarriorIdleSpriteSheet.png");

	currentTexture = textureRun;
}

MainCharacter::~MainCharacter()
{
	if (textureRun) SDL_DestroyTexture(textureRun);
	if (textureJump) SDL_DestroyTexture(textureJump);
	if (textureIdle) SDL_DestroyTexture(textureIdle);
}


void MainCharacter::handleEvent(SDL_Event& e,
								SDL_Rect leftButtonRect, SDL_Rect rightButtonRect,
								SDL_Rect upButtonRect, SDL_Rect downButtonRect,
								int windowWidth, int windowHeight)
{
#ifdef _WIN32
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
        switch (e.key.keysym.sym) {
        case SDLK_w: case SDLK_UP: case SDLK_SPACE:
            if (!isJumping) { isJumping = true; jumpVelocity = jumpStrength; }
            break;
        case SDLK_a: case SDLK_LEFT: mMovingLeft = true; facingRight = false; isMoving = true; break;
        case SDLK_d: case SDLK_RIGHT: mMovingRight = true; facingRight = true; isMoving = true; break;
        case SDLK_s: case SDLK_DOWN: if (onPlatform) { droppingThrough = true; dropTimer = SDL_GetTicks(); } break;
        }
    } else if (e.type == SDL_KEYUP && e.key.repeat == 0) {
        switch (e.key.keysym.sym) {
        case SDLK_a: case SDLK_LEFT: mMovingLeft = false; isMoving = mMovingRight; break;
        case SDLK_d: case SDLK_RIGHT: mMovingRight = false; isMoving = mMovingLeft; break;
        }
    }
#endif // _WIN32

#ifdef __ANDROID__
	if (e.type == SDL_FINGERDOWN) {
		int x = static_cast<int>(e.tfinger.x * windowWidth);
		int y = static_cast<int>(e.tfinger.y * windowHeight);
		SDL_Point p = { x, y };

		if (SDL_PointInRect(&p, &leftButtonRect)) { mMovingLeft = true; facingRight = false; isMoving = true; }
		else if (SDL_PointInRect(&p, &rightButtonRect)) { mMovingRight = true; facingRight = true; isMoving = true; }
		else if (SDL_PointInRect(&p, &upButtonRect)) { if (!isJumping) { isJumping = true; jumpVelocity = jumpStrength; } }
		else if (SDL_PointInRect(&p, &downButtonRect)) { if (onPlatform) { droppingThrough = true; dropTimer = SDL_GetTicks(); } }
	}
	else if (e.type == SDL_FINGERUP) {
		int x = static_cast<int>(e.tfinger.x * windowWidth);
		int y = static_cast<int>(e.tfinger.y * windowHeight);
		SDL_Point p = { x, y };

		if (SDL_PointInRect(&p, &leftButtonRect)) { mMovingLeft = false; isMoving = mMovingRight; }
		else if (SDL_PointInRect(&p, &rightButtonRect)) { mMovingRight = false; isMoving = mMovingLeft; }
	}
#endif // __ANDROID__
}



void MainCharacter::move(Uint32 deltaTime)
{
	// Postojeće kretanje levo-desno
	if (mMovingLeft) xPos -= mVelX * deltaTime;
	if (mMovingRight) xPos += mVelX * deltaTime;

	// Ako je aktivno spuštanje kroz platformu
	if (droppingThrough) {
		// Spusti lika polako dole (npr. -20.0f(gravity) piksela po frame-u, možeš prilagoditi)
		yPos += gravity;

		// Ako je prošlo više od 70 ms od pritiska na 'S', isključi spuštanje
		if (SDL_GetTicks() - dropTimer > 70) {
			droppingThrough = false;
			onPlatform = false;  // više nisi na platformi jer si prošao kroz nju
		}
	}
	else if (isJumping) {
		yPos += jumpVelocity;
		jumpVelocity += gravity;
		if (yPos >= groundLevel) {
			yPos = groundLevel;
			isJumping = false;
			jumpVelocity = 0;
		}
	}

	int winW = 0, winH = 0;
	SDL_GetWindowSize(Game::window, &winW, &winH);

	if (xPos < -100)
		xPos = -100;
	if (xPos > winW - destRect.w + 100)
		xPos = winW - destRect.w + 100;

	if (yPos < 0)
		yPos = 0;
	if (yPos > winH - destRect.h)
		yPos = winH - destRect.h;

	destRect.x = xPos;
	destRect.y = yPos;
}


void MainCharacter::Update()
{
	extern Platform* platform1;
	extern Platform* platform2;
	extern Platform* platform3;
	extern Platform* platform4;
	extern Platform* platform5;
	extern Platform* platform6;
	extern Platform* platform7;
	extern Platform* platform8;

	// Platforme u niz za lakšu proveru
	Platform* platforms[] = { platform1, platform2, platform3, platform4, platform5, platform6, platform7, platform8 };

	// Ako igrač pada kroz platformu
	if (droppingThrough) {
		bool belowAnyPlatform = false;

		for (int i = 0; i < 8; ++i) {
			if (!platforms[i]) continue;

			SDL_Rect platformBox = platforms[i]->getHitbox();
			
			// Ako je igrač ispod platforme, prestani sa spuštanjem kroz nju
			if (yPos > platformBox.y + platformBox.h) {
				belowAnyPlatform = false;
				break;
			}
		}

		if (belowAnyPlatform) {
			droppingThrough = false;
			onPlatform = true;
		}
	}
	


	// Animacije i ispis stanja (kao što si imao)
	std::string state;
	if (isJumping) {
		state = facingRight ? "JUMPING RIGHT" : "JUMPING LEFT";
	}
	else if (isMoving) {
		state = facingRight ? "RUNNING RIGHT " : "RUNNING LEFT";
	}
	else {
		state = facingRight ? "IDLE RIGHT" : "IDLE LEFT";
	}
	
	state = droppingThrough ? "DROPPING THROUGH" : "FALSE";
	state = onPlatform ? "ON PLATFORM" : "NOT ON PLATFORM";

	if (!game->isPaused) {
		std::cout << "\033[32m     Your xPos: " << std::setw(5) << xPos;
		std::cout << "\033[32m     Your yPos: " << std::setw(5) << yPos //<< " onPl: " << std::setw(5) << (onPlatform ? "YES" : "NO")//
			<< " | YourState: " << std::setw(12) << state << "\033[0m" << std::flush;
	}

	Uint32 now = SDL_GetTicks();

	// Animacije
	if (isJumping)
	{
		if (now - lastFrameTime > jumpAnimationSpeed) {
			currentFrame = (currentFrame + 1) % jumpFrameCount;
			lastFrameTime = now;
		}

		srcRect.x = (currentFrame % 2) * frameWidth;
		srcRect.y = (currentFrame / 2) * frameHeight;
		srcRect.w = jumpFrameWidth;
		srcRect.h = jumpFrameHeight;

		destRect.w = jumpFrameWidth * 4;
		destRect.h = jumpFrameHeight * 4;

		currentTexture = textureJump;
	}
	else if (isMoving)
	{
		if (now - lastFrameTime > animationSpeed) {
			currentFrame = (currentFrame + 1) % frameCount;
			lastFrameTime = now;
		}

		srcRect.x = (currentFrame % 3) * frameWidth;
		srcRect.y = (currentFrame / 3) * frameHeight;
		srcRect.w = frameWidth;
		srcRect.h = frameHeight;

		destRect.w = frameWidth * 4;
		destRect.h = frameHeight * 4;

		currentTexture = textureRun;
	}
	else
	{
		if (now - idleLastFrameTime > idleAnimationSpeed) {
			idleCurrentFrame = (idleCurrentFrame + 1) % idleFrameCount;
			idleLastFrameTime = now;
		}

		srcRect.x = (idleCurrentFrame % 3) * idleFrameWidth;
		srcRect.y = (idleCurrentFrame / 3) * idleFrameHeight;
		srcRect.w = idleFrameWidth;
		srcRect.h = idleFrameHeight;

		destRect.w = idleFrameWidth * 4;
		destRect.h = idleFrameHeight * 4;

		currentTexture = textureIdle;
	}

	destRect.x = xPos;

	SDL_Rect playerFeet = { destRect.x + destRect.w / 2 - 10, destRect.y + destRect.h - 10, 20, 10 };

	bool standingOnPlatform = false;

	// Provera da li igrač stoji na nekoj platformi
	for (int i = 0; i < 8; ++i) {
		if (!platforms[i]) continue;

		SDL_Rect platformBox = platforms[i]->getHitbox();

		if (droppingThrough) {
			continue;
		}

		// Da li su stopala horizontalno iznad platforme?
		bool footOverPlatform = (playerFeet.x + playerFeet.w > platformBox.x) && (playerFeet.x < platformBox.x + platformBox.w);

		// Da li su stopala u vertikalnoj zoni blizu vrha platforme? (dozvola od +/- 5-20 piksela)
		bool footNearPlatformTop = (playerFeet.y + playerFeet.h <= platformBox.y + 20) && (playerFeet.y + playerFeet.h >= platformBox.y - 5);

		if (footOverPlatform && footNearPlatformTop) {
			standingOnPlatform = true;

			// Postavi igrača tačno na vrh platforme
			yPos = platformBox.y - destRect.h + 10;
			destRect.y = yPos;

			mVelY = 0;
			jumpVelocity = 0;
			isJumping = false;
			onPlatform = true;

			break;
		}
	}

	if (!standingOnPlatform) {
		// Nije na platformi, pusti gravitaciju da radi ako nije na tlu
		onPlatform = false;

		if (yPos < groundLevel) {
			// Padanje
			yPos += gravity; // gravity koristiš kao konstantu ili stari jumpVelocity + gravity iz tvog koda
			isJumping = true;
		}
		else {
			// Ako si na tlu, resetuj stanje
			yPos = groundLevel;
			isJumping = false;
			onPlatform = true;
			jumpVelocity = 0;
		}

		destRect.y = yPos;
	}

	destRect.x = xPos;
	destRect.y = yPos;
}


void MainCharacter::Render()
{
	SDL_RendererFlip flip = facingRight ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;

	if (destRect.w == 0 || destRect.h == 0) {
		SDL_Log("WARNING: destRect.w or h is 0! w: %d, h: %d", destRect.w, destRect.h);
		SDL_Log("srcRect: x=%d y=%d w=%d h=%d", srcRect.x, srcRect.y, srcRect.w, srcRect.h);

	}

	SDL_RenderCopyEx(Game::renderer, currentTexture, &srcRect, &destRect, 0, NULL, flip);
}

void MainCharacter::StopMovement()
{
	mMovingLeft = false;
	mMovingRight = false;
	isMoving = false;
	mVelX = 0;
	mVelY = 0;
	jumpVelocity = 0;
	isJumping = false;
}


void MainCharacter::setPosition(int x, int y) {
	xPos = x;
	yPos = y;
	destRect.x = x;
	destRect.y = y;
}
void MainCharacter::updateGroundLevel(SDL_Window* window)
{
    int w, h;
    SDL_GetWindowSize(window, &w, &h);

    // groundLevel je dno prozora minus visina lika
    groundLevel = h - destRect.h;
}









//=======================================================================================================================================================================================







CloudObjects::CloudObjects(const char* texturesheet, int x, int y)
{
	cloudTexture = TextureManager::LoadTexture(texturesheet);
	if (!cloudTexture) {
		SDL_Log("Failed to load texture for GameObject: %s", texturesheet);
	}

	xPosCloud = x;
	yPosCloud = y;
}

void CloudObjects::Update()
{
	xPosCloud--;

	srcRect.h = 400;
	srcRect.w = 1024;
	srcRect.x = 0;
	srcRect.y = 0;

	destRect.x = xPosCloud;
	destRect.y = yPosCloud;
	destRect.h = srcRect.h * 2;
	destRect.w = srcRect.w * 2;


}

void CloudObjects::Render()
{
	SDL_RenderCopy(Game::renderer, cloudTexture, &srcRect, &destRect);
}






//=======================================================================================================================================================================================



Particles::Particles(const char* texturesheet, int x, int y)
{
	particleTexture = TextureManager::LoadTexture(texturesheet);
	if (!particleTexture) {
		SDL_Log("Failed to load texture for Particles: %s", texturesheet);
	}

	Uint32 lastParticleSpawnTime = 0;
	Uint32 nextParticleDelay = 300 + (rand() % 1400);

	xPos = x;
	yPos = y;

	particleFrameWidth = 256;
	particleFrameHeight = 256;
	particleFrameCount = 30;
	particleAnimationSpeed = 60;

	textureParticleExplosion = TextureManager::LoadTexture("particles/Firework1.png");

	currentTexture = textureParticleExplosion;


	lastSpawnTime = SDL_GetTicks();
	ResetSpawnDelay();
	isAnimating = false;
}

void Particles::Update()
{
	Uint32 now = SDL_GetTicks();

	if (!isAnimating) {
		if (now - lastSpawnTime >= nextSpawnDelay) {
			isAnimating = true;
			particleCurrentFrame = 0;
			lastFrameTime = now;
		}
		else {
			return; // još ne spawnuje
		}
	}

	// Animacija u toku
	if (now - lastFrameTime > particleAnimationSpeed) {
		particleCurrentFrame++;
		lastFrameTime = now;

		if (particleCurrentFrame >= particleFrameCount) {
			// animacija završena
			isAnimating = false;
			lastSpawnTime = now;
			ResetSpawnDelay();
			return;
		}
	}

	srcRect.x = (particleCurrentFrame % 6) * particleFrameWidth;
	srcRect.y = (particleCurrentFrame / 6) * particleFrameHeight;
	srcRect.w = particleFrameWidth;
	srcRect.h = particleFrameHeight;

	destRect.x = xPos;
	destRect.y = yPos;
	destRect.w = particleFrameWidth * 2;
	destRect.h = particleFrameHeight * 2;

	currentTexture = textureParticleExplosion;
}


void Particles::Render()
{
	if (isAnimating)
		SDL_RenderCopy(Game::renderer, particleTexture, &srcRect, &destRect);
}

void Particles::ResetSpawnDelay() {
	nextSpawnDelay = 300 + (rand() % 1700); // 300–2000 ms
}

void Particles::EnableMovement()
{
	isAnimating = true;
}

void Particles::setPosition(int x, int y)
{
    xPos = x;
    yPos = y;

    destRect.x = x;
    destRect.y = y;
}