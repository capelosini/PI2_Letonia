#ifndef SAVING_H
#define SAVING_H

#include "../include/globals.h"
#include <string.h>
#include <stdio.h>
#define VERIFICATION_CODE1 0xCAE123
#define VERIFICATION_CODE2 0xCAE456

struct SaveFile{
    char filename[50];
    FILE* fileconn;
} __attribute__((packed));

struct SaveData{
    unsigned long verification1;
    struct PlayerStatus playerStatus;
    unsigned long verification2;
} __attribute__((packed));

void readSaveData(struct SaveFile* save, struct SaveData* out);
unsigned char isSaveFileValid(struct SaveFile* save);
void closeSaveFile(struct SaveFile* save);
void saveGame(struct SaveFile* save);
struct SaveFile openSaveFile(const char* filename);

#endif
