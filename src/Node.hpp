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
	START, // начало программы
	EXPR,
	STATEMENT,
	IF,
	ELSE,
	VAR, // переменная
	NODE, // узел
	CONSTANT, // Число
	ADD,  // Сложение
	SUB,  // Вычитание
	MUL, // Умножение
	DIV,   // Деление
	BINOP, // бинарная операция
	STR, // строка
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
	INPUT,
	OUTPUT,
	FOR_LOOP,
	STATEMENT_LIST,
	VAR_SECTION,
	WITH,
	WHILE,
	ARRAY_TYPE,
	PROCEDURE,
	PARAMS,
	PARAM,
	FUNCTION,
	FUNCCALL,
	AND,
  };
  std::string value;
  nodeType type;
  Node *op1;
  Node *op2;
  Node *op3;
  Node *op4;
  int priority = 1000;
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

/**
 * @brief язел числа
 */
class NumberNode : public Node {
 public:
  explicit NumberNode(std::string value) : Node(Node::nodeType::CONSTANT, std::move(value)) {}
};

/**
 * @brief узел бинарного выражения
 */
class binOpNode : public Node {
 public:
  binOpNode(Node *lhs, std::string operation, Node *rhs) : Node(Node::nodeType::BINOP, std::move(operation), lhs,
																rhs) {}
};

/**
 * @brief узел переменной
 */
class VarNode : public Node {
 public:
  explicit VarNode(std::string name) : Node(Node::nodeType::VAR, std::move(name)) {}
};

/**
 * @brief узел строки
 */
class StringNode : public Node {
 public:
  explicit StringNode(std::string value) : Node(Node::nodeType::STR, std::move(value)) {}
};

/**
 * @brief узел скобки
 */
class ParenNode : public Node {
 public:
  explicit ParenNode(std::string paren) : Node(Node::nodeType::PAR, std::move(paren)) {}
};

/**
 * @brief узел списка переменных
 */
class VarList : public Node {
 public:
  VarList() : Node(Node::nodeType::VARLIST) {}
};

/**
 * @brief узел с условием
 */
class ifNode : public Node {
 public:
  ifNode(Node *condition, Node *action, Node *elseStatement) : Node(Node::nodeType::IF, "", condition, action,
																	elseStatement) {}
};

#endif //SPO_COMPILER_NODE_HPP
