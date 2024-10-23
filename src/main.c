#include "../CAE/include/CAE.h"
#include "../include/globals.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

CAEngine* engine;
Scene* mainMenu;
Scene* stageOne;
Scene* respawn;
Scene* sinopse;
GameObject* player;
GameObject* ghostPlayerMenu;
GameObject* constructionQG;
GameObject* letterObj;
Font* lettersFont;
Text* letterTest;
Text* pressEMessage;
Text* sinopseTchau;
bool letterPicked = false;
float leafMatrix[100][3];

int walkIndex = 0;

//action handle on events
void onEvent(ALLEGRO_EVENT event, Scene* scene, CAEngine* engine) {
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
            else if (letterPicked) {
                letterTest->visible = !letterTest->visible;
            }
        }
    }
}


//stage change
void onStartRespawn(Scene* scene) {
    changeScene(engine, respawn);
}
void onStartStageOne(Scene* scene) {
    player->position = (Vector2){ stageOne->camera.maxLimit.x / 2 - player->width - 10, stageOne->camera.maxLimit.y / 2 + 10 };
    stageOne->camera.offset = (Vector2){ stageOne->camera.maxLimit.x - engine->displayWidth, stageOne->camera.maxLimit.y - engine->displayHeight };
    changeScene(engine, stageOne);
}
void onStartSinopse(Scene* scene) {
    changeScene(engine, sinopse);
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

    if (!(letterPicked)) {
        if (dist(player->position.x, player->position.y, player->width, player->height, letterObj->position.x, letterObj->position.y, letterObj->width, letterObj->height)
            <= 60) {
            pressEMessage->visible = 1;
        }
        else {
            pressEMessage->visible = 0;
            letterTest->visible = 0;
        }
    }
}

