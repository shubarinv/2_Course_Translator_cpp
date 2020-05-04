//
// Created by vhundef on 28.04.2020.
//

#ifndef SPO_COMPILER_AST_HPP
#define SPO_COMPILER_AST_HPP


#include <iostream>
#include <utility>
#include "Node.hpp"
#include "exceptions.cpp"

///@brief
class ExprAST {
public:
    virtual ~ExprAST() = default;
};


class BinOpAst : public ExprAST {
    char Op;
    ExprAST *left, *right;
public:
    /**
     * Используется для бинарных операций Например: 2+2 или a>b
     * @param op Символ операции
     * @param _left Левый лист
     * @param _right Правый лист
     */
    BinOpAst(char op, ExprAST *_left, ExprAST *_right) : Op(op), left(_left), right(_right) {}
};

class FuncCallAst : public ExprAST {
public:
    FuncCallAst() {
        throw NotImplementedException("Not Implemented");
    }
};

class NumberAst : public ExprAST {
    double value;
public:
    explicit NumberAst(double _value) : value(_value) {}
};

class VariableAst : public ExprAST {
    std::string name;
public:
    explicit VariableAst(std::string _name) : name(std::move(_name)) {}
};


#endif //SPO_COMPILER_AST_HPP
