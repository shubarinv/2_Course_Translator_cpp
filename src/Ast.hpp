//
// Created by vhundef on 28.04.2020.
//

#ifndef SPO_COMPILER_AST_HPP
#define SPO_COMPILER_AST_HPP


#include <iostream>
#include "Node.hpp"

class Ast {
private:
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

public:
    Node *tree{nullptr};

    void printRecursive(Node *currentNode, size_t level) {
        using namespace std;
        // если узел не существует, то просто выходим из функции
        if (currentNode == nullptr)
            return;


        for (int i = 0; i < level; ++i) {
            // выводим level раз отступ в два пробела
            cout << "  ";
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
        }

        cout << endl;

        // т.к рекурсивная
        printRecursive(currentNode->op1, level + 1);
        printRecursive(currentNode->op2, level + 1);
        printRecursive(currentNode->op3, level + 1);
        printRecursive(currentNode->op4, level + 1);
    }

};


#endif //SPO_COMPILER_AST_HPP
