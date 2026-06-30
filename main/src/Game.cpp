#include "Game.h"
#include "TextureManager.h"
#include "GameObject.h"
#include "StaticObject.h"
#include "levelElements.h"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_main.h"
#include "Utils.h"
#include "Platform.h"
#include <iostream>
#include <string>

using namespace std;

std::vector<Platform*> Game::platforms;


extern MainCharacter* player;

Platform* platform1;
Platform* platform2;
Platform* platform3;
Platform* platform4;
Platform* platform5;
Platform* platform6;
Platform* platform7;
Platform* platform8;

LevelElements* lvl1plan1;
LevelElements* lvl1plan2;
LevelElements* lvl1plan3;
LevelElements* lvl1plan4;
LevelElements* lvl1plan5;

LevelElements* lvl2plan1;
LevelElements* lvl2plan2;
LevelElements* lvl2plan3;
LevelElements* lvl2plan4;
LevelElements* lvl2plan5;
LevelElements* lvl2plan6;

LevelElements* lvl3plan1;
LevelElements* lvl3plan2;
LevelElements* lvl3plan3;
LevelElements* lvl3plan4;
LevelElements* lvl3plan5;

LevelElements* lvl4plan1;
LevelElements* lvl4plan2;
LevelElements* lvl4plan3;
LevelElements* lvl4plan4;
LevelElements* lvl4plan5;


MainCharacter* player;
GameObject* enemy;
GameObject* enemy2;

Particles* firework1;
Particles* firework2;
Particles* firework3;
Particles* firework4;
Particles* firework5;
Particles* firework6;
Particles* firework7;
Particles* firework8;
Particles* firework9;
Particles* firework10;
Particles* firework11;
Particles* firework12;
Particles* firework13;
Particles* firework14;
Particles* firework15;
Particles* firework16;
Particles* firework17;
Particles* firework18;
Particles* firework19;
Particles* firework20;
Particles* firework21;
Particles* firework22;

CloudObjects* clearCloudLayer1;
CloudObjects* clearCloudLayer2;
CloudObjects* clearCloudLayer3;
CloudObjects* grayCloudLayer1;
CloudObjects* grayCloudLayer2;
CloudObjects* grayCloudLayer3;
CloudObjects* darkcloudLayer1;
CloudObjects* darkcloudLayer2;
CloudObjects* darkcloudLayer3;

SDL_Renderer* Game::renderer = nullptr;
SDL_Window* Game::window = nullptr;

bool checkCollision(const SDL_Rect& a, const SDL_Rect& b)
{
    return (a.x + a.w > b.x) &&
        (a.x < b.x + b.w) &&
        (a.y + a.h > b.y) &&
        (a.y < b.y + b.h);
}

Game::Game()
{}

Game::~Game()
{}

