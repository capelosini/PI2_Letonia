#include "../include/globals.h"
#include <math.h>

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
