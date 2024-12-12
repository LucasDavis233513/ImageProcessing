#define SWAP(a,b) tempr = (a); (a) = (b); (b) = tempr

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

double ImageProcessing::ComputeMagnitude(float real, float imag) {
    return sqrt(real * real + imag * imag);
}

float ClampValues(float value, float min, float max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

// Public Methods

// ----------------------------------------------------------------------------------------------
// Question 1
// ----------------------------------------------------------------------------------------------

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

// ----------------------------------------------------------------------------------------------
// Question 2
// ----------------------------------------------------------------------------------------------

int ImageProcessing::Correlation(ImageType& image, ImageType& kernel) {
    int N, M, Q, kN, kM, kQ, a, b;
    image.GetImageInfo(N, M, Q);    // Get image dimensions
    kernel.GetImageInfo(kN, kM, kQ); // Get kernel dimensions

    unsigned char* buffer = new unsigned char[N * M](); // Buffer to store results

    a = kN / 2;
    b = kM / 2;

    // Iterate over the image pixels
    for (int i = a; i < N - a; i++) {
        for (int j = b; j < M - b; j++) {
            int sum = 0; // Use int to prevent overflow

            // Apply kernel
            for (int s = 0; s < kN; s++) {
                for (int t = 0; t < kM; t++) {
                    sum += image.GetPixelVal(i - a + s, j - b + t) * kernel.GetPixelVal(s, t);
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

int ImageProcessing::Sharpen(ImageType& image, float* xMask, float* yMask) {
    int N, M, Q;
    image.GetImageInfo(N, M, Q);
    ImageType gradient_x(N, M, Q);
    ImageType gradient_y(N, M, Q);

    // Apply Prewitt filters to compute gradients
    for (int i = 1; i < N - 1; i++) { // Loop through pixels avoiding borders
        for (int j = 1; j < M - 1; j++) {
            float gx = 0.0f;
            float gy = 0.0f;

            // Convolve the Prewitt kernels with the image
            for (int k = -1; k <= 1; k++) {
                for (int l = -1; l <= 1; l++) {
                    gx += xMask[(k + 1) * 3 + (l + 1)] * image.GetPixelVal(i + k, j + l);
                    gy += yMask[(k + 1) * 3 + (l + 1)] * image.GetPixelVal(i + k, j + l);
                }
            }

            // Store the computed gradients directly (no inversion)
            gradient_x.SetPixelVal(i, j, static_cast<unsigned char>(ClampValues(gx, 0.0f, 255.0f)));
            gradient_y.SetPixelVal(i, j, static_cast<unsigned char>(ClampValues(gy, 0.0f, 255.0f)));
        }
    }

    // Calculate the final image based on the gradient magnitudes
    unsigned char* buffer = new unsigned char[N * M];

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            int gx = gradient_x.GetPixelVal(i, j);
            int gy = gradient_y.GetPixelVal(i, j);
            float magnitude = sqrt(gx * gx + gy * gy);

            // Thresholding to reduce noise
            unsigned char pixelValue = (magnitude > 50) ? static_cast<unsigned char>(ClampValues(magnitude, 0.0f, 255.0f)) : 0;

            // Keep the final image non-inverted
            buffer[i * M + j] = pixelValue; // Use pixelValue directly, no inversion
        }
    }

    // Set the computed final image
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            image.SetPixelVal(i, j, buffer[i * M + j]);
        }
    }

    // Write the gradient images
    cout << "Writing the x gradient...\n";
    gradient_x.WriteImage();

    cout << "Writing the y gradient...\n";
    gradient_y.WriteImage();

    delete[] buffer;
    return 0;
}

int ImageProcessing::SharpenWithLaplacian(ImageType& image) {
    int N, M, Q;
    image.GetImageInfo(N, M, Q);

    float laplacian[9] = { 0, 1, 0, 1, -4, 1, 0, 1, 0 };
    unsigned char* buffer = new unsigned char[N * M];

    // Apply the Laplacian filter
    int offset = 1; // For a 3x3 filter
    for (int i = offset; i < N - offset; i++) {
        for (int j = offset; j < M - offset; j++) {
            float sum = 0.0f;

            // Slide the filter over the image
            for (int s = 0; s < 3; s++) {
                for (int t = 0; t < 3; t++) {
                    sum += image.GetPixelVal(i - offset + s, j - offset + t) * laplacian[s * 3 + t];
                }
            }

            // Thresholding the result
            buffer[i * M + j] = (abs(sum) > 30) ? static_cast<unsigned char>(ClampValues(sum + image.GetPixelVal(i, j), 0.0f, 255.0f)) : 0; // Only keep strong edges
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

    unsigned int seed = 123;
    // Seed the random number generator
    srand(seed);

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

// ----------------------------------------------------------------------------------------------
// Assignment 3
// ----------------------------------------------------------------------------------------------
// Padd a given array with zeros; zeros between each value, and one at the beginning and end of the array
int ImageProcessing::PadArray(float padded[], float data[], int N) {
    int paddedSize = 2 * N + 1;
    int j = 0;

    padded[j++] = 0.0f; 

    for (int i = 0; i < N; ++i) {
        padded[j++] = data[i];
        if (j < paddedSize) {
            padded[j++] = 0.0f;
        }
    }

    padded[j] = 0.0f;

    return 0;
}



// Save the pixel values of the image to an array representing the real values, initialize
// an imaginary array with zeros.
int ImageProcessing::ConvertImgToFloat(ImageType& image, float **real, float **imag) {
    int N, M, Q;
    image.GetImageInfo(N, M, Q);

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            real[i][j] = image.GetPixelVal(i, j);

            // Shift the frequencies to the center
            if((i + j) % 2 != 0) real[i][j] *= -1.0;
            imag[i][j] = 0;
        }
    }
    
    return 0;
}

// Save the real values back to the image, disregarding the imaginary values
int ImageProcessing::ConvertFloatToImg(ImageType& image, float **real, bool shift) {
    int N, M, Q, val;
    image.GetImageInfo(N, M, Q);
    
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if (shift) {
                // Undo the shift to the center
                if((i + j) % 2 != 0) real[i][j] *= -1.0;
            }

            val = (int)real[i][j];

            image.SetPixelVal(i, j, val);
        }
    }
    
    return 0;
}

// Generate a cosine wave
int ImageProcessing::GenerateCosineWave(float wave[], int N, double u) {
    for (int i = 0; i < N; i++) {
        wave[i] = cos((2 * M_PI * u * i) / N);
    }

    return 0;
}

// Shift the frequencies to the the center
int ImageProcessing::ShiftFrequencyToCenter(float signal[], int N) {
    for (int i = 0; i < N; i++) {
        signal[i] *= pow(-1, i);
    }

    return 0;
}

// Normalize the FFT; this process only needs to be done on the forward FFT transform
int ImageProcessing::NormalizeFFT(float data[], int nn) {
    int n, i;

    if (!(nn % 2 == 0)) {
        cerr << "This method only works with inputs of even length";
        return 1;
    }

    n = nn << 1;

    // Normalize the FFT by dividing each value by n
    for (i = 0; i < n; i++) {
        data[i] /= (float)nn;
    }

    return 0;
}

// This function will convert the real and imaginary values 
float** ImageProcessing::NormalizeMagnitude(int N, int M, float **real_Fuv, float **imag_Fuv) {
    float min, max;

    float **magn_Fuv = (float **)malloc(N * sizeof(float *));
    for (int i = 0; i < N; i++) {
        magn_Fuv[i] = (float *)malloc(M * sizeof(float *));
    }

    min = max = log(1.0 + hypot(real_Fuv[0][0], imag_Fuv[0][0]));

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            magn_Fuv[i][j] = log(1.0 + hypot(real_Fuv[i][j], imag_Fuv[i][j]));

            if(magn_Fuv[i][j] > max) max = magn_Fuv[i][j];
            if(magn_Fuv[i][j] < min) min = magn_Fuv[i][j];
        }
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            magn_Fuv[i][j] = 255.0 * (magn_Fuv[i][j] - min) / (max - min);
        }
    }

    return magn_Fuv;
}