void Game::init(const char* title, int width, int height, bool fullscreen)
{
    int flags = 0;

    if (fullscreen)
    {
        flags = SDL_WINDOW_FULLSCREEN;
    }

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        isRunning = false;
        return;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        SDL_Log("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        isRunning = false;
        return;
    }

    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
    if (!window)
    {
        SDL_Log("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        isRunning = false;
        return;
    }
    Game::window = window;

    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer)
    {
        SDL_Log("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        isRunning = false;
        return;
    }



    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    // Posle window i renderer kreiranja
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);
    scaleX = static_cast<float>(windowWidth) / 1920.0f; // tvoja default rezolucija
    scaleY = static_cast<float>(windowHeight) / 1080.0f; // tvoja default rezolucija


    isRunning = true;

    player = new MainCharacter("WarriorRunRightSpriteSheet.png", scaleXPos(850), scaleYPos(400));
    player->updateGroundLevel(window);


    enemy = new GameObject("EnemyRun1.png", scaleXPos(1200), scaleYPos(770), 1, 2.0f);
    enemy2 = new GameObject("EnemyRun1.png", scaleXPos(950), scaleYPos(770), -1, 3.5f);

    pauseIconTexture = TextureManager::LoadTexture("androidButtons/pause.png");
    resumeIconTexture = TextureManager::LoadTexture("androidButtons/resume.png");

    pauseButtonRect = { 20, 20, 86, 86 };   // x, y, width, height
    resumeButtonRect = { 20, 20, 86, 86 }; // x, y, width, height

// Veličina i pozicija dugmeta u gornjem desnom uglu
    int buttonSize = 128 * scaleX; // skalirano
    pauseButtonRect.w = buttonSize;
    pauseButtonRect.h = buttonSize;
    pauseButtonRect.x = windowWidth - pauseButtonRect.w - 20; // desno + margin
    pauseButtonRect.y = 20; // gore + margin

    buttonSize = 128 * scaleX; // skalirano
    resumeButtonRect.w = buttonSize;
    resumeButtonRect.h = buttonSize;
    resumeButtonRect.x = windowWidth - resumeButtonRect.w - 20; // desno + margin
    resumeButtonRect.y = 20; // gore + margin


    // Učitaj teksturu
    restartButtonTexture = TextureManager::LoadTexture("androidButtons/restart1.png");

// Postavi poziciju dugmeta (npr. centrirano)
    restartButtonRect.w = 280;
    restartButtonRect.h = 280;
    restartButtonRect.x = (windowWidth - restartButtonRect.w) / 2;
    restartButtonRect.y = (windowHeight - restartButtonRect.h) / 2 + 150;

#ifdef __ANDROID__
    UpIconTexture = TextureManager::LoadTexture("androidButtons/up.png");
    DownIconTexture = TextureManager::LoadTexture("androidButtons/down.png");
    LeftIconTexture = TextureManager::LoadTexture("androidButtons/left.png");
    RightIconTexture = TextureManager::LoadTexture("androidButtons/right.png");
#endif


    keybindTexture = TextureManager::LoadTexture("KeyBindWASD2.png");
    if (!keybindTexture) {
        SDL_Log("Failed to load KeyBindWASD.png");
    }

    keybindTexture1 = TextureManager::LoadTexture("KeyBindEscRCQ3.png");
    if (!keybindTexture1) {
        SDL_Log("Failed to load KeyBindEscCQ.png");
    }

    pauseTexture = TextureManager::LoadTexture("GamePaused1.png");
    if (!pauseTexture) {
        SDL_Log("Failed to load GamePaused1.png");
    }

    gameOverTexture = TextureManager::LoadTexture("GameOver4.png");
    if (!gameOverTexture) {
        SDL_Log("Failed to load GameOver3.png");
    }

    isGameOver = false;


    levelCompletedTexture = TextureManager::LoadTexture("LvlCompleted2.png");

    if (!levelCompletedTexture) {
        SDL_Log("Failed to load level complete textures.");
    }
    levelStartTime = SDL_GetTicks();


    for (int i = 0; i < 60; i++) {
        std::string path = "timer/" + std::to_string(i + 1) + ".png";
        timerTextures[i] = TextureManager::LoadTexture(path.c_str());
        if (!timerTextures[i]) {
            SDL_Log("Failed to load timer image: %s", path.c_str());
        }
    }
    timerStartTime = SDL_GetTicks();

    clearCloudLayer1 = new CloudObjects("clearCloudLayer1.png", scaleXPos(0), scaleYPos(0));
    clearCloudLayer2 = new CloudObjects("clearCloudLayer1.png", scaleXPos(1700), scaleYPos(0));
    clearCloudLayer3 = new CloudObjects("clearCloudLayer1.png", scaleXPos(3400), scaleYPos(0));
    grayCloudLayer1 = new CloudObjects("grayCloudLayer1.png", scaleXPos(5100), scaleYPos(0));
    grayCloudLayer2 = new CloudObjects("grayCloudLayer1.png", scaleXPos(6800), scaleYPos(0));
    grayCloudLayer3 = new CloudObjects("grayCloudLayer1.png", scaleXPos(8500), scaleYPos(0));
    darkcloudLayer1 = new CloudObjects("darkCloudLayer1.png", scaleXPos(10200), scaleYPos(0));
    darkcloudLayer2 = new CloudObjects("darkCloudLayer1.png", scaleXPos(11900), scaleYPos(0));
    darkcloudLayer3 = new CloudObjects("darkCloudLayer1.png", scaleXPos(13600), scaleYPos(0));


	lvl1plan5 = new LevelElements("levels/lvl1/plan5.png", scaleXPos(0), scaleYPos(0));
    lvl1plan5->setSize(scaleWidth(1920), scaleHeight(1080));
    lvl1plan4 = new LevelElements("levels/lvl1/plan4.png", scaleXPos(0), scaleYPos(0));
    lvl1plan4->setSize(scaleWidth(1920), scaleHeight(1080));
    lvl1plan3 = new LevelElements("levels/lvl1/plan3.png", scaleXPos(0), scaleYPos(0));
    lvl1plan3->setSize(scaleWidth(1920), scaleHeight(1080));
    lvl1plan2 = new LevelElements("levels/lvl1/plan2.png", scaleXPos(0), scaleYPos(0));
    lvl1plan2->setSize(scaleWidth(1920), scaleHeight(1080));
    lvl1plan1 = new LevelElements("levels/lvl1/plan1.png", scaleXPos(0), scaleYPos(-50));
    lvl1plan1->setSize(scaleWidth(1920), scaleHeight(1080));

    lvl2plan6 = new LevelElements("levels/lvl2/plan6.png", scaleXPos(0), scaleYPos(0));
    lvl2plan6->setSize(scaleWidth(1920), scaleHeight(1080));
    lvl2plan5 = new LevelElements("levels/lvl2/plan5.png", scaleXPos(0), scaleYPos(0));
    lvl2plan5->setSize(scaleWidth(1920), scaleHeight(1080));
    lvl2plan4 = new LevelElements("levels/lvl2/plan4.png", scaleXPos(0), scaleYPos(0));
    lvl2plan4->setSize(scaleWidth(1920), scaleHeight(1080));
    lvl2plan3 = new LevelElements("levels/lvl2/plan3.png", scaleXPos(0), scaleYPos(0));
    lvl2plan3->setSize(scaleWidth(1920), scaleHeight(1080));
    lvl2plan2 = new LevelElements("levels/lvl2/plan2.png", scaleXPos(0), scaleYPos(0));
    lvl2plan2->setSize(scaleWidth(1920), scaleHeight(1080));
    lvl2plan1 = new LevelElements("levels/lvl2/plan1.png", scaleXPos(0), scaleYPos(-50));
    lvl2plan1->setSize(scaleWidth(1920), scaleHeight(1080));

    lvl3plan5 = new LevelElements("levels/lvl3/plan5.png", scaleXPos(0), scaleYPos(0));
    lvl3plan5->setSize(scaleWidth(1920), scaleHeight(1080));
    lvl3plan4 = new LevelElements("levels/lvl3/plan4.png", scaleXPos(0), scaleYPos(0));
    lvl3plan4->setSize(scaleWidth(1920), scaleHeight(1080));
    lvl3plan3 = new LevelElements("levels/lvl3/plan3.png", scaleXPos(0), scaleYPos(0));
    lvl3plan3->setSize(scaleWidth(1920), scaleHeight(1080));
    lvl3plan2 = new LevelElements("levels/lvl3/plan2.png", scaleXPos(0), scaleYPos(0));
    lvl3plan2->setSize(scaleWidth(1920), scaleHeight(1080));
	lvl3plan1 = new LevelElements("levels/lvl3/plan1.png", scaleXPos(0), scaleYPos(0));
    lvl3plan1->setSize(scaleWidth(1920), scaleHeight(1080));

	lvl4plan5 = new LevelElements("levels/lvl4/plan5.png", scaleXPos(0), scaleYPos(0));
    lvl4plan5->setSize(scaleWidth(1920), scaleHeight(1080));
    lvl4plan4 = new LevelElements("levels/lvl4/plan4.png", scaleXPos(0), scaleYPos(0));
    lvl4plan4->setSize(scaleWidth(1920), scaleHeight(1080));
    lvl4plan3 = new LevelElements("levels/lvl4/plan3.png", scaleXPos(0), scaleYPos(0));
    lvl4plan3->setSize(scaleWidth(1920), scaleHeight(1080));
    lvl4plan2 = new LevelElements("levels/lvl4/plan2.png", scaleXPos(0), scaleYPos(0));
    lvl4plan2->setSize(scaleWidth(1920), scaleHeight(1080));
	lvl4plan1 = new LevelElements("levels/lvl4/plan1.png", scaleXPos(0), scaleYPos(0));
    lvl4plan1->setSize(scaleWidth(1920), scaleHeight(1080));

	platform1 = new Platform("platforms/platform128x16.png", scaleXPos(850), scaleYPos(600), scaleWidth(128), scaleHeight(16));
    platform2 = new Platform("platforms/platformLong256x16.png", scaleXPos(1150), scaleYPos(730), scaleWidth(256), scaleHeight(16));
    platform3 = new Platform("platforms/platformLong512x16.png", scaleXPos(100), scaleYPos(650), scaleWidth(512), scaleHeight(16));
    platform4 = new Platform("platforms/platformLong512x16.png", scaleXPos(1500), scaleYPos(600), scaleWidth(512), scaleHeight(16));
    platform5 = new Platform("platforms/platform128x16.png", scaleXPos(1600), scaleYPos(800), scaleWidth(128), scaleHeight(16));
    platform6 = new Platform("platforms/platformLong256x16.png", scaleXPos(1120), scaleYPos(340), scaleWidth(256), scaleHeight(16));
    platform7 = new Platform("platforms/platform128x16.png", scaleXPos(800), scaleYPos(410), scaleWidth(128), scaleHeight(16));
    platform8 = new Platform("platforms/platformLong512x16.png", scaleXPos(0), scaleYPos(440), scaleWidth(512), scaleHeight(16));

    firework1 = new Particles("particles/Firework1.png", scaleXPos(-100), scaleYPos(-100));
    firework2 = new Particles("particles/Firework1.png", scaleXPos(1672), scaleYPos(-30));
    firework3 = new Particles("particles/Firework1.png", scaleXPos(-78), scaleYPos(472));
    firework4 = new Particles("particles/Firework1.png", scaleXPos(1572), scaleYPos(622));
    firework5 = new Particles("particles/Firework1.png", scaleXPos(122), scaleYPos(22));
    firework6 = new Particles("particles/Firework1.png", scaleXPos(1522), scaleYPos(-78));
    firework7 = new Particles("particles/Firework1.png", scaleXPos(-28), scaleYPos(672));
    firework8 = new Particles("particles/Firework1.png", scaleXPos(1632), scaleYPos(712));
    firework9 = new Particles("particles/Firework1.png", scaleXPos(72), scaleYPos(-108));
    firework10 = new Particles("particles/Firework1.png", scaleXPos(1622), scaleYPos(522));
    firework11 = new Particles("particles/Firework1.png", scaleXPos(-58), scaleYPos(572));
    firework12 = new Particles("particles/Firework1.png", scaleXPos(872), scaleYPos(-28));
    firework13 = new Particles("particles/Firework1.png", scaleXPos(-8), scaleYPos(692));
    firework14 = new Particles("particles/Firework1.png", scaleXPos(1772), scaleYPos(52));
    firework15 = new Particles("particles/Firework1.png", scaleXPos(-98), scaleYPos(272));
    firework16 = new Particles("particles/Firework1.png", scaleXPos(1452), scaleYPos(672));
    firework17 = new Particles("particles/Firework1.png", scaleXPos(372), scaleYPos(-78));
    firework18 = new Particles("particles/Firework1.png", scaleXPos(1372), scaleYPos(-58));
    firework19 = new Particles("particles/Firework1.png", scaleXPos(22), scaleYPos(552));
    firework20 = new Particles("particles/Firework1.png", scaleXPos(1592), scaleYPos(422));
    firework21 = new Particles("particles/Firework1.png", scaleXPos(772), scaleYPos(-48));
    firework22 = new Particles("particles/Firework1.png", scaleXPos(152), scaleYPos(652));


    isRunning = true;
    timerStartTime = SDL_GetTicks();
    pauseDuration = 0;
    timerActive = true;


}

