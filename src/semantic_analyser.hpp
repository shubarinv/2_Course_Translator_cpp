//
// Created by Vladimir Shubarin on 17.05.2020.
//

#ifndef SPO_COMPILER_SRC_SEMANTIC_ANALYSER_HPP_
#define SPO_COMPILER_SRC_SEMANTIC_ANALYSER_HPP_

/* TODO: local variables
   TODO: check if procedure was defined
   TODO: check if function was defined
   TODO: func required and func call arg type check
   TODO: func return type check
   */
#include "Node.hpp"
#include "Parser.h"
#include "variable_table.hpp"
class SemanticAnalyser {
 private:
  Node *tree{};
  Parser *parser{};
  VariableTable *variables;
 public:
  explicit SemanticAnalyser(const std::string &_filename) {
	parser = new Parser(_filename);
	parser->parse();
	tree = parser->GetTree();
	variables = new VariableTable();
	analyze();
  }
 private:

  void analyze() {
	lookForVariableDeclaration(tree);
	variables->printToConsole();
	checkBinOps(tree);
	checkForTypeMismatch(tree);
  }
  void lookForVariableDeclaration(Node *currentNode) {
	// если текущая нода ноль, то делать с ней ничего нельзя
	// так что выходим из функции
	if (currentNode == nullptr)
	  return;
	if (currentNode->type == Node::nodeType::VARDECL) {
	  std::string varType = currentNode->op2->value;
	  for (auto &varName:currentNode->op1->list) {
		variables->addVar(new Variable(varName->value, Variable::determineVarType(varType)));
	  }
	} else if (currentNode->type == Node::nodeType::VAR) {
	  if (!variables->isVarDefined(currentNode->value)) {
		throw VariableNotDefinedError(currentNode->value);
	  }
	}

	lookForVariableDeclaration(currentNode->op1);
	lookForVariableDeclaration(currentNode->op2);
	lookForVariableDeclaration(currentNode->op3);
	lookForVariableDeclaration(currentNode->op4);
	for (auto &node :currentNode->list) {
	  lookForVariableDeclaration(node);
	}
  }
  void checkBinOps(Node *currentNode) {
	// если текущая нода ноль, то делать с ней ничего нельзя
	// так что выходим из функции
	if (currentNode == nullptr)
	  return;

	Node *tmp = currentNode;
	Variable::varType prevVarType = Variable::varType::UNKNOWN;
	while (tmp != nullptr) {
	  if (tmp->type == Node::nodeType::BINOP) {
		if (tmp->op2->type == Node::nodeType::BINOP) {
		  if (prevVarType == Variable::varType::UNKNOWN) {
			prevVarType = variables->getVarType(tmp->op1->value);
		  } else if (!Variable::areTypesCompatible(variables->getVarType(tmp->op1->value), prevVarType)) {
			throw TypeMismatchError(Variable::varTypeToString(variables->getVarType(tmp->op1->value)),
									Variable::varTypeToString(prevVarType));
		  }
		}
		else if (prevVarType != Variable::varType::UNKNOWN) {
		  if (!Variable::areTypesCompatible(variables->getVarType(tmp->op1->value), prevVarType)) {
			throw TypeMismatchError(Variable::varTypeToString(prevVarType),
									Variable::varTypeToString(variables->getVarType(tmp->op1->value)));
		  } else if (!Variable::areTypesCompatible(prevVarType, variables->getVarType(tmp->op2->value))) {
			throw TypeMismatchError(Variable::varTypeToString(prevVarType),
									Variable::varTypeToString(variables->getVarType(tmp->op2->value)));
		  }
		} else if (!Variable::areTypesCompatible(variables->getVarType(tmp->op1->value), variables->getVarType(tmp->op2->value))) {
		  throw TypeMismatchError(Variable::varTypeToString(variables->getVarType(tmp->op1->value)),
								  Variable::varTypeToString(variables->getVarType(tmp->op2->value)));
		}
		if (tmp->op2->type != Node::nodeType::BINOP)break;
	  }
	  tmp = tmp->op2;
	}
	checkBinOps(currentNode->op1);
	checkBinOps(currentNode->op2);
	checkBinOps(currentNode->op3);
	checkBinOps(currentNode->op4);
	for (auto &node :currentNode->list) {
	  checkBinOps(node);
	}
  }

  void checkForTypeMismatch(Node *currentNode) {
	// если текущая нода ноль, то делать с ней ничего нельзя
	// так что выходим из функции
	if (currentNode == nullptr)
	  return;

	if (currentNode->type == Node::nodeType::FOR_LOOP) {
	  if (currentNode->op2->type == Node::nodeType::CONSTANT) {
		if (!Variable::areTypesCompatible(Variable::varType::INTEGER,
										  variables->getVarByName(currentNode->op4->value)->getType())) {
		  throw TypeMismatchError(Variable::varTypeToString(Variable::varType::INTEGER),
								  Variable::varTypeToString(variables->getVarByName(currentNode->op4->value)->getType()));
		}
	  } else if (currentNode->op4->type == Node::nodeType::CONSTANT) {
		if (!Variable::areTypesCompatible(variables->getVarByName(currentNode->op2->value)->getType(),
										  Variable::varType::INTEGER)) {

		  throw TypeMismatchError(Variable::varTypeToString(variables->getVarByName(currentNode->op4->value)->getType()),
								  Variable::varTypeToString(Variable::varType::INTEGER));
		}
	  } else if (currentNode->op2->type == Node::nodeType::CONSTANT &&
		  currentNode->op4->type == Node::nodeType::CONSTANT) {

	  } else if (!Variable::areTypesCompatible(variables->getVarByName(currentNode->op2->value)->getType(),
											   variables->getVarByName(currentNode->op4->value)->getType())) {
		throw TypeMismatchError(Variable::varTypeToString(variables->getVarByName(currentNode->op2->value)->getType()),
								Variable::varTypeToString(variables->getVarByName(currentNode->op4->value)->getType()));
	  }
	}

	checkForTypeMismatch(currentNode->op1);
	checkForTypeMismatch(currentNode->op2);
	checkForTypeMismatch(currentNode->op3);
	checkForTypeMismatch(currentNode->op4);
	for (auto &node :currentNode->list) {
	  checkForTypeMismatch(node);
	}
  }
};

#endif //SPO_COMPILER_SRC_SEMANTIC_ANALYSER_HPP_
