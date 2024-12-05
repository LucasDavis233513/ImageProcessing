#include <stdlib.h>
#include <iostream>
#include <fstream>

#include "ImageType.h"
#include "ImageProcessing.h"
#include "Histogram.h"
#include "RectangleFunction.h"

using namespace std;

int main() {
    ImageType image;
    ImageType mask;
    ImageProcessing process;

    bool running = true;
    char choice;
    int factor, levels, size, precentage;

    float f[] = {0, 2, 0, 3, 0, 4, 0, 4, 0, 5, 0, 6, 0, 7, 0, 8, 0, 9, 0, 10};

    int N, M, Q;
    double u = 8;

    float wave[128];
    float paddedWave[128 * 2];

    float paddedRect[128 * 2];

    float **real_Fuv, **imag_Fuv, **mag;

    // Prewitt kernels
    float prewitt_x[9] = { -1, 0, 1, -1, 0, 1, -1, 0, 1 }; // Horizontal
    float prewitt_y[9] = { -1, -1, -1, 0, 0, 0, 1, 1, 1 }; // Vertical
    
    // Sobel kernels
    float sobel_x[9] = { -1, 0, 1, -2, 0, 2, -1, 0, 1 }; // Horizontal
    float sobel_y[9] = { 1, 2, 1, 0, 0, 0, -1, -2, -1 }; // Vertical

    cout << endl;
	cout << "--------------------------------------------------------" << endl;
	cout << "\t\tMain Menu\n";
	cout << "--------------------------------------------------------" << endl;
	cout << endl;
	cout << "\ta  :  Sample\n";
	cout << "\tb  :  Quantization\n";
	cout << "\tc  :  Histogram Equalization\n";
    cout << "\n";
	cout << "\td  :  Correlation\n";
    cout << "\te  :  Averaging\n";
    cout << "\tf  :  Guassian\n";
    cout << "\tg  :  Median\n";
    cout << "\th  :  Sharpen with Prewitt\n";
    cout << "\ti  :  Sharpen with Sobel\n";
    cout << "\tj  :  Sharpen with Laplacian\n";
    cout << "\n";
    cout << "\tk  :  Salt and Pepper Image\n";
    cout << "\n";
    cout << "\tl  :  1D Fast Fourier Transform\n";
    cout << "\tm  :  1D FFT on Cosine function\n";
    cout << "\tn  :  1D FFT with a rectangle function\n";
    cout << "\n";
    cout << "\to  :  Generate a square image\n";
    cout << "\tp  :  2D FFT\n";
    cout << "\n";
    cout << "";
    cout << "\n";
    cout << "\tr  :  Open an Image\n";
    cout << "\tw  :  Write an Image\n";
    cout << "\n";
	cout << "\tq  :  Quit the program\n";
    cout << "--------------------------------------------------------" << endl;
	cout << endl;

    do {
        ImageType squareImage(512, 512, 255);

	    cout << "Enter your choice [ a - d or q ]: ";
        cin >> choice;

        switch(choice) {
            case 'a':
                cout << "Preforming Sample method...\n";
                cout << "By what factor? ";
                cin >> factor;

                process.Sample(factor, image);

                break;
            case 'b':
                cout << "Preforming Quantization method...\n";
                cout << "What is the grey levels? ";
                cin >> levels;

                process.Quantization(levels, image);

                break;
            case 'c':
                cout << "Preforming Histogram Equalization...\n";

                process.HisEqualization(image);
                break;
            case 'd':
                cout << "Preforming Correlation...\n";
                cout << "Please specify the mask: ";
                mask.ReadImage();

                process.Correlation(image, mask);

                break;
            case 'e':
                cout << "Preforming Averaging...\n";
                cout << "What is the size of the filter? (7x7 = 7) ";
                cin >> size;

                process.Smoothing(image, process.CreateFilter("averaging", size), size);
                break;
            case 'f':
                cout << "Preforming Gaussian smoothing...\n";
                cout << "What is the size of the filter? (7x7 = 7) ";
                cin >> size;

                process.Smoothing(image, process.CreateFilter("gaussian", size), size);
                break;
            case 'g':
                cout << "Preforming Median filtering...\n";

                process.Median(image);
                break;
            case 'h':
                cout << "Sharpen with Prewitt...\n";

                process.Sharpen(image, prewitt_x, prewitt_y);
                break;
            case 'i':
                cout << "Sharpen with Sobel...\n";

                process.Sharpen(image, sobel_x, sobel_y);
                break;
            case 'j':
                cout << "Sharpen with Laplacian...\n";

                process.SharpenWithLaplacian(image);
                break;
            case 'k':
                cout << "Salt and Peppering an image...\n";
                cout << "By what precentage: ";
                cin >> precentage;

                process.SaltandPepperImage(image, precentage);
                break;
            case 'l':
                cout << "Preforming the fft on the 1D array..\n";

                process.fft1D(f, 10, 1);        // Preform the forward transform
                process.NormalizeFFT(f, 10);    // Normalize by 1 / N

                process.fft1D(f, 10, -1);       // Preform the inverse transform

                for (int i = 0; i < 20; i++) {
                    printf("%f ", f[i]);
                }
                printf("\n");

                break;
            case 'm':
                cout << "Preforming the fft on a cosine wave...\n";

                process.GenerateCosineWave(wave, 128, u);
                process.ShiftFrequencyToCenter(wave, u);

                process.PadArray(paddedWave, wave, 128);

                process.fft1D(paddedWave, 128, 1);
                process.NormalizeFFT(paddedWave, 128);

                break;
            case 'n':             
                cout << "Preform the fft on a rectangle function...\n";
                process.PadArray(paddedRect, rectangle, 128);

                process.fft1D(paddedRect, (unsigned long)128, -1);
                process.NormalizeFFT(paddedRect, 128);

                break;
            case 'o':
                cout << "What is the size of the square? ";
                cin >> size;

                squareImage.Square(size);
                squareImage.WriteImage();

                break;
            case 'p':
                cout << "Preforming the fft on a 2D image...\n";

                image.GetImageInfo(N, M, Q);

                real_Fuv = (float **)malloc(N * sizeof(float *));
                for (int i = 0; i < N; i++) {
                    real_Fuv[i] = (float *)malloc(M * sizeof(float));
                }

                imag_Fuv = (float **)malloc(N * sizeof(float *));
                for (int i = 0; i < M; i++) {
                    imag_Fuv[i] = (float *)malloc(M * sizeof(float));
                }

                process.ConvertImgToFloat(image, real_Fuv, imag_Fuv);

                process.fft2D(N, M, real_Fuv, imag_Fuv, 1);

                mag = process.NormalizeMagnitude(N, M, real_Fuv, imag_Fuv);
                process.ConvertFloatToImg(image, mag, false);

                image.WriteImage();

                process.fft2D(N, M, real_Fuv, imag_Fuv, -1);

                process.ConvertFloatToImg(image, real_Fuv, true);

                break;
            case 'r':
                cout << "Opening an image file...\n";

                image.ReadImage();
                break;
            case 'w':
                cout << "Writing an image file...\n";
                image.WriteImage();
                break;
            case 'q':
                running = false;
                break;
        }
    } while (running);

    return 0;
}