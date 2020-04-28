#include "exceptions.cpp"
#include "lexer.h"
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <iostream>

using namespace boost;

int main() {
    std::string filename;
    std::cin >> filename;
    // Код находящийся ниже проверяет наличие указанного файла и его расширение
    std::string requiredFileExtension = ".exe"; // TODO: Поставить тут расширение выбранного языка
    while (!filesystem::exists(filename) || !boost::algorithm::ends_with(filename, requiredFileExtension)) {
        if (!boost::algorithm::ends_with(filename, requiredFileExtension)) // проверка расширения файла
            std::cout << "Unexpected file extension" << std::endl;
        else
            std::cout << "No such file" << std::endl;
        std::cout << "Try again: \nFilename: ";
        std::cin >> filename;
    }

    lexer lexer;
    // Читает файл и отправляет его в lexer для обработки и токенизации
    std::ifstream file(filename);
    if (file.is_open()) {
        throw NotImplementedException();
        // lexer.QueueLine();
    }

    return 0;
}
