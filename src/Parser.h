//
// Created by vhundef on 27.04.2020.
//
#ifndef SPO_COMPILER_PARSER_H
#define SPO_COMPILER_PARSER_H

#include <stdexcept>
#include "Lexer.h"
#include "Node.hpp"
#include "exceptions.cpp"

class Parser {
private:
    Lexer *lexer;

public:
    void parse() {

    }

    explicit Parser(const std::string &_filename) {
        lexer = new Lexer(_filename);
        parse();
    }

    void expect(Token::tokenType tokenType) {
        if (lexer->getCurrentToken()->getType() == tokenType) {
            lexer->nextToken();
        } else {
            std::string error = "Expected token of type: ";
            error += Token::typeToString(tokenType);
            error += ", but got: ";
            error += Token::typeToString(lexer->getCurrentToken()->getType());
            throw ParsingError(error);
        }
    }

    void printRecursive(Node *currentNode, size_t level) {
        using namespace std;
        // если узел не существует, то просто выходим из функции
        if (currentNode == nullptr)
            return;

        for (int i = 0; i < level; ++i) {
            // выводим level раз отступ в два пробела
            cout << "|  ";
        }

        // выводим обозначение узла
        cout << "+-";

        // в зависимости от типа узла
        // выводим нужное
        switch (currentNode->type) {
            case Node::nodeType::START:
                cout << "START ";
                break;
            case Node::nodeType::EXPR:
                cout << "EXPR ";
                break;
            case Node::nodeType::CONST:
                cout << "CONST ";
                // так как имеет значение, то выводим его в скобках
                cout << "(" << currentNode->value << ")";
                break;
            case Node::nodeType::ADD:
                cout << "ADD ";
                break;
            case Node::nodeType::SUB:
                cout << "SUB ";
                break;
            case Node::nodeType::MUL:
                cout << "MUL ";
                break;
            case Node::nodeType::DIV:
                cout << "DIV ";
                break;
            case Node::nodeType::STATEMENT:
                cout << "STATEMENT ";
                break;
            case Node::nodeType::IF:
                cout << "IF ";
                break;
            case Node::nodeType::ELSE:
                cout << "ELSE ";
                break;
            case Node::nodeType::VAR:
                cout << "VAR ";
                break;
            case Node::nodeType::BINOP:
                cout << "BINOP ";
                cout << "(" << currentNode->value << ")";
                break;
            case Node::nodeType::STR:
                cout << "STR ";
                break;
            case Node::nodeType::FACTOR:
                cout << "FACTOR ";
                break;
            case Node::nodeType::PAR:
                cout << "PAR";
                cout << " ('" << currentNode->value << "')";
                break;
            case Node::nodeType::TERM:
                cout << "TERM ";
                break;
            case Node::nodeType::RESERVED:
                cout << "RESERVED ";
                break;
        }

        cout << endl;

        // т.к рекурсивная
        printRecursive(currentNode->op1, level + 1);
        printRecursive(currentNode->op2, level + 1);
        printRecursive(currentNode->op3, level + 1);
        printRecursive(currentNode->op4, level + 1);
    }

    void recursiveTraversal(Node *currentNode) {
        // если текущая нода ноль, то делать с ней ничего нельзя
        // так что выходим из функции
        if (currentNode == nullptr)
            return;

        recursiveTraversal(currentNode->op1);
        recursiveTraversal(currentNode->op2);
        recursiveTraversal(currentNode->op3);
        recursiveTraversal(currentNode->op4);
    }

    ~Parser() { delete lexer; }
};

#endif // SPO_COMPILER_PARSER_H
