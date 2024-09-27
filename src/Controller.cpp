#include <stdlib.h>
#include <iostream>

#include "ImageType.h"
#include "ImageProcessing.h"
#include "Histogram.h"

using namespace std;

int main() {
    ImageType image;
    ImageProcessing process;

    image.ReadImage();
    process.HisEqualization(image);

    return 0;
}