#ifndef IMAGETRANSFORM_H
#define IMAGETRANSFORM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <imageIO.h>
#include <math.h>
#include <assert.h>
#include "../benchmark/benchmark.h"


IMG* rgbToGreyscale(IMG* img);
IMG* copyImg(IMG* img);
uint8_t* linearMatrixCompressor(uint8_t* oldGsImg, int width, int height, int downscale_factor);
double** create_translations(unsigned int start, unsigned int end, double step, bool linked);
double** create_rotations(unsigned int start, unsigned int end, double angle, unsigned int width, unsigned int height);
double** create_scalings(unsigned int start, unsigned int end, double step, bool linked);
double** create_shears(unsigned int start, unsigned int end, double step, bool linked);
uint8_t* affineTransform(uint8_t* rangeBlock, unsigned int width, unsigned int height, unsigned int width_matrix, double* affineTransformMatrix);
unsigned int euclidean_distance(const uint8_t* restrict rangeBlock, const uint8_t* restrict domainBlock, const int rangeBlockHeight, const int rangeBlockWidth, const int width, const int downscale_factor);
#endif