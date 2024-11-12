#ifndef IMAGE_PROCESSING_H
#define IMAGE_PROCESSING_H

#include <cstddef>
#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <algorithm>
#include <cstdlib>
#include <ctime>

#include "ImageType.h"
#include "Histogram.h"

using namespace std;

class ImageProcessing {
    private:
      void GetHist(ImageType&, Histogram&); // Get the histogram from an image and save it to a pgm file
      float clamp(float, float, float);
    public:
      ImageProcessing();
      ~ImageProcessing();

      // Assignment 1
      int Sample(int, ImageType&);                 // Change the Saptial resolution of an Image
      int Quantization(int, ImageType&);           // Change the grey levels in an image
      int HisEqualization(ImageType&);             // Preform Histogram Equalization on a given image

      // Assignment 2
      int Correlation(ImageType&, ImageType&);     // Preform a Correlation filter between an image and a mask
      int Smoothing(ImageType&, float*, int);      // Preform smoothing based on the filter passed to it.
      int Median(ImageType&);                      // Preform median filtering
      int Sharpen(ImageType&, float*, float*);
      int SharpenWithLaplacian(ImageType&);

      float* CreateFilter(string, int);            // Create a smoothing filter
      int SaltandPepperImage(ImageType&, int);     // Randomly changes pixel values to black or white

      // Assignment 3
      int NormalizeFFT(float*, unsigned long);     // Normalize the forward FFT

      int fft1D(float*, unsigned long, int);       // Preform the Fast Fourier Transformation on a 1D object (code provided by the teacher)
      int fft2D(ImageType& image);                 // Preform the Fast Fourier Transformation on an Image
};

#endif