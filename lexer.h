//
// Created by vhundef on 27.04.2020.
//
#ifndef SPO_COMPILER_LEXER_H
#define SPO_COMPILER_LEXER_H

#include <boost/algorithm/string.hpp>
#include <boost/range/algorithm/remove_if.hpp>
#include <boost/algorithm/string/split.hpp>

#include <iostream>
#include <vector>
class lexer {
private:
  std::vector<std::string> strToToken(std::string* str){
    std::vector<std::string>tokens;
    boost::split(tokens,*str,boost::is_any_of("\t,\n, "));
    return tokens;
  }
public:

  void tokenize(std::string program) {
      std::cout<<"[";
      for(auto &str:strToToken(&program)){
        std::cout<<str<<", ";
      }
    std::cout<<"]";
  }
};

#endif // SPO_COMPILER_LEXER_H
