#ifndef PPROCESS_COMMANDS
#define PPROCESS_COMMANDS

#include "Command.cpp"
#include "Receiver.cpp"

// Sample a image
class SampleCommand : public Command {
    private:
        Receiver &receiver;
        ImageType &image;
        int &factor;
    public:
        // ConcreteCommand takes a reference to a Receiver
        // object in its constructor.
        SampleCommand(Receiver& rec, ImageType& img, int& fct) : receiver(rec), image(img), factor(fct) { }

        // The execute method calls the action on the Receiver.
        void execute() override { 
            receiver.SampleImage(factor, image);
        }
};

// Sample a image
class QuantizationCommand : public Command {
    private:
        Receiver &receiver;
        ImageType &image;
        int &level;
    public:
        // ConcreteCommand takes a reference to a Receiver
        // object in its constructor.
        QuantizationCommand(Receiver& rec, ImageType& img, int& lvl) : receiver(rec), image(img), level(lvl) { }

        // The execute method calls the action on the Receiver.
        void execute() override { 
            receiver.QuantizeImage(level, image);
        }
};

// Sample a image
class HistogramEqualizationCommand : public Command {
    private:
        Receiver &receiver;
        ImageType &image;
    public:
        // ConcreteCommand takes a reference to a Receiver
        // object in its constructor.
        HistogramEqualizationCommand(Receiver& rec, ImageType& img) : receiver(rec), image(img) { }

        // The execute method calls the action on the Receiver.
        void execute() override { 
            receiver.EqualizeImage(image);
        }
};

// Preform Correlation between an image and a mask
class CorrelationCommand : public Command {
    private:
        Receiver &receiver;
        ImageType &image;
        ImageType &mask;
    public:
        CorrelationCommand(Receiver& rec, ImageType& img, ImageType& msk) : receiver(rec), image(img), mask(msk) { }

        void execute() override {
            receiver.CorrelateImages(image, mask);
        }
};

// Smooth an image using the Averaging filter
class AveragingSmoothingCommand : public Command {
    private:
        Receiver &receiver;
        ImageType &image;
        int& size;
        string filterType = "averaging";
    public:
        // ConcreteCommand takes a reference to a Receiver
        // object in its constructor.
        AveragingSmoothingCommand(Receiver& rec, ImageType& img, int& sz) : receiver(rec), image(img), size(sz) { }

        // The execute method calls the action on the Receiver.
        void execute() override { 
            receiver.SmoothImage(image, filterType, size);
        }
};

// Smooth an image using the Gaussian filter
class GaussianSmoothingCommand : public Command {
    private:
        Receiver &receiver;
        ImageType &image;
        int& size;
        string filterType = "gaussian";
    public:
        GaussianSmoothingCommand(Receiver& rec, ImageType& img, int& sz) : receiver(rec), image(img), size(sz) { }

        void execute() override {
            receiver.SmoothImage(image, filterType, size);
        }
};

// Sharpen an image using the Prewitt Kernel
class PrewittSharpeningCommand : public Command {
    private:
        Receiver &receiver;
        ImageType &image;
        
        // Prewitt kernels
        float prewitt_x[9] = { -1, 0, 1, -1, 0, 1, -1, 0, 1 }; // Horizontal Kernel
        float prewitt_y[9] = { -1, -1, -1, 0, 0, 0, 1, 1, 1 }; // Vertical Kernel
    public:
        PrewittSharpeningCommand(Receiver& rec, ImageType& img) : receiver(rec), image(img) { }

        void execute() override {
            receiver.SharepenImage(image, prewitt_x, prewitt_y);
        }
};

// Sharpen an image using the Sobel Kernel
class SobelSharpeningCommand : public Command {
    private:
        Receiver &receiver;
        ImageType &image;
        
        // Sobel kernels
        float sobel_x[9] = { -1, 0, 1, -2, 0, 2, -1, 0, 1 }; // Horizontal Kernel
        float sobel_y[9] = { 1, 2, 1, 0, 0, 0, -1, -2, -1 }; // Vertical Kernel
    public:
        SobelSharpeningCommand(Receiver& rec, ImageType& img) : receiver(rec), image(img) { }

        void execute() override {
            receiver.SharepenImage(image, sobel_x, sobel_y);
        }
};

class LaplacianSharpeningCommand : public Command {
    private:
        Receiver &receiver;
        ImageType &image;
    public:
        LaplacianSharpeningCommand(Receiver& rec, ImageType& img) : receiver(rec), image(img) { }

        void execute() override {
            receiver.LapSharepenImage(image);
        }
};

#endif