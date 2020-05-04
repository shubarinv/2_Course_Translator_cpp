//
// Created by vhundef on 28.04.2020.
//

#ifndef SPO_COMPILER_NODE_HPP
#define SPO_COMPILER_NODE_HPP


#include <string>
#include <utility>

class Node {
public:
    enum class nodeType {
        START,
        EXPR,
        STATEMENT,
        IF,
        ELSE,

        CONST, // Число
        ADD,  // Сложение
        SUB,  // Вычитание
        MUL, // Умножение
        DIV, // Деление
    };
    std::string value;
    nodeType type;
    Node* op1;
    Node* op2;
    Node* op3;
    Node* op4;

    explicit Node(nodeType _type,std::string _value="",Node* _op1=nullptr,Node* _op2=nullptr,Node* _op3=nullptr,Node* _op4=nullptr) {
        type = _type;
        value = std::move(_value);
        op1 = _op1;
        op2 = _op2;
        op3 = _op3;
        op4 = _op4;
    }

};
#endif //SPO_COMPILER_NODE_HPP
