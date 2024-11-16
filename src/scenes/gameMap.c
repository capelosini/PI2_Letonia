#include "../../include/globals.h"
#include <allegro5/color.h>

void loadGameMap(){
    gameMap = createScene(engine, gameSceneScript);
    gameMap->camera.minLimit.x = 0;
    gameMap->camera.minLimit.y = 0;
    //gameMap->camera.maxLimit.x = 200 * 16;
    //gameMap->camera.maxLimit.y = 100 * 16;
    gameMap->camera.maxLimit.x = 5000;
    gameMap->camera.maxLimit.y = 5000;
    gameMap->camera.zoom = 1.5;

    map = createGameObject(SOLID, 0, 0, 5000, 5000, gameMap);
    //GameObject* map = createGameObject(SOLID, 0, 0, 200 * 16, 100 * 16, gameMap);
    map->color = al_map_rgba(0, 0, 0, 0);
    map->collisionEnabled = 1;
    map->collisionType = COLLISION_RECT;
    map->invertedCollision = 1;

    // creating the vertical roads
    for (int j=0; j<map->height/500; j++){
        for (int i=0; i<map->width/500; i++){
            GameObject* t = createGameObject(SPRITE, 337+i*500, j*500, 166, 500, gameMap);
            setGameObjectBitmap(t, roadV);
        }
    }

    // creating the horizontal roads
    for (int j=1; j<map->height/500; j++){
        for (int i=0; i<map->width/500; i++){
            GameObject* t = createGameObject(SPRITE, i*500, j*500, 500, 166, gameMap);
            setGameObjectBitmap(t, roadH);
        }
    }

    // quartel entry
    gridGameMap = createGameObject(SPRITE, map->width - 250, 0, 250, 5000, gameMap);
    ALLEGRO_BITMAP* ExitGameMapBM = loadBitmap(engine, "./assets/images/grid.png");
    setGameObjectBitmap(gridGameMap, ExitGameMapBM);

    GameObject* treeGrid1 = createGameObject(SOLID, map->width - 215, 0, 230, 4600, gameMap);
    treeGrid1->color = al_map_rgba(0, 0, 0, 0);
    treeGrid1->collisionEnabled = 1;
    GameObject* treeGrid3 = createGameObject(SOLID, map->width - 215, map->height - 220, 230, 370, gameMap);
    treeGrid3->color = al_map_rgba(0, 0, 0, 0);
    treeGrid3->collisionEnabled = 1;

    exitGameMap = createGameObject(SOLID, map->width - 30, treeGrid1->height, 40, treeGrid3->position.y - treeGrid1->height, gameMap);
    exitGameMap->color = al_map_rgba(0, 0, 0, 0);
    exitGameMap->collisionEnabled = 1;

    // create enemies
    for (int i=0; i<enemiesCount; i++){
        enemies[i]=createGameObject(ANIMATED_SPRITE, 0, 0, 36, 38, gameMap);
        setGameObjectAnimation(enemies[i], enemyBM2, 16, 18, 8, 15);
        enemies[i]->physics.enabled=1;
        enemies[i]->physics.friction=0.4;
        enemies[i]->physics.maxSpeed=4;
        enemies[i]->collisionEnabled=1;
        setOnGameObjectCollisionFunction(enemies[i], onEnemyCollision);
    }

    restartEnemiesPos();


    player = createGameObject(ANIMATED_SPRITE, 700, 50, 44, 50, insideBase);
    player->position.x = 450;
    player->position.y = 300;
    player->physics.enabled = 1;
    player->physics.friction = 0.4;
    player->physics.maxSpeed = 4;
    player->collisionEnabled = 1;
    player->collisionType = COLLISION_RECT;
    player->startCollisionOffset.y = 35;
    player->onCollision = onPlayerCollision;
    setGameObjectAnimation(player, loadBitmap(engine, "./assets/images/player-sprite-sheet.png"), 17, 20, 8, 15);
    insideBase->camera.followTarget = player;
    gameMap->camera.followTarget = player;

    //setupSceneWorld(gameMap, loadBitmap(engine, "./assets/images/map-sheet.png"), 16, 16);
    //loadMap("./map.CAE", gameMap);
    setupSceneWorld(gameMap, loadBitmap(engine, "./assets/images/gamemap.png"), 5000, 5000);
    addWorldTile(gameMap, 0, 0, 0, 0);

    baseObj = createGameObject(SPRITE, 0, 0, 335, 330, gameMap);
    ALLEGRO_BITMAP* baseBitMap = loadBitmap(engine, "./assets/images/base.png");
    setGameObjectBitmap(baseObj, baseBitMap);
    baseObj->collisionEnabled = 1;
    baseObj->collisionType = COLLISION_RECT;
    baseObj->startCollisionOffset.x = 80;
    baseObj->startCollisionOffset.y = 210;
    baseObj->endCollisionOffset.x = -60;
    baseObj->endCollisionOffset.y = -85;

    // add player to gamemap
    addGameObjectToScene(gameMap, player);


    // houses generation
    for (int i=0; i<map->width/500-1; i++){
        for (int j=0; j<map->height/500-1; j++){
            if (i == 0 && j == 0){ continue; }
            GameObject* h = createGameObject(SPRITE, 30+i*500, 170+j*500, 300, 300, gameMap);
            int houseId = randInt(0, 5);
            setGameObjectBitmap(h, housesBM[houseId]);
            h->collisionEnabled=1;
            h->startCollisionOffset.y = 150;
            h->endCollisionOffset.y=30;

            GameObject* plate = createGameObject(SPRITE, h->position.x+h->width-50, h->position.y+h->height-50, 50, 49, gameMap);
            setGameObjectBitmap(plate, plateBM);

        }
    }

    addTextToScene(gameMap, tutorialLetterContent);
    GameObject* houseTop = createGameObject(SPRITE, baseObj->position.x, baseObj->position.y-10, baseObj->width, baseObj->height/2 +70, gameMap);
    setGameObjectBitmap(houseTop, createSubBitmap(engine, baseBitMap,0,0,500,500/2 + 70));
    returnBase = createGameObject(SOLID, baseObj->position.x+ 170, baseObj->position.y +255, 5, 2, gameMap);
    returnBase->color = al_map_rgba(0, 0, 0, 0);
    returnBase->collisionEnabled = 1;
    GameObject* gridl1 = createGameObject(SOLID, baseObj->position.x + 79 , baseObj->position.y + 293, 55, 1, gameMap);
    gridl1->color = al_map_rgba(0, 0, 0, 0);
    gridl1->collisionEnabled = 1;
    GameObject* gridl2 = createGameObject(SOLID, baseObj->position.x + 79, baseObj->position.y + 280, 1, 10, gameMap);
    gridl2->color = al_map_rgba(0, 0, 0, 0);
    gridl2->collisionEnabled = 1;
    GameObject* gridr1 = createGameObject(SOLID, baseObj->position.x + 213, baseObj->position.y + 293, 59, 1, gameMap);
    gridr1->color = al_map_rgba(0, 0, 0, 0);
    gridr1->collisionEnabled = 1;
    GameObject* gridr2 = createGameObject(SOLID, baseObj->position.x + 273, baseObj->position.y + 278, 1, 13, gameMap);
    gridr2->color = al_map_rgba(0, 0, 0, 0);
    gridr2->collisionEnabled = 1;

    testBush=createGameObject(ANIMATED_SPRITE, 3000, 3000, 65, 65, gameMap);
    setGameObjectAnimation(testBush, loadBitmap(engine, "./assets/images/bush-sheet.png"), 16, 16, 4, 10);

    timeGameMap=createGameObject(SOLID, 0, 0, map->width, map->height, gameMap);

    addTextToScene(gameMap, mainMissionText);

    addButtonToScene(gameMap, letterStatus);
    Font* numberHouseFont = loadTTF(engine, "./assets/fonts/roboto.ttf", 20);
    closeHouseNumber = createText("Casa 72", engine->displayWidth/2-al_get_text_width(numberHouseFont->font, "Casa 100")/2, 20, 100, al_map_rgb(255, 100,0), al_map_rgba(0,0,0,0), NULL, numberHouseFont, 0, 0, gameMap);

    addTextToScene(gameMap, pressEMessage);

}
