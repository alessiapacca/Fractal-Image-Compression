#include "imageIO.h"

/**
 * Print image information
 **/
void printImageInfo(IMG* img, bool detailed){
    printf("File type          = %s\n", img->bmFileHeader->bfType);
    printf("File size          = %d bytes\n", img->bmFileHeader->bfSize);
    printf("Data offset        = %d bytes\n", img->bmFileHeader->bfOffBits);
    printf("Info header size   = %d bytes\n", img->bmInfoHeader->biSize);
    printf("Width              = %ld pixels\n", img->bmInfoHeader->biWidth);
    printf("Height             = %ld pixels\n", img->bmInfoHeader->biHeight);
    printf("Planes             = %d\n", img->bmInfoHeader->biPlanes);
    printf("Bit count          = %d bits/pixel\n", img->bmInfoHeader->biBitCount);
    printf("Compression        = %d\n", img->bmInfoHeader->biCompression);
    printf("Size image         = %d bytes\n", img->bmInfoHeader->biSizeImage);
    printf("X pixels per meter = %d\n", img->bmInfoHeader->biXPixPerMeter);
    printf("Y pixels per meter = %d\n", img->bmInfoHeader->biYPixPerMeter);
    printf("Color used         = %d colors\n", img->bmInfoHeader->biClrUsed);

    if (detailed) {

        printf("---- COLOR PALETTE ----------------------------- \n");

        for (int i = 0; i<256; i++) {
            printf("%d %d %d %d \n", img->palette[i].r,img->palette[i].g, img->palette[i].b, img->palette[i].padding);
        }

        printf("---- PIXEL ARRAY  ----------------------------- \n");
        for (int i = 0; i< img->bmInfoHeader->biWidth * img->bmInfoHeader->biHeight * img->channels; i++) {
            if (img->channels == 3) {
                printf("%d ",img->rgbData[i].r);
                printf("%d ",img->rgbData[i].g);
                printf("%d ",img->rgbData[i].b);
                printf(" | ");
            }
            else {
                printf("%d ",img->greyData[i]);
            }
        }
    }

    
}

/**
 * Read a bmp image from file
 **/
IMG* readImage(char* file){
    FILE *fp;
    IMG *img;
    img = (IMG*) malloc(sizeof(IMG));

    if ((fp = fopen(file, "rb")) == NULL) {
        printf("Cannot open file: %s\n\n", file);
        exit(1);
    }

    img->bmFileHeader = readBMFileHeader(fp);
    if (strcmp(img->bmFileHeader->bfType, "BM") != 0) {
        printf("The file is not BITMAP.\n");
        exit(1);
    }
    img->headersize = sizeOfInformationHeader(fp);
    if (img->headersize == 40) {
        img->bmInfoHeader = readBMInfoHeader(fp);
    } else {
        printf("Unsupported BITMAP.\n");
        exit(1);
    }
   
    #ifdef DEBUG
        printImageInfo(img);
    #endif

    if (img->bmFileHeader->bfOffBits == 1078) {
        img->palette = malloc(sizeof(CLP) * 256);
        for (int i = 0; i<256; i++) {
            fread(&img->palette[i].b, 1, 1, fp);
            fread(&img->palette[i].g, 1, 1, fp);
            fread(&img->palette[i].r, 1, 1, fp);
            fread(&img->palette[i].padding, 1, 1, fp);
        }
    }

    unsigned char channels=(img->bmInfoHeader->biBitCount/8);
    img->channels = channels;
    unsigned size = img->bmInfoHeader->biWidth * img->bmInfoHeader->biHeight * channels;
    unsigned char* data = malloc(size);

    unsigned rowSize = img->bmInfoHeader->biWidth*channels;
    unsigned padding = (4 - rowSize%4)%4;
    char dummy[4] = {'\0', '\0', '\0', '\0'};


    for(int i=0; i < img->bmInfoHeader->biHeight; ++i){
        fread(data+(i*rowSize), 1, rowSize, fp);
        fread(&dummy, 1, padding,fp); // useless padding
    }


    img->greyData = (unsigned char*) malloc(sizeof(unsigned char) * img->bmInfoHeader->biWidth * img->bmInfoHeader->biHeight);
    img->rgbData = (RGB*) malloc(sizeof(RGB) * img->bmInfoHeader->biWidth * img->bmInfoHeader->biHeight);
    
    unsigned element=0;
    if(channels == 1){
        for(unsigned i = 0; i < size; i++){
            img->greyData[i] = data[i];
            #ifdef DEBUG
                //printf("%d ",img->greyData[i]);
            #endif
        }
    } else {
        for (unsigned i = 0; i < size; element++)
        {  
            // reverse order blue green red
            img->rgbData[element].b = data[i++]; 
            img->rgbData[element].g = data[i++];
            img->rgbData[element].r = data[i++];

            DEBUG_PRINT(("%d ",img->rgbData[element].b));
            DEBUG_PRINT(("%d ",img->rgbData[element].g));
            DEBUG_PRINT(("%d ",img->rgbData[element].r));

        }
    }
    
    fclose(fp);
    return img;
}

