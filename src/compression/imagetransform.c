#include "imagetransform.h"

#define PI 3.14159265

IMG *copyImg(IMG *img) {
    DEBUG_PRINT(("started copy\n"));

    IMG *newimg = (IMG *) malloc(sizeof(IMG));
    BITMAPFILEHEADER *localFileHeader = malloc(sizeof(BITMAPFILEHEADER));
    BITMAPINFOHEADER *localInfoHeader = malloc(sizeof(BITMAPINFOHEADER));

    *localFileHeader = *(img->bmFileHeader);
    *localInfoHeader = *(img->bmInfoHeader);

    unsigned char *localGreyData = malloc(
            sizeof(unsigned char) * img->bmInfoHeader->biWidth * img->bmInfoHeader->biHeight);
    memcpy(localGreyData, img->greyData,
           sizeof(unsigned char) * img->bmInfoHeader->biWidth * img->bmInfoHeader->biHeight);

    RGB *localRgbData = malloc(sizeof(RGB) * img->bmInfoHeader->biWidth * img->bmInfoHeader->biHeight);
    memcpy(localRgbData, img->rgbData, sizeof(RGB) * img->bmInfoHeader->biWidth * img->bmInfoHeader->biHeight);

    if (img->bmFileHeader->bfOffBits == 1078) {
        CLP *localPalette = malloc(sizeof(CLP) * 256);
        memcpy(localPalette, img->palette, sizeof(CLP) * 256);
        newimg->palette = localPalette;
    }

    newimg->bmFileHeader = localFileHeader;
    newimg->bmInfoHeader = localInfoHeader;
    newimg->greyData = localGreyData;
    newimg->rgbData = localRgbData;

    newimg->channels = img->channels;
    newimg->headersize = img->headersize;

    DEBUG_PRINT(("finished copy\n"));
    return newimg;
}

IMG *rgbToGreyscale(IMG *img) {
#ifdef RGBTOGRAYSCALE
    initializeBenchmark("rgbToGreyscale");
    incrementFlopCount(0, 0, 0, 0, (img->bmInfoHeader->biWidth * img->bmInfoHeader->biHeight) * 2, (img->bmInfoHeader->biWidth * img->bmInfoHeader->biHeight) * 3, 0, 0, false);
    myInt64 start_time = startBenchmark();
#endif

    DEBUG_PRINT(("started conversion to RGB\n"));


    IMG *newimg = copyImg(img);

    for (unsigned i = 0; i < newimg->bmInfoHeader->biWidth * newimg->bmInfoHeader->biHeight; i++) {
        unsigned char r = newimg->rgbData[i].r, g = newimg->rgbData[i].g, b = newimg->rgbData[i].b;
        newimg->greyData[i] = (0.2 * r + 0.59 * g + 0.11 * b);
        newimg->rgbData[i].r = newimg->greyData[i];
        newimg->rgbData[i].b = newimg->greyData[i];
        newimg->rgbData[i].g = newimg->greyData[i];
    }

    newimg->bmFileHeader->bfSize =
            newimg->bmInfoHeader->biWidth * newimg->bmInfoHeader->biHeight + newimg->bmInfoHeader->biSize;
    newimg->bmFileHeader->bfOffBits = 1078;
    newimg->bmInfoHeader->biBitCount = 8;
    newimg->bmInfoHeader->biSizeImage = 0; //This is the size of the raw bitmap data; a dummy 0 can be given for non compressed bmp
    newimg->channels = 1;
    newimg->headersize = img->headersize;

    if (img->bmFileHeader->bfOffBits != 1078) {
        newimg->palette = malloc(sizeof(CLP) * 256);
        for (int i = 0; i < 256; i++) {
            newimg->palette[i].r = i;
            newimg->palette[i].g = i;
            newimg->palette[i].b = i;
            newimg->palette[i].padding = '\0';
        }
    }

    DEBUG_PRINT(("finished conversion to RGB\n"));
    DEBUG_PRINT(("--------------- BEFORE RETURN CONVERSION IMAGE -------------------------\n"));
    DEBUG_PRINT(printImageInfo(newimg, false));


#ifdef RGBTOGRAYSCALE
    stopBenchmark(start_time, "rgbToGreyscale");
#endif
    return newimg;
}


