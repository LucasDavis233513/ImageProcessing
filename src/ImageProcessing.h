#ifndef IMAGE_PROCESSING_H
#define IMAGE_PROCESSING_H

#include "ImageType.h"

class ImageProcessing {
    public:
      ImageProcessing();
      ~ImageProcessing();
      int WriteImage(char[], ImageType&);     // Write an image to a file
      int ReadImage(char[], ImageType&);      // Read an image from a file
};

#endif IMAGE_PROCESSING_H