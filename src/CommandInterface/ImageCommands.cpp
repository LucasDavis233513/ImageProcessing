#ifndef IMAGE_COMMANDS
#define IMAGE_COMMANDS

#include "Command.cpp"
#include "Receiver.cpp"

// Read an image into memory
class ReadImage : public Command {
    private:
        Receiver &receiver;
        ImageType &image;
    public:
        // ConcreteCommand takes a reference to a Receiver
        // object in its constructor.
        ReadImage(Receiver& rec, ImageType& img) : receiver(rec), image(img) { }

        // The execute method calls the action on the Receiver.
        void execute() override { 
            receiver.ReadImage(image);
        }
};

// Read an image into memory
class WriteImage : public Command {
    private:
        Receiver &receiver;
        ImageType &image;
    public:
        // ConcreteCommand takes a reference to a Receiver
        // object in its constructor.
        WriteImage(Receiver& rec, ImageType& img) : receiver(rec), image(img) { }

        // The execute method calls the action on the Receiver.
        void execute() override { 
            receiver.WriteImage(image);
        }
};

#endif