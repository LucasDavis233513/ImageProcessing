#include <cstddef>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "ImageProcessing.h"

using namespace std;

ImageProcessing::ImageProcessing() { }

ImageProcessing::~ImageProcessing() { }

// Write an image to a file
int ImageProcessing::WriteImage(char *fname, ImageType &image) {
    int N, M, Q, val;               // Row, Column, Quantization, and pixel value declaration
    unsigned char *charImage;
    ofstream ofp;

    image.GetImageInfo(N, M, Q);
    charImage = (unsigned char *)new unsigned char [M*N];
    if (charImage == NULL) {
        cerr << "Failed to allocate memory for charImage\n";
        return 1;
    }

    // Convert integer values to unsigned char
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            image.GetPixelVal(i, j, val);
            charImage[i*M+j] = (unsigned char)val;
        }
    }

    ofp.open(fname, ios::out | ios::binary);
    if(!ofp) { // Couldn't open the file
        cerr << "Can't Open file " << fname << endl;
        delete[] charImage; // Freeing the allocated memory
        return 2;
    }

    ofp << "P5" << endl;
    ofp << N << " " << M << endl;
    ofp << Q << endl;

    ofp.write(reinterpret_cast<char *>(charImage), (M*N)*sizeof(unsigned char));
    if(ofp.fail()) { // Couldn't write the image
        cerr << "Can't Write Image " << fname << endl;
        delete[] charImage; // Freeing the allocated memory
        return 3;
    }

    ofp.close();
    delete[] charImage; // Freeing the allocated memory
    return 0; // Return 0 on success
}

// Read an image to a file
int ImageProcessing::ReadImage(char *fname, ImageType &image) {
    int N, M, Q, val;               // Row, Column, Quantization, and pixel value declaration
    unsigned char *charImage;
    char header [100], *ptr;
    ifstream ifp;

    ifp.open(fname, ios::in | ios::binary);

    if (!ifp) {
        cerr << "Can't read image: " << fname << endl;
        return 1;
    }

    // read header
    ifp.getline(header,100,'\n');
    if ( (header[0]!=80) ||    /* 'P' */
        (header[1]!=53) ) {   /* '5' */
        cerr << "Image " << fname << " is not PGM\n";
        return 2;
    }

    ifp.getline(header,100,'\n');
    while(header[0]=='#')
        ifp.getline(header,100,'\n');

    M=strtol(header,&ptr,0);
    N=atoi(ptr);

    ifp.getline(header,100,'\n');
    Q=strtol(header,&ptr,0);

    charImage = (unsigned char *) new unsigned char [M*N];
    if (charImage == nullptr) {
        cerr << "Memory allocation for charImage failed!\n";
        return 3;
    }

    ifp.read( reinterpret_cast<char *>(charImage), (M*N)*sizeof(unsigned char));
    cout << "Bytes read: " << ifp.gcount() << endl;

    if (ifp.fail()) {
        cerr << "Image " << fname << " has wrong size\n";
        delete[] charImage;
        return 4;
    }

    ifp.close();

    image.SetImageInfo(N, M, Q);

    // Convert the unsigned characters to integers
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < M; j++) {
            val = static_cast<int>(charImage[i * M + j]);
            image.SetPixelVal(i, j, val);
        }
    }

    delete[] charImage;

    return 0;
}

// Change the spatial resolution of an image by sub-sampling by a defined factor
int ImageProcessing::Sample(int factor, ImageType& image) {
    int N, M, Q, val;               // Rows, Colummns, Quantization, and pixel value variables
    unsigned char *charImage;       // Array to hold the pixel values of the image

    image.GetImageInfo(N, M, Q);
    int subN = N / factor;
    int subM = M / factor;

    // Allocate memory for the subsample image array
    // if it failed to allocate memory, it will return error code 1
    charImage = (unsigned char *)new unsigned char [subN * subM];
    if(charImage == NULL) {
        cerr << "Failed to allocate memory for sub-sampled char image\n";
        return 1;
    }

    // Subsample the image
    for (int i = 0; i < subN; i++) {
        for (int j = 0; j < subM; j++) {
            image.GetPixelVal(i * factor, j * factor, val);
            charImage[i*subM+j] = (unsigned char)val;
        }
    }

    // Resize the image back to the original size using bilinear interoplation
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            // Scale the destination coordinates back to the original image
            float orgX = i / factor;
            float orgY = j / factor;

            // Get integer coordinates for the top-left corner
            int topLeftX = static_cast<int>(orgX);
            int topLeftY = static_cast<int>(orgY);

            // Make sure that the image stays within its bounds
            int bottomRightX = (topLeftX + 1 < subM) ? topLeftX + 1 : topLeftX;
            int bottomRightY = (topLeftY + 1 < subM) ? topLeftY + 1 : topLeftY;

            // Get the pixel values from the four neighboring points
            int topLeft = static_cast<int>(charImage[topLeftX * subM + topLeftY]);              // a
            int topRight = static_cast<int>(charImage[topLeftX * subM + bottomRightY]);         // b
            int bottomLeft = static_cast<int>(charImage[bottomRightX * subM + topLeftY]);       // c
            int bottomRight = static_cast<int>(charImage[bottomRightX * subM + bottomRightY]);  // d

            // Calculate how far the current point is within the grid cell
            float fractionalX = orgX - topLeftX;
            float fractionalY = orgY - topLeftY;

            // Calculate the bilinear interpolation of the image using the equation
            // I(x, y) = ax + by + cxy + d
            val = 
                topLeft * fractionalX +                     // ax
                topRight * fractionalY +                    // by
                bottomLeft * fractionalX * fractionalY +    // cxy
                bottomRight;                                // d

            // Set the interpolated pixel value in the resized image
            image.SetPixelVal(i, j, val);
        }
    }

    delete[] charImage;

    return 0;
}