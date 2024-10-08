#include "CAE/include/CAE.h"
#include <stdio.h>

CAEngine* engine;
Scene* mainMenu;
Scene* stageOne;
GameObject* player;

void onEvent(ALLEGRO_EVENT event, Scene* scene, CAEngine* engine){
    if (event.type == ALLEGRO_EVENT_KEY_DOWN){
        if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE){
            engine->isAlive = 0;
        }
    }
}

void onStartGame (Scene* scene) {
    changeScene(engine, stageOne);
    freeScene(mainMenu);
}

void gameSceneScript(Scene* self) {
    ALLEGRO_KEYBOARD_STATE keyState;
    al_get_keyboard_state(&keyState);
    
    char up = al_key_down(&keyState, ALLEGRO_KEY_W);
    char down = al_key_down(&keyState, ALLEGRO_KEY_S);
    char left = al_key_down(&keyState, ALLEGRO_KEY_A);
    char right = al_key_down(&keyState, ALLEGRO_KEY_D);

    if (up) {
        player->physics.directions.y = -1;
        player->physics.acc.y = 1;
        player->animation.index.y = 2;
    } else if (down) {
        player->physics.directions.y = 1;
        player->physics.acc.y = 1;
        player->animation.index.y = 1;
    } else {
        player->physics.acc.y = 0;
    }
    
    if (left) {
        player->physics.directions.x = -1;
        player->physics.acc.x = 1;
        player->animation.index.y = 0;
    } else if (right) {
        player->physics.directions.x = 1;
        player->physics.acc.x = 1;
        player->animation.index.y = 0;
    } else {
        player->physics.acc.x = 0;
    }
}

int main (){
    GameConfig engineConfig;
    engineConfig.fps=60;
    engineConfig.posX=0;
    engineConfig.posY=0;
    engineConfig.sizeX=1280;
    engineConfig.sizeY=720;
    engineConfig.title="Letonia Project";
    engineConfig.fullscreen=1;

    engine = initEngine(engineConfig);
    setEventFunction(engine, onEvent);

    // MENU
    mainMenu = createScene(engine, NULL);

    ALLEGRO_BITMAP *bgImage = loadBitmap(engine, "assets/images/menu.jpg");
    GameObject* background = createGameObject(SPRITE, 0, 0, engine->displayWidth, engine->displayHeight, mainMenu);
    setGameObjectBitmap(background, bgImage);

    Font* titleFont = loadTTF(engine, "assets/fonts/kalam-bold.ttf", 80);
    char* titleText = "Revolução em cartas";
    addTextToScene(mainMenu, createText(titleText, engine->displayWidth/2 - al_get_text_width(titleFont->font, titleText)/2, 50, al_map_rgb(255, 255, 255), titleFont));

    addButtonToScene(mainMenu, createButton(engine, engine->displayWidth / 2 - 75, engine->displayHeight / 2 - 25, 150, 50, al_map_rgb(217, 95, 54), al_map_rgb(255, 255, 255), "Começar", "assets/fonts/roboto.ttf", NULL, onStartGame));

    changeScene(engine, mainMenu);

    // FASE
    stageOne = createScene(engine, gameSceneScript);
    player = createGameObject(ANIMATED_SPRITE, 100, 100, 17, 20, stageOne);
    setGameObjectAnimation(player, loadBitmap(engine, "assets/images/walk-sheet.png"), 17, 20, 8, 10);
    stageOne->camera.followTarget = player;

    setupSceneWorld(stageOne, loadBitmap(engine, "assets/images/map.webp"), 10, 10);
    for (int i=0; i<10; i++){
        for (int j=0; j<10; j++){
            if (i==0 && j==0){
                addWorldTile(stageOne, 0, 0, j, i);
            } else if (i==0){
                addWorldTile(stageOne, 1, 0, j, i);
            } 
            else{
                addWorldTile(stageOne, 1, 1, j, i);
            }
        }
    }


    while (engine->isAlive){
        render(engine);
    }

    freeEngine(engine);

    return 0;
}