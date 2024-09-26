#include <stdlib.h>
#include <iostream>

#include "ImageType.h"
#include "ImageProcessing.h"
#include "Histogram.h"

#define MAX_PATH_LENGTH 256

using namespace std;

int main() {
    Histogram hist;

    hist.SetHistData(256, 10);
    hist.SetHistData(128, 3);
    hist.SetHistData(32, 1);
    hist.SetHistData(8, 20);
    hist.SetHistData(2, 5);

    hist.DisplayHistogram(hist);

    return 0;
}