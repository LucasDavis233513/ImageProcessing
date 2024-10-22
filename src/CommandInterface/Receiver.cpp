#ifndef RECEIVER
#define RECEIVER

#include "ImageType.h"
#include "ImageProcessing.h"
#include "Histogram.h"

// Receiver
class Receiver {
    private:
        ImageProcessing process;
    public:
        // Image commands
        // Read an image into memory
        void ReadImage(ImageType& image) {
            cout << "Opening an image file...\n";

            image.ReadImage();
        }

        // Write an image to the disk
        void WriteImage(ImageType& image) {
            cout << "Writing the image to the disk...\n";

            image.WriteImage();
        }

        // ------------------------------------------------------------
        // Process Commands
        // Preform SubSample on an image
        void SampleImage(int& factor, ImageType& image) {
            cout << "Sub sampling an image by " << factor << "...\n";

            process.Sample(factor, image);
        }

        // Preform Quantization on an Image
        void QuantizeImage(int& level, ImageType& image) {
            cout << "Preforming Quantization on the image...\n";

            process.Quantization(level, image);
        }

        // Preform Histogram Equalization
        void EqualizeImage(ImageType& image) {
            cout << "Preforming Histogram Equalization...\n";

            process.HisEqualization(image);
        }

        void CorrelateImages(ImageType& image, ImageType& mask) {
            cout << "Preforming Correlation between two images...\n";

            process.Correlation(image, mask);
        }

        // Smooth an image
        void SmoothImage(ImageType& image, string& filterType, int& size) {
            cout << "Preforming Smoothing using the " << filterType << " filter...\n";

            float* filter = process.CreateFilter(filterType, size); // Create the appropriate filter
            process.Smoothing(image, filter, size);                 // Smooth the image using the filter
        }

        void SharepenImage(ImageType& image, float* xKernel, float* yKernel) {
            cout << "Preforming Sharpening...\n";

            process.Sharpen(image, xKernel, yKernel);
        }

        void LapSharepenImage(ImageType& image) {
            cout << "Preforming Sharpening with Laplacian...\n";

            process.SharpenWithLaplacian(image);
        }

        void SaltPepperImage(ImageType& image, int& precentage) {
            cout << "Salt and Peppering an image...\n";

            process.SaltandPepperImage(image, precentage);
        }
};

#endif