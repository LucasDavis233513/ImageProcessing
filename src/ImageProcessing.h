#ifndef IMAGE_PROCESSING_H
#define IMAGE_PROCESSING_H

#include "ImageType.h"
#include "Histogram.h"

#define MAX_PATH_LENGTH 256

class ImageProcessing {
    private:
      char* FindImage();                      // Get the file path to the image
    public:
      ImageProcessing();
      ~ImageProcessing();

      int WriteImage(ImageType&);             // Write an image to a file
      int ReadImage(ImageType&);              // Read an image from a file
      
      // Assignment 1
      int Sample(int, ImageType&);            // Question 1
      int Quantization(int, ImageType&);      // Quesiton 2
      int HisEqualization(ImageType&);        // Question 3
      int HisSpecification(ImageType&);       // Question 4
};

#endif