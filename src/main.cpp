#include "Lexer.h"
#include "Parser.h"
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <iostream>
// BOOST_ENABLE_ASSERT_DEBUG_HANDLER is defined for the whole project
#include <stdexcept>    // std::logic_error
#include <iostream>     // std::cerr
using namespace boost;

int main(int argc, char* argv[]) {
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

    return 0;
}
