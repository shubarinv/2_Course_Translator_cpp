//
// Created by vhundef on 27.04.2020.
//
#ifndef SPO_COMPILER_PARSER_H
#define SPO_COMPILER_PARSER_H


#include <dbghelp.h>
#include "Lexer.h"
#include "exceptions.cpp"
#include "Node.hpp"

class Parser {
private:
    Lexer *lexer;


public:
    explicit Parser(const std::string &_filename) {
        lexer = new Lexer(_filename);
        throw NotImplementedException("Not implemented yet");
    }

    void expect(Token::tokenType tokenType) {
        if (lexer->getCurrentToken()->getType() == tokenType) {
            lexer->nextToken();
        } else {
            throw NotImplementedException("SyntaxException");
        }
    }

    Node *program() {
        return nullptr;
    }

    Node *expr() {
        /*
         *  SimpleExpression [RelOp SimpleExpression]...
         */
        return nullptr;
    }

    Node *factor() {
        /*
         *   -> Number
         *   -> String
         *   -> NIL                         // not implemented
         *   -> '(' Expression ')'
         *   -> NOT Factor                  // not implemented
         *   -> TypeId '(' Expression ')'   // not implemented
         */
        Node *node;
        switch (lexer->getCurrentToken()->getType()) {
            case Token::tokenType::Num: ///< factor() -> Number
                node = new NumberNode(lexer->getCurrentToken()->getText());
                lexer->nextToken();
                return node;
            case Token::tokenType::String: ///< factor() -> String
                node = new StringNode(lexer->getCurrentToken()->getText());
                lexer->nextToken();
                return node;
            case Token::tokenType::LPAR: ///< factor() -> '(' Expression ')'
                expect(Token::tokenType::LPAR);
                node = new Node(Node::nodeType::FACTOR);
                node->op1 = new ParenNode(")");
                node->op2 = expr();
                expect(Token::tokenType::RPAR);
                node->op3 = expr();
                return node;
            default:
                throw ParsingError(
                        "ParsingError: was expecting a Left_Paren|Number|String, but got " +
                        lexer->getCurrentToken()->getText() + " , which is" +
                        Token::typeToString(lexer->getCurrentToken()->getType()));
        }
    }

    Node *multOp() {
        /*
             * -> '*'
             * -> '/'
             * -> DIV
             * -> MOD
             * -> AND
         */
        return nullptr;
    }

    Node *term() {
        /*
           * Factor (MulOp Factor)*
        */
        return nullptr;
    }

    Node *simpleExpression() {
        /*
           * SimpleExpression -> Term (AddOp Term)*
         */
        return nullptr;
    }

    Node *statement() {
        /*
         * -> LabelId ':' [SimpleStatement]
         * SimpleStatement
         */
        return nullptr;
    }

    Node *addOp() {
        /*
             * -> '+'
             * -> '-'
             * -> OR
             * -> XOR
        */
        return nullptr;
    }

    void evaluate() {
        throw NotImplementedException("eval not implemented");
    }

    void parse() {
        lexer->tokenize();

    }

    ~ Parser() {
        delete lexer;
    }
};


#endif //SPO_COMPILER_PARSER_H
