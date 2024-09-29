#include <stdlib.h>
#include <iostream>

#include "ImageType.h"
#include "ImageProcessing.h"
#include "Histogram.h"

using namespace std;

char Menu() {
	char choice;

	cout << endl;
	cout << "--------------------------------------------------------" << endl;
	cout << "\t\tMain Menu\n";
	cout << "--------------------------------------------------------" << endl;
	cout << endl;
	cout << "\ta  :  Sample\n";
	cout << "\tb  :  Quantization\n";
	cout << "\tc  :  Histogram Equalization\n";
	cout << "\td  :  Histogram Specification\n";
    cout << "\tr  :  Open an Image\n";
    cout << "\tw  :  Write an Image\n";
	cout << "\tq  :  Quit the program\n";
	cout << endl;
	cout << "--------------------------------------------------------" << endl;
	cout << "Enter your choice [ a - d or q ]:\n";
	cout << "--------------------------------------------------------" << endl;
	cin >> choice ;

	return choice;
}

int main() {
    ImageType image;
    ImageProcessing process;

    int factor, levels;

    bool running = true;

    do {
        switch(Menu()) {
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
                process.HisSpecification(image);
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