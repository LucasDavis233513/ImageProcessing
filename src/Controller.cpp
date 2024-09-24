#include <stdlib.h>
#include <iostream>

#include "ImageType.h"
#include "ImageProcessing.h"

#define MAX_PATH_LENGTH 256

using namespace std;

// Asks the user for the path of the image
char* FindImage() {
    // Alocate memory for the imageName
    char* imageName = (char*)malloc(MAX_PATH_LENGTH * sizeof(char));

    // Exit if we failed to allocate memory for the image path
    if (imageName == NULL) {
        cerr << "Failed to alocate memory for the image path\n";
        exit(1);
    }

    printf("Path of the image and its name: ");
    cin >> imageName;

    return imageName;
}

int main() {
    ImageType image;
    ImageProcessing process;
    int result, factor;

    printf("Reading Image\n");
    result = process.ReadImage(FindImage(), image);

    if (result != 0) {
        printf("Error Code: %d", result);
        return 1;
    }

    printf("\nBy what factor do you want to sample the image? ");
    cin >> factor;

    process.Sample(factor, image);
    
    printf("\nWriting Image\n");
    process.WriteImage(FindImage(), image);

    return 0;
}