#include "Histogram.h"

Histogram::Histogram() {  }

// Clear the map
Histogram::~Histogram() { 
    this->Data.clear();         
}

// Private Methods
// Asks the user for the path of the image
char* Histogram::FindImage() {
    // Alocate memory for the imageName
    char* imageName = (char*)malloc(MAX_PATH_LENGTH * sizeof(char));

    // Exit if we failed to allocate memory for the image path
    if (imageName == NULL) {
        cerr << "Failed to alocate memory for the image path\n";
        return NULL;
    }

    printf("What is the path of the histogram image you would like to save? ");
    cin >> imageName;

    return imageName;
}

// Public Methods
// Set a value at a given key
void Histogram::SetHistData(int key) {
    map<int, int>::iterator it = this->Data.find(key);

    if (it == this->Data.end()) {
        // Key not found in the histogram, insert it with a value of 1
        this->Data.insert(pair<int, int>(key, 1));
    } else {
        // Key already exists, increament its value
        it->second++;
    }
}

// Get the value at a specific key
int Histogram::GetHistData(int key) {
    return this->Data[key];
}

// Find the maximum value of the histogram
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

// Clear the histogram data
void Histogram::Clear() {
    this->Data.clear();
}

// Write the histogram to a pgm image
int Histogram::SaveHistImg() {
    int N = 512;
    int M = 400;
    int barWidth, x;
    float scaling, barHeight;
    ofstream ofp;
    unsigned char *charImage;
    map<int, int>::iterator maxValue = this->GetMax();
    char *fname = this->FindImage();

    charImage = (unsigned char *)new unsigned char [M*N];
    if (charImage == NULL) {
        cerr << "Failed to allocate memory for the charImage\n";
        delete[] charImage; // Freeing the allocated memory
        return 1;
    }

    // Initialize the image with a White background
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            charImage[j * N + i] = (unsigned char)255;
        }
    }

    // Scale the histogram values to fit in the image height
    scaling = min(static_cast<float>(N) / static_cast<float>(maxValue->second), static_cast<float>(1.00) - static_cast<float>(N) / static_cast<float>(maxValue->second));
    // Define bar width based on the number of histogram entries and image width
    barWidth = max(1, static_cast<int>(N / this->Data.size()));
    
    // Create the black histogram bars
    x = 0;
    for (map<int, int>::iterator it = this->Data.begin(); it != this->Data.end(); ++it) {
        barHeight = it->second * scaling;  // Scale the bar height to fit in the image

        // Draw vertical bars for each key in the histogram
        for (int y = 0; y < barHeight; ++y) {
            for (int i = 0; i < barWidth; ++i) {
                // Fill the appropriate pixels with black (0) for the bar
                if ((x + i) < N && (M - 1 - y) >= 0) {
                    charImage[(M - 1 - y) * N + (x + i)] = 0;
                }
            }
        }
        x += barWidth;
    }

    ofp.open(fname, ios::out | ios::binary);
    if (!ofp) { // Couldn't open the file
        cerr << "Can't Open file " << fname << endl;
        delete[] charImage; // Free the allocated memory
        return 2;
    }

    // Write the header information
    ofp << "P5" << endl;
    ofp << N << " " << M << endl;
    ofp << 255 << endl;
    
    // Write the pixel values
    ofp.write(reinterpret_cast<char *>(charImage), (M*N)*sizeof(unsigned char));
    if(ofp.fail()) { // Couldn't Write the image
        cerr << "Can't Write Image " << fname << endl;
        delete[] charImage;
        return 3;
    }

    ofp.close();

    cout << "The image " << fname << " was written\n\n";

    delete[] charImage;
    return 0;
}