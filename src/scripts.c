#include "../include/globals.h"

// t = current time;  T = total time;  vmax = max speed;  dt = delta time
float t = 0;
float T = 7.1;
float vmax = 18;
float dt = 0.016;
int lastHouse = 0;
time_t lastTime;

float easeInOut(float t, float T, float vmax) {
    if (t >= T) {
        playerStatus.firstZoomIn = 1;
        return 0;
    }
    float x = t / T;
    return vmax * 22 * x * x * (1 - x) * (1 - x);
}

//character movement and letter handling
void gameSceneScript(Scene* self) {
    ALLEGRO_KEYBOARD_STATE keyState;
    al_get_keyboard_state(&keyState);

    Vector2 mov = getMovementVector2(&keyState, ALLEGRO_KEY_A, ALLEGRO_KEY_D, ALLEGRO_KEY_W, ALLEGRO_KEY_S);

    player->physics.acc = (Vector2){ abs(mov.x), abs(mov.y) };
    
    if (mov.x != 0 || mov.y != 0) {
        playAudioStream(stepsSound);
    } else{
        pauseAudioStream(stepsSound);
    }

    if (engine->currentScene == gameMap && !al_get_audio_stream_playing(cityNoise)){
        playAudioStream(cityNoise);
    } else if (engine->currentScene != gameMap && al_get_audio_stream_playing(cityNoise)){
        pauseAudioStream(cityNoise);
    }

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

    // jump third mission
    /* if (playerStatus.mainMissionId == 7) {
        pressEMessage->visible = 1;
    } */

    // base tutorial intro letter check
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
    // base second mission letter
    else if (!playerStatus.carryingLetter && engine->currentScene == insideBase && playerStatus.mainMissionId == 3) {
        playerStatus.closeLetterId=1;
        letterObj->visible=1;
        if (dist(player->position.x, player->position.y, player->width, player->height, letterObj->position.x, letterObj->position.y, letterObj->width, letterObj->height)
            <= 60) {
            pressEMessage->visible = 1;
        }
        else {
            pressEMessage->visible = 0;
            tutorialLetterContent->visible = 0;
            politician->visible = 1;
        }
    }
    //interac with politician
    else if (!playerStatus.carryingLetter && engine->currentScene == gameMap && playerStatus.mainMissionId == 4) {
        playerStatus.closeLetterId = 4;
        if (dist(player->position.x, player->position.y, player->width, player->height, politician->position.x, politician->position.y, politician->width, politician->height) <= 80) {
            pressEMessage->visible = 1;
            playerDialog->visible = 1;
        } else {
            pressEMessage->visible = 0;
            playerDialog->visible = 0;
        }
    }
    // base third mission letter
    else if (!playerStatus.carryingLetter && engine->currentScene == insideBase && playerStatus.mainMissionId == 6) {
        playerStatus.closeLetterId=2;
        letterObj->visible=1;
        if (dist(player->position.x, player->position.y, player->width, player->height, letterObj->position.x, letterObj->position.y, letterObj->width, letterObj->height)
            <= 60) {
            pressEMessage->visible = 1;
        }
        else {
            pressEMessage->visible = 0;
            tutorialLetterContent->visible = 0;
        }
    }


    //politician movement
    float politicianToPlayerDist = dist(player->position.x, player->position.y, player->width, player->height, politician->position.x, politician->position.y, politician->width, politician->height);
    if(playerStatus.carryingLetter && playerStatus.mainMissionId == 5 && politicianToPlayerDist < 100 && politicianToPlayerDist > politician->width+5) {
        double dx = player->position.x - politician->position.x;
        double dy = player->position.y - politician->position.y;
        double hip = hypot(dx, dy);

        politician->physics.acc.x = fabs(dx) / hip;
        politician->physics.acc.y = fabs(dy) / hip;

        politician->physics.directions.x = (dx > 0) ? 1 : -1;
        politician->physics.directions.y = (dy > 0) ? 1 : -1;
        politician->animation.direction.x = politician->physics.directions.x;

        politician->animation.index.y = 3;
        if (politician->physics.acc.x < 0.2) {
            politician->animation.index.y = (dy > 0) ? 4 : 5;
        }
    } else {
        politician->physics.acc.x = 0;
        politician->physics.acc.y = 0;
        politician->animation.index.y = 0;
    }

    // enemies movement
    playerStatus.enemiesFollowing = 0;
    for (int i = 0; i < enemiesCount; i++) {
        if (dist(enemies[i]->position.x, enemies[i]->position.y, enemies[i]->width, enemies[i]->height, player->position.x, player->position.y, player->width, player->height) <= 200) {
            playerStatus.enemiesFollowing = 1;
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
                    enemies[i]->animation.index.y = 4;
                }
            }
            else {
                enemies[i]->physics.directions.y = -1;
                if (enemies[i]->physics.acc.x < 0.20) {
                    enemies[i]->animation.index.y = 5;
                }
            }
            if (enemies[i]->physics.acc.x >= 0.20)
                enemies[i]->animation.index.y = 3;

            if (playerStatus.isHidden)
            {
                playerStatus.enemiesFollowing = 0;
                enemies[i]->physics.directions.x *= -1;
                enemies[i]->physics.directions.y *= -1;
                enemies[i]->animation.direction.x *= -1;
            }

        }
        else {
            enemies[i]->physics.acc.x = 0;
            enemies[i]->physics.acc.y = 0;
            if (enemies[i]->animation.index.y > 2)
                enemies[i]->animation.index.y-=3;
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


    // night in mission 2
    if (playerStatus.mainMissionId == 3) {
        timeGameMap->color = al_map_rgba(0, 0, 10, 100);
    } else if (playerStatus.mainMissionId == 5 && engine->currentScene == quartel) {
        timeGameMap->color = al_map_rgba(0, 0, 20, 80);
    } else if (playerStatus.mainMissionId == 4 || playerStatus.mainMissionId == 5) {
        timeGameMap->color = al_map_rgba(0, 0, 10, 180);
    } else {
        timeGameMap->color = al_map_rgba(0, 0, 0, 0);
    }

    


    // letter status change
    if (letterStatus->bitmap == letterStatusFalseBM && playerStatus.carryingLetter == 1){
        letterStatus->bitmap = letterStatusTrueBM;
    } else if(letterStatus->bitmap == letterStatusTrueBM && playerStatus.carryingLetter == 0){
        letterStatus->bitmap = letterStatusFalseBM;
    }

    // last letter mission get letter
    if (playerStatus.mainMissionId == 8 && engine->currentScene == roomR && playerStatus.carryingLetter == 0 && !pressEMessage->visible){
        pressEMessage->visible=1;
        playerStatus.closeLetterId=5;
    }

    //letters give action
    if (playerStatus.carryingLetter &&
    ((engine->currentScene == roomL && playerStatus.mainMissionId == 2) || (engine->currentScene == roomM && playerStatus.mainMissionId == 5) || engine->currentScene == roomR && playerStatus.mainMissionId == 9)) {
        if (!playerStatus.inDialog) {
            pressEMessage->visible=1;
        } else {
            playerDialog->visible = 1;
        }
    }

    else if (playerStatus.mainMissionId == 2 || playerStatus.mainMissionId == 5 || playerStatus.mainMissionId == 9){
        pressEMessage->visible=0;
    }

    memcpy(&playerStatus.lastPosition, &player->position, sizeof(Vector2));

    // UNDER THIS ONLY THINGS THAT WILL WORK ONLY IN GAMEMAP SCENE
    if (engine->currentScene != gameMap){
        return;
    }

    // first zoom in handle
    if (!playerStatus.firstZoomIn){
        if (al_get_audio_stream_playing(stepsSound)){
            pauseAudioStream(stepsSound);
        } 
        if (al_get_audio_stream_playing(cityNoise)){
            pauseAudioStream(cityNoise);
        }
        if (!al_get_audio_stream_playing(introMusic)){
            playAudioStream(introMusic);
        }
        player->physics.acc = (Vector2){0,0};

        gameMap->camera.followMaxSpeed = easeInOut(t, T, vmax);
        t += dt;
        if (t >= T) {
            gameMap->camera.followMaxSpeed = 4;
        }

    }

    // on player been following
    if (playerStatus.firstZoomIn && playerStatus.enemiesFollowing){
        //timeGameMap->color = al_map_rgba(30, 10, 0, 1);
        if (gameMap->camera.zoom < 1.7){
            gameMap->camera.zoom+=0.01;
            // smooth the camera movement based on the changed zoom factor
            gameMap->camera.offset.x+=gameMap->camera.offset.x*0.01;
            gameMap->camera.offset.y+=gameMap->camera.offset.y*0.01;
        }
        playAudioStream(chaseMusic);
    } else if (playerStatus.firstZoomIn){
        //timeGameMap->color = al_map_rgba(30, 20, 0, 1);
        if (gameMap->camera.zoom > 1.5){
            gameMap->camera.zoom-=0.005;
            gameMap->camera.offset.x-=gameMap->camera.offset.x*0.005;
            gameMap->camera.offset.y-=gameMap->camera.offset.y*0.005;
        } else{
            gameMap->camera.zoom=1.5;
        }
        stopAudioStream(chaseMusic);
    }

    int currentHouse=getPlayerNearHouse();

    if (currentHouse > 0 && currentHouse < 70 && currentHouse != lastHouse){
        lastTime=time(NULL);
        lastHouse=currentHouse;
        closeHouseNumber->visible=1;
        // if (currentHouse==0){
        //     closeHouseNumber->visible=0;
        // } else{
        //     closeHouseNumber->visible=1;
        // }

        char tempstr[9];
        sprintf(tempstr, "Casa %d", currentHouse);
        changeText(closeHouseNumber, tempstr);
    } else{
        if (time(NULL)-lastTime>2){
            closeHouseNumber->visible=0;
        }
    }

    // house get letter mission
    if (playerStatus.mainMissionId == 1 && currentHouse == 43 && !pressEMessage->visible && !playerStatus.carryingLetter){
        pressEMessage->visible=1;
        playerStatus.closeLetterId=3;
    }

    if (playerStatus.mainMissionId == 7) {
        for (int i = 0; i < 5; i++) {
            if (dist(player->position.x, player->position.y, player->width, player->height, goldObjects[i]->position.x, goldObjects[i]->position.y, goldObjects[i]->width, goldObjects[i]->height) < 60) {
                pressEMessage->visible = 1;
                break;
            }
            else {
                pressEMessage->visible = 0;
            }
        }
    }

    // invensible, just for development
    // playerStatus.isHidden=1;
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
void letterShowScript(Scene* self){
}