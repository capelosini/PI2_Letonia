#include "../CAE/include/CAE.h"
#include "../include/globals.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

CAEngine* engine;
Scene* mainMenu;
Scene* gameMap;
Scene* insideBase;
Scene* sinopse;
GameObject* player;
GameObject* ghostPlayerMenu;
GameObject* baseObj;
GameObject* letterObj;
GameObject* obj8;
GameObject* returnBase;
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
    player->position = (Vector2){ gameMap->camera.maxLimit.x / 2 - player->width - 140, gameMap->camera.maxLimit.y / 2 - 95  };
    gameMap->camera.offset = (Vector2){ gameMap->camera.maxLimit.x - engine->displayWidth, gameMap->camera.maxLimit.y - engine->displayHeight };
    changeScene(engine, gameMap);
}

void onOpenSinopse(Scene* scene) {
    changeScene(engine, sinopse);
}

void onPlayerCollision(GameObject* self, GameObject* obj){
    if(obj == obj8)
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

    // fonts
    Font* titleFont = loadTTF(engine, "./assets/fonts/kalam-bold.ttf", 80);
    Font* stdMessageFont = loadTTF(engine, "./assets/fonts/kalam.ttf", 17);
    lettersFont = loadTTF(engine, "./assets/fonts/kalam.ttf", 10);

    // - - - MENU - - -
    mainMenu = createScene(engine, mainMenuScript);

    for (int i = 0; i < 100; i++) {
        fallingLeafs[i][0] = randInt(-100, engine->displayWidth - 5);
        fallingLeafs[i][1] = randInt(-500, 10);
        fallingLeafs[i][2] = randFloat(1, 2);
    }

    char* titleText = "Revolução Em Cartas";
    createText(titleText, engine->displayWidth / 2 - al_get_text_width(titleFont->font, titleText) / 2, 50, 0, al_map_rgb(255, 255, 255), al_map_rgba(0, 0, 0, 0), NULL, titleFont, 0, 0, mainMenu);

    addButtonToScene(mainMenu, createButton(engine, engine->displayWidth / 2 - 75, engine->displayHeight / 2 - 25, 150, 50, al_map_rgb(217, 95, 54), al_map_rgb(255, 255, 255), "Jogar", "./assets/fonts/roboto.ttf", NULL, onOpenBase));

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
    
    // DID: ao invés do botão, criar um if dentro de um swich case no while e verificar (player.y >= 950 & player.x >= esquerda da escada & <= direita da escada) que ative onOpenGameMap
    //addButtonToScene(insideBase, createButton(engine, 0, 0, 100, 50, al_map_rgb(150, 120, 70), al_map_rgb(20, 20, 20), "Mundo Aberto", "./assets/fonts/roboto.ttf", NULL, onOpenGameMap));

    letterObj = createGameObject(ANIMATED_SPRITE, 467, 280, 12, 12, insideBase);
    setGameObjectAnimation(letterObj, loadBitmap(engine, "./assets/images/letter-sheet.png"), 12, 12, 5, 16);
    //como o letterContent tá sendo passado para >base, quando tenta abrir a carta em >gameMap ele não abre já que não está nessa cena e não dá pra incluir letterContent em >gameMap pq o addGameObjectToScene() não aceita parâmetro do tipo Text*
    //dependendo da resolução vai precisar mudar dentro de gameSceneScript que é onde gerencia a visibilidade da carta 
    letterContent = createText("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Ut ut tincidunt elit. Nunc a magna at nulla tempor iaculis. Curabitur at enim sollicitudin, varius nisi vel, viverra odio. Ut porta metus sed metus gravida elementum. Pellentesque ut mi id quam euismod convallis. Duis vulputate tempus sagittis. Quisque aliquam justo justo, eget lobortis neque tempor non.Integer porta volutpat turpis, nec venenatis ante volutpat sit amet. Proin condimentum vitae augue id tincidunt. Donec tristique lectus non dui pellentesque tincidunt. In sit amet leo suscipit, feugiat leo id, condimentum tellus. Proin vel tempor metus. Mauris in auctor velit. Donec justo justo, iaculis eget pellentesque eget, interdum a nibh. Aenean tincidunt tempor sem. Integer eget elementum metus. Suspendisse non fringilla nunc, sit amet suscipit diam.Suspendisse a justo lorem. Phasellus ac nulla sed arcu fermentum sollicitudin.",
        300, 200, 250, al_map_rgb(0, 0, 0), al_map_rgba(155, 122, 73, 200), NULL, lettersFont, 40, 20, insideBase);
    letterContent->visible = 0;

    pressEMessage = createText("Pressione E para pegar a carta", engine->displayWidth / 2, 200, 0, al_map_rgb(255, 255, 255), al_map_rgba(0, 0, 0, 100), NULL, stdMessageFont, 40, 20, insideBase);
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
    player->startCollisionOffset.y = 15;
    player->onCollision = onPlayerCollision;
    setGameObjectAnimation(player, loadBitmap(engine, "./assets/images/player-sprite-sheet.png"), 17, 20, 8, 15);
    insideBase->camera.followTarget = player;
    GameObject*  obj1 = createGameObject(SOLID, 10, 10, 78, 38, insideBase);
    obj1 ->color = al_map_rgba(0, 0, 0, 0);
    obj1->collisionEnabled = 1 ;
    GameObject* obj2 = createGameObject(SOLID, 10, 150, 17, 70, insideBase);
    obj2->color = al_map_rgba(0, 0, 0, 0);
    obj2->collisionEnabled = 1;
    GameObject* obj3 = createGameObject(SOLID, 10, 275, 17, 45, insideBase);
    obj3->color = al_map_rgba(0, 0, 0, 0);
    obj3->collisionEnabled = 1;
    GameObject* obj4 = createGameObject(SOLID, 340, 175, 300, 100, insideBase);
    obj4->color = al_map_rgba(0, 0, 0, 0);
    obj4->collisionEnabled = 1;
    GameObject* obj5 = createGameObject(SOLID, 940, 20, 30 , 105, insideBase);
    obj5->color = al_map_rgba(0, 0, 0, 0);
    obj5->collisionEnabled = 1;
    GameObject* obj6 = createGameObject(SOLID, 975, 175, 30, 100, insideBase);
    obj6->color = al_map_rgba(0, 0, 0, 0);
    obj6->collisionEnabled = 1;
    GameObject* obj7 = createGameObject(SOLID, 975, 325, 30, 45, insideBase);
    obj7->color = al_map_rgba(0, 0, 0, 0);
    obj7->collisionEnabled = 1;
    obj8 = createGameObject(SOLID, 350, 985, 275, 10, insideBase); 
    obj8->color = al_map_rgba(0, 0, 0, 0); 
    obj8->collisionEnabled = 1; 


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

    baseObj = createGameObject(SPRITE, gameMap->camera.maxLimit.x / 2 - 335, gameMap->camera.maxLimit.y / 2 - 330, 335, 330, gameMap);
    ALLEGRO_BITMAP* baseBitMap = loadBitmap(engine, "./assets/images/base.png");
    setGameObjectBitmap(baseObj, baseBitMap);
    baseObj->collisionEnabled = 1;
    baseObj->collisionType = COLLISION_RECT;
    baseObj->startCollisionOffset.x = 80;
    baseObj->startCollisionOffset.y = 210;
    baseObj->endCollisionOffset.x = -60;
    baseObj->endCollisionOffset.y = -85;
    addGameObjectToScene(gameMap, player);
    GameObject* houseTop = createGameObject(SPRITE, baseObj->position.x, baseObj->position.y-10, baseObj->width, baseObj->height/2 +70, gameMap);
    setGameObjectBitmap(houseTop, createSubBitmap(engine, baseBitMap,0,0,500,500/2 + 70));
    returnBase = createGameObject(SOLID, baseObj->position.x+ 170, baseObj->position.y +245, 5, 2, gameMap);
    returnBase->color = al_map_rgba(0, 0, 0, 0);
    returnBase->collisionEnabled = 1;
    GameObject* left = createGameObject(SOLID, baseObj->position.x + 79 , baseObj->position.y + 293, 55, 1, gameMap);
    left->color = al_map_rgba(0, 0, 0, 0);
    left->collisionEnabled = 1;
    GameObject* left2 = createGameObject(SOLID, baseObj->position.x + 79, baseObj->position.y + 280, 1, 10, gameMap);
    left2->color = al_map_rgba(0, 0, 0, 0);
    left2->collisionEnabled = 1;
    GameObject* right = createGameObject(SOLID, baseObj->position.x + 213, baseObj->position.y + 293, 59, 1, gameMap);
    right->color = al_map_rgba(0, 0, 0, 0);
    right->collisionEnabled = 1;
    GameObject* right2 = createGameObject(SOLID, baseObj->position.x + 273, baseObj->position.y + 278, 1, 13, gameMap);
    right2->color = al_map_rgba(0, 0, 0, 0);
    right2->collisionEnabled = 1;

    addTextToScene(gameMap, letterContent);

  
    //addGameObjectToScene(gameMap, letterContent);


    //addButtonToScene(gameMap, createButton(engine, 0, 0, 100, 50, al_map_rgb(150, 120, 70), al_map_rgb(20, 20, 20), "sinopse", "./assets/fonts/roboto.ttf", NULL, onOpenSinopse));

    // - - - SINOPSE - - -
    sinopse = createScene(engine, NULL);
    sinopse->backgroundColor = al_map_rgb(20, 20, 20);
    lettersFont->size = 200;
    sinopseTchau = createText("Final do jogo",
        0, 0, engine->displayWidth, al_map_rgb(0, 0, 0), al_map_rgb(155, 122, 73), NULL, titleFont, 520, 550, sinopse);


    while (engine->isAlive) {
        render(engine);
    }

    freeEngine(engine);
    printf("\nSuccessfully exited!\n");

    return 0;
}