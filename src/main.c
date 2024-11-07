#include "../CAE/include/CAE.h"
#include "../include/globals.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

CAEngine* engine;
Scene* mainMenu;
Scene* restart;
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
// enemiesCount and enemies[] length needs be equal
int enemiesCount=50;
GameObject* enemies[50];
GameObject* testBush;
GameObject* timeGameMap;
ALLEGRO_BITMAP* enemyBM1;
ALLEGRO_BITMAP* enemyBM2;
ALLEGRO_BITMAP* housesBM[6];
ALLEGRO_BITMAP* roadH;
ALLEGRO_BITMAP* roadV;
Font* lettersFont;
Text* tutorialLetterContent;
Text* pressEMessage;
Text* sinopseTchau;
Text* gameOverText;
float fallingLeafs[100][3];
float timeSet = 0;
char timeSetDir= 1;
// FIRST IS TUTORIAL
char* lettersTexts[3]= {
    "Aldo, sua missão como escoteiro será ajudar os aliados pró-revolução e entregar as cartas para deixar todos no quartel informados, mas cuidado, pois alguns soldados estão pelas ruas querendo prender qualquer sujeito que tente ajudar a revolução.",
    "Letter content 2",
    "Letter content 3"
};

int walkIndex = 0;

struct playerStatus{
    unsigned char isHidden; // ele esta escondido?
    unsigned char carryingLetter; // carregando uma carta?
    unsigned char firstZoomIn; // ja vez o primeiro zoom in?
    unsigned char tutorialLetter; // ja pegou a carta tutorial?
    int letterId; // qual conteudo da carta que ele esta levando
    int closeLetterId; // a carta que ele esta proximo, ou que vai pegar
    int gameOverCount; // quantas vezes ele foi pego
};
struct playerStatus playerStatus;

void onOpenMenu(Scene* scene)
{
    restart = engine->currentScene;
    changeScene(engine, mainMenu);
}

