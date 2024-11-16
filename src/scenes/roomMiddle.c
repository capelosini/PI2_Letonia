#include "../../include/globals.h"

void loadRoomMiddle(){
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

    GameObject* general3 = createGameObject(ANIMATED_SPRITE, 225, 0, 50, 65, roomM);
    ALLEGRO_BITMAP* general3Bm = loadBitmap(engine, "./assets/images/general3-sprite.png");
    setGameObjectAnimation(general3, general3Bm, 18, 25, 4, 16);

    returnQuartelRoomM = createGameObject(SOLID, 140, 478, 208, 28, roomM);
    returnQuartelRoomM->color = al_map_rgba(0, 0, 0, 0);
    returnQuartelRoomM->collisionEnabled = 1;
    returnQuartelRoomM->collisionType = COLLISION_RECT;

    addButtonToScene(roomM, letterStatus);
    addTextToScene(roomM, pressEMessage);
}
