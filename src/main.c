#include "../CAE/include/CAE.h"
#include "../include/globals.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

CAEngine* engine;
Scene* mainMenu;
Scene* insideBase;
Scene* gameMap;
Scene* quartel;
Scene* sinopse;
GameObject* player;
GameObject* ghostPlayerMenu;
GameObject* baseObj;
GameObject* letterObj;
GameObject* exitBase;
GameObject* returnBase;
GameObject* quartelobj;
Font* lettersFont;
Text* letterContent;
Text* pressEMessage;
Text* sinopseTchau;
bool letterPicked = false;
float fallingLeafs[100][3];

int walkIndex = 0;

//action handle on events
void onEvent(ALLEGRO_EVENT event, Scene * scene, CAEngine * engine) {
    if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
        if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
            engine->isAlive = 0;
        }
    }
    else if (event.type == ALLEGRO_EVENT_KEY_UP) {
        if (event.keyboard.keycode == ALLEGRO_KEY_E) {
            if (!(letterPicked) && pressEMessage->visible) {
                letterPicked = true;
                letterObj->visible = 0;
                pressEMessage->visible = 0;
            }
            else if (letterPicked ) {
                letterContent->visible = !letterContent->visible;
            }
        }
    }
}


//stage change
void onOpenBase(Scene* scene) {
    if (engine->currentScene == gameMap)
    {
        player->position.x = 450;
        player->position.y = 900;
    }
    changeScene(engine, insideBase);
}
void onOpenGameMap(Scene* scene) {
    player->position = (Vector2){ baseObj->position.x + baseObj->width/2 - 18, baseObj->position.y + baseObj->height - 98 };
    gameMap->camera.offset = (Vector2){ player->position.x, player->position.y };
    changeScene(engine, gameMap);
}
void onOpenQuartel(Scene* scene)
{
    player->position = (Vector2){ quartelobj->position.x + quartelobj->width / 2 - 100 ,  quartelobj->height - 60 };
    quartel->camera.offset = (Vector2){ player->position.x, player->position.y };
    changeScene(engine, quartel);

}
void onOpenSinopse(Scene* scene) {
    changeScene(engine, sinopse);
}

void onPlayerCollision(GameObject* self, GameObject* obj)
{
    if(obj == exitBase)
    onOpenGameMap(NULL);
    if (obj == returnBase)
    onOpenBase(NULL);

}


//character movement and letter handling
void gameSceneScript(Scene* self) {
    ALLEGRO_KEYBOARD_STATE keyState;
    al_get_keyboard_state(&keyState);

    Vector2 mov = getMovementVector2(&keyState, ALLEGRO_KEY_A, ALLEGRO_KEY_D, ALLEGRO_KEY_W, ALLEGRO_KEY_S);

    player->physics.acc = (Vector2){ abs(mov.x), abs(mov.y) };

    if (mov.y != 0) {
        player->physics.directions.y = mov.y;
        if (mov.y < 0) {
            walkIndex = 2;
        }
        else {
            walkIndex = 1;
        }
    }
    if (mov.x != 0) {
        walkIndex = 0;
        player->physics.directions.x = mov.x;
        player->animation.direction.x = mov.x;
    }

    player->animation.index.y = 3 + walkIndex;
    if (!(mov.x || mov.y))
        player->animation.index.y = walkIndex;

    if (!letterPicked) {
        if (dist(player->position.x, player->position.y, player->width, player->height, letterObj->position.x, letterObj->position.y, letterObj->width, letterObj->height)
            <= 60) {
            pressEMessage->visible = 1;
        }
        else {
            pressEMessage->visible = 0;
            letterContent->visible = 0;
        }
    }

}


