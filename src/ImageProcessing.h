#ifndef IMAGE_PROCESSING_H
#define IMAGE_PROCESSING_H

#include <cstddef>
#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <vector>

#include "ImageType.h"
#include "Histogram.h"

using namespace std;

class ImageProcessing {
    private:
      void GetHist(ImageType&, Histogram&); // Get the histogram from an image and save it to a pgm file

      double ComputeMagnitude(float, float);       // Compute the magnitude of the real and imaginary parts of
                                                   // the fourier transform

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
      int PadArray(float*, float*, int N);
      int ConvertImgToFloat(ImageType&, float**, float**);
      int ConvertFloatToImg(ImageType&, float**, bool);

      int GenerateCosineWave(float*, int, double);
      int ShiftFrequencyToCenter(float*, int);

      int NormalizeFFT(float*, int);     // Normalize the forward FFT
      float** NormalizeMagnitude(int, int, float**, float**);
      int fft1D(float*, unsigned long, int);       // Preform the Fast Fourier Transformation on a 1D object (code provided by the teacher)
      int fft2D(int, int, float**, float**, int);    // Preform the Fast Fourier Transformation on an Image

      // Assignment 4
      int BandFilter(int, int, int, int, int, float**, float**, bool);
      int NotchFilter(int, int, int, int, int, float**, float**, bool);
      int freqConv(int, int, float**, float**, float**, float**);
      int HomomorphicFilter(int, int, float**, float**, double, double, double, double);
};

#endif