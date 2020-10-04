#include "compression/imageCompressor.h"

int main(int argc, char *argv[])
{
    IMG* img = readImage(argv[1]);
    setFileParameter(argv[1], argv[3]);

    printf("------------------------- READ IMAGE -------------------------\n");
    printImageInfo(img, false);

    IMG* greyscale = rgbToGreyscale(img);
    printf("------------------------- CONVERTED IMAGE -------------------------\n");
    printImageInfo(greyscale, false);

    int downscale_factor = 4;
    compressed_img* comp = compress(greyscale, 32, 32,4, 4, 4, 4, 2000, downscale_factor);

    encode(*comp);

    IMG* decompressed_image = decompress(comp);
    saveGreyImage(decompressed_image, argv[2]);

    printf("------------------------- SAVED IMAGE -------------------------\n");
    printImageInfo(decompressed_image, false);

    freeImage(img);
    freeImage(greyscale);

    return 0;
}