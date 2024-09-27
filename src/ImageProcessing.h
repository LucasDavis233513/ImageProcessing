#ifndef IMAGE_PROCESSING_H
#define IMAGE_PROCESSING_H

#include "ImageType.h"
#include "Histogram.h"

class ImageProcessing {
    private:
      void GetHist(char[], ImageType&, Histogram&); // Get the histogram from an image and save it to a pgm file
    public:
      ImageProcessing();
      ~ImageProcessing();
      
      int Sample(int, ImageType&);                 // Change the Saptial resolution of an Image
      int Quantization(int, ImageType&);           // Change the grey levels in an image
      int HisEqualization(ImageType&);             // Preform Histogram Equalization on a given image
      int HisSpecification(ImageType&);            // Question 4
};

#endif