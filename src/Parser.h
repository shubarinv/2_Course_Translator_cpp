//
// Created by vhundef on 27.04.2020.
//
#ifndef SPO_COMPILER_PARSER_H
#define SPO_COMPILER_PARSER_H


#include "Lexer.h"
#include "Ast.hpp"

class Parser {
private:
    Lexer * lexer;

    Ast * ast;

public:
    Parser(){
        throw NotImplementedException();
    }
};


#endif //SPO_COMPILER_PARSER_H
