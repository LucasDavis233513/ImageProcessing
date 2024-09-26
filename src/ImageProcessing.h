#ifndef IMAGE_PROCESSING_H
#define IMAGE_PROCESSING_H

#include "ImageType.h"
#include "Histogram.h"

class ImageProcessing {
    public:
      ImageProcessing();
      ~ImageProcessing();
      
      int Sample(int, ImageType&);            // Change the Saptial resolution of an Image
      int Quantization(int, ImageType&);      // Change the grey levels in an image
      int HisEqualization(ImageType&);        // Question 3
      int HisSpecification(ImageType&);       // Question 4
};

#endif