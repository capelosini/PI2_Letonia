#include "../CAE/include/CAE.h"
#include "../include/globals.h"

extern CAEngine* engine;
extern Scene* mainMenu;
extern Scene* stageOne;
extern GameObject* player;
int walkIndex = 0;

//kill engine
void onEvent(ALLEGRO_EVENT event, Scene* scene, CAEngine* engine){
    if (event.type == ALLEGRO_EVENT_KEY_DOWN){
        if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE){
            engine->isAlive = 0;
        }
    }
}

//stage change
void onStartStageOne(Scene* scene) {
    changeScene(engine, stageOne);
}

//character movement and letter handling
void gameSceneScript(Scene* self) {
    ALLEGRO_KEYBOARD_STATE keyState;
    al_get_keyboard_state(&keyState);
    
    char up = al_key_down(&keyState, ALLEGRO_KEY_W);
    char down = al_key_down(&keyState, ALLEGRO_KEY_S);
    char left = al_key_down(&keyState, ALLEGRO_KEY_A);
    char right = al_key_down(&keyState, ALLEGRO_KEY_D);
    char e = al_key_down(&keyState, ALLEGRO_KEY_E);
    
    if (up) {
        player->physics.directions.y = -1;
        player->physics.acc.y = 1;
        walkIndex = 2;
    } else if (down) {
        player->physics.directions.y = 1;
        player->physics.acc.y = 1;
        walkIndex = 1;
    } else {
        player->physics.acc.y = 0;
    }
    
    if (left) {
        player->physics.directions.x = -1;
        player->physics.acc.x = 1;
        player->animation.direction.x = -1;
        walkIndex = 0;
    } else if (right) {
        player->physics.directions.x = 1;
        player->physics.acc.x = 1;
        player->animation.direction.x = 1;
        walkIndex = 0;
    } else {
        player->physics.acc.x = 0;
    }

    player->animation.index.y = 3 + walkIndex;
    if (!(up || down || left || right))
        player->animation.index.y = walkIndex;

    if(e){
        textBox(300, 200, 250, 10, "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Ut ut tincidunt elit. Nunc a magna at nulla tempor iaculis. Curabitur at enim sollicitudin, varius nisi vel, viverra odio. Ut porta metus sed metus gravida elementum. Pellentesque ut mi id quam euismod convallis. Duis vulputate tempus sagittis. Quisque aliquam justo justo, eget lobortis neque tempor non.Integer porta volutpat turpis, nec venenatis ante volutpat sit amet. Proin condimentum vitae augue id tincidunt. Donec tristique lectus non dui pellentesque tincidunt. In sit amet leo suscipit, feugiat leo id, condimentum tellus. Proin vel tempor metus. Mauris in auctor velit. Donec justo justo, iaculis eget pellentesque eget, interdum a nibh. Aenean tincidunt tempor sem. Integer eget elementum metus. Suspendisse non fringilla nunc, sit amet suscipit diam.Suspendisse a justo lorem. Phasellus ac nulla sed arcu fermentum sollicitudin. Suspendisse potenti. Aenean a augue venenatis, rhoncus sapien ut, laoreet felis. Vivamus mi neque, iaculis ac ligula eget, fermentum maximus elit. Praesent at elementum lorem, et tincidunt leo. Nunc ut lacinia ligula. Aliquam eu est finibus, iaculis ipsum vitae, dignissim risus. Proin pulvinar urna sit amet metus pharetra, eget pulvinar nisl sodales. Proin aliquam dolor at urna dignissim maximus. Nulla imperdiet varius pulvinar");
    }
}


//letter rendering and text formatting
void textBox(int x, int y, int width, int padding, char* text) {
    ALLEGRO_FONT* font = al_load_ttf_font("../assets/fonts/kalam.ttf", 10, 0);
    char *copyText = malloc(2048 * sizeof(char));
    strcpy(copyText, text);
    char *newText = malloc(2048 * sizeof(char));
    newText[0] = '\0';
    char *line = malloc(256 * sizeof(char));
    line[0] = '\0';
    char *word = strtok(copyText, " ");
    int lineCount = 1;
    int lineHeight = al_get_font_line_height(font);
    while (word) {
        if (al_get_text_width(font, line) + al_get_text_width(font, word) >= width - padding) {
            strcat(newText, line);
            strcat(newText, "\n");
            line[0] = '\0';
            lineCount++;
        }
        strcat(line, word);
        strcat(line, " ");
        word = strtok(NULL, " ");
    }
    if (line[0] != '\0') {
        strcat(newText, line);
    }

    al_draw_filled_rectangle(x - width/2, y, x + width/2, y + lineCount*lineHeight + padding*2, al_map_rgba_f(0, 0, 0, 0.6));

    char *buffer  = strtok(newText, "\n");
    lineCount = 0;
    while (buffer) {
        al_draw_text(font, al_map_rgb(255, 255, 255), x - width/2 + padding/2, y + lineCount*lineHeight + padding, 0, buffer);
        buffer = strtok(NULL, "\n");
        lineCount++;
    }

    free(line);
    free(copyText);
    free(newText);
    free(font);
}