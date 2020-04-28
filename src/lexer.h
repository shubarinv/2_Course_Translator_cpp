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
    static std::vector<std::string> tokenizeString(std::string *str) {
        std::vector<std::string> tokens;
        boost::split(tokens, *str, boost::is_any_of("\t,\n, "));
        return tokens;
    }

public:

    std::vector<std::string> tokenize(std::string program) {
        tokenizeString(&program);
        return tokenizeString(&program);
    }
};

#endif // SPO_COMPILER_LEXER_H
