#include "../../include/globals.h"

void loadMainMenu(){
    mainMenu = createScene(engine, mainMenuScript);

    for (int i = 0; i < 100; i++) {
        fallingLeafs[i][0] = randInt(-100, engine->displayWidth - 5);
        fallingLeafs[i][1] = randInt(-300, 10);
        fallingLeafs[i][2] = randFloat(1, 2);
    }

    titleFont = loadTTF(engine, "./assets/fonts/kalam-bold.ttf", 80);
    stdMessageFont = loadTTF(engine, "./assets/fonts/kalam.ttf", 17);
    lettersFont = loadTTF(engine, "./assets/fonts/kalam.ttf", 18);
    char* titleText = "Revolução Em Cartas";
    createText(titleText, engine->displayWidth / 2 - al_get_text_width(titleFont->font, titleText) / 2, 50, al_get_text_width(titleFont->font, titleText)+10, al_map_rgb(255, 255, 255), al_map_rgba(0, 0, 0, 0), NULL, titleFont, 0, 0, mainMenu);

    addButtonToScene(mainMenu, createButton(engine, engine->displayWidth / 2 - 200./2, engine->displayHeight / 2 - 25, 200, 50, al_map_rgb(217, 95, 54), al_map_rgb(255, 255, 255), "Jogar", "./assets/fonts/roboto.ttf", NULL, onOpenRestart));
    addButtonToScene(mainMenu, createButton(engine, engine->displayWidth / 2 - 75, engine->displayHeight / 2 + 55, 150, 40, al_map_rgb(210, 20, 20), al_map_rgb(255, 255, 255), "Sair", "./assets/fonts/roboto.ttf", NULL, onGameExit));
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

    char out[] = "Você foi pego 0000 vezes";
    sprintf(out, "Você foi pego %d vezes", playerStatus.gameOverCount);
    gameOverCountText = createText(out, engine->displayWidth/2-al_get_text_width(stdMessageFont->font, out)/2, engine->displayHeight-40, al_get_text_width(stdMessageFont->font, out)+10, al_map_rgb(255, 255, 255), al_map_rgba(0, 0, 0, 0), NULL, stdMessageFont, 0, 0, mainMenu);
    gameOverCountText->visible = 0;
}
