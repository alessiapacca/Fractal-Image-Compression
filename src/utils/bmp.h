#ifndef BMP_H
#define BMP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/*
 * Bitmap file header
 */
typedef struct tagBITMAPFILEHEADER {
    char           bfType[3];    /* 2 bytes + null char */ // string "BM\0"
    unsigned int   bfSize;       /* 4 bytes */  // The size of the BMP file in bytes
    unsigned short bfReserved1;  /* 2 bytes */ // Zeros
    unsigned short bfReserved2;  /* 2 bytes */ // Zeros
    unsigned int  bfOffBits;    /* 4 bytes */ //Starting address, of the byte where the bitmap image data (pixel array) can be found.
                                // If no colorPalette is 54. If colorPalette is 256*4+54 = 1078
} BITMAPFILEHEADER;

/*
 * Bitmap info header (Windows)
 */
typedef struct tagBITMAPINFOHEADER {
    unsigned int   biSize;          /* 4 bytes */ //the size of this header, in bytes (40)
    long           biWidth;         /* 4 bytes */ //the bitmap width in pixels
    long           biHeight;        /* 4 bytes */ //the bitmap height in pixels
    unsigned short biPlanes;        /* 2 bytes */ //the number of color planes (must be 1)
    unsigned short biBitCount;      /* 2 bytes */ //the number of bits per pixel, which is the color depth of the image. 8 for greyscale, 24 for RGB
    unsigned int   biCompression;   /* 4 bytes */ //Compression is 0 if none (4bytes)
    unsigned int   biSizeImage;     /* 4 bytes */ //the image size. This is the size of the raw bitmap data; a dummy 0 can be given for uncompressed
    unsigned int   biXPixPerMeter;  /* 4 bytes */ //the horizontal resolution of the image.
    unsigned int   biYPixPerMeter;  /* 4 bytes */ //the vertical resolution of the image.
    unsigned int   biClrUsed;       /* 4 bytes */ //the number of colors in the color palette -> set to 0
    unsigned int   biClrImportant;  /* 4 bytes */ //the number of important colors used -> set to 0
} BITMAPINFOHEADER;

typedef struct Rgb{
    unsigned char b;
    unsigned char g;
    unsigned char r;
} RGB;

typedef struct colorPalette{
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char padding;
} CLP;

typedef struct Image{
    BITMAPFILEHEADER *bmFileHeader;
    BITMAPINFOHEADER *bmInfoHeader;
    int headersize;
    int channels;
    RGB* rgbData;
    unsigned char* greyData;
    CLP* palette;
} IMG;

BITMAPFILEHEADER *readBMFileHeader(FILE *fp);
BITMAPINFOHEADER *readBMInfoHeader(FILE *fp);

int sizeOfInformationHeader(FILE *fp);
unsigned int readLE4(FILE *fp);
unsigned short readLE2(FILE *fp);

#endif

