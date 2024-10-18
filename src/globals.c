#include "../CAE/include/CAE.h"
#include "../include/globals.h"
#include <stdlib.h>
#include <time.h>

//letter rendering and text formatting
void textBox(int x, int y, int width, int padding, char* text) {
    ALLEGRO_FONT* font = al_load_ttf_font("./assets/fonts/kalam.ttf", 10, 0);
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
    al_destroy_font(font);
}
