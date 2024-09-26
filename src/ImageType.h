#ifndef IMAGE_TYPE_H
#define IMAGE_TYPE_H

#include <stdlib.h>
#include <iostream>
#include <fstream>

#define MAX_PATH_LENGTH 256

class ImageType {
  private:
    int N, M, Q;
    int **pixelValue;

    char* FindImage();                      // Get the file path to the image
  public:
    ImageType();
    ImageType(int, int, int);
    ImageType(ImageType&);
    ~ImageType();

    void GetImageInfo(int&, int&, int&);
    void SetImageInfo(int, int, int);
    void GetPixelVal(int, int, int&);
    void SetPixelVal(int, int, int);

    int WriteImage();                       // Write an image to a file
    int ReadImage();                        // Read an image from a file
};

#endif
