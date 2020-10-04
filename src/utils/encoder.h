#ifndef ENCODER_H
#define ENCODER_H

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef DEBUG
# define DEBUG_PRINT(x) printf x
#else
# define DEBUG_PRINT(x)
#endif

#define DOUBLE_SIZE sizeof(double)
#define UINT_SIZE sizeof(unsigned int)
#define UINT8_SIZE sizeof(uint8_t)

typedef struct rdt_tuple_s{ // RANGE->DOMAIN-TRANSFORM tuple
    uint16_t domain_x;  // pixel
    uint16_t domain_y;  // pixel
    uint16_t range_x;   // pixel
    uint16_t range_y;   // pixel
    uint8_t transformMatrixIndex;
    uint8_t quadtreeDepth; // 0 -> no division, 1 -> split one time, .... , n -> split n times
} rdt_tuple;

typedef struct compressed_img_s{
    unsigned int domainHeight;
    unsigned int domainWidth;
    unsigned int rangeHeight;
    unsigned int rangeWidth;
    unsigned int domainBlockHeight;
    unsigned int domainBlockWidth;
    unsigned int numTransformMatrix;
    unsigned int numRangeBlock;
    double** transformMatrixList;
    rdt_tuple* transformMapping; // mapping vector
    uint8_t* domain;
} compressed_img;

void* encode(compressed_img);
compressed_img* decode(void*);
size_t getEncodedImageSize(void*);

void writeEncodedImage(char* fileName, void* encondedImageData);
void* readEncodedImage(char* fileName);

#endif