void Game::handleEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
                isRunning = false;
                break;

                // ---------------- WINDOWS TASTATURA ----------------
#ifdef _WIN32
            case SDL_KEYDOWN:
                // ALT + ENTER fullscreen
                if (event.key.keysym.sym == SDLK_RETURN && (event.key.keysym.mod & KMOD_ALT))
                {
                    isFullScreen = !isFullScreen;
                    Uint32 fullscreenFlag = isFullScreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0;
                    SDL_SetWindowFullscreen(window, fullscreenFlag);
                    SDL_ShowCursor(!isFullScreen);
                    break;
                }

                // ESC pauza
                if (event.key.keysym.sym == SDLK_ESCAPE && !isGameOver && !isLevelComplete)
                {
                    if (!isPaused) { pauseStartTime = SDL_GetTicks(); isPaused = true; }
                    else { Uint32 now = SDL_GetTicks(); pauseDuration += now - pauseStartTime; isPaused = false; }
                    break;
                }

                // LEVEL COMPLETE
                if (isLevelComplete) {
                    if (event.key.keysym.sym == SDLK_r) requestRestart();
                    else if (event.key.keysym.sym == SDLK_q) isRunning = false;
                    else if (event.key.keysym.sym == SDLK_RETURN) {
                        switch (currentLevel) {
                            case Level::LEVEL_1: currentLevel = Level::LEVEL_2; break;
                            case Level::LEVEL_2: currentLevel = Level::LEVEL_3; break;
                            case Level::LEVEL_3: currentLevel = Level::LEVEL_4; break;
                            case Level::LEVEL_4: currentLevel = Level::GAME_COMPLETED; break;
                            default: break;
                        }
                        loadLevel(currentLevel);
                    }
                    break;
                }

                // GAME OVER
                if (isGameOver) {
                    if (event.key.keysym.sym == SDLK_r) requestRestart();
                    else if (event.key.keysym.sym == SDLK_q) isRunning = false;
                    break;
                }

                // PAUSE MENU
                if (isPaused) {
                    if (event.key.keysym.sym == SDLK_c) isPaused = false;
                    else if (event.key.keysym.sym == SDLK_r) requestRestart();
                    else if (event.key.keysym.sym == SDLK_q) isRunning = false;
                    break;
                }

                // Ostale tipke igraču
                if (!isPaused && !isGameOver && !isLevelComplete)
                    player->handleEvent(event);
                break;