//menu animation
void mainMenuScript(Scene* self) {
    for (int i = 0; i < 100; i++) {
        al_draw_filled_rectangle(fallingLeafs[i][0], fallingLeafs[i][1], fallingLeafs[i][0] + 7, fallingLeafs[i][1] + 5, al_map_rgb(50, 180, 50));
        fallingLeafs[i][1] += fallingLeafs[i][2];
        fallingLeafs[i][0] += 0.5;

        if (fallingLeafs[i][1] > engine->displayHeight) {
            fallingLeafs[i][0] = randInt(-100, engine->displayWidth - 5);
            fallingLeafs[i][1] = randInt(-20, 0);
            fallingLeafs[i][2] = randFloat(1, 2);
        }
    }

    if (ghostPlayerMenu->position.x >= engine->displayWidth / 2 - ghostPlayerMenu->width) {
        ghostPlayerMenu->physics.acc.x = 0;
        ghostPlayerMenu->animation.index.y = 1;
        if (al_get_timer_count(engine->timer) / 60 >= 3) {
            ghostPlayerMenu->physics.acc.x = 1;
            ghostPlayerMenu->animation.index.y = 3;
            ghostPlayerMenu->physics.directions.x = 1;
        }
    }
    else {
        al_set_timer_count(engine->timer, 0);
    }

}


