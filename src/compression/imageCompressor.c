#include "imageCompressor.h"

unsigned char *
create_codebook(IMG *img, uint8_t *scaled_image, int blockWidth, int blockHeight, int rotation_num, int translation_num,
                int shear_num,
                int scale_num, double **rotations, double **translations, double **shears, double **scalings,
                int downscale_factor) {

#ifdef OPS
    incrementFlopCount(3, 1, 0, 2, 0, 0, 0, 0, false);
#endif
    int h = (img->bmInfoHeader->biHeight / downscale_factor) * (rotation_num + translation_num + shear_num + scale_num);
    int w = img->bmInfoHeader->biWidth / downscale_factor;

    unsigned char *matrix = (unsigned char *) malloc(h * w);
#ifdef IDX_OPS
    incrementFlopCount(0, 1, 0, 0, 0, 0, 0, 0, false);
#endif
    int offset = 0;

    for (int i = 0; i < rotation_num; i++) {
#ifdef IDX_OPS
        incrementFlopCount(1, 0, 0, 0, 0, 0, 0, 0, false);
#endif
        for (int y = 0; y < w; y += blockHeight) {
#ifdef IDX_OPS
            incrementFlopCount(1, 0, 0, 0, 0, 0, 0, 0, false);
#endif
            for (int x = 0; x < w; x += blockWidth) {
#ifdef IDX_OPS
                incrementFlopCount(1, 0, 0, 0, 0, 0, 0, 0, false);
                incrementFlopCount(2, 1, 0, 0, 0, 0, 0, 0, false);
#endif

                uint8_t *transformed = affineTransform(scaled_image + (y * w) + x,
                                                       blockWidth, blockHeight, w,
                                                       rotations[i]);

                for (int y2 = 0; y2 < blockHeight; y2++) {
#ifdef IDX_OPS
                    incrementFlopCount(1, 0, 0, 0, 0, 0, 0, 0, false);
#endif
                    for (int x2 = 0; x2 < blockWidth; x2++) {
#ifdef IDX_OPS
                        incrementFlopCount(1, 0, 0, 0, 0, 0, 0, 0, false);
                        incrementFlopCount(3, 2, 0, 0, 0, 0, 0, 0, false);
#endif
                        matrix[offset + (y2 * w) + x2] = transformed[y2 * blockWidth + x2];
                    }
                }
                offset += blockWidth;
#ifdef OPS
                incrementFlopCount(1, 0, 0, 0, 0, 0, 0, 0, false);
#endif
            }
            offset += (blockHeight - 1) * (w);
#ifdef OPS
            incrementFlopCount(1, 1, 1, 0, 0, 0, 0, 0, false);
#endif
        }
    }

    for (int i = 0; i < translation_num; i++) {
#ifdef IDX_OPS
        incrementFlopCount(1, 0, 0, 0, 0, 0, 0, 0, false);
#endif
        for (int y = 0; y < w; y += blockHeight) {
#ifdef IDX_OPS
            incrementFlopCount(1, 0, 0, 0, 0, 0, 0, 0, false);
#endif
            for (int x = 0; x < w; x += blockWidth) {
#ifdef IDX_OPS
                incrementFlopCount(1, 0, 0, 0, 0, 0, 0, 0, false);
                incrementFlopCount(2, 1, 0, 0, 0, 0, 0, 0, false);
#endif
                uint8_t *transformed = affineTransform(scaled_image + (y * w) + x,
                                                       blockWidth, blockHeight, w,
                                                       translations[i]);

                for (int y2 = 0; y2 < blockHeight; y2++) {
#ifdef IDX_OPS
                    incrementFlopCount(1, 0, 0, 0, 0, 0, 0, 0, false);
#endif
                    for (int x2 = 0; x2 < blockWidth; x2++) {
#ifdef IDX_OPS
                        incrementFlopCount(1, 0, 0, 0, 0, 0, 0, 0, false);
                        incrementFlopCount(3, 2, 0, 0, 0, 0, 0, 0, false);
#endif
                        matrix[offset + (y2 * w) + x2] = transformed[y2 * blockWidth + x2];
                    }
                }
                offset += blockWidth;
#ifdef OPS
                incrementFlopCount(1, 0, 0, 0, 0, 0, 0, 0, false);
#endif
            }
            offset += (blockHeight - 1) * (w);
#ifdef OPS
            incrementFlopCount(1, 1, 1, 0, 0, 0, 0, 0, false);
#endif
        }
    }

    for (int i = 0; i < shear_num; i++) {
#ifdef IDX_OPS
        incrementFlopCount(1, 0, 0, 0, 0, 0, 0, 0, false);
#endif
        for (int y = 0; y < w; y += blockHeight) {
#ifdef IDX_OPS
            incrementFlopCount(1, 0, 0, 0, 0, 0, 0, 0, false);
#endif
            for (int x = 0; x < w; x += blockWidth) {
#ifdef IDX_OPS
                incrementFlopCount(1, 0, 0, 0, 0, 0, 0, 0, false);
                incrementFlopCount(2, 1, 0, 0, 0, 0, 0, 0, false);
#endif

                uint8_t *transformed = affineTransform(scaled_image + (y * w) + x,
                                                       blockWidth, blockHeight, w,
                                                       shears[i]);

                for (int y2 = 0; y2 < blockHeight; y2++) {
#ifdef IDX_OPS
                    incrementFlopCount(1, 0, 0, 0, 0, 0, 0, 0, false);
#endif
                    for (int x2 = 0; x2 < blockWidth; x2++) {
#ifdef IDX_OPS
                        incrementFlopCount(1, 0, 0, 0, 0, 0, 0, 0, false);
                        incrementFlopCount(3, 2, 0, 0, 0, 0, 0, 0, false);
#endif
                        matrix[offset + (y2 * w) + x2] = transformed[y2 * blockWidth + x2];
                    }
                }
                offset += blockWidth;
#ifdef OPS
                incrementFlopCount(1, 0, 0, 0, 0, 0, 0, 0, false);
#endif
            }
            offset += (blockHeight - 1) * (w);
#ifdef OPS
            incrementFlopCount(1, 1, 1, 0, 0, 0, 0, 0, false);
#endif
        }
    }

    for (int i = 0; i < scale_num; i++) {
#ifdef IDX_OPS
        incrementFlopCount(1, 0, 0, 0, 0, 0, 0, 0, false);
#endif
        for (int y = 0; y < w; y += blockHeight) {
#ifdef IDX_OPS
            incrementFlopCount(1, 0, 0, 0, 0, 0, 0, 0, false);
#endif
            for (int x = 0; x < w; x += blockWidth) {
#ifdef IDX_OPS
                incrementFlopCount(1, 0, 0, 0, 0, 0, 0, 0, false);
                incrementFlopCount(2, 1, 0, 0, 0, 0, 0, 0, false);
#endif

                uint8_t *transformed = affineTransform(scaled_image + (y * w) + x,
                                                       blockWidth, blockHeight, w,
                                                       scalings[i]);

                for (int y2 = 0; y2 < blockHeight; y2++) {
#ifdef IDX_OPS
                    incrementFlopCount(1, 0, 0, 0, 0, 0, 0, 0, false);
#endif
                    for (int x2 = 0; x2 < blockWidth; x2++) {
#ifdef IDX_OPS
                        incrementFlopCount(1, 0, 0, 0, 0, 0, 0, 0, false);
                        incrementFlopCount(3, 2, 0, 0, 0, 0, 0, 0, false);
#endif
                        matrix[offset + (y2 * w) + x2] = transformed[y2 * blockWidth + x2];
                    }
                }
                offset += blockWidth;
#ifdef OPS
                incrementFlopCount(1, 0, 0, 0, 0, 0, 0, 0, false);
#endif
            }
            offset += (blockHeight - 1) * (w);
#ifdef IDX_OPS
            incrementFlopCount(1, 1, 1, 0, 0, 0, 0, 0, false);
#endif
        }
    }

    //DEBUG PRINT OF CODEBOOK
    /*
   IMG *local_codebook;
   local_codebook = copyImg(img);
   local_codebook->greyData = matrix;
   local_codebook->bmInfoHeader->biHeight = h;
   local_codebook->bmInfoHeader->biWidth = w;
   saveGreyImage(local_codebook, "codebook.bmp");*/


    return matrix;
}