#endif // _WIN32

                // ---------------- ANDROID TOUCH ----------------
#ifdef __ANDROID__
            case SDL_FINGERDOWN:
            {
                int touchX = static_cast<int>(event.tfinger.x * windowWidth);
                int touchY = static_cast<int>(event.tfinger.y * windowHeight);
                SDL_Point touchPoint = { touchX, touchY };

                // GameOver restart
                if (isGameOver && SDL_PointInRect(&touchPoint, &restartButtonRect)) {
                    requestRestart();
                    break;
                }

                // Level Complete
                if (isLevelComplete) {
                    switch (currentLevel) {
                        case Level::LEVEL_1: currentLevel = Level::LEVEL_2; break;
                        case Level::LEVEL_2: currentLevel = Level::LEVEL_3; break;
                        case Level::LEVEL_3: currentLevel = Level::LEVEL_4; break;
                        case Level::LEVEL_4: currentLevel = Level::GAME_COMPLETED; break;
                        default: break;
                    }
                    loadLevel(currentLevel);
                    break;
                }

                // Pauza / Resume dugmad (tap)
                if (!isPaused && SDL_PointInRect(&touchPoint, &pauseButtonRect)) {
                    pauseStartTime = SDL_GetTicks();
                    isPaused = true;
                }
                else if (isPaused && SDL_PointInRect(&touchPoint, &resumeButtonRect)) {
                    Uint32 now = SDL_GetTicks();
                    pauseDuration += now - pauseStartTime;
                    isPaused = false;
                }

                // Prosleđivanje player-u
                if (!isPaused && !isGameOver && !isLevelComplete)
                    player->handleEvent(event, leftButtonRect, rightButtonRect, upButtonRect, downButtonRect, windowWidth, windowHeight);

                break;
            }
