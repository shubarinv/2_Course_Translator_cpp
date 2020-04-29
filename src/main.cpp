#include "exceptions.cpp"
#include "Lexer.h"
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

    Lexer *lexer;
    // Читает файл и отправляет его в Lexer для обработки и токенизации
    std::ifstream file(filename);
    if (file.is_open()) {
        lexer=new Lexer("int a = b;\n"
                        "if ( a == b ) {\n"
                        "int b = 0; }");
        lexer->tokenize();
        //lexer->printAllTokens();
        lexer->printToFile();
    }

    return 0;
}
