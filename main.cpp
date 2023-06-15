#include "include/system/Dispatcher.h"

int main(int argc, char* argv[]) {

    if (argc != 2) {
        std::cerr << "Неправильное кол-во аргументов. Имя файла не было введено. \n";
        return 1;
    }

    std::ifstream inputFile(argv[1]);
    if (!inputFile.is_open()) {
        std::cerr << "Ошибка при открытии файла. \n";
        return 1;
    }
    try {
        Dispatcher dispatcher = Dispatcher(inputFile);
        dispatcher.showAllEvents();

        inputFile.close();

        if (inputFile.fail()) {
            std::cerr << "Ошибка при закрытии файла. \n";
            return 1;
        }
    }
    catch (const FormatException &ex) {
        std::cerr << ex.what() << std::endl;
        return 1;
    }
    return 0;
}