double **create_translations(unsigned int start, unsigned int end, double step, bool linked) {

    int total_translations;
    assert(end > start); // (end-start) must be divisible for step

    if (linked) //if linked x and y grow simultaneously
        total_translations = (end - start) / step;
    else //if not linked: only x grows, only y grows, both grow together
        total_translations = (end - start) / step * 3;

    double **result_list = (double **) malloc(sizeof(double *) * total_translations);

#ifdef CREATE_TRANSLATIONS
    initializeBenchmark("create_translations");
    if(linked)
        incrementFlopCount(0, 0, 0, 0, total_translations * 2, total_translations * 2, 0, 0, false);
    else
        incrementFlopCount(0, 0, 0, 0, total_translations/3 * 4, total_translations/3 * 4, 0, 0, false);
    myInt64 start_time = startBenchmark();
#endif
    if (linked) {

        for (int i = 0; i < total_translations; i++) {
            double *new_translation = calloc(9, sizeof(double));
            new_translation[0] = 1;
            new_translation[2] = start + step * i;
            new_translation[4] = 1;
            new_translation[5] = start + step * i;
            new_translation[8] = 1;

            result_list[i] = new_translation;
        }
    } else {

        for (int i = 0; i < total_translations / 3; i++) {
            double *new_translation = calloc(9, sizeof(double));
            double *new_translation_2 = calloc(9, sizeof(double));
            double *new_translation_3 = calloc(9, sizeof(double));

            new_translation[0] = 1;
            new_translation[2] = start + step * i;
            new_translation[4] = 1;
            new_translation[8] = 1;

            new_translation_2[0] = 1;
            new_translation_2[4] = 1;
            new_translation_2[5] = start + step * i;
            new_translation_2[8] = 1;

            new_translation_3[0] = 1;
            new_translation_3[2] = start + step * i;
            new_translation_3[4] = 1;
            new_translation_3[5] = start + step * i;
            new_translation_3[8] = 1;

            result_list[i] = new_translation;
            result_list[i + total_translations / 3] = new_translation_2;
            result_list[i + 2 * total_translations / 3] = new_translation_3;
        }
    }

#ifdef CREATE_TRANSLATIONS
    stopBenchmark(start_time, "create_translations");
#endif
    return result_list;

}


double **create_rotations(unsigned int start, unsigned int end, double angle, unsigned int width, unsigned int height) {

    int total_rotations;
    assert(end > start); // (end-start) must be divisible for step

    total_rotations = (end - start) / angle;

    double **result_list = (double **) malloc(sizeof(double *) * total_rotations);


#ifdef CREATE_ROTATIONS
    initializeBenchmark("create_rotations");
    incrementFlopCount(0, 0, 0, 0, 0, 0, 0, 0, false);
    myInt64 start_time = startBenchmark();
#endif
    for (int i = 0; i < total_rotations; i++) {
        double *new_rotation = calloc(9, sizeof(double));

        new_rotation[0] = cos((start + angle * i) * PI / 180);
        new_rotation[1] = sin((start + angle * i) * PI / 180);
        new_rotation[3] = -sin((start + angle * i) * PI / 180);
        new_rotation[4] = cos((start + angle * i) * PI / 180);
        new_rotation[8] = 1;

        result_list[i] = new_rotation;
    }

#ifdef CREATE_ROTATIONS
    stopBenchmark(start_time, "create_rotations");
#endif

    return result_list;

}