double **
create_transformationMatrixList(int rotation_num, int translation_num, int shear_num, int scale_num,
                                double **rotations, double **translations, double **shears, double **scalings) {

    double **transformMatrixList = (double **) malloc(
            sizeof(double *) * (rotation_num + translation_num + shear_num + scale_num));

    int offset = 0;

    for (int i = 0; i < rotation_num; i++) {
        transformMatrixList[offset + i] = rotations[i];
    }
    offset += rotation_num;
    for (int i = 0; i < translation_num; i++) {
        transformMatrixList[offset + i] = translations[i];
    }
    offset += translation_num;
    for (int i = 0; i < shear_num; i++) {
        transformMatrixList[offset + i] = shears[i];
    }
    offset += shear_num;
    for (int i = 0; i < scale_num; i++) {
        transformMatrixList[offset + i] = scalings[i];
    }
    return transformMatrixList;
}


rdt_tuple *
add_block_mapping(int *capacity_transformMapping, rdt_tuple *transformMapping, int *last_position_mapping, int x_range,
                  int y_range, int x_domain, int y_domain, int chosen_transformation, unsigned int quadtreeDepth) {

    rdt_tuple local_tuple;
    local_tuple.domain_x = x_domain;
    local_tuple.domain_y = y_domain;
    local_tuple.range_x = x_range;
    local_tuple.range_y = y_range;
    local_tuple.transformMatrixIndex = chosen_transformation;
    local_tuple.quadtreeDepth = quadtreeDepth;

    if (*last_position_mapping < *capacity_transformMapping) {
        transformMapping[*last_position_mapping] = local_tuple;
        *last_position_mapping = *last_position_mapping + 1;
#ifdef IDX_OPS
        incrementFlopCount(1, 0, 0, 0, 0, 0, 0, 0, false);
#endif
    } else {
        *capacity_transformMapping = *capacity_transformMapping * 2;
        transformMapping = realloc(transformMapping, *capacity_transformMapping * sizeof(rdt_tuple));
        transformMapping[*last_position_mapping] = local_tuple;
        *last_position_mapping = *last_position_mapping + 1;
#ifdef IDX_OPS
        incrementFlopCount(1, 2, 0, 0, 0, 0, 0, 0, false);
#endif
    }

    return transformMapping;
}


