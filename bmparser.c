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
   if (fread(&ret, UINT_8, 1, file) != 1) {
       ERROR("couldn't read uint8\n");
       exit(2);
   }
   return ret;
}

uint16 fileReadUi16(FILE *file) {
    if (!file) {
        ERROR("NULL file ptr\n");
        exit(1);
    }
    uint16 ret;
    if (fread(&ret, UINT_16, 1, file) != 1) {
        ERROR("couldn't read uint16\n");
        exit(2);
    }
    return ret;
}

uint32 fileReadUi32(FILE *file) {
    if (!file) {
        ERROR("NULL file ptr\n");
        exit(1);
    }
    uint32 ret;
    if (fread(&ret, UINT_32, 1, file) != 1) {
        ERROR("couldn't read uint32\n");
        exit(2);
    }
    return ret;
}

int32 fileReadInt32(FILE *file) {
    if (!file) {
        ERROR("NULL file ptr\n");
        exit(1);
    }
    int32 ret;
    if (fread(&ret, INT_32, 1, file) != 1) {
        ERROR("couldn't read int32\n");
        exit(2);
    }
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

    // Validate type
    uint8 byte1;
    byte1 = fileReadUi8(file);
    if (byte1 != 'B') {
        ERROR("Invalid file type\n");
        return NULL;
    }
    byte1 = fileReadUi8(file);
    if (byte1 != 'M') {
        ERROR("Invalid file type\n");
        return NULL;
    }

    // Type is valid
    fseek(file, 0, SEEK_SET);
    Bmp *bmp = bmpCreate();
    if ((bmp = malloc(sizeof *bmp)) == NULL) {
        ERROR("malloc\n");
        return NULL;
    }
    bmp->isSupported = true; // Initially set to true

    // Read the header
    if ((bmp->header = malloc(sizeof *bmp->header)) == NULL) {
        ERROR("malloc\n");
        bmpDestroy(bmp);
        return NULL;
    }
    bmp->header->bfType = fileReadUi16(file);
    bmp->header->bfSize = fileReadUi32(file);
    bmp->header->bfReserved1 = fileReadUi16(file);
    bmp->header->bfReserved2 = fileReadUi16(file);
    bmp->header->bfOffBits = fileReadUi32(file);
    // Read the infoHeader
    if ((bmp->infoHeader = malloc(sizeof *bmp->infoHeader)) == NULL) {
        ERROR("malloc\n");
        bmpDestroy(bmp);
        return NULL;
    }
    bmp->infoHeader->biSize = fileReadUi32(file);
    bmp->infoHeader->biWidth = fileReadInt32(file);
    bmp->infoHeader->biHeight = fileReadInt32(file);
    bmp->infoHeader->biPlanes = fileReadUi16(file);
    bmp->infoHeader->biBitCount = fileReadUi16(file);
    bmp->infoHeader->biCompression = fileReadUi32(file);
    bmp->infoHeader->biSizeImage = fileReadUi32(file);
    bmp->infoHeader->biXPelsPerMeter = fileReadInt32(file);
    bmp->infoHeader->biYPelsPerMeter = fileReadInt32(file);
    bmp->infoHeader->biClrUsed = fileReadUi32(file);
    bmp->infoHeader->biClrImportant = fileReadUi32(file);
    if (bmp->infoHeader->biCompression != 0 || bmp->infoHeader->biBitCount != 24)
        bmp->isSupported = false;

    // Print the content of headers
    fprintf(stdout, "BITMAPFILEHEADER:\n");
    fprintf(stdout, " bfType:\t\t%d %d (BM)\n", 'B', 'M'); // I could split uint16 into two and pass it, but at this point I'm 100% sure that's the case
    fprintf(stdout, " bfSize:\t\t%u\n", bmp->header->bfSize);
    fprintf(stdout, " bfReserved1:\t\t%u\n", bmp->header->bfReserved1);
    fprintf(stdout, " bfReserved2:\t\t%u\n", bmp->header->bfReserved2);
    fprintf(stdout, " bfOffBits:\t\t%u\n", bmp->header->bfOffBits);
    fprintf(stdout, "BITMAPINFOHEADER:\n");
    fprintf(stdout, " biSize:\t\t%u\n", bmp->infoHeader->biSize);
    fprintf(stdout, " biWidth:\t\t%d\n", bmp->infoHeader->biWidth);
    fprintf(stdout, " biHeight:\t\t%d\n", bmp->infoHeader->biHeight);
    fprintf(stdout, " biPlanes:\t\t%u\n", bmp->infoHeader->biPlanes);
    fprintf(stdout, " biBitCount:\t\t%u\n", bmp->infoHeader->biBitCount);
    fprintf(stdout, " biCompression:\t\t%u\n", bmp->infoHeader->biCompression);
    fprintf(stdout, " biSizeImage:\t\t%u\n", bmp->infoHeader->biSizeImage);
    fprintf(stdout, " biXPelsPerMeter:\t%d\n", bmp->infoHeader->biXPelsPerMeter);
    fprintf(stdout, " biYPelsPerMeter:\t%d\n", bmp->infoHeader->biYPelsPerMeter);
    fprintf(stdout, " biClrUsed:\t\t%u\n", bmp->infoHeader->biClrUsed);
    fprintf(stdout, " biClrImportant:\t%u\n", bmp->infoHeader->biClrImportant);

    // TODO(evaluate supported)

    fclose(file);
}

void bmpCreateGreyscale(const Bmp *bmpIn) {
    // TODO
}

void bmpDestroy(Bmp *bmp) {
    if (!bmp)
        return;
    if (!bmp->pixelData) {
        // TODO
    }
    if (!bmp->infoHeader)
        free(bmp->infoHeader);
    if (!bmp->header)
        free(bmp->header);
    free(bmp);
}
