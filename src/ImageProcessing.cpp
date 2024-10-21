#include "ImageProcessing.h"

ImageProcessing::ImageProcessing() {  }

ImageProcessing::~ImageProcessing() {  }

// Private Methods
// Get the histogram from an image and save it to a pgm file
void ImageProcessing::GetHist(ImageType& image, Histogram &hist) {
    int N, M, Q, val;
    image.GetImageInfo(N, M, Q);

    // Find the occurance of each pixel value and update the Histogram
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            val = image.GetPixelVal(i, j);

            hist.SetHistData(val);
        }
    }

    hist.SaveHistImg();
}

// Public Methods
// Question 1
// Change the spatial resolution of an image by sub-sampling by a defined factor
int ImageProcessing::Sample(int factor, ImageType& image) {
    int N, M, Q, val;               // Rows, Colummns, Gery Level, and pixel value variables
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
            val = image.GetPixelVal(i * factor, j * factor);
            charImage[i * subM + j] = (unsigned char)val;
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

    cout << "Done Processing Image\n\n";

    delete[] charImage;

    return 0;
}

// Question 2
// Change the grey levels in an image
int ImageProcessing::Quantization(int levels, ImageType& image) {
    int N, M, Q, val, Qx;               // Rows, Colummns, Gery Level, pixel value, and the Qx quantized intensity value
    image.GetImageInfo(N, M, Q);
    float delta = Q / (levels);         // The size of each quantization interval.

    // Loop through each pixel value of a given image
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            val = image.GetPixelVal(i, j);

            // Apply the Quantization formula to get the new intensity value
            Qx = static_cast<int>(val / delta) * delta + (delta / 2);

            image.SetPixelVal(i, j, Qx);
        }
    }

    cout << "Done Processing Image\n\n";

    return 0;
}

// Question 3
// Preform Histogram Equalization on a given image
int ImageProcessing::HisEqualization(ImageType& image) {
    Histogram hist;                     // The Image Histogram
    int N, M, Q, val;                   // Rows, Columns, Grey Levels, and Pixel Values
    image.GetImageInfo(N, M, Q);

    float* Pr = (float*)malloc((Q+1) * sizeof(float));  // probability of occurrence of intensity level
    float* s = (float*)malloc((Q+1) * sizeof(float));   // the equalized pixel values
    float* cdf = (float*)malloc((Q+1) * sizeof(float)); // The cumulative distribution function
    if(Pr == NULL || s == NULL || cdf == NULL) {
        cerr << "Failed to allocate memory!\n";
        return 1;
    }

    cout << "The original Histogram\n";
    this->GetHist(image, hist);

    // Calculate probability of occurrence on each value of the histogram.
    for (int i = 0; i <= Q; i++) {
        Pr[i] = float(hist.GetHistData(i)) / float(M*N);
    }

    // Compute the Cumulative Distribution Function (CDF)
    cdf[0] = Pr[0];
    for (int i = 1; i <= Q; i++) {
        cdf[i] = cdf[i - 1] + Pr[i];
    }

    // Scale the CDF values to the range [0, Q-1]
    for (int i = 0; i <= Q; i++) {
        s[i] = round((Q - 1) * cdf[i]);
    }

    // Map the equalized pixel intensity back to the image
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            val = image.GetPixelVal(i, j);
            image.SetPixelVal(i, j, s[val]);
        }
    }

    cout << "Done Processing Image\n\n";

    hist.Clear();

    cout << "The equalized Histogram\n";
    this->GetHist(image, hist);

    free(Pr);
    free(s);
    free(cdf);
    return 0;
}

// _______________________________________________________________________________

int ImageProcessing::Correlation(ImageType& image, ImageType& kernel) {
    int N, M, Q, kN, kM, kQ;
    image.GetImageInfo(N, M, Q);    // Get image dimensions
    kernel.GetImageInfo(kN, kM, kQ); // Get kernel dimensions

    unsigned char* buffer = new unsigned char[N * M](); // Buffer to store results

    // Iterate over the image pixels
    for (int i = kN / 2; i < N - kN / 2; i++) {
        for (int j = kM / 2; j < M - kM / 2; j++) {
            int sum = 0; // Use int to prevent overflow

            // Apply kernel
            for (int s = 0; s < kN; s++) {
                for (int t = 0; t < kM; t++) {
                    sum += image.GetPixelVal(i - kN / 2 + s, j - kM / 2 + t) * kernel.GetPixelVal(s, t);
                }
            }

            // Store the result directly, without clamping or normalizing
            buffer[i * M + j] = static_cast<unsigned char>(sum); // Cast to unsigned char
        }
    }

    // Transfer the results from buffer to image
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            image.SetPixelVal(i, j, buffer[i * M + j]);
        }
    }

    delete[] buffer; // Free allocated memory
    return 0;
}

