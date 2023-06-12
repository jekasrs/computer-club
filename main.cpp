#include "include/system/Dispatcher.h"

int main() {
    try {
        std::ifstream file("../example.txt");
        Dispatcher dispatcher = Dispatcher(file);
        dispatcher.showAllEvents();
    }
    catch (FormatException &ex) {
        std::cerr << ex.what() << std::endl;
    }

    return 0;
}
