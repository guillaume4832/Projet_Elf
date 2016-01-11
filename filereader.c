#include "filereader.h"

char* readFileBytes(const char *name)
{
    FILE *fl = fopen(name, "r");
    fseek(fl, 0, SEEK_END);
    long len = ftell(fl);
    char *ret = malloc(len);
    fseek(fl, 0, SEEK_SET);
    fread(ret, 1, len, fl);
    fclose(fl);
    return ret;
}

int writeFileBytes(char* fileBytes, char* fileName, int size){
    void * file_to_write = fopen(fileName, "ab+");
    if(file_to_write != NULL){
        int i = 0;
        for(i;i<size;i++){
            fwrite(&fileBytes[i], 1, sizeof(fileBytes[i]), file_to_write);
            i++;
        }
        fclose(file_to_write);
        return 0;
    }
    else{
        fclose(file_to_write);
        return 1;
    }
}