#endif // __ANDROID

            default:
                // Ostali eventi, npr. miš ili keyup
                if (!isPaused && !isGameOver && !isLevelComplete)
                    player->handleEvent(event, leftButtonRect, rightButtonRect, upButtonRect, downButtonRect, windowWidth, windowHeight);
                break;
        }
    }
}






void Game::update()
{


    // Ako je level završen – update samo vatromet i ništa drugo
    if (isLevelComplete) {
        timerActive = false;
        firework1->Update();
        firework2->Update();
        firework3->Update();
        firework4->Update();
        firework5->Update();
        firework6->Update();
        firework7->Update();
        firework8->Update();
        firework9->Update();
        firework10->Update();
        firework11->Update();
        firework12->Update();
        firework13->Update();
        firework14->Update();
        firework15->Update();
        firework16->Update();
        firework17->Update();
        firework18->Update();
        firework19->Update();
        firework20->Update();
        firework21->Update();
        firework22->Update();
        return; // samo vatromet
    }


    // Ako je pauza – preskoči update
    if (isPaused) {
        return;
    }

    // Timer
    if (timerActive) {
        Uint32 elapsedTime = SDL_GetTicks() - timerStartTime - pauseDuration;
        int secondsPassed = elapsedTime / 1000;

        if (secondsPassed < 60) {
            currentTimerFrame = 59 - secondsPassed;
        } else {
            timerActive = false;
            isLevelComplete = true;
        }
    }

    // Normalan gameplay update
    Uint32 deltaTime = 5;
    player->move(deltaTime);

    // Primer kolizije sa tlom
    if (checkCollision(player->hitbox, lvl1plan1->getHitbox())) {
        if (player->yPos + player->destRect.h <= lvl1plan1->getHitbox().y + 10) {
            player->yPos = lvl1plan1->getHitbox().y - player->destRect.h;
            player->isJumping = false;
            player->jumpVelocity = 0;
            player->groundLevel = player->yPos;
        }
    } else {
        player->groundLevel = 770;
    }

    player->Update();
    enemy->Update();
    enemy2->Update();

    // Hitbox logika
    platformHitbox1 = ShrinkRect(platform1->getHitbox(), 0);
    platformHitbox2 = ShrinkRect(platform2->getHitbox(), 0);
    platformHitbox3 = ShrinkRect(platform3->getHitbox(), 0);
    platformHitbox4 = ShrinkRect(platform4->getHitbox(), 0);
    platformHitbox5 = ShrinkRect(platform5->getHitbox(), 0);
    platformHitbox6 = ShrinkRect(platform6->getHitbox(), 0);
    platformHitbox7 = ShrinkRect(platform7->getHitbox(), 0);
    platformHitbox8 = ShrinkRect(platform8->getHitbox(), 0);

    SDL_Rect playerFeet = { player->getDestRect().x,
                            player->getDestRect().y + player->getDestRect().h - 10,
                            player->getDestRect().w,
                            10 };

    playerHitbox = ShrinkRect(player->getDestRect(), 30);
    enemyHitbox = ShrinkRect(enemy->getDestRect(), 40);
    enemyHitbox2 = ShrinkRect(enemy2->getDestRect(), 40);

    // levo,desno,gore,dole
    playerHitbox = AdjustHitbox(player->getDestRect(), 110, 110, 60, -30);
    enemyHitbox = AdjustHitbox(enemy->getDestRect(), 80, 80, 50, -30);
    enemyHitbox2 = AdjustHitbox(enemy2->getDestRect(), 80, 80, 50, -30);

    // Kolizija sa neprijateljem
    if (CheckCollision(playerHitbox, enemyHitbox) ||
        CheckCollision(playerHitbox, enemyHitbox2))
    {
        isGameOver = true;
        isPaused = true;
    }

    // Update background slojeva
    clearCloudLayer1->Update();
    clearCloudLayer2->Update();
    clearCloudLayer3->Update();
    grayCloudLayer1->Update();
    grayCloudLayer2->Update();
    grayCloudLayer3->Update();
    darkcloudLayer1->Update();
    darkcloudLayer2->Update();
    darkcloudLayer3->Update();

    // Update level planova
    lvl1plan5->Update();
    lvl1plan4->Update();
    lvl1plan3->Update();
    lvl1plan2->Update();
    lvl1plan1->Update();

    lvl2plan6->Update();
    lvl2plan5->Update();
    lvl2plan4->Update();
    lvl2plan3->Update();
    lvl2plan2->Update();
    lvl2plan1->Update();

    lvl3plan5->Update();
    lvl3plan4->Update();
    lvl3plan3->Update();
    lvl3plan2->Update();
    lvl3plan1->Update();

    lvl4plan5->Update();
    lvl4plan4->Update();
    lvl4plan3->Update();
    lvl4plan2->Update();
    lvl4plan1->Update();

    // Update platformi
    platform1->Update();
    platform2->Update();
    platform3->Update();
    platform4->Update();
    platform5->Update();
    platform6->Update();
    platform7->Update();
    platform8->Update();
}


