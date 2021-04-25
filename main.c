#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <float.h>
#include <string.h>
#include "bmparser.h"

// Driver program for the .bmp file parser
int main(int argc, char *argv[])
{
    if (argc == 1) {
        fprintf(stdout, "No file path provided\n");
        return 1;
    }
    if (argc == 1 || argc > 4) {
        fprintf(stdout, "Minimum 1 argument required - path to the bmp file to be parsed\n");
        fprintf(stdout, "For greyscale creation, give 2 arguments: file path and path to out, greyscale file\n");
        fprintf(stdout, "For message encoding, use the '-e' flag, specify path to the file to be encoded and write and the message (255 char max)\n");
        fprintf(stdout, "For message decoding, use the '-d' flag and specify path to the file to be decoded\n");
        exit(1);
    }

    Bmp *bmp;
    if (argc == 2) {
        // Only parse
        bmp = bmpReadFile(argv[1]);
        bmpDestroy(bmp);
    } else if (argc == 4) {
        // Message to be encoded
        if (strcmp(argv[1], "-e") != 0) {
            if (strcmp(argv[1], "-d") == 0) {
                fprintf(stdout, "Too many arguments for decoding\n");
            } else {
                fprintf(stdout, "Unknown flag '%s'\n", argv[1]);
            }
            fprintf(stdout, "Minimum 1 argument required - path to the bmp file to be parsed\n");
            fprintf(stdout, "For greyscale creation, give 2 arguments: file path and path to out, greyscale file\n");
            fprintf(stdout, "For message encoding, use the '-e' flag, specify path to the file to be encoded and write and the message (255 char max)\n");
            fprintf(stdout, "For message decoding, use the '-d' flag and specify path to the file to be decoded\n");
            exit(1);
        }
        bmpEncode(argv[2], argv[3]);
    } else {
        if (strcmp(argv[1], "-d") == 0) {
            // Message to be decoded
            bmpDecode(argv[2]);
        } else {
            // Parse and create greyscale
            bmp = bmpReadFile(argv[1]);
            bmpCreateGreyscale(bmp, argv[2]);
            bmpDestroy(bmp);
        }
    }

	return 0;
}