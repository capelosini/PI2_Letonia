#include "../CAE/include/CAE.h"
#include "../include/globals.h"
#include <allegro5/bitmap.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

CAEngine* engine;
Scene* mainMenu;
Scene* lastSceneBeforeMenu;
Scene* insideBase;
Scene* gameMap;
Scene* quartel;
Scene* roomL;
Scene* roomM;
Scene* roomR;
Scene* sinopse;
GameObject* player;
GameObject* ghostPlayerMenu;
GameObject* baseObj;
GameObject* letterObj;
GameObject* exitBase;
GameObject* returnBase;
GameObject* exitGameMap;
GameObject* roomLC;
GameObject* roomMC;
GameObject* roomRC;
GameObject* quartelobj;
GameObject* roomLobj;
GameObject* returnQuartelRoomL;
GameObject* roomMobj;
GameObject* returnQuartelRoomM;
GameObject* roomRobj;
GameObject* returnQuartelRoomR;
GameObject* gridGameMap;
GameObject* map;
GameObject* exitQuartel;
GameObject* enemies[50];
GameObject* testBush;
GameObject* timeGameMap;
ALLEGRO_BITMAP* enemyBM1;
ALLEGRO_BITMAP* enemyBM2;
ALLEGRO_BITMAP* housesBM[6];
ALLEGRO_BITMAP* roadH;
ALLEGRO_BITMAP* roadV;
ALLEGRO_BITMAP* plateBM;
ALLEGRO_BITMAP* letterStatusTrueBM;
ALLEGRO_BITMAP* letterStatusFalseBM;
Font* lettersFont;
Font* stdMessageFont;
Font* titleFont;
Text* tutorialLetterContent;
Text* pressEMessage;
Text* sinopseTchau;
Text* gameOverText;
Text* mainMissionText;
Text* closeHouseNumber;
Text* playerDialog;
Button* letterStatus;
float fallingLeafs[100][3];

// enemiesCount and enemies[] length needs be equal
int enemiesCount=50;
float timeSet = 0;
char timeSetDir= 1;
// FIRST IS TUTORIAL
char* lettersTexts[3]= {
    "Aldo, sua missão como escoteiro será ajudar os aliados pró-revolução e entregar as cartas para deixar todos no quartel informados, mas cuidado, pois alguns soldados estão pelas ruas querendo prender qualquer sujeito que tente ajudar a revolução.\n \nPressione Z para abrir/fechar.",
    "Letter content 2",
    "Letter content 3"
};

char* mainMissions[10] = {
    "Pegue a carta na mesa da base.",
    "Vá até a casa 72, e pegue a carta.",
    "Entregue a carta para ... no quartel que fica na parte inferior direita da cidade.",
    "Volte para a base para pegar a carta que te deixaram lá.",
    "Vá ao quartel entregar a carta para ...",
    "Entregue a carta para a estação ferroviaria da cidade."
};

char* dialogsTexts[3] = {
    "dialogo 1",
    "dialogo 2",
    "dialogo 3"
};

int walkIndex = 0;

struct playerStatus playerStatus;

//action handle on events
void onEvent(ALLEGRO_EVENT event, Scene * scene, CAEngine * engine) {
    if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
        gameOverText->visible=0;
        if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
            if(engine->currentScene != mainMenu)
            onOpenMenu(NULL);
        }
    }
    else if (event.type == ALLEGRO_EVENT_KEY_UP) {
        if (event.keyboard.keycode == ALLEGRO_KEY_E) {
            if (!(playerStatus.carryingLetter) && pressEMessage->visible) {
                // if letter is tutorial
                if (playerStatus.closeLetterId == 0) {
                    playerStatus.tutorialLetter=1;
                    tutorialLetterContent->visible = 1;
                } else {
                    playerStatus.letterId = playerStatus.closeLetterId;
                    playerStatus.carryingLetter=1;
                }
                letterObj->visible = 0;
                pressEMessage->visible = 0;
                playerStatus.mainMissionId++;
                changeText(mainMissionText, mainMissions[playerStatus.mainMissionId]);
            } else if (playerStatus.carryingLetter && pressEMessage->visible){ 
                playerStatus.inDialog = 1;
                pressEMessage->visible=0;
                changeText(playerDialog, dialogsTexts[playerStatus.dialogId]);
            }
        }
        else if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT && playerStatus.inDialog == 1) {
            playerDialog->visible=0;
            playerStatus.inDialog=0;
            playerStatus.carryingLetter=0;
            playerStatus.mainMissionId++;
            playerStatus.dialogId++;
            changeText(mainMissionText, mainMissions[playerStatus.mainMissionId]);
        }
        else if (event.keyboard.keycode == ALLEGRO_KEY_Z){
            if (playerStatus.tutorialLetter) {
                tutorialLetterContent->visible = !tutorialLetterContent->visible;
            }
        }
    }
}