// Preform the Fast Fourier Transformation on a 1D array; or an object in the time domain (code provided by the teacher)
// This function doesnt normalize the results
int ImageProcessing::fft1D(float data[], unsigned long nn, int isign) {
	unsigned long n, mmax, m, j, istep, i;
	double wtemp, wr, wpr, wi, theta;
	float tempr, tempi;
    
    // Check to make sure that the input is a power of 2
    // Throw an error if it doesn't
    if (!(nn % 2 == 0)) {
        cerr << "This method only works with inputs of even length\n";
        return 1;
    }

	n = nn << 1;                                // Shift nn to the left by 1; This will result in the length
                                                // doubling
	j = 1;

    // This loop rearranges the data array based on the bit-reversed order of the indices
	for (i = 1; i < n; i += 2) {
		if (j > i) {
			SWAP(data[j], data[i]);
			SWAP(data[j + 1], data[i + 1]);
		}

		m = n >> 1;

		while (m >= 2 && j > m) {
			j -= m;
			m >>= 1;
		}

		j += m;
	}

	mmax = 2;

    // Main loop for the 1D fourier transformation
	while (n > mmax) {
		istep = mmax << 1;

		theta = isign * (6.28318530717959 / mmax);
		wtemp = sin(0.5 * theta);
		wpr = -2.0 * wtemp * wtemp;

		wr = 1.0;
		wi = 0.0;
		
        for (m = 1; m < mmax; m += 2) {
			for (i = m; i <= n; i += istep) {
				j = i + mmax;
				
                tempr = wr * data[j] - wi * data[j + 1];
				tempi = wr * data[j + 1] + wi * data[j];

				data[j] = data[i] - tempr;
				data[j + 1] = data[i + 1] - tempi;
				data[i] += tempr;
				data[i + 1] += tempi;
			}

			wr = (wtemp = wr) * wpr - wi * sin(theta) + wr;
			wi = wi * wpr + wtemp * sin(theta) + wi;
		}
		mmax = istep;
	}

    return 0;
}

