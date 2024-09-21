#ifndef MANIPULATE_IMAGE_H
#define MANIPULATE_IMAGE_H

#include "ImageType.h"

class ManipulateImage {
    public:
      ManipulateImage();
      ~ManipulateImage();
      int WriteImage(char[], ImageType&);     // Write an image to a file
      int ReadImage(char[], ImageType&);      // Read an image from a file
};

#endif MANIPULATE_IMAGE_H