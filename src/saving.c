#include "../include/saving.h"

void readSaveData(struct SaveFile* save, struct SaveData* out){
    if (save->fileconn != NULL){
        rewind(save->fileconn);
        fread(out, sizeof(struct SaveData), 1, save->fileconn);
    }
}

unsigned char isSaveFileValid(struct SaveFile* save){
    if (save->fileconn == NULL)
        return 0;
    struct SaveData data;
    readSaveData(save, &data);
    if (data.verification1 != VERIFICATION_CODE1 || data.verification2 != VERIFICATION_CODE2){
        return 0;
    }
    return 1;
}

void closeSaveFile(struct SaveFile* save){
    if (save->fileconn != NULL){
        fclose(save->fileconn);
        save->fileconn = NULL;
    }
}

void saveGame(struct SaveFile* save){
    struct SaveData data;
    data.verification1 = VERIFICATION_CODE1;
    data.verification2 = VERIFICATION_CODE2;
    memcpy(&data.playerStatus, &playerStatus, sizeof(struct PlayerStatus));
    closeSaveFile(save);
    save->fileconn = fopen(save->filename, "wb");
    rewind(save->fileconn);
    fwrite(&data, sizeof(struct SaveData), 1, save->fileconn);
    closeSaveFile(save);
    save->fileconn = fopen(save->filename, "rb");
}

struct SaveFile openSaveFile(const char* filename){
    struct SaveFile save;
    strcpy(save.filename, filename);
    if (!al_filename_exists(filename))
        fclose(fopen(filename, "wb"));
    save.fileconn = fopen(filename, "rb");
    if (!isSaveFileValid(&save)){
        if(save.fileconn != NULL){
            printf("\nRecovering save file!");
            saveGame(&save);
        } else {
            printf("\nCan't access this save file!");
        }
    }
    return save;
}