#include "../../include/globals.h"

void loadBase(){
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

    gameOverText=createText("Você foi pego!", 0, 0, al_get_text_width(titleFont->font, "Você foi pego!")+41, al_map_rgb(210, 10, 10), al_map_rgba(0, 0, 0, 20), NULL, titleFont, 20, 20, insideBase);
    gameOverText->position = (Vector2){engine->displayWidth/2-al_get_text_width(titleFont->font, gameOverText->text)/2, engine->displayHeight/2-al_get_font_line_height(titleFont->font)/2};
    gameOverText->visible=0;


    letterObj = createGameObject(ANIMATED_SPRITE, 467, 280, 12, 12, insideBase);
    setGameObjectAnimation(letterObj, loadBitmap(engine, "./assets/images/letter-sheet.png"), 12, 12, 5, 16);
 
    tutorialLetterContent = createText(lettersTexts[0],
        10, 200, 500, al_map_rgb(0, 0, 0), al_map_rgba(165, 132, 83, 245), NULL, lettersFont, 40, 50, insideBase);
    tutorialLetterContent->visible = 0;

    pressEMessage = createText("Pressione E para interagir", engine->displayWidth / 2, 200, al_get_text_width(stdMessageFont->font, "Pressione E para interagir")+21, al_map_rgb(255, 255, 255), al_map_rgba(0, 0, 0, 100), NULL, stdMessageFont, 20, 20, insideBase);
    pressEMessage->position.x -= al_get_text_width(stdMessageFont->font, pressEMessage->text) / 2;
    pressEMessage->visible = 0;

    playerDialog = createText(dialogsTexts[playerStatus.dialogId], engine->displayWidth / 2 - 400, engine->displayHeight - 200, 800, al_map_rgb(255, 255, 255), al_map_rgba(0, 0, 0, 100), NULL, lettersFont, 20, 20, insideBase);
    playerDialog->visible = 0;


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

    mainMissionText = createText(mainMissions[0], engine->displayWidth-380, 130, 350, al_map_rgb(255,255,255), al_map_rgba(0,0,0,240)
    , NULL, lettersFont, 40, 30, insideBase);

    letterStatus = createButton(engine, 20, 20, 50, 50, al_map_rgba(0, 0, 0, 0), al_map_rgba(0, 0, 0, 0), "", "./assets/fonts/roboto.ttf", letterStatusFalseBM, NULL);
    addButtonToScene(insideBase, letterStatus);

}
