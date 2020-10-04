#ifndef IMAGEREADER_H
#define IMAGEREADER_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bmp.h"

#ifdef DEBUG
# define DEBUG_PRINT(x) printf x
#else
# define DEBUG_PRINT(x)
#endif

IMG* readImage(char* file);
void freeImage(IMG* img);
void printImageInfo(IMG* img, bool detailed);
void saveGreyImage(IMG* img, char* fileName);
void saveRGBImage(IMG* img, char* fileName);

#endif
