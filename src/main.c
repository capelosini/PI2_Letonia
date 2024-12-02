#include "../include/globals.h"
#include "../include/saving.h"

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
Scene* letterShow;
GameObject* player;
GameObject* ghostPlayerMenu;
GameObject* politician;
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
ALLEGRO_SAMPLE* clickSound;
ALLEGRO_AUDIO_STREAM* menuMusic;
ALLEGRO_AUDIO_STREAM* stepsSound;
ALLEGRO_AUDIO_STREAM* cityNoise;
ALLEGRO_AUDIO_STREAM* introMusic;
ALLEGRO_AUDIO_STREAM* chaseMusic;
Font* lettersFont;
Font* stdMessageFont;
Font* titleFont;
Text* tutorialLetterContent;
Text* pressEMessage;
Text* sinopseTchau;
Text* gameOverText;
Text* mainMissionText;
Text* closeHouseNumber;
Text* letterShowText;
Text* playerDialog;
Text* gameOverCountText;
Button* letterStatus;
float fallingLeafs[100][3];

// enemiesCount and enemies[] length needs be equal
int enemiesCount = 50;
float timeSet = 0;
char timeSetDir = 1;
// FIRST IS TUTORIAL
char* lettersTexts[5]= {
    "Aldo, sua missão como escoteiro será ajudar os aliados pró-revolução e entregar as cartas para deixar todos no quartel informados, mas cuidado, pois alguns soldados estão pelas ruas querendo prender qualquer sujeito que tente ajudar a revolução.\nW/A/S/D: Movimentação | E: Interação\n \nPressione F para abrir/fechar.",
    "Aldo, um político precisa ser levado até o general Isidoro, precisamos que você o guie até o quartel sem que sejam pegos pelos soldados, espere a noite cair para que diminuam os soldados nas ruas",
    "9 de julho de 1932\n \nGeneral, São Paulo não aceitará a centralização do poder imposta por Getúlio Vargas. O povo clama por uma nova Constituição, e lideranças políticas e militares se uniram para defender a democracia. A revolução começou, e todos os esforços estão concentrados na organização das tropas.",
    "25 de julho de 1932\n \nAs batalhas são intensas. Resistimos bravamente em várias frentes, mas estamos em desvantagem contra as forças federais. O apoio da população é nossa força: eles arrecadam ouro, doam mantimentos e costuram uniformes. Mesmo em meio às dificuldades, lutamos por uma causa justa.",
    "18 de setembro de 1932\n \nApós meses de luta, nossos recursos chegaram ao fim, e as forças inimigas são superiores. Apesar da derrota militar, nossa causa ecoou no Brasil. Getúlio Vargas já anuncia uma Assembleia Constituinte. Nosso sacrifício não foi em vão: a democracia renascerá."
};

char* mainMissions[10] = {
    "Pegue a carta na mesa da base.",
    "Vá até a casa 43, e pegue a carta.",
    "Entregue a carta para o General Bertoldo Klinger no quartel na parte inferior direita da cidade.",
    "Volte para a base para pegar a carta que te deixaram lá.",
    "Vá até a casa 12",
    "Vá ao quartel levar o político para o General Isidoro Dias Lopes",
    "Pegue uma carta com o General Euclides de Oliveira Figueiredo da sala à direita.",
    "Entregue a carta para os aliados na estação ferroviaria da cidade.",
    "Aproveite o mapa e se divirta fugindo dos soldados!",
    ""
};

char* dialogsTexts[4] = {
    "Estamos muito contentes de ter um jovem como você disposto a ajudar nossa causa, ótimo trabalho Aldo!",
    "Você é o Aldo? Vá na frente eu te acompanho!",
    "Os soldados lá fora podem parecer assustadores mas lembre, não é o medo que deve te guiar, mas o desejo de cumprir seu dever.",
    "Não sabemos por quanto tempo essa guerra ainda durará... Mas eu gostaria de agradecê-lo por todos seu serviços Aldo, é uma honra tê-lo ao nosso lado."
};

int walkIndex = 0;
Scene* allScenes[9];

struct PlayerStatus playerStatus;

