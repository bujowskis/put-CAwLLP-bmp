#ifndef BMPARSER_H
#define BMPARSER_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <float.h>

#define UINT_8 1
#define UINT_16 2
#define UINT_32 4
#define INT_32  4

/* Print printf-style error message */
#define ERROR(...) \
    do { \
        fprintf(stderr, "[ERROR] (%s:%d) (%s) %s", __FILE__, __LINE__, __func__, __VA_ARGS__); \
    } while (0)


/** TYPEDEFS **/

typedef struct Bmp Bmp;
typedef struct BmpHeader BmpHeader;
typedef struct BmpInfoHeader BmpInfoHeader;
typedef struct Pixel Pixel;

typedef uint16_t uint16;    // WORD
typedef uint32_t uint32;    // DWORD
typedef int32_t int32;      // LONG
typedef uint8_t uint8;      // 0-255 for a single color component of pixel

/** STRUCTURES **/

struct Bmp {
    BmpHeader *header;
    BmpInfoHeader *infoHeader;
    /* Matrix of pixel data */
    Pixel **pixelData;
    /* Flag whether type is supported (no compression, bitCount = 24) */
    bool isSupported;
};

struct BmpHeader {
    uint16 bfType;
    uint32 bfSize;
    uint16 bfReserved1;
    uint16 bfReserved2;
    uint32 bfOffBits;
};

struct BmpInfoHeader {
    uint32 biSize;
    int32 biWidth;
    int32 biHeight;
    uint16 biPlanes;
    uint16 biBitCount;
    uint32 biCompression;
    uint32 biSizeImage;
    int32 biXPelsPerMeter;
    int32 biYPelsPerMeter;
    uint32 biClrUsed;
    uint32 biClrImportant;
};

struct Pixel {
    uint8 red;
    uint8 green;
    uint8 blue;
};

/** FUNCTIONS **/

    // Utility Functions for simplified operating on the binary file
/* Reads and returns a single uint8 from the file */
uint8 fileReadUi8(FILE *file);

/* Reads and returns a single uint16 from the file */
uint16 fileReadUi16(FILE *file);

/* Reads and returns a single uint32 from the file */
uint32 fileReadUi32(FILE *file);

/* Reads and returns a single int32 from the file */
int32 fileReadInt32(FILE *file);

/* Writes a single uint8 to the file in binary mode */
void fileWriteUi8(FILE *file, uint8 input);

/* Writes a single uint16 to the file in binary mode */
void fileWriteUi16(FILE *file, uint16 input);

/* Writes a single uint32 to the file in binary mode */
void fileWriteUi32(FILE *file, uint32 input);

/* Writes a single int32 to the file in binary mode */
void fileWriteInt32(FILE *file, int32 input);

    // BMP-reading-related functions
/* Creates a new instance of Bmp structure */
Bmp *bmpCreate();

/* Creates greyscale bmp out of the input bmp */
void bmpCreateGreyscale(const Bmp *bmpIn, const char *path);

/*
 * Reads given bmp file and stores it in struct Bmp
 * Returns NULL on failure
 */
Bmp *bmpReadFile(char *path);

/* Encodes given text in given bmp, gives the option to decrypt the message */
void bmpEncode(Bmp *bmp, const char *text);

/* Frees all the memory taken up by the struct Bmp */
void bmpDestroy(Bmp *bmp);

#endif //BMPARSER_H
