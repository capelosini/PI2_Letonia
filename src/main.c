#include "../CAE/include/CAE.h"
#include "../include/globals.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

CAEngine* engine;
Scene* mainMenu;
Scene* stageOne;
GameObject* player;
float leafMatrix[100][3];


int main () {
    srand(time(NULL));

    GameConfig engineConfig;
    engineConfig.fps=60;
    engineConfig.posX=20;
    engineConfig.posY=20;
    engineConfig.sizeX=1280;
    engineConfig.sizeY=720;
    engineConfig.title="Letonia Project";
    engineConfig.fullscreen=1;

    engine = initEngine(engineConfig);
    setEventFunction(engine, onEvent);

    // MENU
    mainMenu = createScene(engine, leafEffect);

    for (int i = 0; i < 100; i++) {
        leafMatrix[i][0] = rand() % (engine->displayWidth + 100) - 100;
        leafMatrix[i][1] = rand() % engine->displayHeight - engine->displayHeight;
        leafMatrix[i][2] = rand() % 2 + 1;
    }
    
    Font* titleFont = loadTTF(engine, "../assets/fonts/kalam-bold.ttf", 80);
    char* titleText = "Revolução Em Cartas";
    addTextToScene(mainMenu, createText(titleText, engine->displayWidth/2 - al_get_text_width(titleFont->font, titleText)/2, 50, al_map_rgb(255, 255, 255), titleFont));

    addButtonToScene(mainMenu, createButton(engine, engine->displayWidth / 2 - 75, engine->displayHeight / 2 - 25, 150, 50, al_map_rgb(217, 95, 54), al_map_rgb(255, 255, 255), "Jogar", "../assets/fonts/roboto.ttf", NULL, onStartStageOne));

    changeScene(engine, mainMenu);

    // FASE 1
    stageOne = createScene(engine, gameSceneScript);
    stageOne->camera.maxLimit.x = 3840;
    stageOne->camera.maxLimit.y = 2560;
    player = createGameObject(ANIMATED_SPRITE, 700, 50, 34, 40, stageOne);
    player->physics.enabled = 1;
    player->physics.friction = 0.4;
    player->physics.maxSpeed = 4;
    player->collisionEnabled = 1;
    player->collisionType = COLLISION_RECT;
    setGameObjectAnimation(player, loadBitmap(engine, "../assets/images/player-sprite-sheet.png"), 17, 20, 8, 15);
    stageOne->camera.followTarget = player;

    setupSceneWorld(stageOne, loadBitmap(engine, "../assets/images/map.png"), 3840, 2560);
    addWorldTile(stageOne, 0, 0, 0, 0); 

    GameObject* map= createGameObject(SOLID, 0, 0, 3840, 2560, stageOne) ;
    map->color = al_map_rgba(0, 0, 0, 0);
    map->collisionEnabled = 1;
    map->collisionType = COLLISION_RECT;
    map->invertedCollision = 1;
    while (engine->isAlive){
        render(engine);
    }

    freeEngine(engine);

    return 0;
}