rdt_tuple *
quadTree(int x, int y, int rangeblockHeight, int rangeblockWidth, IMG *img, uint8_t *codebook, int codebook_height,
         int codebook_width, unsigned int threshold_similarity, int *capacity_transformMapping,
         rdt_tuple *transformMapping,
         int *last_position_mapping, int quadtreeDepth, int downscale_factor) {

    double lowest_sim = INT32_MAX;
    int x_block_sim = 0;
    int y_block_sim = 0;
    for (int y_code = 0; y_code < codebook_height; y_code += rangeblockHeight) {
#ifdef IDX_OPS
        incrementFlopCount(1, 0, 0, 0, 0, 0, 0, 0, false);
#endif
        for (int x_code = 0; x_code < codebook_width; x_code += rangeblockWidth) {
#ifdef IDX_OPS
            incrementFlopCount(1, 0, 0, 0, 0, 0, 0, 0, false);
            incrementFlopCount(4, 2, 0, 0, 0, 0, 0, 0, false);
#endif
            unsigned int local_sim = euclidean_distance((img->greyData + (y * img->bmInfoHeader->biWidth) + x),
                                                        (codebook + ((y_code) * codebook_width) + x_code),
                                                        rangeblockHeight,
                                                        rangeblockWidth, img->bmInfoHeader->biWidth, downscale_factor);

            if (local_sim < lowest_sim) {
                lowest_sim = local_sim;
                x_block_sim = x_code;
                y_block_sim = y_code;
                DEBUG_PRINT(("Similarity for %d %d: %f \n", y, x, lowest_sim));
            }
        }
    }

#ifdef OPS
    incrementFlopCount(0, 0, 0, 4, 0, 0, 0, 0, false);
#endif
    int chosen_transformation = y_block_sim / (img->bmInfoHeader->biHeight / downscale_factor);
    int y_domain = y_block_sim % (img->bmInfoHeader->biHeight / downscale_factor);


    if (lowest_sim > threshold_similarity && (rangeblockHeight != 8 || rangeblockWidth != 8)) {
#ifdef IDX_OPS
        incrementFlopCount(8, 0, 0, 12, 0, 0, 0, 0, false);
#endif

        transformMapping = quadTree(x, y, rangeblockHeight / 2, rangeblockWidth / 2, img, codebook, codebook_height,
                                    codebook_width, threshold_similarity, capacity_transformMapping, transformMapping,
                                    last_position_mapping, quadtreeDepth + 1, downscale_factor);

        transformMapping = quadTree(x + rangeblockWidth / 2, y, rangeblockHeight / 2, rangeblockWidth / 2, img,
                                    codebook, codebook_height,
                                    codebook_width, threshold_similarity, capacity_transformMapping, transformMapping,
                                    last_position_mapping, quadtreeDepth + 1, downscale_factor);

        transformMapping = quadTree(x, y + rangeblockHeight / 2, rangeblockHeight / 2, rangeblockWidth / 2, img,
                                    codebook, codebook_height,
                                    codebook_width, threshold_similarity, capacity_transformMapping, transformMapping,
                                    last_position_mapping, quadtreeDepth + 1, downscale_factor);

        transformMapping = quadTree(x + rangeblockWidth / 2, y + rangeblockHeight / 2, rangeblockHeight / 2,
                                    rangeblockWidth / 2, img,
                                    codebook, codebook_height,
                                    codebook_width, threshold_similarity, capacity_transformMapping, transformMapping,
                                    last_position_mapping, quadtreeDepth + 1, downscale_factor);

    } else {
        transformMapping = add_block_mapping(capacity_transformMapping, transformMapping,
                                             last_position_mapping, x, y, x_block_sim, y_domain,
                                             chosen_transformation, quadtreeDepth);
    }

    return transformMapping;
}