// Preform the Fast Fourier Transformation on an Image
// This function normalizes the results for you...
int ImageProcessing::fft2D(int N, int M, float **real_Fuv, float **imag_Fuv, int isign) {
    float **real_Fxv, **imag_Fxv, *data;

    data = (float *)malloc(2 * N * sizeof(float));

    real_Fxv = (float **)malloc(N * sizeof(float *));
    for (int i = 0; i < N; i++) 
        real_Fxv[i] = (float *)malloc(M * sizeof(float));

    imag_Fxv = (float **)malloc(N * sizeof(float *));
    for (int i = 0; i < N; i++) {
        imag_Fxv[i] = (float *)malloc(M * sizeof(float));
    }

    // Preform the 1D FFT on each row
    for (int row = 0; row < N; row++) {
        for (int col = 0; col < M; col++) {
            data[2 * col] = real_Fuv[row][col];
            data[2 * col + 1] = imag_Fuv[row][col];
        }

        fft1D(data - 1, (unsigned long)M, isign);

        for (int col = 0; col < M; col++) {
            real_Fxv[row][col] = data[2 * col];
            imag_Fxv[row][col] = data[2 * col + 1];
        }
    }

    // Preform the 1D FFT on each Column
    for (int col = 0; col < M; col++) {
        for (int row = 0; row < N; row++) {
            data[2 * row] = real_Fxv[row][col];
            data[2 * row + 1] = imag_Fxv[row][col];
        }

        fft1D(data - 1, (unsigned long)N, isign);

        for (int row = 0; row < N; row++) {
            real_Fuv[row][col] = data[2 * row] / (float)N;
            imag_Fuv[row][col] = data[2 * row + 1] / (float)N;
        }
    }

    free(data);
    for (int i = 0; i < N; i++) {
        free(real_Fxv[i]);
        free(imag_Fxv[i]);
    }
    free(real_Fxv);
    free(imag_Fxv);

    return 0;
}

