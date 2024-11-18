#include "../../include/globals.h"

void loadRoomLeft(){
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

    GameObject* general1 = createGameObject(ANIMATED_SPRITE, 175, 0,50, 65, roomL);
    ALLEGRO_BITMAP* general1Bm = loadBitmap(engine, "./assets/images/general-sprite.png");
    setGameObjectAnimation(general1, general1Bm, 18, 25, 4, 16);


    returnQuartelRoomL = createGameObject(SOLID, 30, 478, 208, 28, roomL);
    returnQuartelRoomL->color = al_map_rgba(0, 0, 0, 0);
    returnQuartelRoomL->collisionEnabled = 1;
    returnQuartelRoomL->collisionType = COLLISION_RECT;

    addButtonToScene(roomL, letterStatus);
    addTextToScene(roomL, pressEMessage);
    addTextToScene(roomL, playerDialog);
    addTextToScene(roomL, tutorialLetterContent);
}