compressed_img *
compress(IMG *img, int rangeblockWidth, int rangeblockHeight, int rotation_num, int translation_num, int shear_num,
         int scale_num, unsigned int threshold_similarity, int downscale_factor) {

    double **rotations = create_rotations(0, 360, 2, img->bmInfoHeader->biWidth, img->bmInfoHeader->biHeight);
    double **translations = create_translations(0, 50, 1, false);
    double **shears = create_shears(0, 50, 0.1, false);
    double **scalings = create_scalings(1, 28, 1.5, false);

    int capacity_transformMapping =
            (img->bmInfoHeader->biHeight / rangeblockHeight) * (img->bmInfoHeader->biWidth / rangeblockWidth);
    int last_position_mapping = 0;
    rdt_tuple *transformMapping = (rdt_tuple *) malloc(sizeof(rdt_tuple) * capacity_transformMapping);
    double **transformationMatrixList = create_transformationMatrixList(rotation_num, translation_num, shear_num,
                                                                        scale_num, rotations, translations, shears,
                                                                        scalings);
    int codebook_height =
            (img->bmInfoHeader->biHeight / downscale_factor) * (rotation_num + translation_num + shear_num + scale_num);
    int codebook_width = img->bmInfoHeader->biWidth / downscale_factor;

#ifdef COMPRESS_CYCLES
    initializeBenchmark("compress");
    myInt64 start_time = startBenchmark();
#endif
    uint8_t *scaled_image = linearMatrixCompressor(img->greyData, img->bmInfoHeader->biWidth,
                                                   img->bmInfoHeader->biHeight, downscale_factor);
    uint8_t *codebook = create_codebook(img, scaled_image, rangeblockWidth, rangeblockHeight, rotation_num,
                                        translation_num,
                                        shear_num, scale_num, rotations, translations, shears, scalings,
                                        downscale_factor);

    for (int y = 0; y < img->bmInfoHeader->biHeight; y += rangeblockHeight) {
#ifdef IDX_OPS
        incrementFlopCount(1, 0, 0, 0, 0, 0, 0, 0, false);
#endif
        for (int x = 0; x < img->bmInfoHeader->biWidth; x += rangeblockWidth) {
#ifdef IDX_OPS
            incrementFlopCount(1, 0, 0, 0, 0, 0, 0, 0, false);
#endif
            transformMapping = quadTree(x, y, rangeblockHeight, rangeblockWidth, img, codebook, codebook_height,
                                        codebook_width, threshold_similarity, &capacity_transformMapping,
                                        transformMapping,
                                        &last_position_mapping, 0, downscale_factor);

        }
    }

#ifdef COMPRESS_CYCLES
    stopBenchmark(start_time, "compress");
#endif

    free(codebook);
    compressed_img *result = (compressed_img *) malloc(sizeof(compressed_img));

    result->domainHeight = img->bmInfoHeader->biHeight / downscale_factor;
    result->domainWidth = img->bmInfoHeader->biWidth / downscale_factor;
    result->rangeHeight = img->bmInfoHeader->biHeight;
    result->rangeWidth = img->bmInfoHeader->biWidth;
    result->domainBlockHeight = rangeblockHeight;
    result->domainBlockWidth = rangeblockWidth;
    result->numTransformMatrix = rotation_num + translation_num + shear_num + scale_num;
    result->numRangeBlock = last_position_mapping;
    result->transformMatrixList = transformationMatrixList;
    result->transformMapping = transformMapping;
    result->domain = scaled_image;

    DEBUG_PRINT(("Original total blocks are %ld\n", (img->bmInfoHeader->biHeight / rangeblockHeight) *
                                                    (img->bmInfoHeader->biWidth / rangeblockWidth)));
    DEBUG_PRINT(("Total blocks added is %d\n", last_position_mapping));

    return result;
}

