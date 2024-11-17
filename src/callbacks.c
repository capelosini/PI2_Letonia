#include "../include/globals.h"

void onOpenMenu(Scene* scene)
{   
    char out[] = "Você foi pego 0000 vezes";
    sprintf(out, "Você foi pego %d vezes", playerStatus.gameOverCount);
    changeText(gameOverCountText, out);
    lastSceneBeforeMenu = engine->currentScene;
    changeScene(engine, mainMenu);
    playClickSound();
    playAudioStream(menuMusic);
}

void onOpenRestart(Scene* scene)
{
    changeScene(engine, lastSceneBeforeMenu);
    playClickSound();
    pauseAudioStream(menuMusic);
}

void onGameExit(Scene* scene){
    engine->isAlive=0;
}

//stage change
void onOpenBase(Scene* scene) {
    playerStatus.isLastSafeZoneQuartel=0;
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
    if (!gameOverCountText->visible)
        gameOverCountText->visible = 1;
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
    gameMap->camera.offset = (Vector2){ 9999, 9999 };
    restartEnemiesPos();
    changeScene(engine, gameMap);
    if (playerStatus.mainMissionId == 6){
        playEndCutscene();
        playerStatus.mainMissionId++;
        changeText(mainMissionText, mainMissions[playerStatus.mainMissionId]);
        stopAudioStream(menuMusic);
        onOpenMenu(NULL);
    }
}

void onOpenQuartel(Scene* scene)
{
    playerStatus.isLastSafeZoneQuartel=1;
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
            if (playerStatus.isLastSafeZoneQuartel){
                onOpenQuartel(NULL);
            } else {
                onOpenBase(NULL);
            }
        }
    }
}
