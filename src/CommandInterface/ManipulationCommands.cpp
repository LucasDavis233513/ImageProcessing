#ifndef MANIPULATION_COMMANDS
#define MANIPULATION_COMMANDS

#include "Command.cpp"
#include "Receiver.cpp"

// Read an image into memory
class SaltAndPepperCommand : public Command {
    private:
        Receiver &receiver;
        ImageType &image;
        int& precentage;
    public:
        // ConcreteCommand takes a reference to a Receiver
        // object in its constructor.
        SaltAndPepperCommand(Receiver& rec, ImageType& img, int& prc) : receiver(rec), image(img), precentage(prc) { }

        // The execute method calls the action on the Receiver.
        void execute() override { 
            receiver.SaltPepperImage(image, precentage);
        }
};

#endif