#include <cstddef>
#include <iostream>
#include <stdlib.h>
#include "ImageProcessing.h"

using namespace std;

ImageProcessing::ImageProcessing() {  }

ImageProcessing::~ImageProcessing() {  }

// Private Methods
// Get the histogram from an image and save it to a pgm file
void ImageProcessing::GetHist(char *fname, ImageType& image, Histogram &hist) {
    int N, M, Q, val;
    image.GetImageInfo(N, M, Q);

    // Find the occurance of each pixel value and update the Histogram
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            image.GetPixelVal(i, j, val);

            hist.SetHistData(val);
        }
    }

    hist.SaveHistImg(fname);
}

// Public Methods
// Change the spatial resolution of an image by sub-sampling by a defined factor
int ImageProcessing::Sample(int factor, ImageType& image) {
    int N, M, Q, val;               // Rows, Colummns, Gery Level, and pixel value variables
    unsigned char *charImage;       // Array to hold the pixel values of the image

    image.GetImageInfo(N, M, Q);
    int subN = N / factor;
    int subM = M / factor;

    // Allocate memory for the subsample image array
    // if it failed to allocate memory, it will return error code 1
    charImage = (unsigned char *)new unsigned char [subN * subM];
    if(charImage == NULL) {
        cerr << "Failed to allocate memory for sub-sampled char image\n";
        return 1;
    }

    // Subsample the image
    for (int i = 0; i < subN; i++) {
        for (int j = 0; j < subM; j++) {
            image.GetPixelVal(i * factor, j * factor, val);
            charImage[i * subM + j] = (unsigned char)val;
        }
    }

    // Resize the image back to the original size using bilinear interoplation
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            // Scale the destination coordinates back to the original image
            float orgX = i / factor;
            float orgY = j / factor;

            // Get integer coordinates for the top-left corner
            int topLeftX = static_cast<int>(orgX);
            int topLeftY = static_cast<int>(orgY);

            // Make sure that the image stays within its bounds
            int bottomRightX = (topLeftX + 1 < subM) ? topLeftX + 1 : topLeftX;
            int bottomRightY = (topLeftY + 1 < subM) ? topLeftY + 1 : topLeftY;

            // Get the pixel values from the four neighboring points
            int topLeft = static_cast<int>(charImage[topLeftX * subM + topLeftY]);              // a
            int topRight = static_cast<int>(charImage[topLeftX * subM + bottomRightY]);         // b
            int bottomLeft = static_cast<int>(charImage[bottomRightX * subM + topLeftY]);       // c
            int bottomRight = static_cast<int>(charImage[bottomRightX * subM + bottomRightY]);  // d

            // Calculate how far the current point is within the grid cell
            float fractionalX = orgX - topLeftX;
            float fractionalY = orgY - topLeftY;

            // Calculate the bilinear interpolation of the image using the equation
            // I(x, y) = ax + by + cxy + d
            val = 
                topLeft * fractionalX +                     // ax
                topRight * fractionalY +                    // by
                bottomLeft * fractionalX * fractionalY +    // cxy
                bottomRight;                                // d

            // Set the interpolated pixel value in the resized image
            image.SetPixelVal(i, j, val);
        }
    }

    delete[] charImage;

    return 0;
}

// Change the grey levels in an image
int ImageProcessing::Quantization(int levels, ImageType& image) {
    int N, M, Q, val, Qx;               // Rows, Colummns, Gery Level, pixel value, and the Qx quantized intensity value
    image.GetImageInfo(N, M, Q);
    float delta = Q / (levels);         // The size of each quantization interval.

    // Loop through each pixel value of a given image
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            image.GetPixelVal(i, j, val);

            // Apply the Quantization formula to get the new intensity value
            Qx = static_cast<int>(val / delta) * delta;

            image.SetPixelVal(i, j, Qx);
        }
    }

    return 0;
}

// Preform Histogram Equalization on a given image
int ImageProcessing::HisEqualization(ImageType& image) {
    Histogram hist;                     // The Image Histogram
    int N, M, Q, val;                   // Rows, Columns, Grey Levels, and Pixel Values
    image.GetImageInfo(N, M, Q);

    float* pdf = (float*)malloc(Q+1 * sizeof(float));
    float* s = (float*)malloc(Q+1 * sizeof(float));
    float* cdf = (float*)malloc(Q+1 * sizeof(float));

    this->GetHist((char*)"/Users/lucasdavis/Code/ImageProcessing/bld/img/histogram.pgm", image, hist);

    for (int i = 0; i < Q; i++) {
        pdf[i] = float(hist.GetHistData(i)) / float(M*N);
    }

    // Compute the CDF (Cumulative Distribution Function)
    cdf[0] = pdf[0];
    for (int i = 1; i <= Q; i++) {
        cdf[i] = cdf[i - 1] + pdf[i];
    }

    // Scale the CDF values to the range [0, Q-1]
    for (int i = 0; i <= Q; i++) {
        s[i] = round((Q - 1) * cdf[i]);
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            image.GetPixelVal(i, j, val);
            image.SetPixelVal(i, j, s[val]);
        }
    }

    hist.Clear();

    this->GetHist((char*)"/Users/lucasdavis/Code/ImageProcessing/bld/img/equalizedHist.pgm", image, hist);    

    delete[] pdf;
    delete[] s;
    delete[] cdf;
    return 0;
}

int ImageProcessing::HisSpecification(ImageType& image) {
    return 0;
}