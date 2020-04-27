#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
using namespace boost;
int main() {
    std::string filename;
    std::cin>>filename;

    std::string requiredFileExtention=".exe";
    while(!filesystem::exists(filename)||!boost::algorithm::ends_with(filename,requiredFileExtention)){
        if(!boost::algorithm::ends_with(filename,requiredFileExtention))
            std::cout<<"Unexpected file extension"<<std::endl;
        else
            std::cout << "No such file"<<std::endl;
        std::cout << "Try again: \nFilename: ";
        std::cin>>filename;
    }
    return 0;
}
