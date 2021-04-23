#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "bmparser.h"

// Simplified reading

uint8 fileReadUi8(FILE *file) {
   if (!file) {
       ERROR("NULL file ptr\n");
       exit(1);
   }
   uint8 ret;
   fread(&ret, UINT_8, 1, file);
   return ret;
}

uint16 fileReadUi16(FILE *file) {
    if (!file) {
        ERROR("NULL file ptr\n");
        exit(1);
    }
    uint16 ret;
    fread(&ret, UINT_16, 1, file);
    return ret;
}

uint32 fileReadUi32(FILE *file) {
    if (!file) {
        ERROR("NULL file ptr\n");
        exit(1);
    }
    uint32 ret;
    fread(&ret, UINT_32, 1, file);
    return ret;
}

int32 fileReadInt32(FILE *file) {
    if (!file) {
        ERROR("NULL file ptr\n");
        exit(1);
    }
    int32 ret;
    fread(&ret, INT_32, 1, file);
    return ret;
}

// BMP-related

Bmp *bmpCreate() {
    // A new Bmp structure is created empty
    return NULL;
}

Bmp *bmpReadFile(char *path) {
    if (!path) {
        ERROR("no such file\n");
        return NULL;
    }
    FILE *file;
    if ((file = fopen(path, "rb")) == NULL) {
        ERROR("did not open file\n");
        return NULL;
    }
    uint8 test;
    test = fileReadUi8(file);
    printf("Read %d\n", (int) test);
    if (test == 'B')
        printf("It is B\n");


    fclose(file);
}

void bmpDestroy(Bmp *bmp) {
    if (!bmp)
        return;
    //TODO
}
