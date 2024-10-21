#include <stdlib.h>
#include <iostream>

#include "ImageType.h"
#include "ImageProcessing.h"
#include "Histogram.h"

using namespace std;

int main() {
    ImageType image;
    ImageType mask;
    ImageProcessing process;

    bool running = true;
    char choice;
    int factor, levels, size, precentage;

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
	cout << "\td  :  Correlation\n";
    cout << "\te  :  Averaging\n";
    cout << "\tf  :  Guassian\n";
    cout << "\tg  :  Median\n";
    cout << "\th  :  Salt and Pepper Image\n";
    cout << "\ti  :  Sharpen with Prewitt\n";
    cout << "\tj  :  Sharepen with Sobel\n";
    cout << "\tk  :  Sharpend with Laplacian\n";
    cout << "\tr  :  Open an Image\n";
    cout << "\tw  :  Write an Image\n";
	cout << "\tq  :  Quit the program\n";
    cout << "--------------------------------------------------------" << endl;
	cout << endl;

    do {
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
                cout << "Salt and Peppering an image...\n";
                cout << "By what precentage: ";
                cin >> precentage;

                process.SaltandPepperImage(image, precentage);
                break;
            case 'i':
                cout << "Sharpen with Prewitt...\n";

                process.Sharpen(image, prewitt_x, prewitt_y);
                break;
            case 'j':
                cout << "Sharpen with Sobel...\n";

                process.Sharpen(image, sobel_x, sobel_y);
                break;
            case 'k':
                cout << "Sharpen with Laplacian...\n";

                process.SharpenWithLaplacian(image);
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