// ----------------------------------------------------------------------------------------------
// Assignment 4
// ----------------------------------------------------------------------------------------------
// This function will preform either a band-reject or band-pass filter
// If pass is true the function will apply the band-pass filter; if false it will apply the band-reject
// This function is setup to use the Butterworth low-pass filter
int ImageProcessing::BandFilter(int N, int M, int width, int sharpness, int radius, float **real_Fuv, float **imag_Fuv, bool pass) {
    int D;
    double filter;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            D      = sqrt(pow(i - N / 2, 2) + pow(j - M / 2, 2));
            filter = 1 / (1 + pow((D * width) / (pow(D, 2) - pow(radius, 2)), 2 * sharpness));

            if (pass) {
                filter = 1 - filter;
            }
            
            real_Fuv[i][j] *= filter;
            imag_Fuv[i][j] *= filter;
        }
    }

    return 0;
}

// This function will preform either a notch-reject or notch-pass filter
// if pass is true the function will apply the notch-pass fiulter; if false it will apply the notch-reject
// This function is setup to use the Butterworth high-pass filter
int ImageProcessing::NotchFilter(int N, int M, int pairs, int sharpness, int radius, float **real_Fuv, float **imag_Fuv, bool pass) {
    int D, negD;
    double filter;

    // Two pairs mean 4 values in total
    float uk[4] = {15, -15, 15, -15};
    float vk[4] = {30, -30, -30, 30};

    // Step 3: Apply the notch filter
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            filter = 1;
            for (int k = 0; k < pairs * 2; k++) {
                D    = sqrt(pow(i - N / 2 - uk[k], 2) + pow(j - N / 2 - vk[k], 2));
                negD = sqrt(pow(i - N / 2 + uk[k], 2) + pow(j - N / 2 + vk[k], 2));

                filter *= (1 / (1 + pow(radius / D ,sharpness))) * (1 / (1 + pow(radius / negD, sharpness)));
            }

            // Subtract 1 by the calculated filter to obtain the pass filtter
            if (pass) {
                filter = 1 - filter;
            }
            
            real_Fuv[i][j] *= filter;
            imag_Fuv[i][j] *= filter;
        }
    }

    return 0;
}

// This function will convolve the Fuv and Muv.
// This will only work if you have the frequency repensentations of the image and the mask
// and will require that the image is padded.
int ImageProcessing::freqConv(int N, int M, float **real_Fuv, float **imag_Fuv, float **real_Muv, float **imag_Muv) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            real_Fuv[i][j] = real_Fuv[i][j] * real_Muv[i][j] - imag_Fuv[i][j] * imag_Muv[i][j];
            imag_Fuv[i][j] = real_Fuv[i][j] * imag_Muv[i][j] + imag_Fuv[i][j] * real_Muv[i][j];
        }
    }

    return 0;
}

int ImageProcessing::HomomorphicFilter(int N, int M, float **real_Fuv, float **imag_Fuv, double gammaH, double gammaL, double D0, double c) {
    double D, filter;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            D = sqrt(pow(i - N / 2, 2) + pow(j - M / 2, 2));

            double exponent = -c * (D * D) / (D0 * D0);
            filter = (gammaH - gammaL) * (1.0 - std::exp(exponent)) + gammaL;

            real_Fuv[i][j] *= filter;
            imag_Fuv[i][j] *= filter;
        }
    }

    return 0;
}

#undef SWAP