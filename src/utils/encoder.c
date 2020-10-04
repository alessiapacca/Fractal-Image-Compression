#include "encoder.h"

void* write_uint(void* dest_addr, unsigned int data){
    memcpy(dest_addr, &data, UINT_SIZE);
    return dest_addr+UINT_SIZE;
}

void* read_uint(unsigned int* dest_uint, void* source_addr){
    memcpy(dest_uint, source_addr, UINT_SIZE);
    return source_addr+UINT_SIZE;
}

void* encode(compressed_img c_img){

    size_t headerSize = UINT_SIZE*8;
    size_t domainSize = UINT_SIZE * c_img.domainHeight * c_img.domainWidth;
    size_t codebookSize = UINT_SIZE*c_img.numTransformMatrix*9;
    size_t codebookIndexSize = sizeof(rdt_tuple)*c_img.numRangeBlock;

    size_t encodedImageSize = headerSize + domainSize + codebookSize + codebookIndexSize;
    printf("compressed image size: %d Byte\n", (int)encodedImageSize);
    void* encoded_img_ptr= (void*) malloc(encodedImageSize);

    encoded_img_ptr=write_uint(encoded_img_ptr, c_img.domainHeight);
    encoded_img_ptr=write_uint(encoded_img_ptr, c_img.domainWidth);
    encoded_img_ptr=write_uint(encoded_img_ptr, c_img.rangeHeight);
    encoded_img_ptr=write_uint(encoded_img_ptr, c_img.rangeWidth);
    encoded_img_ptr=write_uint(encoded_img_ptr, c_img.domainBlockHeight);
    encoded_img_ptr=write_uint(encoded_img_ptr, c_img.domainBlockWidth);
    encoded_img_ptr=write_uint(encoded_img_ptr, c_img.numRangeBlock);
    encoded_img_ptr=write_uint(encoded_img_ptr, c_img.numTransformMatrix);

    memcpy(encoded_img_ptr, c_img.transformMapping, codebookIndexSize);
    encoded_img_ptr+=codebookIndexSize;

    for(int matrix_i=0; matrix_i<c_img.numTransformMatrix; matrix_i++){
        memcpy(encoded_img_ptr, c_img.transformMatrixList[matrix_i], DOUBLE_SIZE * 9);
        encoded_img_ptr+= DOUBLE_SIZE * 9;
    }

    memcpy(encoded_img_ptr, c_img.domain, UINT8_SIZE*c_img.domainWidth*c_img.domainHeight);

    return encoded_img_ptr;
}

compressed_img* decode(void* encoded_img){
    compressed_img* c_img = (compressed_img*) malloc(sizeof(compressed_img));
    void* encoded_img_ptr=encoded_img;
    encoded_img_ptr=read_uint(&c_img->domainHeight, encoded_img_ptr);
    encoded_img_ptr=read_uint(&c_img->domainWidth, encoded_img_ptr);
    encoded_img_ptr=read_uint(&c_img->rangeHeight, encoded_img_ptr);
    encoded_img_ptr=read_uint(&c_img->rangeWidth, encoded_img_ptr);
    encoded_img_ptr=read_uint(&c_img->domainBlockHeight, encoded_img_ptr);
    encoded_img_ptr=read_uint(&c_img->domainBlockWidth, encoded_img_ptr);
    encoded_img_ptr=read_uint(&c_img->numRangeBlock, encoded_img_ptr);
    encoded_img_ptr=read_uint(&c_img->numTransformMatrix, encoded_img_ptr);

    c_img->transformMapping = (rdt_tuple*)malloc(sizeof(rdt_tuple)*c_img->numRangeBlock);
    memcpy(c_img->transformMapping, encoded_img_ptr, sizeof(rdt_tuple)*c_img->numRangeBlock);
    encoded_img_ptr+=sizeof(rdt_tuple)*c_img->numRangeBlock;

    c_img->transformMatrixList = (double**)malloc(c_img->numTransformMatrix*sizeof(double*));
    for(int matrix_i=0; matrix_i<c_img->numTransformMatrix; matrix_i++){
        c_img->transformMatrixList[matrix_i] = (double*) malloc(9*DOUBLE_SIZE);
        memcpy(c_img->transformMatrixList[matrix_i], encoded_img_ptr, 9*DOUBLE_SIZE);
        encoded_img_ptr+= 9*DOUBLE_SIZE;
    }

    c_img->domain = (uint8_t*) malloc(UINT8_SIZE*c_img->domainHeight*c_img->domainWidth);
    memcpy(c_img->domain, encoded_img_ptr, UINT8_SIZE*c_img->domainHeight*c_img->domainWidth);

    return c_img;
}

size_t getEncodedImageSize(void* encodedImageData){

    compressed_img* c_img = (compressed_img*) malloc(sizeof(compressed_img));

    void* encoded_img_ptr=encodedImageData;

    encoded_img_ptr=read_uint(&c_img->domainHeight, encoded_img_ptr);
    encoded_img_ptr=read_uint(&c_img->domainWidth, encoded_img_ptr);
    encoded_img_ptr=read_uint(&c_img->rangeHeight, encoded_img_ptr);
    encoded_img_ptr=read_uint(&c_img->rangeWidth, encoded_img_ptr);
    encoded_img_ptr=read_uint(&c_img->domainBlockHeight, encoded_img_ptr);
    encoded_img_ptr=read_uint(&c_img->domainBlockWidth, encoded_img_ptr);
    encoded_img_ptr=read_uint(&c_img->numRangeBlock, encoded_img_ptr);
    encoded_img_ptr=read_uint(&c_img->numTransformMatrix, encoded_img_ptr);

    size_t encodedImgSize = sizeof(uint8_t)*c_img->domainHeight*c_img->domainWidth;

    encodedImgSize += sizeof(double)*9*c_img->numTransformMatrix;
    encodedImgSize += sizeof(rdt_tuple)*c_img->numRangeBlock;

    encodedImgSize += sizeof(unsigned int)*8;

    return encodedImgSize;
}

void writeEncodedImage(char* fileName, void* encondedImageData){
    FILE *fileptr;
    fileptr=fopen(fileName, "wb");
    size_t encodedImageSize = getEncodedImageSize(encondedImageData);
    fwrite(encondedImageData, encodedImageSize, 1, fileptr);
    fclose(fileptr);
}

void* readEncodedImage(char* fileName){
    FILE *fileptr;
    fileptr=fopen(fileName, "rb");
    fseek(fileptr, 0L, SEEK_END);
    rewind(fileptr);
    size_t encodedImageSize = ftell(fileptr);
    void* encodedImageData = (void*)malloc(encodedImageSize);
    fread(encodedImageData, encodedImageSize, 1, fileptr);

    return encodedImageData;
}


/*
float max(float a, float b){
    if(a>=b)
        return a;
    return b;
}

#define m 2112

void calculate3(float *A, float* v, int n){
    int remaining_steps= n%15;
    for(int j_step=0; j_step<m; j_step+=64){
        for(int i_step=0; i_step+15<n; i_step+=15){

            for(int j=0; j<64; j++){
                for(int i=0; i<15; i++){
                    v[i+i_step] = max(v[i+i_step], A[(i+i_step)*m+j_step+j]);
                }
            }

        }
        for(int j=0; j<64; j++){
            for(int i=0; i<remaining_steps; i++){
                v[i+(n-remaining_steps)] = max(v[i+(n-remaining_steps)], A[(i+(n-remaining_steps))*m+j_step+j]);
            }
        }

    }
}*/