IMG *create_greyscale_image(unsigned int height, unsigned int width) {
#ifdef CREATE_GRAYSCALE_IMG
    initializeBenchmark("create_greyscale_image");
    myInt64 start_time = startBenchmark();
#endif

    IMG *image = (IMG *) malloc(sizeof(IMG));
    image->headersize = 0;
    image->channels = 1;
    image->rgbData = (RGB *) malloc(sizeof(RGB));

    image->greyData = (unsigned char *) malloc(sizeof(unsigned char *) * width * height);

    image->palette = (CLP *) malloc(sizeof(CLP) * 256);
    for (int palette_i = 0; palette_i < 256; palette_i++) {
        image->palette[palette_i].r = palette_i;
        image->palette[palette_i].g = palette_i;
        image->palette[palette_i].b = palette_i;
        image->palette[palette_i].padding = '\0';
    }


    image->bmFileHeader = (BITMAPFILEHEADER *) malloc(sizeof(BITMAPFILEHEADER));
    image->bmFileHeader->bfType[0] = 'B';
    image->bmFileHeader->bfType[1] = 'M';
    image->bmFileHeader->bfType[2] = '\0';
    image->bmFileHeader->bfSize = 1118 + (width * height);
    image->bmFileHeader->bfReserved1 = 0;
    image->bmFileHeader->bfReserved2 = 0;
    image->bmFileHeader->bfOffBits = 1078;

    image->bmInfoHeader = (BITMAPINFOHEADER *) malloc(sizeof(BITMAPINFOHEADER));
    image->bmInfoHeader->biSize = 40;
    image->bmInfoHeader->biWidth = width;
    image->bmInfoHeader->biHeight = height;
    image->bmInfoHeader->biPlanes = 1;
    image->bmInfoHeader->biBitCount = 8;
    image->bmInfoHeader->biCompression = 0;
    image->bmInfoHeader->biSizeImage = 0;
    image->bmInfoHeader->biXPixPerMeter = 100;
    image->bmInfoHeader->biYPixPerMeter = 100;
    image->bmInfoHeader->biClrUsed = 0;
    image->bmInfoHeader->biClrImportant = 0;

#ifdef CREATE_GRAYSCALE_IMG
    stopBenchmark(start_time, "create_greyscale_image");
#endif

    return image;
}


