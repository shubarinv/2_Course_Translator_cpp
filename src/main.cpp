#include "Lexer.h"
#include "Parser.h"
#include "aixlog.hpp"
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <iostream>
// BOOST_ENABLE_ASSERT_DEBUG_HANDLER is defined for the whole project
#include <stdexcept>    // std::logic_error
#include <iostream>     // std::cerr
using namespace boost;

int main(int argc, char* argv[]) {

    auto sink_cout = std::make_shared<AixLog::SinkCout>(AixLog::Severity::trace, AixLog::Type::normal);
    auto sink_file = std::make_shared<AixLog::SinkFile>(AixLog::Severity::trace, AixLog::Type::all, "logfile.log");
    AixLog::Log::init({sink_cout, sink_file});
    LOG(INFO) << "Hello, World!\n";
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