void onOpenRestart(Scene* scene)
{
    changeScene(engine, restart);
}

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
                if (playerStatus.closeLetterId == 0){
                    playerStatus.tutorialLetter=1;
                } else{
                    playerStatus.letterId = playerStatus.closeLetterId;
                    playerStatus.carryingLetter=1;
                }
                letterObj->visible = 0;
                pressEMessage->visible = 0;
            }
            else if (playerStatus.tutorialLetter) {
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

void onGameExit(Scene* scene){
    engine->isAlive=0;
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
    if (!playerStatus.tutorialLetter)
        return;
    player->position = (Vector2){ baseObj->position.x + baseObj->width/2 - 18, baseObj->position.y + baseObj->height - 98 };
    gameMap->camera.offset = (Vector2){ player->position.x, player->position.y };
    if (!playerStatus.firstZoomIn){
        gameMap->camera.zoom=0.2;
        playerStatus.firstZoomIn=1;
    }
    restartEnemiesPos();
    changeScene(engine, gameMap);
}

void onOpenGameMapR(Scene* scene) {
    player->position = (Vector2){ map->width  - 80, map->height - 360 };
    gameMap->camera.offset = (Vector2){ player->position.x, player->position.y };
    changeScene(engine, gameMap);
}

void onOpenQuartel(Scene* scene)
{
    player->position = (Vector2){ quartelobj->position.x + quartelobj->width / 2 - 100 ,  quartelobj->height - 60 };
    quartel->camera.offset = (Vector2){ player->position.x, player->position.y };
    changeScene(engine, quartel);

}
void onOpenQuartelRL(Scene* scene)
{
    player->position = (Vector2){ quartelobj->position.x + quartelobj->width / 2 - 300 ,  quartelobj->position.y + 930 };
    quartel->camera.offset = (Vector2){ player->position.x, player->position.y };
    changeScene(engine, quartel);

}

void onOpenQuartelRM(Scene* scene)
{
    player->position = (Vector2){ quartelobj->position.x + quartelobj->width / 2 + 70 ,  quartelobj->position.y + 930 };
    quartel->camera.offset = (Vector2){ player->position.x, player->position.y };
    changeScene(engine, quartel);

}

void onOpenQuartelRR(Scene* scene)
{
    player->position = (Vector2){ quartelobj->position.x + quartelobj->width / 2 + 440 ,  quartelobj->position.y + 930 };
    quartel->camera.offset = (Vector2){ player->position.x, player->position.y };
    changeScene(engine, quartel);

}

void onOpenRoomL(Scene* scene)
{
    player->position = (Vector2){ roomLobj->position.x + roomLobj->width / 2 - 100 ,  roomLobj->height - 60 };
    roomL->camera.offset = (Vector2){ player->position.x, player->position.y };
    changeScene(engine, roomL);

}

void onOpenRoomM(Scene* scene)
{
    player->position = (Vector2){ roomMobj->position.x + roomMobj->width / 2 - 10 ,  roomMobj->height - 60 };
    roomM->camera.offset = (Vector2){ player->position.x, player->position.y };
    changeScene(engine, roomM);

}

void onOpenRoomR(Scene* scene)
{
    player->position = (Vector2){ roomRobj->position.x + roomRobj->width / 2 + 130 ,  roomRobj->height - 60 };
    roomR->camera.offset = (Vector2){ player->position.x, player->position.y };
    changeScene(engine, roomR);
}

void onOpenSinopse(Scene* scene) {
    changeScene(engine, sinopse);
}

void onPlayerCollision(GameObject* self, GameObject* obj)
{
    if(obj == exitBase)
        onOpenGameMap(NULL);
    else if (obj == returnBase)
        onOpenBase(NULL);
    else if (obj == roomLC)
        onOpenRoomL(NULL);
    else if (obj == exitGameMap)
        onOpenQuartel(NULL);
    else if (obj == roomMC)
        onOpenRoomM(NULL);
    else if (obj == roomRC)
        onOpenRoomR(NULL);
    else if(obj == returnQuartelRoomL)
        onOpenQuartelRL(NULL);
    else if(obj == returnQuartelRoomM)
        onOpenQuartelRM(NULL);
    else if (obj == returnQuartelRoomR)
        onOpenQuartelRR(NULL);
    else if (obj == exitQuartel)
        onOpenGameMapR(NULL);
}

void onEnemyCollision(GameObject* self, GameObject* obj)
{
    if (obj == player)
    {
        if (playerStatus.isHidden == 0)
        {
            playerStatus.gameOverCount++;
            gameOverText->visible=1;
            onOpenBase(NULL);
        }
    }
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

    if (!playerStatus.tutorialLetter) {
        if (dist(player->position.x, player->position.y, player->width, player->height, letterObj->position.x, letterObj->position.y, letterObj->width, letterObj->height)
            <= 60) {
            playerStatus.closeLetterId = 0;
            pressEMessage->visible = 1;
        }
        else {
            pressEMessage->visible = 0;
            tutorialLetterContent->visible = 0;
        }
    }

    // enemies movement
    for (int i = 0; i < enemiesCount; i++) {
        if (dist(enemies[i]->position.x, enemies[i]->position.y, enemies[i]->width, enemies[i]->height, player->position.x, player->position.y, player->width, player->height) <= 200) {
            double hy = hypot(player->position.x - enemies[i]->position.x, player->position.y - enemies[i]->position.y);
            // find angle by cos formula
            enemies[i]->physics.acc.x = fabs(player->position.x - enemies[i]->position.x) / hy;
            // find angle by sin formula
            enemies[i]->physics.acc.y = fabs(player->position.y - enemies[i]->position.y) / hy;

            if (enemies[i]->position.x < player->position.x) {
                enemies[i]->physics.directions.x = 1;
                enemies[i]->animation.direction.x = 1;
            }
            else {
                enemies[i]->physics.directions.x = -1;
                enemies[i]->animation.direction.x = -1;
            }
            if (enemies[i]->position.y < player->position.y) {
                enemies[i]->physics.directions.y = 1;
                if (enemies[i]->physics.acc.x < 0.20) {
                    enemies[i]->animation.index.y = 1;
                }
            }
            else {
                enemies[i]->physics.directions.y = -1;
                if (enemies[i]->physics.acc.x < 0.20) {
                    enemies[i]->animation.index.y = 2;
                }
            }
            if (enemies[i]->physics.acc.x >= 0.20)
                enemies[i]->animation.index.y = 0;

            if (playerStatus.isHidden)
            {
                enemies[i]->physics.directions.x *= -1;
                enemies[i]->physics.directions.y *= -1;
                enemies[i]->animation.direction.x *= -1;
            }

        }
        else {
            enemies[i]->physics.acc.x = 0;
            enemies[i]->physics.acc.y = 0;
        }
    }


    // invisible bush test
    if (checkCollisionRect(player->position.x, player->position.y, player->width, player->height, testBush->position.x, testBush->position.y, testBush->width,  testBush->height)){
        testBush->animation.index.y = 1;
        playerStatus.isHidden = 1;
    } else{
        testBush->animation.index.y = 0;
        playerStatus.isHidden = 0;
    }


    if (timeSet > 140 || timeSet < 0){
        timeSetDir *= -1;
    }
    timeSet+=0.05*timeSetDir;
    timeGameMap->color = al_map_rgba(0,0,30/140*timeSet,(int)timeSet);

    if (engine->currentScene == gameMap && gameMap->camera.zoom < 1.5){
        gameMap->camera.zoom+=0.01;
        player->physics.acc.x=0;
        player->physics.acc.y=0;
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

    // loading the enemies sprite sheets
    enemyBM1= loadBitmap(engine, "./assets/images/idle-soldier1-sheet.png");
    enemyBM2= loadBitmap(engine, "./assets/images/idle-soldier2-sheet.png");

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
    playerStatus.closeLetterId = 0;
    playerStatus.gameOverCount = 0;

    // - - - MENU - - -
    mainMenu = createScene(engine, mainMenuScript);

    for (int i = 0; i < 100; i++) {
        fallingLeafs[i][0] = randInt(-100, engine->displayWidth - 5);
        fallingLeafs[i][1] = randInt(-500, 10);
        fallingLeafs[i][2] = randFloat(1, 2);
    }

    Font* titleFont = loadTTF(engine, "./assets/fonts/kalam-bold.ttf", 80);
    Font* stdMessageFont = loadTTF(engine, "./assets/fonts/kalam.ttf", 17);
    lettersFont = loadTTF(engine, "./assets/fonts/kalam.ttf", 18);
    char* titleText = "Revolução Em Cartas";
    createText(titleText, engine->displayWidth / 2 - al_get_text_width(titleFont->font, titleText) / 2, 50, 0, al_map_rgb(255, 255, 255), al_map_rgba(0, 0, 0, 0), NULL, titleFont, 0, 0, mainMenu);

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

    gameOverText=createText("Você foi pego!", 0, 0, 0, al_map_rgb(210, 10, 10), al_map_rgba(0, 0, 0, 20), NULL, titleFont, 20, 20, insideBase);
    gameOverText->position = (Vector2){engine->displayWidth/2-al_get_text_width(titleFont->font, gameOverText->text)/2, engine->displayHeight/2-al_get_font_line_height(titleFont->font)/2};
    gameOverText->visible=0;


    letterObj = createGameObject(ANIMATED_SPRITE, 467, 280, 12, 12, insideBase);
    setGameObjectAnimation(letterObj, loadBitmap(engine, "./assets/images/letter-sheet.png"), 12, 12, 5, 16);
    //como o letterContent tá sendo passado para >base, quando tenta abrir a carta em >gameMap ele não abre já que não está nessa cena e não dá pra incluir letterContent em >gameMap pq o addGameObjectToScene() não aceita parâmetro do tipo Text*
    //dependendo da resolução vai precisar mudar dentro de gameSceneScript que é onde gerencia a visibilidade da carta
    tutorialLetterContent = createText(lettersTexts[0],
        10, 200, 400, al_map_rgb(0, 0, 0), al_map_rgba(165, 132, 83, 245), NULL, lettersFont, 40, 20, insideBase);
    tutorialLetterContent->visible = 0;

    pressEMessage = createText("Pressione E para pegar a carta", engine->displayWidth / 2, 200, 0, al_map_rgb(255, 255, 255), al_map_rgba(0, 0, 0, 100), NULL, stdMessageFont, 40, 20, insideBase);
    pressEMessage->position.x -= al_get_text_width(stdMessageFont->font, pressEMessage->text) / 2;
    pressEMessage->visible = 0;


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
        enemies[i]=createGameObject(ANIMATED_SPRITE, 0, 0, 36, 40, gameMap);
        setGameObjectAnimation(enemies[i], enemyBM2, 16, 20, 4, 15);
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
    for (int i=0; i<map->width/500; i++){
        for (int j=0; j<map->height/500; j++){
            if (i == 0 && j == 0){ continue; }
            GameObject* h = createGameObject(SPRITE, 30+i*500, 170+j*500, 300, 300, gameMap);
            int houseId = randInt(0, 5);
            setGameObjectBitmap(h, housesBM[houseId]);
            h->collisionEnabled=1;
            h->startCollisionOffset.y = 150;
            h->endCollisionOffset.y=10;
            h->startCollisionOffset.x = 50;
            h->endCollisionOffset.x = -50;

            if (houseId == 3 || houseId == 4){
                h->startCollisionOffset.x = 20;
                h->endCollisionOffset.x = -20;
            }

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

    testBush=createGameObject(ANIMATED_SPRITE, 1000, 1000, 65, 65, gameMap);
    setGameObjectAnimation(testBush, loadBitmap(engine, "./assets/images/bush-sheet.png"), 16, 16, 4, 10);

     timeGameMap=createGameObject(SOLID, 0, 0, map->width, map->height, gameMap);

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

    //ROOMS

    roomLC = createGameObject(SOLID, 920, 940, 120, 10, quartel);
    roomLC->color = al_map_rgba(0, 0, 0, 0);
    roomLC->collisionEnabled = 1;
    roomLC->collisionType = COLLISION_RECT;

    roomMC = createGameObject(SOLID, 1290, 940, 130, 10, quartel);
    roomMC->color = al_map_rgba(0, 0, 0, 0);
    roomMC->collisionEnabled = 1;
    roomMC->collisionType = COLLISION_RECT;

    roomRC = createGameObject(SOLID, 1620, 940, 170, 10, quartel);
    roomRC->color = al_map_rgba(0, 0, 0, 0);
    roomRC->collisionEnabled = 1;
    roomRC->collisionType = COLLISION_RECT;

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

    exitQuartel = createGameObject(SOLID, treeD1->width, treeD1->position.y + 50, treeD2->position.x - treeD1->width, 10, quartel);
    exitQuartel->color = al_map_rgba(0, 0, 0, 0);
    exitQuartel->collisionEnabled = 1;
    exitQuartel->collisionType = COLLISION_RECT;

    //- - - SALA_ESQ - - -
    roomL = createScene(engine, gameSceneScript);
    roomL->camera.minLimit.x = 0;
    roomL->camera.minLimit.y = 0;
    roomL->camera.maxLimit.x = 1001;
    roomL->camera.maxLimit.y = 501;
    roomL->camera.zoom = 2.5;
    roomL->camera.followTarget = player;
    setupSceneWorld(roomL, loadBitmap(engine, "./assets/images/roomL.png"), 500, 500);
    addWorldTile(roomL, 0, 0, 0, 0);
    addGameObjectToScene(roomL, player);
    roomLobj = createGameObject(SOLID, 12, 12, 448, 468, roomL);
    roomLobj->color = al_map_rgba(0, 0, 0, 0);
    roomLobj->collisionEnabled = 1;
    roomLobj->collisionType = COLLISION_RECT;
    roomLobj->invertedCollision = 1;

    GameObject* fundoRoomL = createGameObject(SOLID, 500, 0, 1080, 1080, roomL);
    fundoRoomL->color = al_map_rgba(184, 118, 93, 255);

    GameObject* tableRoomL = createGameObject(SOLID, 70, 68, 260, 90, roomL);
    tableRoomL->color = al_map_rgba(0, 0, 0, 0);
    tableRoomL->collisionEnabled = 1;
    tableRoomL->collisionType = COLLISION_RECT;

    GameObject* benchRoomL = createGameObject(SOLID, 138, 25, 125, 28, roomL);
    benchRoomL->color = al_map_rgba(0, 0, 0, 0);
    benchRoomL->collisionEnabled = 1;
    benchRoomL->collisionType = COLLISION_RECT;

    returnQuartelRoomL = createGameObject(SOLID, 30, 478, 208, 28, roomL);
    returnQuartelRoomL->color = al_map_rgba(0, 0, 0, 0);
    returnQuartelRoomL->collisionEnabled = 1;
    returnQuartelRoomL->collisionType = COLLISION_RECT;

    //- - - SALA_M - - -
    roomM = createScene(engine, gameSceneScript);
    roomM->camera.minLimit.x = 0;
    roomM->camera.minLimit.y = 0;
    roomM->camera.maxLimit.x = 1001;
    roomM->camera.maxLimit.y = 501;
    roomM->camera.zoom = 2.5;
    roomM->camera.followTarget = player;
    setupSceneWorld(roomM, loadBitmap(engine, "./assets/images/roomM.png"), 500, 500);
    addWorldTile(roomM, 0, 0, 0, 0);
    addGameObjectToScene(roomM, player);
    roomMobj = createGameObject(SOLID, 12, 12, 468, 468, roomM);
    roomMobj->color = al_map_rgba(0, 0, 0, 0);
    roomMobj->collisionEnabled = 1;
    roomMobj->collisionType = COLLISION_RECT;
    roomMobj->invertedCollision = 1;

    GameObject* fundoRoomM = createGameObject(SOLID, 500, 0, 1080, 1080, roomM);
    fundoRoomM->color = al_map_rgba(184, 118, 93, 255);

    GameObject* bookCaseRM1 = createGameObject(SOLID, 40, 250, 20, 405, roomM);
    bookCaseRM1->color = al_map_rgba(0, 0, 0, 0);
    bookCaseRM1->collisionEnabled = 1;
    bookCaseRM1->collisionType = COLLISION_RECT;

    GameObject* bookCaseRM2 = createGameObject(SOLID, 460, 250, 20, 405, roomM);
    bookCaseRM2->color = al_map_rgba(0, 0, 0, 0);
    bookCaseRM2->collisionEnabled = 1;
    bookCaseRM2->collisionType = COLLISION_RECT;

    GameObject* tableRoomM = createGameObject(SOLID, 138, 68, 225, 65, roomM);
    tableRoomM->color = al_map_rgba(0, 0, 0, 0);
    tableRoomM->collisionEnabled = 1;
    tableRoomM->collisionType = COLLISION_RECT;

    GameObject* benchRoomM = createGameObject(SOLID, 170, 25, 160, 30, roomM);
    benchRoomM->color = al_map_rgba(0, 0, 0, 0);
    benchRoomM->collisionEnabled = 1;
    benchRoomM->collisionType = COLLISION_RECT;

    returnQuartelRoomM = createGameObject(SOLID, 140, 478, 208, 28, roomM);
    returnQuartelRoomM->color = al_map_rgba(0, 0, 0, 0);
    returnQuartelRoomM->collisionEnabled = 1;
    returnQuartelRoomM->collisionType = COLLISION_RECT;

    //- - - SALA_M - - -
    roomR = createScene(engine, gameSceneScript);
    roomR->camera.minLimit.x = 0;
    roomR->camera.minLimit.y = 0;
    roomR->camera.maxLimit.x = 1001;
    roomR->camera.maxLimit.y = 501;
    roomR->camera.zoom = 2.5;
    roomR->camera.followTarget = player;
    setupSceneWorld(roomR, loadBitmap(engine, "./assets/images/roomR.png"), 500, 500);
    addWorldTile(roomR, 0, 0, 0, 0);
    addGameObjectToScene(roomR, player);
    roomRobj = createGameObject(SOLID, 12, 12, 468, 468, roomR);
    roomRobj->color = al_map_rgba(0, 0, 0, 0);
    roomRobj->collisionEnabled = 1;
    roomRobj->collisionType = COLLISION_RECT;
    roomRobj->invertedCollision = 1;

    GameObject* fundoRoomR = createGameObject(SOLID, 500, 0, 1080, 1080, roomR);
    fundoRoomR->color = al_map_rgba(184, 118, 93, 255);

    GameObject* bookCaseRR1 = createGameObject(SOLID, 25, 25, 60, 30, roomR);
    bookCaseRR1->color = al_map_rgba(0, 0, 0, 0);
    bookCaseRR1->collisionEnabled = 1;
    bookCaseRR1->collisionType = COLLISION_RECT;

    GameObject* bookCaseRR2 = createGameObject(SOLID, 420, 25, 60, 30, roomR);
    bookCaseRR2->color = al_map_rgba(0, 0, 0, 0);
    bookCaseRR2->collisionEnabled = 1;
    bookCaseRR2->collisionType = COLLISION_RECT;

    GameObject* tableRoomR = createGameObject(SOLID, 138, 68, 225, 65, roomR);
    tableRoomR->color = al_map_rgba(0, 0, 0, 0);
    tableRoomR->collisionEnabled = 1;
    tableRoomR->collisionType = COLLISION_RECT;

    GameObject* benchRoomR = createGameObject(SOLID, 170, 25, 160, 30, roomR);
    benchRoomR->color = al_map_rgba(0, 0, 0, 0);
    benchRoomR->collisionEnabled = 1;
    benchRoomR->collisionType = COLLISION_RECT;

    returnQuartelRoomR = createGameObject(SOLID, 330, 478, 208, 28, roomR);
    returnQuartelRoomR->color = al_map_rgba(0, 0, 0, 0);
    returnQuartelRoomR->collisionEnabled = 1;
    returnQuartelRoomR->collisionType = COLLISION_RECT;



    // - - - SINOPSE - - -
    sinopse = createScene(engine, NULL);
    sinopse->backgroundColor = al_map_rgb(20, 20, 20);
    lettersFont->size = 200;
    sinopseTchau = createText("Final do jogo",
        0, 0, engine->displayWidth, al_map_rgb(0, 0, 0), al_map_rgb(155, 122, 73), NULL, titleFont, 520, 550, sinopse);
    restart = insideBase;

    while (engine->isAlive) {
        render(engine);
    }

    freeEngine(engine);
    printf("\nSuccessfully exited!\n");

    return 0;
}
