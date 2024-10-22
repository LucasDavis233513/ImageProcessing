#ifndef COMMAND
#define COMMAND

// Command interface
class Command {
    public:
        // The execute method is declared in the Command
        // interface.
        virtual void execute() = 0;
};

#endif