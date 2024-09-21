#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "ManipulateImage.h"

using namespace std;

// Write an image to a file
int ManipulateImage::WriteImage(char *fname, ImageType &image) {
    int N, M, Q, val;
    unsigned char *charImage;
    ofstream ofp;

    image.GetImageInfo(N, M, Q);
    charImage = (unsigned char *)new unsigned char [M*N];

    // Convert integer values to un signed char
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
        return 2;
    }

    ofp.close();
    delete[] charImage; // Freeing the allocated memory
    return 0; // Return 0 on success
}

// Read an image to a file
int ManipulateImage::ReadImage(char *fname, ImageType &image) {
    int N, M, Q, val;
    unsigned char *charImage;
    char header[100], *ptr;
    ifstream ifp;

    ifp.open(fname, ios::in | ios::binary);
    if (!ifp) { // Couldn't open the file
        cerr << "Can't Read the image " << fname << endl;
        delete[] charImage;
        return 1;
    }

    // Read Header
    ifp.getline(header, 100,'\n');
    while(header[0]=='#') ifp.getline(header, 100, '\n');

    M = strtol(header, &ptr, 0); // Read M, N
    N = atoi(ptr);

    ifp.getline(header, 100,'\n'); // Read Q
    Q = strtol(header, &ptr, 0);

    charImage = (unsigned char *)new unsigned char [M*N];

    ifp.read(reinterpret_cast<char *>(charImage), (M*N)*sizeof(unsigned char));
    if (ifp.fail()) {
        cerr << "Image " << fname << " has wrong size" << endl;
        delete[] charImage;
        return 2;
    }

    ifp.close();

    // Convert unsigned characters to integers
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            val = (int)charImage[i*M+j];
            image.SetPixelVal(i, j, val);
        }
    }

    return 0;
}