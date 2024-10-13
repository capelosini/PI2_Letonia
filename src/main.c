#include "../CAE/include/CAE.h"
#include "../include/globals.h"
#include <stdio.h>

CAEngine* engine;
Scene* mainMenu;
Scene* stageOne;
GameObject* player;


int main () {
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
    mainMenu = createScene(engine, NULL);

    ALLEGRO_BITMAP *bgImage = loadBitmap(engine, "../assets/images/menu.jpg");
    GameObject* background = createGameObject(SPRITE, 0, 0, engine->displayWidth, engine->displayHeight, mainMenu);
    setGameObjectBitmap(background, bgImage);
    
    Font* titleFont = loadTTF(engine, "../assets/fonts/kalam-bold.ttf", 80);
    char* titleText = "Pênis";
    addTextToScene(mainMenu, createText(titleText, engine->displayWidth/2 - al_get_text_width(titleFont->font, titleText)/2, 50, al_map_rgb(0, 0, 0), titleFont));

    addButtonToScene(mainMenu, createButton(engine, engine->displayWidth / 2 - 75, engine->displayHeight / 2 - 25, 150, 50, al_map_rgb(217, 95, 54), al_map_rgb(255, 255, 255), "Começar", "../assets/fonts/roboto.ttf", NULL, onStartStageOne));

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