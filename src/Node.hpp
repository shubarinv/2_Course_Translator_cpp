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
        VAR,
        NODE,
        CONSTANT, // Число
        ADD,  // Сложение
        SUB,  // Вычитание
        MUL, // Умножение
        DIV,   // Деление
        BINOP,
        STR,
        FACTOR,
        PAR,
        TERM,
        RESERVED,
        COMP,
        VARDECL,
        VARTYPE,
        DECL,
        VARLIST,
        DESIGNATOR,
        PROG,
        UNIT,
        PROGRAM_BLOCK,
        USES,
        INTERFACE_SECTION,
        BLOCK,
        SECTION,
        TYPE,
        ARRAY_CONST,
        SUBRANGE_EXPR,
    };
    std::string value;
    nodeType type;
    Node *op1;
    Node *op2;
    Node *op3;
    Node *op4;
    std::list<Node *> list;

    explicit Node(nodeType _type, std::string _value = " ", Node *_op1 = nullptr, Node *_op2 = nullptr,
                  Node *_op3 = nullptr, Node *_op4 = nullptr) {
        type = _type;
        value = std::move(_value);
        op1 = _op1;
        op2 = _op2;
        op3 = _op3;
        op4 = _op4;
    }
};

class NumberNode : public Node {
public:
    explicit NumberNode(std::string value) : Node(Node::nodeType::CONSTANT, std::move(value)) {}
};

class binOpNode : public Node {
public:
    binOpNode(Node *lhs, std::string operation, Node *rhs) : Node(Node::nodeType::BINOP, std::move(operation), lhs,
                                                                  rhs) {}
};

class VarNode : public Node {
public:
    explicit VarNode(std::string name) : Node(Node::nodeType::VAR, std::move(name)) {}
};

class StringNode : public Node {
public:
    explicit StringNode(std::string value) : Node(Node::nodeType::STR, std::move(value)) {}
};

class ParenNode : public Node {
public:
    explicit ParenNode(std::string paren) : Node(Node::nodeType::PAR, std::move(paren)) {}
};

class VarList : public Node {
public:
    VarList() : Node(Node::nodeType::VARLIST) {}
};

class ifNode : public Node {
public:
    ifNode(Node *condition, Node *action, Node *elseStatement) : Node(Node::nodeType::IF, "", condition, action,
                                                                      elseStatement) {}
};

#endif //SPO_COMPILER_NODE_HPP
