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
#include <sstream>
#include "Token.hpp"

class Lexer {
private:
    static std::string singleSpace(std::string const &input) {
        std::istringstream buffer(input);
        std::ostringstream result;

        std::copy(std::istream_iterator<std::string>(buffer),
                  std::istream_iterator<std::string>(),
                  std::ostream_iterator<std::string>(result, " "));
        return result.str();
    }

    static std::vector<std::string> tokenizeString(std::string *str) {
        std::vector<std::string> tokens;
        std::string strToTokenize = *str;
        strToTokenize=singleSpace(strToTokenize);
        for (int i = 0; i < strToTokenize.length(); i++)
        {
            if (strToTokenize[i] == ';')
            {
                strToTokenize.insert(i++, " ");
            }
        }
        boost::split(tokens, strToTokenize, boost::is_any_of("\t,\n, "));
        int i=0;
        for(auto &token:tokens){
            if(token.empty())
                tokens.erase(tokens.begin() + i);
            i++;
        }
        return tokens;
    }

public:

    std::vector<std::string> tokenize(std::string program) {
        std::list<Token> tokens;
        for(auto &token : tokenizeString(&program)){
            tokens.emplace_back(token);
        }
        return tokenizeString(&program);
    }
};

#endif // SPO_COMPILER_LEXER_H