void Game::render()
{
    SDL_RenderClear(renderer);

    // --- Render background layers based on current level ---
    switch (currentLevel)
    {
        case Level::LEVEL_1:
            lvl1plan5->Render();
            lvl1plan4->Render();
            lvl1plan3->Render();

            clearCloudLayer1->Render();
            clearCloudLayer2->Render();
            clearCloudLayer3->Render();

            grayCloudLayer1->Render();
            grayCloudLayer2->Render();
            grayCloudLayer3->Render();

            darkcloudLayer1->Render();
            darkcloudLayer2->Render();
            darkcloudLayer3->Render();

            lvl1plan2->Render();
            platform1->Render(); platform2->Render(); platform3->Render(); platform4->Render(); platform5->Render();
            platform6->Render(); platform7->Render(); platform8->Render();
            player->Render();
            enemy->Render();
            enemy2->Render();
            lvl1plan1->Render();
            break;

        case Level::LEVEL_2:
            lvl2plan6->Render();
            lvl2plan5->Render();

            clearCloudLayer1->Render();
            clearCloudLayer2->Render();
            clearCloudLayer3->Render();

            grayCloudLayer1->Render();
            grayCloudLayer2->Render();
            grayCloudLayer3->Render();

            darkcloudLayer1->Render();
            darkcloudLayer2->Render();
            darkcloudLayer3->Render();

            lvl2plan4->Render();
            lvl2plan3->Render();
            lvl2plan2->Render();

            platform1->Render(); platform2->Render(); platform3->Render(); platform4->Render(); platform5->Render();
            platform6->Render(); platform7->Render(); platform8->Render();
            player->Render();
            enemy->Render();
            enemy2->Render();
            lvl2plan1->Render();
            break;

        case Level::LEVEL_3:
            lvl3plan5->Render();

            clearCloudLayer1->Render();
            clearCloudLayer2->Render();
            clearCloudLayer3->Render();

            grayCloudLayer1->Render();
            grayCloudLayer2->Render();
            grayCloudLayer3->Render();

            darkcloudLayer1->Render();
            darkcloudLayer2->Render();
            darkcloudLayer3->Render();

            lvl3plan4->Render();
            lvl3plan3->Render();
            lvl3plan2->Render();

            platform1->Render(); platform2->Render(); platform3->Render(); platform4->Render(); platform5->Render();
            platform6->Render(); platform7->Render(); platform8->Render();

            player->Render();
            enemy->Render();
            enemy2->Render();
            lvl3plan1->Render();
            break;

        case Level::LEVEL_4:
            lvl4plan5->Render();
            lvl4plan4->Render();

            clearCloudLayer1->Render();
            clearCloudLayer2->Render();
            clearCloudLayer3->Render();

            grayCloudLayer1->Render();
            grayCloudLayer2->Render();
            grayCloudLayer3->Render();

            darkcloudLayer1->Render();
            darkcloudLayer2->Render();
            darkcloudLayer3->Render();

            lvl4plan3->Render();
            lvl4plan2->Render();

            platform1->Render(); platform2->Render(); platform3->Render(); platform4->Render(); platform5->Render();
            platform6->Render(); platform7->Render(); platform8->Render();

            player->Render();
            enemy->Render();
            enemy2->Render();
            lvl4plan1->Render();
            break;

        default:
            break;
    }

#ifdef __ANDROID__
    TextureManager::Draw(LeftIconTexture, leftButtonRect);
    TextureManager::Draw(RightIconTexture, rightButtonRect);
    TextureManager::Draw(UpIconTexture, upButtonRect);
    TextureManager::Draw(DownIconTexture, downButtonRect);
#endif


    // --- PAUZA i GAME OVER ekran ---
    if (isPaused)
    {
        SDL_Rect overlay = { 0, 0, windowWidth, windowHeight };
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 180);
        SDL_RenderFillRect(renderer, &overlay);

        SDL_Texture* screenTexture = isGameOver ? gameOverTexture : pauseTexture;

        if (screenTexture)
        {
            SDL_Rect imgRect;

            if (isGameOver)
            {
                imgRect.w = 1300;
                imgRect.h = 900;
                imgRect.x = (windowWidth - imgRect.w) / 2;
                imgRect.y = -100;
            }
            else
            {
                imgRect.w = 900;
                imgRect.h = 600;
                imgRect.x = (windowWidth - imgRect.w) / 2;
                imgRect.y = 50;
            }

            SDL_RenderCopy(renderer, screenTexture, nullptr, &imgRect);
        }

        // --- Crtaj restart dugme samo u Game Over ---
        if (isGameOver && restartButtonTexture)
        {
            SDL_RenderCopy(renderer, restartButtonTexture, nullptr, &restartButtonRect);
        }

        // Keybind prikazi samo ako nije Game Over
        if (!isGameOver)
        {
            if (keybindTexture)
            {
                SDL_Rect keybindRect = { 30, windowHeight - 384 - 250, 512, 384 };
                SDL_RenderCopy(renderer, keybindTexture, nullptr, &keybindRect);
            }

            if (keybindTexture1)
            {
                SDL_Rect keybindRect1 = { 30, windowHeight - 384 - 680, 512, 384 };
                SDL_RenderCopy(renderer, keybindTexture1, nullptr, &keybindRect1);
            }
        }
    }

    // --- Timer ---
    if (timerActive && currentTimerFrame >= 0 && currentTimerFrame < 60)
    {
        SDL_Texture* tex = timerTextures[currentTimerFrame];

        int winW, winH;
        SDL_GetWindowSize(Game::window, &winW, &winH);

        int bgWidth = 140;
        int bgHeight = 80;
        SDL_Rect bgRect = { (winW - bgWidth) / 2, 0, bgWidth, bgHeight };

        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 120);
        SDL_RenderFillRect(renderer, &bgRect);

        if (tex)
        {
            int texW = 64;
            int texH = 64;
            SDL_Rect dest = { bgRect.x + (bgRect.w - texW) / 2, bgRect.y + (bgRect.h - texH) / 2, texW, texH };
            TextureManager::Draw(tex, { 0, 0, texW, texH }, dest);
        }
    }


    // --- Level Completed ekran ---
    if (isLevelComplete && levelCompletedTexture)
    {
        SDL_Rect overlay = { 0, 0, windowWidth, windowHeight };
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 180);
        SDL_RenderFillRect(renderer, &overlay);

        SDL_Rect completedRect = { 0, 0, windowWidth, windowHeight };

        // Renderuj vatromete
        firework1->Render(); firework2->Render(); firework3->Render(); firework4->Render(); firework5->Render();
        firework6->Render(); firework7->Render(); firework8->Render(); firework9->Render(); firework10->Render();
        firework11->Render(); firework12->Render(); firework13->Render(); firework14->Render(); firework15->Render();
        firework16->Render(); firework17->Render(); firework18->Render(); firework19->Render(); firework20->Render();
        firework21->Render(); firework22->Render();

        SDL_RenderCopy(renderer, levelCompletedTexture, nullptr, &completedRect);
    }

    // --- Pause / Resume dugme ---
    if (!isGameOver && !isLevelComplete)
    {
        SDL_Texture* icon = isPaused ? resumeIconTexture : pauseIconTexture;
        if (icon)
        {
            SDL_RenderCopy(renderer, icon, nullptr, &pauseButtonRect);
        }
    }

    // --- Debug hitbox ---
    SDL_SetRenderDrawColor(Game::renderer, 255, 0, 0, 255);
    SDL_RenderDrawRect(Game::renderer, &platformHitbox1);
    SDL_RenderDrawRect(Game::renderer, &platformHitbox2);
    SDL_RenderDrawRect(Game::renderer, &platformHitbox3);
    SDL_RenderDrawRect(Game::renderer, &platformHitbox4);
    SDL_RenderDrawRect(Game::renderer, &platformHitbox5);
    SDL_RenderDrawRect(Game::renderer, &platformHitbox6);
    SDL_RenderDrawRect(Game::renderer, &platformHitbox7);
    SDL_RenderDrawRect(Game::renderer, &platformHitbox8);

    SDL_SetRenderDrawColor(Game::renderer, 255, 255, 0, 255);
    SDL_RenderDrawRect(Game::renderer, &playerFeet);


    SDL_RenderPresent(renderer);
}