IMG *decompress(compressed_img *c_img) {
#ifdef DECOMPRESS
    int fpadds = c_img->numRangeBlock * (6*width*height);
    int fpmults = c_img->numRangeBlock * (6*width*height + 1);
    int fpsub = c_img->numRangeBlock * (2*width*height);
    initializeBenchmark("decompress");
    incrementFlopCount(0, 0, 0, 0, fpadds, fpmults, fpsub, 0, false);
    myInt64 start_time = startBenchmark();
#endif

    IMG *image = create_greyscale_image(c_img->rangeHeight, c_img->rangeWidth);

    unsigned int img_x, img_y;
    unsigned int domain_x, domain_y;
    unsigned int transform_matrix_i, quadtree_depth;
    unsigned int domain_width = c_img->domainWidth;
    unsigned int domain_height = c_img->domainHeight;
    unsigned int domain_block_width = c_img->domainBlockWidth;
    unsigned int domain_block_height = c_img->domainBlockHeight;
    unsigned int img_width = c_img->rangeWidth;
    unsigned int img_height = c_img->rangeWidth;

    for (int range_i = 0; range_i < c_img->numRangeBlock; range_i++) {
        img_x = c_img->transformMapping[range_i].range_x;
        img_y = c_img->transformMapping[range_i].range_y;
        domain_x = c_img->transformMapping[range_i].domain_x;
        domain_x -= domain_x % domain_block_width;
        domain_y = c_img->transformMapping[range_i].domain_y;
        domain_y -= domain_y % domain_block_height;

        quadtree_depth = pow(2, c_img->transformMapping[range_i].quadtreeDepth);
        transform_matrix_i = c_img->transformMapping[range_i].transformMatrixIndex;

        uint8_t *img_block = affineTransform(&c_img->domain[domain_y * domain_width + domain_x],
                                             domain_block_height, domain_block_width, domain_width,
                                             c_img->transformMatrixList[transform_matrix_i]);

        for (int y_offset = 0; y_offset < domain_block_height / quadtree_depth; y_offset++) {
            memcpy(&image->greyData[(img_y + y_offset) * img_width + img_x],
                   &img_block[
                           (y_offset + (c_img->transformMapping[range_i].domain_y % domain_block_height)) *
                           domain_block_width +
                           (c_img->transformMapping[range_i].domain_x % domain_block_width)
                   ],
                   UINT8_SIZE * domain_block_width / quadtree_depth);
        }

    }
#ifdef DECOMPRESS
    stopBenchmark(start_time, "decompress");
#endif

    return image;
}

void setFileParameter(char *img_name, char *benchmark_file) {
    setParameters(img_name, benchmark_file);
}