#include "../../include/globals.h"

void loadLetterShow(){
    letterShow = createScene(engine, NULL);
    letterShow->backgroundColor = al_map_rgb(0xA0, 0x71, 0x46);

    GameObject* back = createGameObject(SPRITE, engine->displayWidth/4, 20, engine->displayWidth/2, engine->displayWidth/2*1.399305556, letterShow);
    setGameObjectBitmap(back, loadBitmap(engine, "./assets/images/letter-background.png"));

    letterShowText = createText("", back->position.x+150, back->position.y+180, engine->displayWidth/2-280, al_map_rgb(10,10,10), al_map_rgba(0,0,0,0), NULL, loadTTF(engine, "./assets/fonts/roboto.ttf", 40), 40, 20, letterShow);

}