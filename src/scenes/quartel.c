#include "../../include/scenes.h"

void loadQuartel(){
    Scene* quartel = createScene(engine, gameSceneScript);
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
}
