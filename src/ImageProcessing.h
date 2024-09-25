#ifndef IMAGE_PROCESSING_H
#define IMAGE_PROCESSING_H

#include "ImageType.h"

class ImageProcessing {
    public:
      ImageProcessing();
      ~ImageProcessing();

      int WriteImage(char[], ImageType&);     // Write an image to a file
      int ReadImage(char[], ImageType&);      // Read an image from a file
      
      // Assignment 1
      int Sample(int, ImageType&);            // Question 1
      int Quantization(ImageType&);           // Quesiton 2
};

#endif