int main() {
    GameConfig engineConfig;
    engineConfig.fps = 60;
    engineConfig.posX = 20;
    engineConfig.posY = 20;
    engineConfig.sizeX = 1280;
    engineConfig.sizeY = 720;
    engineConfig.title = "Letonia Project";
    engineConfig.fullscreen = 1;

    engine = initEngine(engineConfig);
    setEventFunction(engine, onEvent);


    // - - - MENU - - -
    mainMenu = createScene(engine, mainMenuScript);

    for (int i = 0; i < 100; i++) {
        fallingLeafs[i][0] = randInt(-100, engine->displayWidth - 5);
        fallingLeafs[i][1] = randInt(-500, 10);
        fallingLeafs[i][2] = randFloat(1, 2);
    }

    Font* titleFont = loadTTF(engine, "./assets/fonts/kalam-bold.ttf", 80);
    Font* stdMessageFont = loadTTF(engine, "./assets/fonts/kalam.ttf", 17);
    lettersFont = loadTTF(engine, "./assets/fonts/kalam.ttf", 10);
    char* titleText = "Revolução Em Cartas";
    addText(titleText, engine->displayWidth / 2 - al_get_text_width(titleFont->font, titleText) / 2, 50, 0, al_map_rgb(255, 255, 255), al_map_rgba(0, 0, 0, 0), NULL, titleFont, 0, 0, mainMenu);

    addButtonToScene(mainMenu, createButton(engine, engine->displayWidth / 2 - 75, engine->displayHeight / 2 - 25, 150, 50, al_map_rgb(217, 95, 54), al_map_rgb(255, 255, 255), "Jogar", "./assets/fonts/roboto.ttf", NULL, onOpenQuartel));

    mainMenu->backgroundColor = al_map_rgb(0, 0, 20);

    ghostPlayerMenu = createGameObject(ANIMATED_SPRITE, -60, engine->displayHeight - 200, 45, 40, mainMenu);
    setGameObjectAnimation(ghostPlayerMenu, loadBitmap(engine, "./assets/images/player-sprite-sheet.png"), 17, 20, 8, 15);
    ghostPlayerMenu->animation.index.y = 3;
    ghostPlayerMenu->physics.enabled = 1;
    ghostPlayerMenu->physics.friction = 0.2;
    ghostPlayerMenu->physics.maxSpeed = 4;
    ghostPlayerMenu->physics.directions.x = 1;
    ghostPlayerMenu->physics.acc.x = 1;

    changeScene(engine, mainMenu);

    // - - - BASE - - -
    insideBase = createScene(engine, gameSceneScript);
    insideBase->camera.minLimit.x = 5;
    insideBase->camera.minLimit.y = 5;
    insideBase->camera.maxLimit.x = 1001;
    insideBase->camera.maxLimit.y = 1001;
    insideBase->camera.zoom = 2;
    setupSceneWorld(insideBase, loadBitmap(engine, "./assets/images/map.png"),1000 , 1000);
    addWorldTile(insideBase, 0, 0, 0, 0);
    GameObject* baseRoom = createGameObject(SOLID, 10, 0, 990, 990, insideBase);
    baseRoom->color = al_map_rgba(0, 0, 0, 0);
    baseRoom->collisionEnabled = 1;
    baseRoom->invertedCollision = 1;
    

    letterObj = createGameObject(ANIMATED_SPRITE, 467, 280, 12, 12, insideBase);
    setGameObjectAnimation(letterObj, loadBitmap(engine, "./assets/images/letter-sheet.png"), 12, 12, 5, 16);
    //como o letterContent tá sendo passado para >base, quando tenta abrir a carta em >gameMap ele não abre já que não está nessa cena e não dá pra incluir letterContent em >gameMap pq o addGameObjectToScene() não aceita parâmetro do tipo Text*
    //dependendo da resolução vai precisar mudar dentro de gameSceneScript que é onde gerencia a visibilidade da carta 
    letterContent = addText("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Ut ut tincidunt elit. Nunc a magna at nulla tempor iaculis. Curabitur at enim sollicitudin, varius nisi vel, viverra odio. Ut porta metus sed metus gravida elementum. Pellentesque ut mi id quam euismod convallis. Duis vulputate tempus sagittis. Quisque aliquam justo justo, eget lobortis neque tempor non.Integer porta volutpat turpis, nec venenatis ante volutpat sit amet. Proin condimentum vitae augue id tincidunt. Donec tristique lectus non dui pellentesque tincidunt. In sit amet leo suscipit, feugiat leo id, condimentum tellus. Proin vel tempor metus. Mauris in auctor velit. Donec justo justo, iaculis eget pellentesque eget, interdum a nibh. Aenean tincidunt tempor sem. Integer eget elementum metus. Suspendisse non fringilla nunc, sit amet suscipit diam.Suspendisse a justo lorem. Phasellus ac nulla sed arcu fermentum sollicitudin.",
        300, 200, 250, al_map_rgb(0, 0, 0), al_map_rgba(155, 122, 73, 200), NULL, lettersFont, 40, 20, insideBase);
    letterContent->visible = 0;

    pressEMessage = addText("Pressione E para pegar a carta", engine->displayWidth / 2, 200, 0, al_map_rgb(255, 255, 255), al_map_rgba(0, 0, 0, 100), NULL, stdMessageFont, 40, 20, insideBase);
    pressEMessage->position.x -= al_get_text_width(stdMessageFont->font, pressEMessage->text) / 2;
    pressEMessage->visible = 0;

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
    GameObject*  boxl = createGameObject(SOLID, 20, 20, 78, 50, insideBase);
    boxl ->color = al_map_rgba(0, 0, 0, 0);
    boxl->collisionEnabled = 1 ;
    GameObject* benchl1 = createGameObject(SOLID, 20, 150, 17, 95, insideBase);
    benchl1->color = al_map_rgba(0, 0, 0, 0);
    benchl1->collisionEnabled = 1;
    GameObject* benchl2 = createGameObject(SOLID, 20, 275, 17, 72, insideBase);
    benchl2->color = al_map_rgba(0, 0, 0, 0);
    benchl2->collisionEnabled = 1;
    GameObject* table = createGameObject(SOLID, 340, 175, 300, 115, insideBase);
    table->color = al_map_rgba(0, 0, 0, 0);
    table->collisionEnabled = 1;
    GameObject* boxr = createGameObject(SOLID, 940, 20, 30 , 125, insideBase);
    boxr->color = al_map_rgba(0, 0, 0, 0);
    boxr->collisionEnabled = 1;
    GameObject* benchr1 = createGameObject(SOLID, 960, 175, 30, 115, insideBase);
    benchr1->color = al_map_rgba(0, 0, 0, 0);
    benchr1->collisionEnabled = 1;
    GameObject* benchr2 = createGameObject(SOLID, 960, 325, 30, 63, insideBase);
    benchr2->color = al_map_rgba(0, 0, 0, 0);
    benchr2->collisionEnabled = 1;
    exitBase = createGameObject(SOLID, 350, 985, 275, 10, insideBase); 
    exitBase->color = al_map_rgba(0, 0, 0, 0); 
    exitBase->collisionEnabled = 1; 









    // - - - MAPA DO JOGO - - -
    gameMap = createScene(engine, gameSceneScript);
    gameMap->camera.minLimit.x = 0;
    gameMap->camera.minLimit.y = 0;
    gameMap->camera.maxLimit.x = 200 * 16;
    gameMap->camera.maxLimit.y = 100 * 16;
    gameMap->camera.zoom = 1.5;
    gameMap->camera.followTarget = player;
    //setupSceneWorld(gameMap, loadBitmap(engine, "./assets/images/map.png"), 3840, 2560);
    setupSceneWorld(gameMap, loadBitmap(engine, "./assets/images/map-sheet.png"), 16, 16);
    loadMap("./map.CAE", gameMap);
    GameObject* map = createGameObject(SOLID, 0, 0, 200 * 16, 100 * 16, gameMap);
    map->color = al_map_rgba(0, 0, 0, 0);
    map->collisionEnabled = 1;
    map->collisionType = COLLISION_RECT;
    map->invertedCollision = 1;

    baseObj = createGameObject(SPRITE, 0, 0, 335, 330, gameMap);
    ALLEGRO_BITMAP* baseBitMap = loadBitmap(engine, "./assets/images/base.png");
    setGameObjectBitmap(baseObj, baseBitMap);
    baseObj->collisionEnabled = 1;
    baseObj->collisionType = COLLISION_RECT;
    baseObj->startCollisionOffset.x = 80;
    baseObj->startCollisionOffset.y = 210;
    baseObj->endCollisionOffset.x = -60;
    baseObj->endCollisionOffset.y = -85;
    addGameObjectToScene(gameMap, player);
    addTextToScene(gameMap, letterContent);
    GameObject* houseTop = createGameObject(SPRITE, baseObj->position.x, baseObj->position.y-10, baseObj->width, baseObj->height/2 +70, gameMap);
    setGameObjectBitmap(houseTop, createSubBitmap(engine, baseBitMap,0,0,500,500/2 + 70));
    returnBase = createGameObject(SOLID, baseObj->position.x+ 170, baseObj->position.y +245, 5, 2, gameMap);
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


    //- - - QUARTEL - - -
    quartel = createScene(engine, gameSceneScript);
    quartel->camera.minLimit.x = 0;
    quartel->camera.minLimit.y = 0;
    quartel->camera.maxLimit.x = 2500;
    quartel->camera.maxLimit.y = 2500;
    quartel->camera.zoom = 1;
    quartel->camera.followTarget = player;
    setupSceneWorld(quartel, loadBitmap(engine, "./assets/images/quartel.png"), 2500, 2500);
    addWorldTile(quartel, 0, 0, 0, 0);
    addGameObjectToScene(quartel, player);
    quartelobj = createGameObject(SOLID, 0, 0, 2500, 2500, quartel);
    quartelobj->color = al_map_rgba(0, 0, 0, 0);
    quartelobj->collisionEnabled = 1;
    quartelobj->collisionType = COLLISION_RECT;
    quartelobj->invertedCollision = 1;

    //LAGO DA ESQUERDA
    GameObject* pondl1 = createGameObject(SOLID, 106, 1766, 750, 600, quartel);
    pondl1->color = al_map_rgba(0, 0, 0, 0);
    pondl1->collisionEnabled = 1;
    pondl1->collisionType = COLLISION_RECT;
    GameObject* pondl2 = createGameObject(SOLID, 125, 1735, 700, 10, quartel);
    pondl2->color = al_map_rgba(0, 0, 0, 0);
    pondl2->collisionEnabled = 1;
    pondl2->collisionType = COLLISION_RECT;
    GameObject* pondl3 = createGameObject(SOLID, 115, 1745, 725, 25, quartel);
    pondl3->color = al_map_rgba(0, 0, 0, 0);
    pondl3->collisionEnabled = 1;
    pondl3->collisionType = COLLISION_RECT;
    GameObject* pondl4 = createGameObject(SOLID, 855, 1790, 30, 600, quartel);
    pondl4->color = al_map_rgba(0, 0, 0, 0);
    pondl4->collisionEnabled = 1;
    pondl4->collisionType = COLLISION_RECT;
    GameObject* pondl5 = createGameObject(SOLID, 885, 1820, 30, 570, quartel);
    pondl5->color = al_map_rgba(0, 0, 0, 0);
    pondl5->collisionEnabled = 1;
    pondl5->collisionType = COLLISION_RECT;
    GameObject* pondl6 = createGameObject(SOLID, 915, 1850, 30, 540, quartel);
    pondl6->color = al_map_rgba(0, 0, 0, 0);
    pondl6->collisionEnabled = 1;
    pondl6->collisionType = COLLISION_RECT;
    GameObject* pondl7 = createGameObject(SOLID, 945, 1880, 30, 510, quartel);
    pondl7->color = al_map_rgba(0, 0, 0, 0);
    pondl7->collisionEnabled = 1;
    pondl7->collisionType = COLLISION_RECT;
    GameObject* pondl8 = createGameObject(SOLID, 975, 1910, 30, 471, quartel);
    pondl8->color = al_map_rgba(0, 0, 0, 0);
    pondl8->collisionEnabled = 1;
    pondl8->collisionType = COLLISION_RECT;
    GameObject* pondl9 = createGameObject(SOLID, 126, 2350, 730, 40, quartel);
    pondl9->color = al_map_rgba(0, 0, 0, 0);
    pondl9->collisionEnabled = 1;
    pondl9->collisionType = COLLISION_RECT;

    //LAGO DA DIREITA
    GameObject* pondr1 = createGameObject(SOLID, 1320, 1755, 1070, 603, quartel);
    pondr1->color = al_map_rgba(0, 0, 0, 00);
    pondr1->collisionEnabled = 1;
    pondr1->collisionType = COLLISION_RECT;
    GameObject* pondr2 = createGameObject(SOLID, 1335, 2366, 1046, 18, quartel);
    pondr2->color = al_map_rgba(0, 0, 0, 0);
    pondr2->collisionEnabled = 1;
    pondr2->collisionType = COLLISION_RECT;
    GameObject* pondr3 = createGameObject(SOLID, 1260, 1736, 1110, 70, quartel);
    pondr3->color = al_map_rgba(0, 0, 0, 0);
    pondr3->collisionEnabled = 1;
    pondr3->collisionType = COLLISION_RECT;
    GameObject* pondr4 = createGameObject(SOLID, 1240, 1750, 30, 30, quartel);
    pondr4->color = al_map_rgba(0, 0, 0, 0);
    pondr4->collisionEnabled = 1;
    pondr4->collisionType = COLLISION_RECT;
    GameObject* pondr5 = createGameObject(SOLID, 1285, 1810, 30, 30, quartel);
    pondr5->color = al_map_rgba(0, 0, 0, 0);
    pondr5->collisionEnabled = 1;
    pondr5->collisionType = COLLISION_RECT;

    //QUARTEL

    GameObject* wallL1 = createGameObject(SOLID, 140, 640, 10, 1000, quartel);
    wallL1->color = al_map_rgba(0, 0, 0, 0);
    wallL1->collisionEnabled = 1;
    wallL1->collisionType = COLLISION_RECT;
    GameObject*  wallL2 = createGameObject(SOLID, 140, 640, 240, 313, quartel);
    wallL2->color = al_map_rgba(0, 0, 0, 0);
    wallL2->collisionEnabled = 1;
    wallL2->collisionType = COLLISION_RECT;
    GameObject* wallL3 = createGameObject(SOLID, 140, 1060, 260, 380, quartel);
    wallL3->color = al_map_rgba(0, 0, 0, 0);
    wallL3->collisionEnabled = 1;
    wallL3->collisionType = COLLISION_RECT;
    GameObject* wallL4 = createGameObject(SOLID, 140, 1440, 240, 250, quartel);
    wallL4->color = al_map_rgba(0, 0, 0, 0);
    wallL4->collisionEnabled = 1;
    wallL4->collisionType = COLLISION_RECT;
    GameObject* wallL5 = createGameObject(SOLID, 380, 1590, 20, 50, quartel);
    wallL5->color = al_map_rgba(0, 0, 0, 0);
    wallL5->collisionEnabled = 1;
    wallL5->collisionType = COLLISION_RECT;

    GameObject* wallD1= createGameObject(SOLID, 380, 1680, 520, 10, quartel);
    wallD1->color = al_map_rgba(0, 0, 0, 0);
    wallD1->collisionEnabled = 1;
    GameObject* wallD2 = createGameObject(SOLID, 1140, 1680, 1220, 10, quartel);
    wallD2->color = al_map_rgba(0, 0, 0, 0);
    wallD1->collisionType = COLLISION_RECT;
    wallD2->collisionEnabled = 1;
    wallD2->collisionType = COLLISION_RECT;

    GameObject* wallR1 = createGameObject(SOLID, 2120, 640, 240, 560, quartel);
    wallR1->color = al_map_rgba(0, 0, 0, 0);
    wallR1->collisionEnabled = 1;
    wallR1->collisionType = COLLISION_RECT;
    GameObject* wallR2= createGameObject(SOLID, 2100, 1060, 260, 360, quartel);
    wallR2->color = al_map_rgba(0, 0, 0, 0);
    wallR2->collisionEnabled = 1;
    wallR2->collisionType = COLLISION_RECT;
    GameObject* wallR3 = createGameObject(SOLID, 2080, 1400, 280, 280, quartel);
    wallR3->color = al_map_rgba(0, 0, 0, 0);
    wallR3->collisionEnabled = 1;
    wallR3->collisionType = COLLISION_RECT;

    GameObject* wallU1 = createGameObject(SOLID, 140, 640, 2220, 10, quartel);
    wallU1->color = al_map_rgba(0, 0, 0, 0);
    wallU1->collisionEnabled = 1;
    wallU1->collisionType = COLLISION_RECT;
    GameObject* wallU2 = createGameObject(SOLID, 760, 640, 1140, 300, quartel);
    wallU2->color = al_map_rgba(0, 0, 0, 0);
    wallU2->collisionEnabled = 1;
    wallU2->collisionType = COLLISION_RECT;

    //FLORESTA
    GameObject* treeL = createGameObject(SOLID, 0,0, 60, 2500, quartel);
    treeL->color = al_map_rgba(0, 0, 0, 0);
    treeL->collisionEnabled = 1;
    treeL->collisionType = COLLISION_RECT;

    GameObject* treeD1 = createGameObject(SOLID, 60, 2440, 1000, 60, quartel);
    treeD1->color = al_map_rgba(0, 0, 0, 0);
    treeD1->collisionEnabled = 1;
    treeD1->collisionType = COLLISION_RECT;
    GameObject* treeD2 = createGameObject(SOLID, 1280, 2440, 1220, 60, quartel);
    treeD2->color = al_map_rgba(0, 0, 0, 0);
    treeD2->collisionEnabled = 1;
    treeD2->collisionType = COLLISION_RECT;

    GameObject* treeR = createGameObject(SOLID, 2440, 0, 60, 2500, quartel);
    treeR->color = al_map_rgba(0, 0, 0, 0);
    treeR->collisionEnabled = 1;
    treeR->collisionType = COLLISION_RECT;

    GameObject* treeU = createGameObject(SOLID, 60, 0, 2380, 570, quartel);
    treeU->color = al_map_rgba(0, 0, 0, 0);
    treeU->collisionEnabled = 1;
    treeU->collisionType = COLLISION_RECT;




    // - - - SINOPSE - - -
    sinopse = createScene(engine, NULL);
    sinopse->backgroundColor = al_map_rgb(20, 20, 20);
    lettersFont->size = 200;
    sinopseTchau = addText("Final do jogo",
        0, 0, engine->displayWidth, al_map_rgb(0, 0, 0), al_map_rgb(155, 122, 73), NULL, titleFont, 520, 550, sinopse);


    while (engine->isAlive) {
        render(engine);
    }

    freeEngine(engine);
    printf("\nSuccessfully exited!\n");

    return 0;
}