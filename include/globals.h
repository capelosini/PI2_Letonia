#ifndef GLOBALS_H
#define GLOBALS_H

struct TileFromMapFile {
    int idX;
    int idY;
    int x;
    int y;
};

// void gameSceneScript(Scene* self);
// void onEvent(ALLEGRO_EVENT event, Scene* scene, CAEngine* engine);
// void onStartStageOne(Scene* scene);
void textBox(int x, int y, int width, int padding, char *text);
// void leafEffect(Scene* self);
void loadMap(const char *filename, Scene* scene);

#endif