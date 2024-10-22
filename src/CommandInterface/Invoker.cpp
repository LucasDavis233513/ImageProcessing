#ifndef INVOKER
#define INVOKER

#include "Command.cpp"

// Invoker
class Invoker {
    private:
        Command* command;

    public:
        // The setCommand method allows setting the command to
        // be executed.
        void setCommand(Command* cmd) { command = cmd; }

        // The executeCommand method triggers the execution of
        // the command.
        void executeCommand() { command->execute(); }
};

#endif