void Game::clean()
{
    if (keybindTexture) {
        SDL_DestroyTexture(keybindTexture);
        keybindTexture = nullptr;
    }

    SDL_DestroyTexture(pauseTexture);
    pauseTexture = nullptr;

    SDL_DestroyTexture(gameOverTexture);
    gameOverTexture = nullptr;

    
    if (levelCompletedTexture) {
        SDL_DestroyTexture(levelCompletedTexture);
        levelCompletedTexture = nullptr;

    }

    for (int i = 0; i < 60; i++) {
        if (timerTextures[i]) {
            SDL_DestroyTexture(timerTextures[i]);
            timerTextures[i] = nullptr;
        }
    }

    for (Platform* p : Game::platforms) {
        delete p;
    }
    Game::platforms.clear();


	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}

void GameObject::setPosition(int x, int y)
{
    xPos = x;
    yPos = y;

    destRect.x = xPos;
    destRect.y = yPos;
}




void Game::loadLevel(Level level)
{
    isLevelComplete = false;
    isGameOver = false;
    timerActive = true;
    currentTimerFrame = 59;
    timerStartTime = SDL_GetTicks();
    pauseDuration = 0;

    // Reset pozicija igrača i neprijatelja
    switch (level)
    {
        case Level::LEVEL_1:
            player->setPosition(scaleXPos(850), scaleYPos(400));
            enemy->setPosition(scaleXPos(1200), scaleYPos(750));
            enemy2->setPosition(scaleXPos(750), scaleYPos(750));
            break;

        case Level::LEVEL_2:
            player->setPosition(scaleXPos(850), scaleYPos(400));
            enemy->setPosition(scaleXPos(1200), scaleYPos(750));
            enemy2->setPosition(scaleXPos(950), scaleYPos(750));
            break;

        case Level::LEVEL_3:
            player->setPosition(scaleXPos(850), scaleYPos(400));
            enemy->setPosition(scaleXPos(1200), scaleYPos(750));
            enemy2->setPosition(scaleXPos(950), scaleYPos(750));
            break;

        case Level::LEVEL_4:
            player->setPosition(scaleXPos(850), scaleYPos(400));
            enemy->setPosition(scaleXPos(1200), scaleYPos(750));
            enemy2->setPosition(scaleXPos(950), scaleYPos(750));
            break;

        case Level::GAME_COMPLETED:
            isRunning = false;
            return;
    }

    // Postavi pozicije platformi za sve nivoe
    platform1->setPosition(scaleXPos(850), scaleYPos(600));
    platform2->setPosition(scaleXPos(1150), scaleYPos(730));
    platform3->setPosition(scaleXPos(100), scaleYPos(650));
    platform4->setPosition(scaleXPos(1500), scaleYPos(600));
    platform5->setPosition(scaleXPos(1600), scaleYPos(800));
    platform6->setPosition(scaleXPos(1120), scaleYPos(340));
    platform7->setPosition(scaleXPos(800), scaleYPos(410));
    platform8->setPosition(scaleXPos(0), scaleYPos(440));

    // Skaliraj veličine platformi
    platform1->setSize(scaleWidth(128), scaleHeight(16));
    platform2->setSize(scaleWidth(256), scaleHeight(16));
    platform3->setSize(scaleWidth(512), scaleHeight(16));
    platform4->setSize(scaleWidth(512), scaleHeight(16));
    platform5->setSize(scaleWidth(128), scaleHeight(16));
    platform6->setSize(scaleWidth(256), scaleHeight(16));
    platform7->setSize(scaleWidth(128), scaleHeight(16));
    platform8->setSize(scaleWidth(512), scaleHeight(16));

    // Fireworks pozicije (ako želiš, može se automatizovati kroz niz)
    firework1->setPosition(scaleXPos(-100), scaleYPos(-100));
    firework2->setPosition(scaleXPos(1672), scaleYPos(-30));
    firework3->setPosition(scaleXPos(-78), scaleYPos(472));
    firework4->setPosition(scaleXPos(1572), scaleYPos(622));
    firework5->setPosition(scaleXPos(122), scaleYPos(22));
    firework6->setPosition(scaleXPos(1522), scaleYPos(-78));
    firework7->setPosition(scaleXPos(-28), scaleYPos(672));
    firework8->setPosition(scaleXPos(1632), scaleYPos(712));
    firework9->setPosition(scaleXPos(72), scaleYPos(-108));
    firework10->setPosition(scaleXPos(1622), scaleYPos(522));
    firework11->setPosition(scaleXPos(-58), scaleYPos(572));
    firework12->setPosition(scaleXPos(872), scaleYPos(-28));
    firework13->setPosition(scaleXPos(-8), scaleYPos(692));
    firework14->setPosition(scaleXPos(1772), scaleYPos(52));
    firework15->setPosition(scaleXPos(-98), scaleYPos(272));
    firework16->setPosition(scaleXPos(1452), scaleYPos(672));
    firework17->setPosition(scaleXPos(372), scaleYPos(-78));
    firework18->setPosition(scaleXPos(1372), scaleYPos(-58));
    firework19->setPosition(scaleXPos(22), scaleYPos(552));
    firework20->setPosition(scaleXPos(1592), scaleYPos(422));
    firework21->setPosition(scaleXPos(772), scaleYPos(-48));
    firework22->setPosition(scaleXPos(152), scaleYPos(652));
}

void Game::stopGameObjects() {
    player->StopMovement();
    enemy->StopMovement();
    enemy2->StopMovement();
}