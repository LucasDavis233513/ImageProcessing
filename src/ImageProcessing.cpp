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
        return 1;
    }

    ofp << "P5" << endl;
    ofp << N << " " << M << endl;
    ofp << Q << endl;

    ofp.write(reinterpret_cast<char *>(charImage), (M*N)*sizeof(unsigned char));
    if(ofp.fail()) { // Couldn't write the image
        cerr << "Can't Write Image " << fname << endl;
        delete[] charImage; // Freeing the allocated memory
        return 2;
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
        cout << "Can't read image: " << fname << endl;
        return 1;
    }

    // read header

    ifp.getline(header,100,'\n');
    if ( (header[0]!=80) ||    /* 'P' */
        (header[1]!=53) ) {   /* '5' */
        cerr << "Image " << fname << " is not PGM" << endl;
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
        cerr << "Memory allocation for charImage failed!" << endl;
        return 3;
    }

    ifp.read( reinterpret_cast<char *>(charImage), (M*N)*sizeof(unsigned char));
    cout << "Bytes read: " << ifp.gcount() << endl;

    if (ifp.fail()) {
        cerr << "Image " << fname << " has wrong size" << endl;
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

    charImage = (unsigned char *)new unsigned char [subN * subM];

    // Subsample the image
    for (int i = 0; i < subN; i++) {
        for (int j = 0; j < subM; j++) {
            image.GetPixelVal(i * factor, j * factor, val);
            charImage[i*subM+j] = (unsigned char)val;
        }
    }

    // Resize the image back to the original size
    for (int o = 0; o < N; o++) {
        for (int k = 0; k < M; k++) {
            int subO = o / factor;
            int subK = k / factor;

            val = static_cast<int>(charImage[subO * subM + subK]);
            image.SetPixelVal(o, k, val);
        }
    }

    delete[] charImage;

    return 0;
}