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
    Parser(std::string _filename){
        lexer = new Lexer(_filename);

        ast = new Ast();
    }

    void parse(){
        lexer->tokenize();
    }

    ~ Parser(){
        delete lexer;
        delete ast;
    }
};


#endif //SPO_COMPILER_PARSER_H