// Preform smoothing based on the filter passed to it
int ImageProcessing::Smoothing(ImageType& image, float* filter, int size) {
    int N, M, Q;
    float sum;
    image.GetImageInfo(N, M, Q);

    unsigned char* buffer = (unsigned char*)new unsigned char [N * M];

    // Calculate the offset based on the filter size
    int offset = size / 2;

    // For each pixel in the original image
    for (int i = offset; i < N - offset; i++) { // Start at offset, end at N - offset
        for (int j = offset; j < M - offset; j++) { // Start at offset, end at M - offset
            sum = 0.0f;

            // Slide the filtering mask over the image and compute the sum
            for (int s = 0; s < size; s++) {
                for (int t = 0; t < size; t++) {
                    // Calculate the pixel index based on the filter
                    sum += image.GetPixelVal(i - offset + s, j - offset + t) * filter[s * size + t];
                }
            }

            // Store the result in the buffer
            buffer[i * M + j] = static_cast<unsigned char>(sum);
        }
    }

    // Set the results of the buffer to the image
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            image.SetPixelVal(i, j, buffer[i * M + j]);
        }
    }

    delete[] filter;
    delete[] buffer;

    return 0;
}

// Preform median filtering
int ImageProcessing::Median(ImageType& image) {
    int N, M, Q;
    image.GetImageInfo(N, M, Q);

    unsigned char* buffer = new unsigned char[N * M]; // Allocate memory for the new image

    // For each pixel in the original image, avoiding the edges
    for (int i = 1; i < N - 1; i++) { // Start from 1 to N - 1 to avoid edges
        for (int j = 1; j < M - 1; j++) { // Start from 1 to M - 1 to avoid edges

            // Create an array to hold the neighboring pixels
            unsigned char temp[9];
            int index = 0;

            // Gather neighboring pixels using four loops
            for (int s = -1; s <= 1; s++) {
                for (int t = -1; t <= 1; t++) {
                    temp[index++] = image.GetPixelVal(i + s, j + t);
                }
            }

            // Sort the array to find the median
            sort(temp, temp + 9);

            // Set the median value (the middle one in sorted order)
            buffer[i * M + j] = temp[4]; // The median value
        }
    }

    // Set the results of the buffer to the image
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            image.SetPixelVal(i, j, buffer[i * M + j]);
        }
    }

    delete[] buffer;

    return 0;
}

float* ImageProcessing::CreateFilter(string type, int size) {
    float* filter = new float[size * size]; // Allocate memory based on the factor

    // Create an averaging filter
    if (type == "averaging") {
        float value = 1.0f / (size * size);

        for (int i = 0; i < size * size; i ++) {
            filter[i] = value;
        }
    } else if (type == "gaussian") { // Create a Guasian filtr
        float sigma = size / 6.0f;
        float sum = 0.0f;
        int center = size / 2; // Calculate the center index

        for (int y = -center; y <= center; y++) {
            for (int x = -center; x <= center; x++) {
                float value = (1.0f / (2.0f * M_PI * sigma * sigma)) *
                              exp(-(x * x + y * y) / (2.0f * sigma * sigma));
                filter[(y + center) * size + (x + center)] = value;
                sum += value; // Sum for normalization
            }
        }

        // Normalize the filter values
        for (int i = 0; i < size * size; i++) {
            filter[i] /= sum; // Normalize to sum to 1
        }
    } else {
        cerr << "Unexpected type";
    }

    return filter;
}

int ImageProcessing::SaltandPepperImage(ImageType& image, int percentage) {
    int N, M, Q;
    image.GetImageInfo(N, M, Q);

    int numPixelsToAlter = (percentage * (N * M)) / 100;

    // Seed the random number generator
    std::srand(static_cast<unsigned int>(time(nullptr)));

    for (int i = 0; i < numPixelsToAlter; ++i) {
        // Generate random coordinates within the image
        int x = rand() % N;
        int y = rand() % M;

        // Randomly decide whether to set the pixel to black or white
        // Here, we consider 0 as black and 255 as white
        if (rand() % 2 == 0) {
            image.SetPixelVal(x, y, 255);
        } else {
            image.SetPixelVal(x, y, 0);
        }
    }

    return 0;
}