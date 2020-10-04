#include "bmp.h"


/*
 * Read bitmap file header
 */
BITMAPFILEHEADER *readBMFileHeader(FILE *fp)
{
    BITMAPFILEHEADER *header;
    char           filetype[3] = {'\0', '\0', '\0'};
    unsigned int   filesize;
    unsigned short reserved1;
    unsigned short reserved2;
    unsigned long  offset;

    /* File type (2 bytes) */
    fread(&filetype, 1, 2, fp);

    /* File size (4 bytes) */
    filesize = (unsigned int) readLE4(fp);

    /* Reserved 1 (2 bytes) */
    fread(&reserved1, 2, 1, fp);

    /* Reserved 2 (2 bytes) */
    fread(&reserved2, 2, 1, fp);

    /* Offset (4 bytes) */
    offset = (unsigned long) readLE4(fp);

    header = (BITMAPFILEHEADER *) malloc(sizeof(BITMAPFILEHEADER));
    strcpy(header->bfType, filetype);
    header->bfSize      = filesize;
    header->bfReserved1 = reserved1;
    header->bfReserved2 = reserved2;
    header->bfOffBits   = offset;

    return header;
}

/*
 * Returns size of information header
 */
int sizeOfInformationHeader(FILE *fp)
{
    int headersize=0;
    unsigned char buf[4];
    int i;

    fread(buf, 1, 4, fp);
    for (i = 3; i >= 0; i--) {
        headersize = (headersize << 8) | (int) buf[i];
    }

    fseek(fp, 14, SEEK_SET);

    return headersize;
}

/*
 * Read bitmap info header (Windows bitmap)
 */
BITMAPINFOHEADER *readBMInfoHeader(FILE *fp)
{
    BITMAPINFOHEADER *header;
    unsigned int   headersize;
    int            width;
    int            height;
    unsigned short planes;
    unsigned short bitcount;
    unsigned int   compression;
    unsigned int   size_image;
    int            x_pix_per_meter;
    int            y_pix_per_meter;
    unsigned int   clr_used;
    unsigned int   clr_important;

    /* Header size (4 bytes) */
    headersize = (unsigned int) readLE4(fp);

    /* Width (4 bytes) */
    width = (int) readLE4(fp);

    /* Height (4 bytes) */
    height = (int) readLE4(fp);

    /* Planes (2 bytes) */
    planes = (unsigned short) readLE2(fp);

    /* Bit Count (2 bytes) */
    bitcount = (unsigned short) readLE2(fp);

    /* Compression (4 bytes) */
    compression = (unsigned int) readLE4(fp);

    /* Size image (4 bytes) */
    size_image = (unsigned int) readLE4(fp);

    /* X pix per meter (4 bytes) */
    x_pix_per_meter = (int) readLE4(fp);

    /* Y pix per meter (4 bytes) */
    y_pix_per_meter = (int) readLE4(fp);

    /* Color used (4 bytes) */
    clr_used = (unsigned int) readLE4(fp);

    /* Color important (4 bytes) */
    clr_important = (unsigned int) readLE4(fp);

    header = (BITMAPINFOHEADER *) malloc(sizeof(BITMAPINFOHEADER));
    header->biSize         = headersize;
    header->biWidth        = width;
    header->biHeight       = height;
    header->biPlanes       = planes;
    header->biBitCount     = bitcount;
    header->biCompression  = compression;
    header->biSizeImage    = size_image;
    header->biXPixPerMeter = x_pix_per_meter;
    header->biYPixPerMeter = y_pix_per_meter;
    header->biClrUsed      = clr_used;
    header->biClrImportant = clr_important;

    return header;
}

/*
 * Read 2 bytes in little endian
 */
unsigned short readLE2(FILE *fp)
{
    unsigned char buf[2];
    unsigned short result = 0;
    int i;

    fread(buf, 1, 2, fp);
    for (i = 1; i >= 0; i--) {
        result = (result << 8) | (unsigned short) buf[i];
    }

    return result;
}

/*
 * Read 4 bytes in little endian
 */
unsigned int readLE4(FILE *fp)
{
    unsigned char buf[4];
    unsigned int result = 0;
    int i;

    fread(buf, 1, 4, fp);
    for (i = 3; i >= 0; i--) {
        result = (result << 8) | (unsigned int) buf[i];
    }
    
    return result;
}

