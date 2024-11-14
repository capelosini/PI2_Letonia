#include "../../include/globals.h"

void loadRoomRight(){
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

    GameObject* general2 = createGameObject(ANIMATED_SPRITE, 225, 0, 50, 65, roomR);
    ALLEGRO_BITMAP* general2Bm = loadBitmap(engine, "./assets/images/general2-sprite.png");
    setGameObjectAnimation(general2, general2Bm, 18, 25, 4, 16);

    returnQuartelRoomR = createGameObject(SOLID, 330, 478, 208, 28, roomR);
    returnQuartelRoomR->color = al_map_rgba(0, 0, 0, 0);
    returnQuartelRoomR->collisionEnabled = 1;
    returnQuartelRoomR->collisionType = COLLISION_RECT;

    addButtonToScene(roomR, letterStatus);
}
