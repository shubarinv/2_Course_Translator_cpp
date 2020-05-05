#include "Lexer.h"
#include "Parser.h"
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <iostream>


using namespace boost;

int main() {
    std::string filename = "delphiTestFile1.dpr";
    //std::cin >> filename;
    // Код находящийся ниже проверяет наличие указанного файла и его расширение

    std::string requiredFileExtension = ".dpr";
    while (!filesystem::exists(filename) || !boost::algorithm::ends_with(filename, requiredFileExtension)) {
        if (!boost::algorithm::ends_with(filename, requiredFileExtension)) // проверка расширения файла
            std::cout << "Unexpected file extension" << std::endl;
        else
            std::cout << "No such file" << std::endl;
        std::cout << "Try again: \nFilename: ";
        std::cin >> filename;
    }

    auto *parser = new Parser(filename);
    parser->parse();

    return 0;
}