//action handle on events
void onEvent(ALLEGRO_EVENT event, Scene * scene, CAEngine * engine) {
    if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
        gameOverText->visible=0;
        if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
            if (engine->currentScene != mainMenu && engine->currentScene != letterShow && !al_get_audio_stream_playing(introMusic))
                onOpenMenu(NULL);
            else if (engine->currentScene == mainMenu && playerStatus.tutorialLetter == 1)
                onOpenRestart(NULL);
        }
    }
    else if (event.type == ALLEGRO_EVENT_KEY_UP) {
        if (event.keyboard.keycode == ALLEGRO_KEY_E) {

            // close letter show
            if (engine->currentScene == letterShow){
                changeScene(engine, lastSceneBeforeMenu);
            }

            // take letter
            if (!(playerStatus.carryingLetter) && pressEMessage->visible) {
                // if letter is tutorial
                if (playerStatus.closeLetterId < 2) {
                    playerStatus.tutorialLetter=1;
                    tutorialLetterContent->visible = 1;
                } else {
                    playerStatus.letterId = playerStatus.closeLetterId;
                    playerStatus.carryingLetter=1;
                }
                if (playerStatus.mainMissionId == 4) {
                    playerStatus.inDialog = 1;
                    playerStatus.mainMissionId--;
                }
                if (playerStatus.mainMissionId == 6){
                    lastSceneBeforeMenu = engine->currentScene;
                    changeText(letterShowText, lettersTexts[playerStatus.letterId]);
                    changeScene(engine, letterShow);
                }
                letterObj->visible = 0;
                pressEMessage->visible = 0;
                playerStatus.mainMissionId++;
                changeText(mainMissionText, mainMissions[playerStatus.mainMissionId]);
                playClickSound();
            }
            //open dialog
            else if (playerStatus.carryingLetter && pressEMessage->visible) {
                pressEMessage->visible = 0;
                playerStatus.inDialog = 1;

                playClickSound();
                return;
            }

            //go to letterShow scene and update text boxes
            if (playerStatus.inDialog) {
                playerDialog->visible = 0;
                playerStatus.inDialog = 0;
                playerStatus.dialogId++;
                playerStatus.mainMissionId++;
                //printf("%d\n", playerStatus.mainMissionId);

                if (playerStatus.dialogId < 4 && playerStatus.mainMissionId < 9) {
                    changeText(letterShowText, lettersTexts[playerStatus.letterId]);
                    changeText(playerDialog, dialogsTexts[playerStatus.dialogId]);
                    changeText(mainMissionText, mainMissions[playerStatus.mainMissionId]);


                    if (!(playerStatus.mainMissionId == 5)) {
                        playerStatus.carryingLetter = 0;
                        pauseAudioStream(stepsSound);
                        lastSceneBeforeMenu = engine->currentScene;
                        changeScene(engine, letterShow);
                    }
                }

            }
        }
        else if (event.keyboard.keycode == ALLEGRO_KEY_F){
            if (playerStatus.tutorialLetter) {
                tutorialLetterContent->visible = !tutorialLetterContent->visible;
                playClickSound();
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
            if ((playerStatus.mainMissionId == 4 || playerStatus.mainMissionId == 5) && i % 2 == 0) {
                enemies[i]->visible = 0;
                enemies[i]->position = (Vector2){350, 3900};
                continue;
            }

            enemies[i]->visible = 1;
            enemies[i]->position.x = randInt(1, 3600);
            enemies[i]->position.y = randInt(700, 3800);
        } while(!isOnRoad(enemies[i])/*  ||
                dist(enemies[i]->position.x, enemies[i]->position.y, enemies[i]->width, enemies[i]->height, politician->position.x, politician->position.y, politician->width, politician->height) < 100 */);
    }
}

int getPlayerNearHouse(){
    return (int)((player->position.x+100)/500+1)+((int)((player->position.y-250)/500)*10);
}

void playEndCutscene(){
    ALLEGRO_VIDEO* video = al_open_video("./assets/videos/fim.ogv");
    if (!video){
        printf("\nFailed to load end cutscene video");
        return;
    }
    al_start_video(video, al_get_default_mixer());
    while(al_is_video_playing(video)){
        ALLEGRO_BITMAP* frame = al_get_video_frame(video);
        if (frame){
            al_draw_scaled_bitmap(frame, 0, 0, al_get_bitmap_width(frame), al_get_bitmap_height(frame), 0, 0, engine->displayWidth, engine->displayHeight, 0);
            al_flip_display();
        }
        al_rest(1.0 / 60);
    }
    al_flush_event_queue(engine->ev_queue);
    al_close_video(video);
}

