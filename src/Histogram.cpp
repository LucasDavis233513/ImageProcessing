#include "Histogram.h"

Histogram::Histogram() {  }

// Clear the map
Histogram::~Histogram() { 
    this->Data.clear();         
}

void Histogram::SetHistData(int key, int value) {
    this->Data.insert(pair<int, int>(key, value));
}

int Histogram::GetHistData(int key) {
    return this->Data[key];
}

map<int, int>::iterator Histogram::GetMax() {
    if (this->Data.empty()) {
        return this->Data.end();
    }

    map<int, int>::iterator max = this->Data.begin();
    for (std::map<int, int>::iterator it = this->Data.begin(); it != this->Data.end(); ++it) {
        if (it->second > max->second) {
            max = it;
        }
    }

    return max;
}

// Display a histogram to the console
void Histogram::DisplayHistogram(Histogram &hist) {
    // Get the max_value of the histogram
    map<int, int>::iterator max_value = hist.GetMax();
    if (max_value == this->Data.end()) {
        cout << "Histogram is empty" << endl;
        return;
    }

    // Display histogram
    for (int i = max_value->second; i >= 0; --i) { // Use max frequency
        cout.width(2);
        cout << i << " |\t";

        for (map<int, int>::iterator it = this->Data.begin(); it != this->Data.end(); ++it) {
            if (it->second >= i) {
                cout << "x\t";
            } else {
                cout << "\t";  // Space to align with 'x'
            }
        }
        cout << endl;
    }

    // Print base line
    cout << "------------------------------------------------------------------------------" << endl;
    cout << "\t";

    // Print keys at the bottom
    for (map<int, int>::iterator it = this->Data.begin(); it != this->Data.end(); ++it) {
        cout << it->first << "\t";
    }
    cout << endl;
}