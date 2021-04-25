#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <float.h>
#include <string.h>
#include "bmparser.h"

// Simplified reading/writing

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

void fileWriteUi8(FILE *file, uint8 input) {
    if (!file) {
        ERROR("no file specified\n");
        exit(1);
    }
    if (fwrite(&input, sizeof input, 1, file) != 1) {
        ERROR("did not write uint8 to the file\n");
        exit(2);
    }
}

void fileWriteUi16(FILE *file, uint16 input) {
    if (!file) {
        ERROR("no file specified\n");
        exit(1);
    }
    if (fwrite(&input, sizeof input, 1, file) != 1) {
        ERROR("did not write uint16 to the file\n");
        exit(2);
    }
}

void fileWriteUi32(FILE *file, uint32 input) {
    if (!file) {
        ERROR("no file specified\n");
        exit(1);
    }
    if (fwrite(&input, sizeof input, 1, file) != 1) {
        ERROR("did not write uint32 to the file\n");
        exit(2);
    }
}

void fileWriteInt32(FILE *file, int32 input) {
    if (!file) {
        ERROR("no file specified\n");
        exit(1);
    }
    if (fwrite(&input, sizeof input, 1, file) != 1) {
        ERROR("did not write int32 to the file\n");
        exit(2);
    }
}

// BMP-related

Bmp *bmpCreate() {
    // A new Bmp structure is created empty
    return NULL;
}

