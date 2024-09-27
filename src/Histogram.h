#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <iostream>
#include <stdlib.h>
#include <map>
#include <fstream>
#include <algorithm>
// #include <opencv.hpp>

using namespace std;

class Histogram {
    private:
        map<int, int> Data;                         // A map of the histograms data, Where the key is the value
                                                    // and the value is the amount of that key.
    public:
        Histogram();
        ~Histogram();

        void SetHistData(int);                      // Set a key and value into the Data map
        int GetHistData(int);                       // Return the value at a given key
        map<int, int>::iterator GetMax();           // Return the max pair in the map
        void Clear();

        int SaveHistImg(char[]);                    // Write the Histogram to a P2 pgm image file
};

#endif