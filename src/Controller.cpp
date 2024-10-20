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
    int factor, levels;
    
    cout << endl;
	cout << "--------------------------------------------------------" << endl;
	cout << "\t\tMain Menu\n";
	cout << "--------------------------------------------------------" << endl;
	cout << endl;
	cout << "\ta  :  Sample\n";
	cout << "\tb  :  Quantization\n";
	cout << "\tc  :  Histogram Equalization\n";
	cout << "\td  :  Correlation\n";
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