double **create_scalings(unsigned int start, unsigned int end, double step, bool linked) {

    int total_scalings;
    assert(end > start); // (end-start) must be divisible for step

    if (linked) //if linked x and y grow simultaneously
        total_scalings = (end - start) / step;
    else //if not linked: only x grows, only y grows, both grow together
        total_scalings = (end - start) / step * 3;

    double **result_list = (double **) malloc(sizeof(double *) * total_scalings);


#ifdef CREATE_SCALINGS
    initializeBenchmark("create_scalings");
    if(linked)
        incrementFlopCount(0, 0, 0, 0, total_scalings * 2, total_scalings * 2, 0, 0, false);
    else
        incrementFlopCount(0, 0, 0, 0, total_scalings/3 * 4, total_scalings/3 * 4, 0, 0, false);
    myInt64 start_time = startBenchmark();
#endif
    if (linked) {

        for (int i = 0; i < total_scalings; i++) {
            double *new_scaling = calloc(9, sizeof(double));
            new_scaling[0] = start + step * i;
            new_scaling[4] = start + step * i;
            new_scaling[8] = 1;

            result_list[i] = new_scaling;
        }
    } else {

        for (int i = 0; i < total_scalings / 3; i++) {
            double *new_scaling = calloc(9, sizeof(double));
            double *new_scaling_2 = calloc(9, sizeof(double));
            double *new_scaling_3 = calloc(9, sizeof(double));

            new_scaling[0] = start + step * i;
            new_scaling[4] = start + step * i;
            new_scaling[8] = 1;

            new_scaling_2[0] = start + step * i;
            new_scaling_2[4] = 1;
            new_scaling_2[8] = 1;

            new_scaling_3[0] = 1;
            new_scaling_3[4] = start + step * i;
            new_scaling_3[8] = 1;

            result_list[i] = new_scaling;
            result_list[i + total_scalings / 3] = new_scaling_2;
            result_list[i + 2 * total_scalings / 3] = new_scaling_3;
        }
    }

#ifdef CREATE_SCALINGS
    stopBenchmark(start_time, "create_scalings");
#endif

    return result_list;
}


double **create_shears(unsigned int start, unsigned int end, double step, bool linked) {

    int total_shears;
    assert(end > start); // (end-start) must be divisible for step

    if (linked) //if linked x and y grow simultaneously
        total_shears = (end - start) / step;
    else //if not linked: only x grows, only y grows, both grow together
        total_shears = (end - start) / step * 3;

    double **result_list = (double **) malloc(sizeof(double *) * total_shears);

#ifdef CREATE_SHEARS
    initializeBenchmark("create_shears");
    if(linked)
        incrementFlopCount(0, 0, 0, 0, total_shears * 2, total_shears * 2, 0, 0, false);
    else
        incrementFlopCount(0, 0, 0, 0, total_shears/3 * 4, total_shears/3 * 4, 0, 0, false);
    myInt64 start_time = startBenchmark();
#endif
    if (linked) {

        for (int i = 0; i < total_shears; i++) {
            double *new_shear = calloc(9, sizeof(double));
            new_shear[0] = 1;
            new_shear[1] = start + step * i;
            new_shear[3] = start + step * i;
            new_shear[4] = 1;
            new_shear[8] = 1;

            result_list[i] = new_shear;
        }
    } else {

        for (int i = 0; i < total_shears / 3; i++) {
            double *new_shear = calloc(9, sizeof(double));
            double *new_shear_2 = calloc(9, sizeof(double));
            double *new_shear_3 = calloc(9, sizeof(double));

            new_shear[0] = 1;
            new_shear[1] = start + step * i;
            new_shear[3] = start + step * i;
            new_shear[4] = 1;
            new_shear[8] = 1;

            new_shear_2[0] = 1;
            new_shear_2[1] = start + step * i;
            new_shear_2[4] = 1;
            new_shear_2[8] = 1;

            new_shear_3[0] = 1;
            new_shear_3[3] = start + step * i;
            new_shear_3[4] = 1;
            new_shear_3[8] = 1;

            result_list[i] = new_shear;
            result_list[i + total_shears / 3] = new_shear_2;
            result_list[i + 2 * total_shears / 3] = new_shear_3;
        }
    }

#ifdef CREATE_SHEARS
    stopBenchmark(start_time, "create_shears");
#endif

    return result_list;
}