/**
 * Free memory allocated for the image
 **/
void freeImage(IMG* img){
    free(img->bmFileHeader);
    free(img->bmInfoHeader);
    if (img->bmInfoHeader->biBitCount != 8)
        free(img->rgbData);
    free(img->greyData);
    free(img);
}

void saveGreyImage(IMG* img, char* fileName){
    FILE *f = fopen(fileName, "wb");

    DEBUG_PRINT(("write size %d", img->bmFileHeader->bfSize));
    DEBUG_PRINT("Start writing\n");
    DEBUG_PRINT(printImageInfo(img));
    unsigned char bmpfileheader[14] = {'B','M', 0,0,0,0, 0,0, 0,0, 0,0,0,0};
    unsigned char bmpinfoheader[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 8,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0};

    bmpfileheader[2] = (unsigned char) (img->bmFileHeader->bfSize);
    bmpfileheader[3] = (unsigned char) (img->bmFileHeader->bfSize >> 8);
    bmpfileheader[4] = (unsigned char) (img->bmFileHeader->bfSize >> 16);
    bmpfileheader[5] = (unsigned char) (img->bmFileHeader->bfSize >> 24);

    bmpfileheader[10] = (unsigned char) (img->bmFileHeader->bfOffBits);
    bmpfileheader[11] = (unsigned char) (img->bmFileHeader->bfOffBits >> 8);
    bmpfileheader[12] = (unsigned char) (img->bmFileHeader->bfOffBits >> 16);
    bmpfileheader[13] = (unsigned char) (img->bmFileHeader->bfOffBits >> 24);


    //printf("biwidth: %d\n", img->bmInfoHeader->biWidth);
    bmpinfoheader[4] = (unsigned char)(img->bmInfoHeader->biWidth);
    bmpinfoheader[5] = (unsigned char)(img->bmInfoHeader->biWidth >> 8);
    bmpinfoheader[6] = (unsigned char)(img->bmInfoHeader->biWidth >> 16);
    bmpinfoheader[7] = (unsigned char)(img->bmInfoHeader->biWidth >> 24);

    bmpinfoheader[8] = (unsigned char)(img->bmInfoHeader->biHeight);
    bmpinfoheader[9] = (unsigned char)(img->bmInfoHeader->biHeight >> 8);
    bmpinfoheader[10] = (unsigned char)(img->bmInfoHeader->biHeight >> 16);
    bmpinfoheader[11] = (unsigned char)(img->bmInfoHeader->biHeight >> 24);

    bmpinfoheader[24] = (unsigned char)(img->bmInfoHeader->biXPixPerMeter);
    bmpinfoheader[25] = (unsigned char)(img->bmInfoHeader->biXPixPerMeter >> 8);
    bmpinfoheader[26] = (unsigned char)(img->bmInfoHeader->biXPixPerMeter >> 16);
    bmpinfoheader[27] = (unsigned char)(img->bmInfoHeader->biXPixPerMeter >> 24);

    bmpinfoheader[28] = (unsigned char)(img->bmInfoHeader->biYPixPerMeter);
    bmpinfoheader[29] = (unsigned char)(img->bmInfoHeader->biYPixPerMeter >> 8);
    bmpinfoheader[30] = (unsigned char)(img->bmInfoHeader->biYPixPerMeter >> 16);
    bmpinfoheader[31] = (unsigned char)(img->bmInfoHeader->biYPixPerMeter >> 24);

    bmpinfoheader[32] = (unsigned char)(img->bmInfoHeader->biClrUsed);
    bmpinfoheader[33] = (unsigned char)(img->bmInfoHeader->biClrUsed >> 8);
    bmpinfoheader[34] = (unsigned char)(img->bmInfoHeader->biClrUsed >> 16);
    bmpinfoheader[35] = (unsigned char)(img->bmInfoHeader->biClrUsed >> 24);

    fwrite(bmpfileheader,1,14,f);
    fwrite(bmpinfoheader,1,40,f);

    if (img->bmFileHeader->bfOffBits == 1078) {
        for (int i = 0; i<256; i++) {
            fwrite(&img->palette[i].b, 1, 1, f);
            fwrite(&img->palette[i].g, 1, 1, f);
            fwrite(&img->palette[i].r, 1, 1, f);
            fwrite(&img->palette[i].padding, 1, 1, f);
        }
    }

    unsigned rowSize = img->bmInfoHeader->biWidth;
    unsigned padding = (4 - rowSize%4)%4;

    unsigned char dummy[4] = {'\0', '\0', '\0', '\0'};
    for (int i = 0;i < img->bmInfoHeader->biHeight; ++i){
        fwrite(&img->greyData[img->bmInfoHeader->biWidth*i], 1, img->bmInfoHeader->biWidth, f);
        fwrite(&dummy, 1, padding, f);
    }

    DEBUG_PRINT("Finish writing\n");
    fclose(f);
}

