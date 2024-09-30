#include "CAE/include/CAE.h"

void onEvent(ALLEGRO_EVENT event, Scene* scene, CAEngine* engine){
    if (event.type == ALLEGRO_EVENT_KEY_DOWN){
        if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE){
            engine->isAlive = 0;
        }
    }
}

int main (){
    GameConfig engineConfig;
    engineConfig.fps=60;
    engineConfig.posX=0;
    engineConfig.posY=0;
    engineConfig.sizeX=1280;
    engineConfig.sizeY=720;
    engineConfig.title="Letonia Project";
    engineConfig.fullscreen=1;

    CAEngine* engine = initEngine(engineConfig);
    setEventFunction(engine, onEvent);

    // SCENES
    Scene* mainMenu = createScene(engine, NULL);
    mainMenu->backgroundColor = al_map_rgb(20,20,40);
    changeScene(engine, mainMenu);

    while (engine->isAlive){
        render(engine);
    }

    freeEngine(engine);

    return 0;
}