void nearest_neighbors(int *returnBlock_temp, int *returnBlock_modified, unsigned int height, unsigned int width) {
    for (int i = 0; i < height; i++) {
#ifdef IDX_OPS
        incrementFlopCount(1, 0, 0, 0, 0, 0, 0, 0, false);
#endif
        for (int j = 0; j < width; j++) {
#ifdef IDX_OPS
            incrementFlopCount(2, 1, 0, 0, 0, 0, 0, 0, false);
#endif
            int index_block = i * width + j;
            returnBlock_modified[index_block] = returnBlock_temp[index_block];
        }
    }

    //change the matrix
    for (unsigned int row_i = 0; row_i < height; row_i++) {
#ifdef IDX_OPS
        incrementFlopCount(1, 0, 0, 0, 0, 0, 0, 0, false);
#endif
        for (unsigned int col_j = 0; col_j < width; col_j++) {
#ifdef IDX_OPS
            incrementFlopCount(2, 1, 0, 0, 0, 0, 0, 0, false);
#endif
            unsigned int index_block = row_i * width + col_j;
            int result = 0;
            //if the element is -1, then change it
            if (returnBlock_modified[index_block] == -1) {
                //max neighbors = 4
                int cont = 4;

                //down
                if (row_i != 0 && returnBlock_modified[(row_i - 1) * width + col_j] != -1) {
                    result += returnBlock_modified[(row_i - 1) * width + col_j]; //index_block - height
#ifdef IDX_OPS
                    incrementFlopCount(1, 1, 1, 0, 0, 0, 0, 0, false);
#endif
#ifdef OPS
                    incrementFlopCount(1, 0, 0, 0, 0, 0, 0, 0, false);
#endif
                } else {
                    cont--;
#ifdef OPS
                    incrementFlopCount(0, 0, 1, 0, 0, 0, 0, 0, false);
#endif
                }

#ifdef IDX_OPS
                incrementFlopCount(1, 0, 0, 0, 0, 0, 0, 0, false);
#endif
                //up
                if (row_i + 1 < height && returnBlock_modified[(row_i + 1) * width + col_j] != -1) {
                    result += returnBlock_modified[(row_i + 1) * width + col_j]; //index_block + height
#ifdef IDX_OPS
                    incrementFlopCount(1, 1, 0, 0, 0, 0, 0, 0, false);
#endif

#ifdef OPS
                    incrementFlopCount(1, 0, 0, 0, 0, 0, 0, 0, false);
#endif
                } else {
                    cont--;
#ifdef OPS
                    incrementFlopCount(0, 0, 1, 0, 0, 0, 0, 0, false);
#endif
                }

                //left
                if (col_j != 0 && returnBlock_modified[row_i * width + col_j - 1] != -1) {
                    result += returnBlock_modified[row_i * width + col_j - 1]; //index_block - 1
#ifdef IDX_OPS
                    incrementFlopCount(1, 1, 1, 0, 0, 0, 0, 0, false);
#endif

#ifdef OPS
                    incrementFlopCount(1, 0, 0, 0, 0, 0, 0, 0, false);
#endif
                } else {
                    cont--;
#ifdef OPS
                    incrementFlopCount(0, 0, 1, 0, 0, 0, 0, 0, false);
#endif
                }

#ifdef IDX_OPS
                incrementFlopCount(1, 0, 0, 0, 0, 0, 0, 0, false);
#endif
                //right
                if (col_j + 1 < width && returnBlock_modified[row_i * width + col_j + 1] != -1) {
                    result += returnBlock_modified[row_i * width + col_j + 1]; //index_block + 1
#ifdef IDX_OPS
                    incrementFlopCount(1, 1, 0, 0, 0, 0, 0, 0, false);
#endif
#ifdef OPS
                    incrementFlopCount(1, 0, 0, 0, 0, 0, 0, 0, false);
#endif
                } else {
                    cont--;
#ifdef OPS
                    incrementFlopCount(0, 0, 1, 0, 0, 0, 0, 0, false);
#endif
                }


                if (cont == 0)
                    result = 1; //then we have all -1 values. assign an arbitrary value
                else {
                    result = result / cont;
#ifdef OPS
                    incrementFlopCount(0, 0, 0, 1, 0, 0, 0, 0, false);
#endif
                }

            } else {
                result = returnBlock_temp[index_block];
            }

            returnBlock_modified[index_block] = result;
        }
    }
}

