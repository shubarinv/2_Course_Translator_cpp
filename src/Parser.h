//
// Created by vhundef on 27.04.2020.
//
#ifndef SPO_COMPILER_PARSER_H
#define SPO_COMPILER_PARSER_H


#include "Lexer.h"
#include "Ast.hpp"

class Parser {
private:
    Lexer *lexer;


public:
    explicit Parser(const std::string &_filename) {
        lexer = new Lexer(_filename);
        throw NotImplementedException("Not implemented yet");
    }

    void expect(Token::tokenType tokenType) {
        if(lexer->getCurrentToken()->getType()==tokenType){
            lexer->nextToken();
        }
        else{
            throw NotImplementedException("SyntaxException");
        }
    }



    void term() {

    }

    void expr() {

    }
    void prim(){

    }

    void parse() {
        lexer->tokenize();
    }

    ~ Parser() {
        delete lexer;
    }
};


#endif //SPO_COMPILER_PARSER_H
