#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <float.h>
#include "bmparser.h"

// Driver program for the .bmp file parser
int main(int argc, char *argv[])
{
    if (argc == 1) {
        ERROR("No file path provided\n");
        return 1;
    }
    if (argc > 3) {
        ERROR("At max 2 arguments required - file path and path to out, greyscale file\n");
        return 2;
    }

    // One command line parameter - only read and display info
    Bmp *bmpIn = bmpReadFile(argv[1]);
    if (argc == 3) {
        // Two command line parameters - if possible, create and export greyscale
        bmpCreateGreyscale(bmpIn);
    }

    bmpDestroy(bmpIn);
	return 0;
}