#include <stdlib.h>
#include "ImageType.h"

using namespace std;

// Create an empty image
ImageType::ImageType() {
    this->N = 0;
    this->M = 0;
    this->Q = 0;
    this->pixelValue = NULL;
}

// Create a blank/white image
ImageType::ImageType(int tmpN, int tmpM, int tmpQ) {
    int i, j;
    this->N = tmpN;
    this->M = tmpM;
    this->Q = tmpQ;
    this->pixelValue = new int *[N];

    for (i = 0; i < N; i++) {
        this->pixelValue[i] = new int[M];
        for (j = 0; j < M; j++)
            this->pixelValue[i][j] = 0;
    }
}

// Create a copy of the image
ImageType::ImageType(ImageType &oldImage) {
    int i, j;
    this->N = oldImage.N;
    this->M = oldImage.M;
    this->Q = oldImage.Q;
    this->pixelValue = new int *[N];

    for (i = 0; i < N; i++) {
        this->pixelValue[i] = new int[M];
        for (j = 0; j < M; j++)
            this->pixelValue[i][j] = oldImage.pixelValue[i][j];
    }
}

// Delete the image
ImageType::~ImageType() {
    int i;
    for (i = 0; i < N; i++)
        delete[] this->pixelValue[i];

    delete[] this->pixelValue;
}

void ImageType::GetImageInfo(int &rows, int &cols, int &levels) {
    rows = this->N;
    cols = this->M;
    levels = this->Q;
}

void ImageType::SetImageInfo(int rows, int cols, int levels) {
    this->N = rows;
    this->M = cols;
    this->Q = levels;
}

void ImageType::SetPixelVal(int i, int j, int val) {
    this->pixelValue[i][j] = val;
}

void ImageType::GetPixelVal(int i, int j, int &val) {
    val = this->pixelValue[i][j];
}