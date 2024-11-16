#ifndef GLOBALS_H
#define GLOBALS_H

#include "../CAE/include/CAE.h"

extern CAEngine* engine;
extern Scene* mainMenu;
extern Scene* lastSceneBeforeMenu;
extern Scene* insideBase;
extern Scene* gameMap;
extern Scene* quartel;
extern Scene* roomL;
extern Scene* roomM;
extern Scene* roomR;
extern Scene* sinopse;
extern Scene* letterShow;
extern GameObject* player;
extern GameObject* ghostPlayerMenu;
extern GameObject* baseObj;
extern GameObject* letterObj;
extern GameObject* exitBase;
extern GameObject* returnBase;
extern GameObject* exitGameMap;
extern GameObject* roomLC;
extern GameObject* roomMC;
extern GameObject* roomRC;
extern GameObject* quartelobj;
extern GameObject* roomLobj;
extern GameObject* returnQuartelRoomL;
extern GameObject* roomMobj;
extern GameObject* returnQuartelRoomM;
extern GameObject* roomRobj;
extern GameObject* returnQuartelRoomR;
extern GameObject* gridGameMap;
extern GameObject* map;
extern GameObject* exitQuartel;
// enemiesCount and enemies[] length needs be equal
extern int enemiesCount;
extern GameObject* enemies[50];
extern GameObject* testBush;
extern GameObject* timeGameMap;
extern ALLEGRO_BITMAP* enemyBM1;
extern ALLEGRO_BITMAP* enemyBM2;
extern ALLEGRO_BITMAP* housesBM[6];
extern ALLEGRO_BITMAP* roadH;
extern ALLEGRO_BITMAP* roadV;
extern ALLEGRO_BITMAP* plateBM;
extern ALLEGRO_BITMAP* letterStatusTrueBM;
extern ALLEGRO_BITMAP* letterStatusFalseBM;
extern Font* lettersFont;
extern Font* stdMessageFont;
extern Font* titleFont;
extern Text* tutorialLetterContent;
extern Text* pressEMessage;
extern Text* sinopseTchau;
extern Text* gameOverText;
extern Text* mainMissionText;
extern Text* closeHouseNumber;
extern Text* letterShowText;
extern Button* letterStatus;
extern float fallingLeafs[100][3];
extern float timeSet;
extern char timeSetDir;
// FIRST IS TUTORIAL
extern char* lettersTexts[4];
extern char* mainMissions[10];

extern int walkIndex;

struct playerStatus{
    unsigned char isHidden; // ele esta escondido?
    unsigned char carryingLetter; // carregando uma carta?
    unsigned char firstZoomIn; // ja vez o primeiro zoom in?
    unsigned char tutorialLetter; // ja pegou a carta tutorial?
    int letterId; // qual conteudo da carta que ele esta levando
    int closeLetterId; // a carta que ele esta proximo, ou que vai pegar
    int gameOverCount; // quantas vezes ele foi pego
    int mainMissionId; // id da missao principal
    unsigned char isLastSafeZoneQuartel; // last safe zone is quartel?
};
extern struct playerStatus playerStatus;

void onOpenMenu(Scene* scene);
void onOpenRestart(Scene* scene);
void onGameExit(Scene* scene);
void onOpenBase(Scene* scene);
void onOpenGameMap(Scene* scene);
void onOpenGameMapR(Scene* scene);
void onOpenQuartel(Scene* scene);
void onOpenQuartelRL(Scene* scene);
void onOpenQuartelRM(Scene* scene);
void onOpenQuartelRR(Scene* scene);
void onOpenRoomL(Scene* scene);
void onOpenRoomM(Scene* scene);
void onOpenRoomR(Scene* scene);
void onOpenSinopse(Scene* scene);
void onPlayerCollision(GameObject* self, GameObject* obj);
void onEnemyCollision(GameObject* self, GameObject* obj);

void restartEnemiesPos();
unsigned char isOnRoad(GameObject* obj);
int getPlayerNearHouse();

void mainMenuScript(Scene* self);
void loadMainMenu();
void gameSceneScript(Scene* self);
void loadBase();
void restartEnemiesPos();
void loadGameMap();
void loadQuartel();
void loadRoomLeft();
void loadSinopse();
void loadRoomRight();
void loadRoomMiddle();
void loadLetterShow();
void letterShowScript(Scene* self);

#endif