// verify if object is on road
unsigned char isOnRoad(GameObject* obj) {
    double deltaY = obj->position.y / 500;
    deltaY -= (int)deltaY;

    double deltaX = obj->position.x / 500;
    deltaX -= (int)deltaX;

    double minY = 500. / 500;
    minY -= (int)minY;
    double maxY = (500. + 166 - obj->height) / 500;
    maxY -= (int)maxY;

    double minX = 837. / 500;
    minX -= (int)minX;
    double maxX = (837. + 166 - obj->width) / 500;
    maxX -= (int)maxX;

    return (deltaY > minY && deltaY < maxY) || (deltaX > minX && deltaX < maxX);
}


void restartEnemiesPos(){
    for (int i = 0; i < enemiesCount; i++)
    {
        do {
            enemies[i]->position.x = randInt(500, 4500);
            enemies[i]->position.y = randInt(500, 4500);
        } while(!isOnRoad(enemies[i]));
    }
}

int getPlayerNearHouse(){
    return (int)((player->position.x+100)/500+1)+((int)((player->position.y-250)/500)*10);
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

    // loading the enemies sprite sheets
    enemyBM1= loadBitmap(engine, "./assets/images/soldier1-sheet.png");
    enemyBM2= loadBitmap(engine, "./assets/images/soldier2-sheet.png");

    // load plate bitmap
    plateBM = loadBitmap(engine, "./assets/images/plate.png");

    // load UI letter status bitmap
    ALLEGRO_BITMAP* mainLetterStatusBM = loadBitmap(engine, "./assets/images/letter-status-sheet.png");
    letterStatusTrueBM=createSubBitmap(engine, mainLetterStatusBM, 0, 0, 12, 12);
    letterStatusFalseBM=createSubBitmap(engine, mainLetterStatusBM, 0, 12, 12, 12);

    // loading houses bitmaps
    for (int i=0; i<6; i++){
        char str[23] = "";
        sprintf(str, "./assets/images/c%d.png", i+1);
        housesBM[i] = loadBitmap(engine, str);
    }

    // loading roads bitmaps
    roadH = loadBitmap(engine, "./assets/images/estradal.png");
    roadV = loadBitmap(engine, "./assets/images/estrada.png");

    // set playerStatus to default
    playerStatus.isHidden = 0;
    playerStatus.carryingLetter = 0;
    playerStatus.firstZoomIn = 0;
    playerStatus.letterId = 0;
    playerStatus.inDialog = 0;
    playerStatus.closeLetterId = 0;
    playerStatus.gameOverCount = 0;
    playerStatus.mainMissionId = 0;
    playerStatus.dialogId = 0;

    loadMainMenu();
    loadBase();
    loadGameMap();
    loadQuartel();
    loadRoomLeft();
    loadRoomMiddle();
    loadRoomRight();
    loadSinopse();

    lastSceneBeforeMenu = insideBase;

    while (engine->isAlive) {
        render(engine);
    }


    // EXIT ANIMATION
    Vector2 pos = (Vector2){0, 0};
    float speed = 0;
    float acc = 0.5;
    Vector2 monSize = getMonitorSize(getCurrentDisplayAdapter(engine));
    changeWindowFullscreen(engine, 0);
    changeWindowNoFrame(engine, 1);
    changeWindowSize(engine, monSize.x, monSize.y);
    while (pos.y < engine->displayHeight){
        speed+=acc;
        pos.y+=speed;
        changeWindowPosition(engine, pos.x, pos.y);
        al_flip_display();
        al_rest(0.03);
    }
    /////////////

    freeEngine(engine);
    printf("\nSuccessfully exited!\n");

    return 0;
}
