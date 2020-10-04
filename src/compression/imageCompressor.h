#ifndef IMAGECOMPRESSOR_H
#define IMAGECOMPRESSOR_H

#include "imagetransform.h"
#include <encoder.h>


void setFileParameter(char* img_name, char* benchmark_file);
compressed_img * compress(IMG* img, int rangeblockWidth, int rangeblockHeight, int rotation_num, int translation_num, int shear_num, int scale_num, unsigned int threshold_similarity, int downscale_factor);
double** create_transformationMatrixList(int rotation_num, int translation_num, int shear_num, int scale_num, double** rotations, double** translations,double** shears,double** scalings);

IMG* decompress(compressed_img* c_img);
#endif
