#include <stdlib.h>
#include <iostream>

#include "ImageType.h"
#include "ImageProcessing.h"

using namespace std;

int main() {
    ImageType image;
    ImageProcessing process;
    char imageName[53] = "/home/ldavis/Code/ImageProcessing/bld/img/aerial.pgm";
    char imageName2[54] = "/home/ldavis/Code/ImageProcessing/bld/img/aerial2.pgm";
    int M, N, Q, val, result;

    printf("Reading Image\n");
    result = process.ReadImage(imageName, image);

    if (result != 0) {
        printf("Error Code: %d", result);
        return 1;
    }

    image.GetImageInfo(N, M, Q);

    printf("The header info is: %d x %d, %d\n", N, M, Q);
    
    printf("\nWriting Image\n");
    process.WriteImage(imageName2, image);

    return 0;
}