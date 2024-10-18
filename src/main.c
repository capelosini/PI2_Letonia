#include "../CAE/include/CAE.h"
#include "../include/globals.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

CAEngine* engine;
Scene* mainMenu;
Scene* stageOne;
GameObject* player;
Font* lettersFont;
Text* letterTest;
float leafMatrix[100][3];

int walkIndex = 0;

//kill engine
void onEvent(ALLEGRO_EVENT event, Scene* scene, CAEngine* engine){
    if (event.type == ALLEGRO_EVENT_KEY_DOWN){
        if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE){
            engine->isAlive = 0;
        }
    }
}

//stage change
void onStartStageOne(Scene* scene) {
    changeScene(engine, stageOne);
}

//character movement and letter handling
void gameSceneScript(Scene* self) {
    ALLEGRO_KEYBOARD_STATE keyState;
    al_get_keyboard_state(&keyState);
    
    char up = al_key_down(&keyState, ALLEGRO_KEY_W);
    char down = al_key_down(&keyState, ALLEGRO_KEY_S);
    char left = al_key_down(&keyState, ALLEGRO_KEY_A);
    char right = al_key_down(&keyState, ALLEGRO_KEY_D);
    char e = al_key_down(&keyState, ALLEGRO_KEY_E);
    
    if (up) {
        player->physics.directions.y = -1;
        player->physics.acc.y = 1;
        walkIndex = 2;
    } else if (down) {
        player->physics.directions.y = 1;
        player->physics.acc.y = 1;
        walkIndex = 1;
    } else {
        player->physics.acc.y = 0;
    }
    
    if (left) {
        player->physics.directions.x = -1;
        player->physics.acc.x = 1;
        player->animation.direction.x = -1;
        walkIndex = 0;
    } else if (right) {
        player->physics.directions.x = 1;
        player->physics.acc.x = 1;
        player->animation.direction.x = 1;
        walkIndex = 0;
    } else {
        player->physics.acc.x = 0;
    }

    player->animation.index.y = 3 + walkIndex;
    if (!(up || down || left || right))
        player->animation.index.y = walkIndex;

    if(e){
        letterTest->visible=1;
    } else {
        letterTest->visible=0;
    }
}

void leafEffect (Scene* self) {
    for (int i = 0; i < 100; i++) {
        al_draw_filled_rectangle(leafMatrix[i][0], leafMatrix[i][1], leafMatrix[i][0] + 7, leafMatrix[i][1] + 5, al_map_rgb(50, 180, 50));
        leafMatrix[i][1] += leafMatrix[i][2];
        leafMatrix[i][0] += 0.5;

        if (leafMatrix[i][1] > engine->displayHeight) {
            leafMatrix[i][0] = rand() % (engine->displayWidth + 100) - 100;
            leafMatrix[i][1] = rand() % engine->displayHeight - engine->displayHeight;
            leafMatrix[i][2] = rand() % 2 + 1;
        }
    }
}

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
    
    Font* titleFont = loadTTF(engine, "./assets/fonts/kalam-bold.ttf", 80);
    lettersFont = loadTTF(engine, "./assets/fonts/kalam.ttf", 10);
    char* titleText = "Revolução Em Cartas";
    createText(titleText, engine->displayWidth/2 - al_get_text_width(titleFont->font, titleText)/2, 50, 0, al_map_rgb(255, 255, 255), al_map_rgba(0,0,0,0), NULL, titleFont, 0, 0, mainMenu);

    addButtonToScene(mainMenu, createButton(engine, engine->displayWidth / 2 - 75, engine->displayHeight / 2 - 25, 150, 50, al_map_rgb(217, 95, 54), al_map_rgb(255, 255, 255), "Jogar", "./assets/fonts/roboto.ttf", NULL, onStartStageOne));

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
    setGameObjectAnimation(player, loadBitmap(engine, "./assets/images/player-sprite-sheet.png"), 17, 20, 8, 15);
    stageOne->camera.followTarget = player;

    letterTest=createText("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Ut ut tincidunt elit. Nunc a magna at nulla tempor iaculis. Curabitur at enim sollicitudin, varius nisi vel, viverra odio. Ut porta metus sed metus gravida elementum. Pellentesque ut mi id quam euismod convallis. Duis vulputate tempus sagittis. Quisque aliquam justo justo, eget lobortis neque tempor non.Integer porta volutpat turpis, nec venenatis ante volutpat sit amet. Proin condimentum vitae augue id tincidunt. Donec tristique lectus non dui pellentesque tincidunt. In sit amet leo suscipit, feugiat leo id, condimentum tellus. Proin vel tempor metus. Mauris in auctor velit. Donec justo justo, iaculis eget pellentesque eget, interdum a nibh. Aenean tincidunt tempor sem. Integer eget elementum metus. Suspendisse non fringilla nunc, sit amet suscipit diam.Suspendisse a justo lorem. Phasellus ac nulla sed arcu fermentum sollicitudin. Suspendisse potenti. Aenean a augue venenatis, rhoncus sapien ut, laoreet felis. Vivamus mi neque, iaculis ac ligula eget, fermentum maximus elit. Praesent at elementum lorem, et tincidunt leo. Nunc ut lacinia ligula. Aliquam eu est finibus, iaculis ipsum vitae, dignissim risus. Proin pulvinar urna sit amet metus pharetra, eget pulvinar nisl sodales. Proin aliquam dolor at urna dignissim maximus. Nulla imperdiet varius pulvinar", 
    300, 200, 250, al_map_rgb(255, 255, 255), al_map_rgba(0, 0, 0, 100), NULL, lettersFont, 40, 20, stageOne);

    setupSceneWorld(stageOne, loadBitmap(engine, "./assets/images/map.png"), 3840, 2560);
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