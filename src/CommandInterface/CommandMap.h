#include <unordered_map>
#include <functional>

using namespace std;

class CommandMap {
    protected:
        static unordered_map<string, function<int>> commandMap;
    public:
        CommandMap();
        ~CommandMap();

        bool ExecuteCommand(const string);
};