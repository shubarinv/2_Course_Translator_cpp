#include "Parser.h"
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <iostream>
#include <stdexcept>    // std::logic_error
#include <iostream>     // std::cerr
using namespace boost;

int main() {

    std::cout<<"Please enter name of the file you want to compile(should end with .dpr): "<<std::endl;
    std::string filename="delphiTestFile1.dpr";
   // std::cin >> filename;
  
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

   // std::system("pause");
    return 0;
}
