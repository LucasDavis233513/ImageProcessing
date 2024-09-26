#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <iostream>
#include <stdlib.h>
#include <map>

using namespace std;

class Histogram {
    private:
        map<int, int> Data;                         // A map of the histograms data, Where the key is the value
                                                    // and the value is the amount of that key.
    public:
        Histogram();
        ~Histogram();

        void SetHistData(int, int);                 // Set a key and value into the Data map
        int GetHistData(int);                       // Return the value at a given key
        map<int, int>::iterator GetMax();           // Return the max pair in the map

        void DisplayHistogram(Histogram &hist);     // Display a Histogram to the screen
};

#endif