#ifndef BMPARSER_H
#define BMPARSER_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

/* Print printf-style error message */
#define ERROR(...) \
    do { \
        fprintf(stderr, "[ERROR] (%s:%d) (%s) %s", __FILE__, __LINE__, __func__, __VA_ARGS__); \
    } while (0)


/** TYPEDEFS **/

typedef struct BmpHeader BmpHeader;
typedef struct BmpInfoHeader BmpInfoHeader;

typedef uint16_t uint16;    // WORD
typedef uint32_t uint32;    // DWORD
typedef int32_t int32;      // LONG

/** STRUCTURES **/

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

/** FUNCTIONS **/

#endif //BMPARSER_H