uint8_t *affineTransform(uint8_t *rangeBlock, unsigned int height, unsigned int width, unsigned int width_matrix,
                         double *affineTransformMatrix) {
#ifdef IDX_OPS
    incrementFlopCount(0, 4, 0, 0, 0, 0, 0, 0, false);
#endif
    int tot_size = width * height;
    //rangeBlock contains the values of the greyScale intensities
    uint8_t *returnBlock = (uint8_t *) malloc(tot_size * sizeof(uint8_t));
    int *returnBlock_temp = (int *) malloc(tot_size * sizeof(int));
    int *returnBlock_modified = (int *) malloc(tot_size * sizeof(int));
    //in a block 16x16 the max index is 256
    for (int i = 0; i < tot_size; i++) {
#ifdef IDX_OPS
        incrementFlopCount(1, 0, 0, 0, 0, 0, 0, 0, false);
#endif
        returnBlock_temp[i] = -1;
    }

    //y is the row, so the y coordinate. we have height rows
    for (int y = 0; y < height; y++) {
#ifdef IDX_OPS
        incrementFlopCount(1, 0, 0, 0, 0, 0, 0, 0, false);
#endif
        //x is the column, so the x coordinate. we have width cols
        for (int x = 0; x < width; x++) {
#ifdef IDX_OPS
            incrementFlopCount(1, 0, 0, 0, 0, 0, 0, 0, false);
#endif
#ifdef OPS
            incrementFlopCount(2, 2, 0, 0, 6, 6, 0, 2, false);
#endif
            //index of the rangeBlock
            int indexOld = y * width_matrix + x;
            double res_x = affineTransformMatrix[0] * x + affineTransformMatrix[1] * y + affineTransformMatrix[2];
            double res_y = affineTransformMatrix[3] * x + affineTransformMatrix[4] * y + affineTransformMatrix[5];
            double res_hom = affineTransformMatrix[6] * x + affineTransformMatrix[7] * y + affineTransformMatrix[8];
            //convert to homogeneous
            double res_row = res_x / res_hom;
            double res_col = res_y / res_hom;

            //now we have the new coordinates
            int floor_row = floor(res_row);
            int floor_col = floor(res_col);
            int indexNew = floor_col * width + floor_row;

            //if((indexNew >= 0)&&(indexNew < maxIndex)){ //PACMAN effect
            if ((floor_row >= 0) && (floor_row < width) && (floor_col >= 0) && (floor_col < height)) {
                returnBlock_temp[indexNew] = *(rangeBlock + indexOld);
#ifdef IDX_OPS
                incrementFlopCount(1, 0, 0, 0, 0, 0, 0, 0, false);
#endif
            }
            //check that the index isn't out of the block
        }
    }

    nearest_neighbors(returnBlock_temp, returnBlock_modified, height, width);
    for (int i = 0; i < tot_size; i++) {
#ifdef IDX_OPS
        incrementFlopCount(1, 0, 0, 0, 0, 0, 0, 0, false);
#endif
        returnBlock[i] = returnBlock_modified[i];
    }

    return returnBlock;
}


