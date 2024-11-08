#include "../../include/scenes.h"

void loadSinopse(){
    Scene* sinopse = createScene(engine, NULL);
    sinopse->backgroundColor = al_map_rgb(20, 20, 20);
    lettersFont->size = 200;
    sinopseTchau = createText("Final do jogo",
        0, 0, engine->displayWidth, al_map_rgb(0, 0, 0), al_map_rgb(155, 122, 73), NULL, titleFont, 520, 550, sinopse);
}