Bmp *bmpReadFile(char *path) {
    if (!path) {
        ERROR("no path given\n");
        return NULL;
    }
    FILE *file;
    if ((file = fopen(path, "rb")) == NULL) {
        fprintf(stdout, "Did not open file %s\n", path);
        return NULL;
    }

    // Validate type
    if (fileReadUi8(file) != 'B') {
        fprintf(stdout, "File is not a BMP\n");
        fclose(file);
        return NULL;
    }
    if (fileReadUi8(file) != 'M') {
        fprintf(stdout, "File is not a BMP\n");
        fclose(file);
        return NULL;
    }

    // Type is valid
    fseek(file, 0, SEEK_SET);
    Bmp *bmp = bmpCreate();
    if ((bmp = malloc(sizeof *bmp)) == NULL) {
        ERROR("malloc\n");
        fclose(file);
        return NULL;
    }
    bmp->isSupported = true; // Initially set to true

    // Read the header
    if ((bmp->header = malloc(sizeof *bmp->header)) == NULL) {
        ERROR("malloc\n");
        bmpDestroy(bmp);
        fclose(file);
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
        fclose(file);
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

    if (!bmp->isSupported) {
        fprintf(stdout, "Histogram calculation is supported only for 24-bit, uncompressed BMP files\n");
    } else {
        // Allocate memory for the rows
        if ((bmp->pixelData = malloc((size_t) bmp->infoHeader->biHeight * sizeof *bmp->pixelData)) == NULL) {
            ERROR("malloc\n");
            bmpDestroy(bmp);
            fclose(file);
            return NULL;
        }
        // Allocate memory for the columns
        for (int32 i = 0; i < bmp->infoHeader->biHeight; i++) {
            if ((bmp->pixelData[i] = malloc((bmp->infoHeader->biWidth + 1) * sizeof(Pixel))) == NULL) { // + 1 to account for padding
                ERROR("malloc\n");
                bmpDestroy(bmp);
                fclose(file);
                return NULL;
            }
        }

        // Read and save the pixels; count components for the histogram
        fseek(file, bmp->header->bfOffBits, SEEK_SET); // Move to the first pixel position
        uint32 redArray[16], greenArray[16], blueArray[16];
        for (short i = 0; i < 16; i++) {
            // Initialize as zeros
            redArray[i] = 0;
            greenArray[i] = 0;
            blueArray[i] = 0;
        }
        for (int32 row = 0; row < bmp->infoHeader->biHeight; row++) {
            int32 column = 0;
            // Read and store pixels
            while (column < bmp->infoHeader->biWidth) {
                // Blue
                bmp->pixelData[row][column].blue = fileReadUi8(file);
                switch (bmp->pixelData[row][column].blue) {
                    case 0 ... 15:
                        blueArray[0]++;
                        break;
                    case 16 ... 31:
                        blueArray[1]++;
                        break;
                    case 32 ... 47:
                        blueArray[2]++;
                        break;
                    case 48 ... 63:
                        blueArray[3]++;
                        break;
                    case 64 ... 79:
                        blueArray[4]++;
                        break;
                    case 80 ... 95:
                        blueArray[5]++;
                        break;
                    case 96 ... 111:
                        blueArray[6]++;
                        break;
                    case 112 ... 127:
                        blueArray[7]++;
                        break;
                    case 128 ... 143:
                        blueArray[8]++;
                        break;
                    case 144 ... 159:
                        blueArray[9]++;
                        break;
                    case 160 ... 175:
                        blueArray[10]++;
                        break;
                    case 176 ... 191:
                        blueArray[11]++;
                        break;
                    case 192 ... 207:
                        blueArray[12]++;
                        break;
                    case 208 ... 223:
                        blueArray[13]++;
                        break;
                    case 224 ... 239:
                        blueArray[14]++;
                        break;
                    case 240 ... 255:
                        blueArray[15]++;
                        break;
                    default:
                        ERROR("pixel blue component not in range\n");
                        bmpDestroy(bmp);
                        fclose(file);
                        return NULL;
                }
                // Green
                bmp->pixelData[row][column].green = fileReadUi8(file);
                switch (bmp->pixelData[row][column].green) {
                    case 0 ... 15:
                        greenArray[0]++;
                        break;
                    case 16 ... 31:
                        greenArray[1]++;
                        break;
                    case 32 ... 47:
                        greenArray[2]++;
                        break;
                    case 48 ... 63:
                        greenArray[3]++;
                        break;
                    case 64 ... 79:
                        greenArray[4]++;
                        break;
                    case 80 ... 95:
                        greenArray[5]++;
                        break;
                    case 96 ... 111:
                        greenArray[6]++;
                        break;
                    case 112 ... 127:
                        greenArray[7]++;
                        break;
                    case 128 ... 143:
                        greenArray[8]++;
                        break;
                    case 144 ... 159:
                        greenArray[9]++;
                        break;
                    case 160 ... 175:
                        greenArray[10]++;
                        break;
                    case 176 ... 191:
                        greenArray[11]++;
                        break;
                    case 192 ... 207:
                        greenArray[12]++;
                        break;
                    case 208 ... 223:
                        greenArray[13]++;
                        break;
                    case 224 ... 239:
                        greenArray[14]++;
                        break;
                    case 240 ... 255:
                        greenArray[15]++;
                        break;
                    default:
                        ERROR("pixel green component not in range\n");
                        bmpDestroy(bmp);
                        fclose(file);
                        return NULL;
                }
                // Red
                bmp->pixelData[row][column].red = fileReadUi8(file);
                switch (bmp->pixelData[row][column].red) {
                    case 0 ... 15:
                        redArray[0]++;
                        break;
                    case 16 ... 31:
                        redArray[1]++;
                        break;
                    case 32 ... 47:
                        redArray[2]++;
                        break;
                    case 48 ... 63:
                        redArray[3]++;
                        break;
                    case 64 ... 79:
                        redArray[4]++;
                        break;
                    case 80 ... 95:
                        redArray[5]++;
                        break;
                    case 96 ... 111:
                        redArray[6]++;
                        break;
                    case 112 ... 127:
                        redArray[7]++;
                        break;
                    case 128 ... 143:
                        redArray[8]++;
                        break;
                    case 144 ... 159:
                        redArray[9]++;
                        break;
                    case 160 ... 175:
                        redArray[10]++;
                        break;
                    case 176 ... 191:
                        redArray[11]++;
                        break;
                    case 192 ... 207:
                        redArray[12]++;
                        break;
                    case 208 ... 223:
                        redArray[13]++;
                        break;
                    case 224 ... 239:
                        redArray[14]++;
                        break;
                    case 240 ... 255:
                        redArray[15]++;
                        break;
                    default:
                        ERROR("pixel red component not in range\n");
                        bmpDestroy(bmp);
                        fclose(file);
                        return NULL;
                }
                column++;
            }
            // Account for padding, which is stored as consecutive, insignificant pixel components
            switch ((size_t) (ceil(floor((24 * bmp->infoHeader->biWidth + 31) / 32) * 4)) - (bmp->infoHeader->biWidth * 3)) {
                case 0:
                    // No padding
                    break;
                case 1:
                    // 1 byte of padding
                    bmp->pixelData[row][column].blue = fileReadUi8(file);
                    break;
                case 2:
                    // 2 bytes of padding
                    bmp->pixelData[row][column].blue = fileReadUi8(file);
                    bmp->pixelData[row][column].green = fileReadUi8(file);
                    break;
                case 3:
                    // 3 bytes of padding
                    bmp->pixelData[row][column].blue = fileReadUi8(file);
                    bmp->pixelData[row][column].green = fileReadUi8(file);
                    bmp->pixelData[row][column].red = fileReadUi8(file);
                    break;
                default:
                    ERROR("improper padding\n");
                    bmpDestroy(bmp);
                    fclose(file);
                    return NULL;
            }
        }

        // Calculate values for the histogram
        uint32 pixelsTotal = (uint32) bmp->infoHeader->biWidth * bmp->infoHeader->biHeight;
        double redHistogram[16], greenHistogram[16], blueHistogram[16];
        for (short i = 0; i < 16; i++) {
            redHistogram[i] = (double) 100 * redArray[i] / pixelsTotal;
            greenHistogram[i] = (double) 100 * greenArray[i] / pixelsTotal;
            blueHistogram[i] = (double) 100 * blueArray[i] / pixelsTotal;
        }

        // Display histogram
        uint8 leftRange, rightRange;
        leftRange = 0;
        rightRange = 15;
        fprintf(stdout, "Red:\n");
        for (uint8 i = 0; i < 16; i++) {
            fprintf(stdout, " %u-%u: %.2f%%\n", leftRange, rightRange, redHistogram[i]);
            leftRange += 16;
            rightRange += 16;
        }
        leftRange = 0;
        rightRange = 15;
        fprintf(stdout, "Green:\n");
        for (uint8 i = 0; i < 16; i++) {
            fprintf(stdout, " %u-%u: %.2f%%\n", leftRange, rightRange, greenHistogram[i]);
            leftRange += 16;
            rightRange += 16;
        }
        leftRange = 0;
        rightRange = 15;
        fprintf(stdout, "Blue:\n");
        for (uint8 i = 0; i < 16; i++) {
            fprintf(stdout, " %u-%u: %.2f%%\n", leftRange, rightRange, blueHistogram[i]);
            leftRange += 16;
            rightRange += 16;
        }
    }

    fclose(file);
    return bmp;
}

void bmpCreateGreyscale(const Bmp *bmpIn, const char *path) {
    if (!bmpIn) {
        ERROR("no bmpIn\n");
        return;
    }
    if (!bmpIn->isSupported) {
        fprintf(stdout, "Creating greyscale is supported only for 24-bit, uncompressed BMP files\n");
        return;
    }
    FILE *file;
    if ((file = fopen(path, "wb")) == NULL) {
        ERROR("did not open fileOut\n");
        return;
    }

    // Write the content of the header
    fileWriteUi16(file, bmpIn->header->bfType);
    fileWriteUi32(file, bmpIn->header->bfSize);
    fileWriteUi16(file, bmpIn->header->bfReserved1);
    fileWriteUi16(file, bmpIn->header->bfReserved2);
    fileWriteUi32(file, bmpIn->header->bfOffBits);

    // Write the content of the infoHeader
    fileWriteUi32(file, bmpIn->infoHeader->biSize);
    fileWriteInt32(file, bmpIn->infoHeader->biWidth);
    fileWriteInt32(file, bmpIn->infoHeader->biHeight);
    fileWriteUi16(file, bmpIn->infoHeader->biPlanes);
    fileWriteUi16(file, bmpIn->infoHeader->biBitCount);
    fileWriteUi32(file, bmpIn->infoHeader->biCompression);
    fileWriteUi32(file, bmpIn->infoHeader->biSizeImage);
    fileWriteInt32(file, bmpIn->infoHeader->biXPelsPerMeter);
    fileWriteInt32(file, bmpIn->infoHeader->biYPelsPerMeter);
    fileWriteUi32(file, bmpIn->infoHeader->biClrUsed);
    fileWriteUi32(file, bmpIn->infoHeader->biClrImportant);

    // Write the content of the pixelData
    fseek(file, bmpIn->header->bfOffBits, SEEK_SET); // Move to the first pixel position
    short paddingBytes = (short) ceil(floor((24 * bmpIn->infoHeader->biWidth + 31) / 32) * 4) - (bmpIn->infoHeader->biWidth * 3);
    uint16 greyValue;
    for (int32 row = 0; row < bmpIn->infoHeader->biHeight; row++) {
        int32 column = 0;
        // Write all pixels
        while (column < bmpIn->infoHeader->biWidth) {
            // Calculate greyscale value
            greyValue = (uint16) (bmpIn->pixelData[row][column].blue + bmpIn->pixelData[row][column].green + bmpIn->pixelData[row][column].red) / 3;
            // Write it as all blue, green and red color components
            fileWriteUi8(file, greyValue);
            fileWriteUi8(file, greyValue);
            fileWriteUi8(file, greyValue);
            column++;
        }
        // Account for padding
        switch (paddingBytes) {
            case 0:
                // No padding
                break;
            case 1:
                // 1 byte of padding
                fileWriteUi8(file, bmpIn->pixelData[row][column].blue);
                break;
            case 2:
                // 2 bytes of padding
                fileWriteUi8(file, bmpIn->pixelData[row][column].blue);
                fileWriteUi8(file, bmpIn->pixelData[row][column].green);
                break;
            case 3:
                // 3 bytes of padding
                fileWriteUi8(file, bmpIn->pixelData[row][column].blue);
                fileWriteUi8(file, bmpIn->pixelData[row][column].green);
                fileWriteUi8(file, bmpIn->pixelData[row][column].red);
                break;
            default:
                ERROR("improper padding\n");
                fclose(file);
                exit(1);
        }
    }
    fprintf(stdout, "Exported greyscale image\n");
    fclose(file);
}

void bmpEncode(const char *path, const char *text) {
    if (!path) {
        ERROR("no path given\n");
        return;
    }
    if (!text) {
        ERROR("no text given\n");
        return;
    }

    // Validate if supported
    FILE *file;
    if ((file = fopen(path, "rb+")) == NULL) {
        fprintf(stdout, "did not open file %s\n", path);
        return;
    }
    if (fileReadUi8(file) != 'B') {
        fprintf(stdout, "file is not a bmp\n");
        fclose(file);
        return;
    }
    if (fileReadUi8(file) != 'M') {
        fprintf(stdout, "file is not a bmp\n");
        fclose(file);
        return;
    }
    fseek(file, 28, SEEK_SET);
    if (fileReadUi8(file) != 24) {
        fprintf(stdout, "Steganography is supported only for 24-bit, uncompressed bmp files\n");
        fclose(file);
        return;
    }
    if (fileReadUi32(file) != 0) {
        fprintf(stdout, "Steganography is supported only for 24-bit, uncompressed bmp files\n");
        fclose(file);
        return;
    }

    // Read/calculate all metadata needed to encode the text
    fseek(file, 10, SEEK_SET);
    int32 offset = fileReadUi32(file);
    fseek(file, 18, SEEK_SET);
    int32 width = fileReadInt32(file);
    int32 height = fileReadInt32(file);
    size_t textLength = strlen(text);
    if (textLength > 255) {
        fprintf(stdout, "max encoded text size is 255 characters (the last one is for '\\0')\n");
        fclose(file);
        return;
    }
    size_t bytesNeeded = (textLength + 1) * 8 * sizeof(char); // + 1 to compensate for '\0'
    size_t bytesAvailable = width * height * 3 * sizeof(uint8); // We don't use padding for encoding
    size_t padding = (size_t) (floor((24 * width + 31) / 32) * 4) - (width * 3);
    if (bytesNeeded > bytesAvailable) {
        fprintf(stdout, "The bmp is not large enough to encode the message\n");
        fprintf(stdout, "bytesNeeded = %zu; bytesAvailable = %zu\n", bytesNeeded, bytesAvailable);
        fclose(file);
        return;
    }
    // At this point, we are certain we can encode this message in the bmp

    // Store each character bit by bit
    bool convertedText[(textLength + 1) * 8];
    size_t convIdx = 0;
    for (int textIdx = 0; textIdx < textLength; textIdx++) {
        for(int bit = 7; 0 <= bit; bit--) {
            convertedText[convIdx] = (bool) ((text[textIdx] >> bit) & 1);
            convIdx++;
        }
    }
    // Put '\0' at the end
    for(int bit = 7; 0 <= bit; bit--) {
        convertedText[convIdx] = (bool) (('\0' >> bit) & 1);
        convIdx++;
    }

    // Alter data in the bmp to encode text
    fseek(file, offset, SEEK_SET);
    int32 lineIdx = 0;
    uint8 byte;
    for (convIdx = 0; convIdx < (textLength + 1) * 8; convIdx++) {
        byte = fileReadUi8(file);
        fseek(file, -1, SEEK_CUR);
        if (((byte >> 0) & 1) != convertedText[convIdx]) {
            if (((byte >> 0) & 1) == 1) {
                // There is 1, whereas we want to encode 0
                byte--;
            } else {
                // There is 0, whereas we want to encode 1
                byte++;
            }
        }
        fileWriteUi8(file, byte);
        lineIdx++;
        // Account for padding
        if (lineIdx == width) {
            switch (padding) {
                case 0:
                    // No padding
                    break;
                case 1:
                    // 1 byte of padding
                    fseek(file, 1, SEEK_CUR);
                    break;
                case 2:
                    // 2 bytes of padding
                    fseek(file, 2, SEEK_CUR);
                    break;
                case 3:
                    // 3 bytes of padding
                    fseek(file, 3, SEEK_CUR);
                    break;
                default:
                    ERROR("improper padding\n");
                    fclose(file);
                    exit(1);
            }
            lineIdx = 0;
        }
    }

    fprintf(stdout, "Successfully encoded the message\n");
    fclose(file);
}

void bmpDecode(const char *path) {
    if (!path) {
        ERROR("no path given\n");
        return;
    }
    // Validate if supported
    FILE *file;
    if ((file = fopen(path, "rb+")) == NULL) {
        fprintf(stdout, "did not open file %s\n", path);
        return;
    }
    if (fileReadUi8(file) != 'B') {
        fprintf(stdout, "file is not a bmp\n");
        fclose(file);
        return;
    }
    if (fileReadUi8(file) != 'M') {
        fprintf(stdout, "file is not a bmp\n");
        fclose(file);
        return;
    }
    fseek(file, 28, SEEK_SET);
    if (fileReadUi8(file) != 24) {
        fprintf(stdout, "Steganography is supported only for 24-bit, uncompressed bmp files\n");
        fclose(file);
        return;
    }
    if (fileReadUi32(file) != 0) {
        fprintf(stdout, "Steganography is supported only for 24-bit, uncompressed bmp files\n");
        fclose(file);
        return;
    }

    // Read/calculate all metadata needed to encode the text
    fseek(file, 10, SEEK_SET);
    int32 offset = fileReadUi32(file);
    fseek(file, 18, SEEK_SET);
    int32 width = fileReadInt32(file);
    int32 height = fileReadInt32(file);
    size_t padding = (size_t) (floor((24 * width + 31) / 32) * 4) - (width * 3);

    // Read the encoded bits, decode and save to char array
    fseek(file, offset, SEEK_SET);
    bool charBin[8];
    char message[256];
    int16_t charConv = 0;
    uint16 messIdx = 0;
    int32 lineIdx = 0;
    int32 heightIdx = 0;
    while (messIdx < 256) {
        for (short bit = 0; bit < 8; bit++) {
            uint8 byte;
            byte = fileReadUi8(file);
            lineIdx++;
            charBin[bit] = (bool) ((byte >> 0) & 1);
            // Account for padding
            if (lineIdx == width) {
                switch (padding) {
                    case 0:
                        // No padding
                        break;
                    case 1:
                        // 1 byte of padding
                        fseek(file, 1, SEEK_CUR);
                        break;
                    case 2:
                        // 2 bytes of padding
                        fseek(file, 2, SEEK_CUR);
                        break;
                    case 3:
                        // 3 bytes of padding
                        fseek(file, 3, SEEK_CUR);
                        break;
                    default:
                        ERROR("improper padding\n");
                        fclose(file);
                        exit(1);
                }
                lineIdx = 0;
                heightIdx++;
                if (heightIdx == height) {
                    fprintf(stdout, "No message detected\n");
                    fclose(file);
                    return;
                }
            }
        }
        // Decrypt the char
        charConv -= 128 * charBin[0];
        charConv += 64 * charBin[1];
        charConv += 32 * charBin[2];
        charConv += 16 * charBin[3];
        charConv += 8 * charBin[4];
        charConv += 4 * charBin[5];
        charConv += 2 * charBin[6];
        charConv += charBin[7];
        message[messIdx] = charConv;
        if (charConv == '\0')
            break;
        charConv = 0;
        messIdx++;
    }
    if (messIdx > 255) {
        fprintf(stdout, "No message detected\n");
        fclose(file);
        return;
    }
    fprintf(stdout, "Decrypted message: \"%s\"\n", message);

    fclose(file);
}

void bmpDestroy(Bmp *bmp) {
    if (!bmp)
        return;
    if (bmp->pixelData) {
        for (int32 i = 0; i < bmp->infoHeader->biHeight; i++)
            free(bmp->pixelData[i]);
        free(bmp->pixelData);
    }
    if (bmp->infoHeader)
        free(bmp->infoHeader);
    if (bmp->header)
        free(bmp->header);
    free(bmp);
}