void saveRGBImage(IMG* img, char* fileName){
    FILE *f = fopen(fileName, "wb");

    //printf("write size %d", img->bmFileHeader->bfSize);
    DEBUG_PRINT("Start writing\n");
    //printImageInfo(img);
    unsigned char bmpfileheader[14] = {'B','M', 0,0,0,0, 0,0, 0,0, 0,0,0,0};
    unsigned char bmpinfoheader[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0};

    bmpfileheader[2] = (unsigned char) (img->bmFileHeader->bfSize);
    bmpfileheader[3] = (unsigned char) (img->bmFileHeader->bfSize >> 8);
    bmpfileheader[4] = (unsigned char) (img->bmFileHeader->bfSize >> 16);
    bmpfileheader[5] = (unsigned char) (img->bmFileHeader->bfSize >> 24);

    bmpfileheader[10] = (unsigned char) (img->bmFileHeader->bfOffBits);
    bmpfileheader[11] = (unsigned char) (img->bmFileHeader->bfOffBits >> 8);
    bmpfileheader[12] = (unsigned char) (img->bmFileHeader->bfOffBits >> 16);
    bmpfileheader[13] = (unsigned char) (img->bmFileHeader->bfOffBits >> 24);


    //printf("biwidth: %d\n", img->bmInfoHeader->biWidth);
    bmpinfoheader[4] = (unsigned char)(img->bmInfoHeader->biWidth);
    bmpinfoheader[5] = (unsigned char)(img->bmInfoHeader->biWidth >> 8);
    bmpinfoheader[6] = (unsigned char)(img->bmInfoHeader->biWidth >> 16);
    bmpinfoheader[7] = (unsigned char)(img->bmInfoHeader->biWidth >> 24);

    bmpinfoheader[8] = (unsigned char)(img->bmInfoHeader->biHeight);
    bmpinfoheader[9] = (unsigned char)(img->bmInfoHeader->biHeight >> 8);
    bmpinfoheader[10] = (unsigned char)(img->bmInfoHeader->biHeight >> 16);
    bmpinfoheader[11] = (unsigned char)(img->bmInfoHeader->biHeight >> 24);

    bmpinfoheader[24] = (unsigned char)(img->bmInfoHeader->biXPixPerMeter);
    bmpinfoheader[25] = (unsigned char)(img->bmInfoHeader->biXPixPerMeter >> 8);
    bmpinfoheader[26] = (unsigned char)(img->bmInfoHeader->biXPixPerMeter >> 16);
    bmpinfoheader[27] = (unsigned char)(img->bmInfoHeader->biXPixPerMeter >> 24);

    bmpinfoheader[28] = (unsigned char)(img->bmInfoHeader->biYPixPerMeter);
    bmpinfoheader[29] = (unsigned char)(img->bmInfoHeader->biYPixPerMeter >> 8);
    bmpinfoheader[30] = (unsigned char)(img->bmInfoHeader->biYPixPerMeter >> 16);
    bmpinfoheader[31] = (unsigned char)(img->bmInfoHeader->biYPixPerMeter >> 24);

    bmpinfoheader[32] = (unsigned char)(img->bmInfoHeader->biClrUsed);
    bmpinfoheader[33] = (unsigned char)(img->bmInfoHeader->biClrUsed >> 8);
    bmpinfoheader[34] = (unsigned char)(img->bmInfoHeader->biClrUsed >> 16);
    bmpinfoheader[35] = (unsigned char)(img->bmInfoHeader->biClrUsed >> 24);

    fwrite(bmpfileheader,1,14,f);
    fwrite(bmpinfoheader,1,40,f);

    unsigned rowSize = img->bmInfoHeader->biWidth*3;
    unsigned padding = (4 - rowSize % 4)%4;

    unsigned dummy;
    for (int i = 0;i < img->bmInfoHeader->biHeight * img->bmInfoHeader->biWidth; ++i){
        fwrite(&img->rgbData[i].b, 1, 1, f);
        fwrite(&img->rgbData[i].g, 1, 1, f);
        fwrite(&img->rgbData[i].r, 1, 1, f);

        if ( (i % img->bmInfoHeader->biWidth == 0) && padding != 0)
            fwrite(&dummy, 1, padding, f);
    }

    DEBUG_PRINT("Finish writing\n");
    fclose(f);
}