void playClickSound(){
    playAudioSample(clickSound, 0.5, 0, 1, ALLEGRO_PLAYMODE_ONCE);
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

    // sounds
    clickSound = loadAudioSample(engine, "./assets/sounds/sfx.wav");
    menuMusic = loadAudioStream(engine, "./assets/sounds/musica-menu.wav", 2, 2048);
    configureAudioStream(menuMusic, 1, 0, 1, ALLEGRO_PLAYMODE_LOOP);
    stepsSound = loadAudioStream(engine, "./assets/sounds/steps.wav", 2, 2048);
    configureAudioStream(stepsSound, 0.7, 0, 1, ALLEGRO_PLAYMODE_LOOP);
    cityNoise = loadAudioStream(engine, "./assets/sounds/city-noise.wav", 2, 2048);
    configureAudioStream(cityNoise, 0.5, 0, 1, ALLEGRO_PLAYMODE_LOOP);
    introMusic = loadAudioStream(engine, "./assets/sounds/intro.wav", 2, 2048);
    configureAudioStream(introMusic, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE);
    chaseMusic = loadAudioStream(engine, "./assets/sounds/chase.wav", 2, 2048);
    configureAudioStream(chaseMusic, 0.7, 0, 1, ALLEGRO_PLAYMODE_LOOP);
    stopAudioStream(stepsSound);
    stopAudioStream(cityNoise);
    stopAudioStream(introMusic);
    stopAudioStream(chaseMusic);

    // set playerStatus to default
    playerStatus.isHidden = 0;
    playerStatus.carryingLetter = 0;
    playerStatus.firstZoomIn = 0;
    playerStatus.letterId = 0;
    playerStatus.inDialog = 0;
    playerStatus.closeLetterId = 0;
    playerStatus.gameOverCount = 0;
    playerStatus.mainMissionId = 0;
    playerStatus.tutorialLetter = 0;
    playerStatus.isLastSafeZoneQuartel = 0;
    playerStatus.dialogId = 0;
    playerStatus.enemiesFollowing = 0;
    playerStatus.lastScene = INSIDE_BASE;
    playerStatus.lastPosition = (Vector2){450, 300};

    struct SaveFile saveFile = openSaveFile("Save.caes");
    struct SaveData saveData;
    readSaveData(&saveFile, &saveData);

    // update to saved data
    //printf("\n %d %d \n", saveData.playerStatus.dialogId, saveData.playerStatus.mainMissionId);
    memcpy(&playerStatus, &(saveData.playerStatus), sizeof(struct PlayerStatus));

    loadMainMenu();
    allScenes[MAIN_MENU]=mainMenu;
    loadBase();
    allScenes[INSIDE_BASE]=insideBase;
    loadGameMap();
    allScenes[GAME_MAP]=gameMap;
    loadQuartel();
    allScenes[QUARTEL]=quartel;
    loadRoomLeft();
    allScenes[ROOM_L]=roomL;
    loadRoomMiddle();
    allScenes[ROOM_M]=roomM;
    loadRoomRight();
    allScenes[ROOM_R]=roomR;
    loadSinopse();
    allScenes[SINOPSE]=sinopse;
    loadLetterShow();
    allScenes[LETTER_SHOW]=letterShow;

    lastSceneBeforeMenu = allScenes[playerStatus.lastScene];
    memcpy(&player->position, &playerStatus.lastPosition, sizeof(Vector2));
    allScenes[playerStatus.lastScene]->camera.offset = player->position;

    while (engine->isAlive) {
        render(engine);

        // BASICALLY GETTING THE ANGLE OF THE MIDDLE BOTTOM OF THE SCREEN TO THE PLAYERS POSITION AND CALCULATING THE COS
        // THE RESULT COS IS PUTTED AS THE STEPS PANNING
        float x=player->position.x*engine->currentScene->camera.zoom+player->width*engine->currentScene->camera.zoom/2;
        float y=player->position.y*engine->currentScene->camera.zoom+player->height*engine->currentScene->camera.zoom/2;
        globalToLocal(engine->currentScene, &x, &y);
        double co = engine->displayHeight-y;
        double ca = x-engine->displayWidth/2;
        double hyp = hypot(co, ca);
        configureAudioStream(stepsSound, 0.5, ca/hyp, 1, ALLEGRO_PLAYMODE_LOOP);
    }

    saveGame(&saveFile);
    closeSaveFile(&saveFile);

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