//menu animation
void mainMenuScript(Scene* self) {
    for (int i = 0; i < 100; i++) {
        al_draw_filled_rectangle(leafMatrix[i][0], leafMatrix[i][1], leafMatrix[i][0] + 7, leafMatrix[i][1] + 5, al_map_rgb(50, 180, 50));
        leafMatrix[i][1] += leafMatrix[i][2];
        leafMatrix[i][0] += 0.5;

        if (leafMatrix[i][1] > engine->displayHeight) {
            leafMatrix[i][0] = randInt(-100, engine->displayWidth - 5);
            leafMatrix[i][1] = randInt(-20, 0);
            leafMatrix[i][2] = randFloat(1, 2);
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

    // MENU
    mainMenu = createScene(engine, mainMenuScript);

    for (int i = 0; i < 100; i++) {
        leafMatrix[i][0] = randInt(-100, engine->displayWidth - 5);
        leafMatrix[i][1] = randInt(-500, 10);
        leafMatrix[i][2] = randFloat(1, 2);
    }

    Font* titleFont = loadTTF(engine, "./assets/fonts/kalam-bold.ttf", 80);
    Font* stdMessageFont = loadTTF(engine, "./assets/fonts/kalam.ttf", 17);
    lettersFont = loadTTF(engine, "./assets/fonts/kalam.ttf", 10);
    char* titleText = "Revolução Em Cartas";
    addText(titleText, engine->displayWidth / 2 - al_get_text_width(titleFont->font, titleText) / 2, 50, 0, al_map_rgb(255, 255, 255), al_map_rgba(0, 0, 0, 0), NULL, titleFont, 0, 0, mainMenu);

    addButtonToScene(mainMenu, createButton(engine, engine->displayWidth / 2 - 75, engine->displayHeight / 2 - 25, 150, 50, al_map_rgb(217, 95, 54), al_map_rgb(255, 255, 255), "Jogar", "./assets/fonts/roboto.ttf", NULL, onStartRespawn));

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

    //BASE
    respawn = createScene(engine, gameSceneScript);
    respawn->camera.zoom = 2;
    respawn->camera.minLimit.x = 50;
    respawn->camera.minLimit.y = 50;
    respawn->camera.maxLimit.x = 1400;
    respawn->camera.maxLimit.y = 1400;
    setupSceneWorld(respawn, loadBitmap(engine, "./assets/images/map.png"), 1400, 1400);
    addWorldTile(respawn, 0, 0, 0, 0);
    GameObject* respawnMap = createGameObject(SOLID, 50, 50, 1350, 1350, respawn);
    respawnMap->color = al_map_rgba(0, 0, 0, 0);
    respawnMap->collisionEnabled = 1;
    respawnMap->invertedCollision = 1;
    addButtonToScene(respawn, createButton(engine, 0, 0, 100, 50, al_map_rgb(150, 120, 70), al_map_rgb(20, 20, 20), "Mundo Aberto", "./assets/fonts/roboto.ttf", NULL, onStartStageOne));

    player = createGameObject(ANIMATED_SPRITE, 700, 50, 44, 50, respawn);
    player->position.x = 600;
    player->position.y = 400;
    player->physics.enabled = 1;
    player->physics.friction = 0.4;
    player->physics.maxSpeed = 4;
    player->collisionEnabled = 1;
    player->collisionType = COLLISION_RECT;
    player->startCollisionOffset.y = 15;
    setGameObjectAnimation(player, loadBitmap(engine, "./assets/images/player-sprite-sheet.png"), 17, 20, 8, 15);
    respawn->camera.followTarget = player;


    // MAPA DO JOGO
    stageOne = createScene(engine, gameSceneScript);
    stageOne->camera.maxLimit.x = 200 * 16;
    stageOne->camera.maxLimit.y = 100 * 16;
    stageOne->camera.minLimit.x = 0;
    stageOne->camera.minLimit.y = 0;
    stageOne->camera.zoom = 2;
    stageOne->camera.followTarget = player;

    constructionQG = createGameObject(SPRITE, stageOne->camera.maxLimit.x / 2, stageOne->camera.maxLimit.y / 2, 160, 160, stageOne);
    setGameObjectBitmap(constructionQG, loadBitmap(engine, "./assets/images/QG.png"));

    constructionQG->collisionEnabled = 1;
    constructionQG->collisionType = COLLISION_RECT;
    constructionQG->startCollisionOffset.x = 20;
    constructionQG->startCollisionOffset.y = 30;
    constructionQG->endCollisionOffset.x = -20;
    constructionQG->endCollisionOffset.y = -15;

    letterObj = createGameObject(ANIMATED_SPRITE, 200, 200, 12, 12, stageOne);
    setGameObjectAnimation(letterObj, loadBitmap(engine, "./assets/images/letter-sheet.png"), 12, 12, 5, 16);
    //letterObj->collisionEnabled = 1;

    addGameObjectToScene(stageOne, player);

    letterTest = addText("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Ut ut tincidunt elit. Nunc a magna at nulla tempor iaculis. Curabitur at enim sollicitudin, varius nisi vel, viverra odio. Ut porta metus sed metus gravida elementum. Pellentesque ut mi id quam euismod convallis. Duis vulputate tempus sagittis. Quisque aliquam justo justo, eget lobortis neque tempor non.Integer porta volutpat turpis, nec venenatis ante volutpat sit amet. Proin condimentum vitae augue id tincidunt. Donec tristique lectus non dui pellentesque tincidunt. In sit amet leo suscipit, feugiat leo id, condimentum tellus. Proin vel tempor metus. Mauris in auctor velit. Donec justo justo, iaculis eget pellentesque eget, interdum a nibh. Aenean tincidunt tempor sem. Integer eget elementum metus. Suspendisse non fringilla nunc, sit amet suscipit diam.Suspendisse a justo lorem. Phasellus ac nulla sed arcu fermentum sollicitudin.",
        300, 200, 250, al_map_rgb(0, 0, 0), al_map_rgba(155, 122, 73, 100), NULL, lettersFont, 40, 20, stageOne);
    letterTest->visible = 0;

    pressEMessage = addText("Pressione E para pegar a carta", engine->displayWidth / 2, 200, 0, al_map_rgb(255, 255, 255), al_map_rgba(0, 0, 0, 100), NULL, stdMessageFont, 40, 20, stageOne);
    pressEMessage->position.x -= al_get_text_width(stdMessageFont->font, pressEMessage->text) / 2;
    pressEMessage->visible = 0;

    //setupSceneWorld(stageOne, loadBitmap(engine, "./assets/images/map.png"), 3840, 2560);
    setupSceneWorld(stageOne, loadBitmap(engine, "./assets/images/map-sheet.png"), 16, 16);
    loadMap("./map.CAE", stageOne);
    GameObject* map = createGameObject(SOLID, 0, 0, 200 * 16, 100 * 16, stageOne);
    map->color = al_map_rgba(0, 0, 0, 0);
    map->collisionEnabled = 1;
    map->collisionType = COLLISION_RECT;
    map->invertedCollision = 1;

    addButtonToScene(stageOne, createButton(engine, 0, 0, 100, 50, al_map_rgb(150, 120, 70), al_map_rgb(20, 20, 20), "sinopse", "./assets/fonts/roboto.ttf", NULL, onStartSinopse));

    //SINOPSE
    sinopse = createScene(engine, NULL);
    sinopse->backgroundColor = al_map_rgb(20, 20, 20);
    lettersFont->size = 200;
    sinopseTchau = addText("Final do jogo",
        0, 0, engine->displayWidth, al_map_rgb(0, 0, 0), al_map_rgb(155, 122, 73), NULL, titleFont, 520, 550, sinopse);


    while (engine->isAlive) {
        printf("x = %f  e y = %f \n", player->position.x, player->position.y);
        render(engine);
    }

    freeEngine(engine);

    printf("\nSuccessfully exited!\n");

    return 0;
}