unsigned int
euclidean_distance(const uint8_t *restrict rangeBlock, const uint8_t *restrict domainBlock, const int rangeBlockHeight,
                   const int rangeBlockWidth, const int width, const int downscale_factor) {

#ifdef OPS //1 add for width/downscale_factor
    incrementFlopCount(0, 0, 0, 1, 0, 0, 0, 0, false);
#endif
    int scaled_w = (width / downscale_factor);
    unsigned int distance = 0;
    int d;
    for (int y = 0; y < rangeBlockHeight; y++) {
#ifdef IDX_OPS
        incrementFlopCount(1, 0, 0, 0, 0, 0, 0, 0, false);
#endif
        for (int x = 0; x < rangeBlockWidth; x++) {
#ifdef IDX_OPS
            incrementFlopCount(3, 2, 0, 0, 0, 0, 0, 0, false);
#endif
            d = domainBlock[y * scaled_w + x] - rangeBlock[y * width + x];
            distance += d * d;
#ifdef OPS
            incrementFlopCount(1, 1, 1, 0, 0, 0, 0, 0, false);
#endif
        }
    }
    return distance;
}


unsigned int m_i(unsigned int row_size, unsigned int row_i, unsigned int col_i) {
#ifdef IDX_OPS
    incrementFlopCount(1, 1, 0, 0, 0, 0, 0, 0, false);
#endif
    return row_size * row_i + col_i;
}

uint8_t *linearMatrixCompressor(uint8_t *oldGsImg, int width, int height, int downscale_factor) {
    int oldGsImageWidth = width;
    int oldGsImageHeight = height;
    int gsImageWidth = oldGsImageWidth / downscale_factor;
    int gsImageHeight = oldGsImageHeight / downscale_factor;
#ifdef OPS
    incrementFlopCount(0, 0, 0, 2, 0, 0, 0, 0, false);
#endif
    uint8_t *greyData = (uint8_t *) malloc(gsImageWidth * gsImageHeight);
#ifdef IDX_OPS
    incrementFlopCount(0, 1, 0, 0, 0, 0, 0, 0, false);
#endif

    for (int row_i = 0; row_i < gsImageHeight; row_i++) {
#ifdef IDX_OPS
        incrementFlopCount(1, 0, 0, 0, 0, 0, 0, 0, false);
#endif
        for (int col_i = 0; col_i < gsImageWidth; col_i++) {
#ifdef IDX_OPS
            incrementFlopCount(1, 0, 1, 0, 0, 0, 0, 0, false);
#endif
            if (col_i == gsImageWidth - 1) {
#ifdef IDX_OPS
                incrementFlopCount(0, 0, 1, 0, 0, 0, 0, 0, false);
#endif
                greyData[m_i(gsImageWidth, row_i, col_i)] = greyData[m_i(gsImageWidth, row_i, col_i - 1)];
            } else if (row_i == gsImageHeight - 1) {
#ifdef IDX_OPS
                incrementFlopCount(0, 0, 2, 0, 0, 0, 0, 0, false);
#endif
                greyData[m_i(gsImageWidth, row_i, col_i)] = greyData[m_i(gsImageWidth, row_i - 1, col_i)];
            } else {
#ifdef IDX_OPS
                incrementFlopCount(4, 8, 0, 0, 0, 0, 0, 0, false);
#endif
                greyData[m_i(gsImageWidth, row_i, col_i)] =
                        (oldGsImg[m_i(oldGsImageWidth, row_i * downscale_factor, col_i * downscale_factor)] +
                         oldGsImg[m_i(oldGsImageWidth, (row_i + 1) * downscale_factor, col_i * downscale_factor)] +
                         oldGsImg[m_i(oldGsImageWidth, row_i * downscale_factor, (col_i + 1) * downscale_factor)] +
                         oldGsImg[m_i(oldGsImageWidth, (row_i + 1) * downscale_factor,
                                      (col_i + 1) * downscale_factor)]) / 4;
#ifdef OPS
                incrementFlopCount(3, 0, 0, 1, 0, 0, 0, 0, false);
#endif
            }
        }
    }
    return greyData;
}