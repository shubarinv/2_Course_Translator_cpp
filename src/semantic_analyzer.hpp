//
// Created by Vladimir Shubarin on 17.05.2020.
//

#ifndef SPO_COMPILER_SRC_SEMANTIC_ANALYZER_HPP_
#define SPO_COMPILER_SRC_SEMANTIC_ANALYZER_HPP_

#include "Node.hpp"
#include "Parser.h"
#include "variable_table.hpp"
#include "function_table.hpp"

class SemanticAnalyzer {
 private:
  Node *tree{};
 public:
  [[nodiscard]] Node *getTree() const {
	return tree;
  }

 private:
  Parser *parser{};
  VariableTable *variables{};
 public:
  /**
   * @brief Возвращает объявленные переменные
   * @return
   */
  [[nodiscard]] VariableTable *getVariables() const {
	return variables;
  }

  /**
   * @brief Возвращает объявленные глобальные переменные
   * @return
   */
  [[nodiscard]] VariableTable *getGlobalVariables() const {
	return globalVariables;
  }
  /**
   * @brief возвращает известные функции
   * @return
   */
  [[nodiscard]]FunctionTable *getFunctions() const {
	return functions;
  }

 private:
  VariableTable *globalVariables{};
  FunctionTable *functions{};
 public:
  explicit SemanticAnalyzer(const std::string &_filename, Parser *_parser) {
	parser = _parser;
	parser->parse();
	tree = parser->GetTree();
	variables = new VariableTable();
	globalVariables = new VariableTable();
	functions = new FunctionTable();
	analyze();
  }

 private:
  /**
   * @brief определяет тип переменной
   * @param currentNode
   * @param func функция в которой переменная находится
   * @return
   */
  Variable::varType getVariableType(Node *currentNode, Function *func = nullptr) {
	if (func != nullptr) {
	  if (func->variables.isVarDefined(currentNode->value)) {
		return func->variables.getVarType(currentNode->value);
	  }
	  if (func->getReturnVar()->getName() == currentNode->value) {
		return func->getReturnVar()->getType();
	  }
	} else {
	  if (variables->isVarDefined(currentNode->value)) { return variables->getVarType(currentNode->value); }
	}
	if (globalVariables->isVarDefined(currentNode->value)) {
	  return globalVariables->getVarType(currentNode->value);
	}
	if (Token::determineTokenType(currentNode->value) != Token::tokenType::Id) {
	  return Variable::determineVarType(Token::typeToString(Token::determineTokenType(currentNode->value)));
	}
	return Variable::varType::UNKNOWN;
  }

  /**
   * @brief проверяет код
   */
  void analyze() {
	std::cout << "=======================\n SemanticAnalyzer \n===========" << std::endl;
	lookForVariableDeclaration();
	lookForFunctionsAndProcedures(tree);
	for (auto &function:functions->getFunctions()) {
	  checkTypeMismatch(function->getFuncAddr(), function);
	}
	if (!functions->getFunctions().empty()) { // if there are functions
	  checkTypeMismatch(tree->op2);
	} else {
	  checkTypeMismatch(tree);
	}
	checkFunctionCalls(tree);
	std::cout << "Global variables:" << std::endl;
	globalVariables->printToConsole();
	std::cout << "------------------\nIn function variables:" << std::endl;
	functions->printToConsole();
	std::cout << "------------------\nLocal variables:" << std::endl;
	variables->printToConsole();
  }

  void lookForVariableDeclaration() {
	if (tree->type == Node::nodeType::STATEMENT) {
	  return;
	}
	if (tree->op1->type == Node::nodeType::VAR_SECTION &&
		tree->op2->type == Node::nodeType::STATEMENT) {
	  declareVariables(tree->op1); // registering local variables
	  // register local variables do not expect functions/procedures
	} else if (tree->op1->list.front()->type == Node::nodeType::VAR_SECTION) {
	  declareVariables(tree->op1->list.front(), nullptr, true); //  registering global variables
	}
	if (tree->op1->list.back()->type == Node::nodeType::VAR_SECTION) {
	  declareVariables(tree->op1->list.back()); // registering local variables
	}
	checkVariableDeclaration(tree->op2);

  }

  /**
   * @brief ищет функции и переменные
   * @param currentNode
   */
  void lookForFunctionsAndProcedures(Node *currentNode) {
	// если текущая нода ноль, то делать с ней ничего нельзя
	// так что выходим из функции
	if (currentNode == nullptr)
	  return;

	if (currentNode->type == Node::nodeType::PROCEDURE ||
		currentNode->type == Node::nodeType::FUNCTION) {
	  functions->addFunction(new Function(currentNode->value, currentNode));
	  if (currentNode->op1->type == Node::nodeType::PARAMS) {
		for (auto &param : currentNode->op1->list) {
		  if (param->type == Node::nodeType::VARLIST) {
			for (auto &var : param->list) {
			  functions->getFuncByName(currentNode->value)->
				  variables.addVar(
				  new Variable(var->value, Variable::determineVarType(param->op1->value), true));
			}
		  } else if (param->type == Node::nodeType::PARAM) {
			for (auto &var:param->op2->list) {
			  functions->getFuncByName(currentNode->value)->setReturnVar(
				  new Variable(var->value, Variable::determineVarType(param->op2->op1->value)));
			}
		  }
		}
	  }
	  if (currentNode->op2->type == Node::nodeType::STATEMENT) {
		checkVariableDeclaration(currentNode, functions->getFuncByName(currentNode->value));
	  }
	  if (currentNode->op2->op1->type == Node::nodeType::VAR_SECTION) {
		declareVariables(currentNode->op2->op1, functions->getFuncByName(currentNode->value), false);
	  }
	}

	lookForFunctionsAndProcedures(currentNode->op1);
	lookForFunctionsAndProcedures(currentNode->op2);
	lookForFunctionsAndProcedures(currentNode->op3);
	lookForFunctionsAndProcedures(currentNode->op4);
	for (auto &node :currentNode->list) {
	  lookForFunctionsAndProcedures(node);
	}
  }

  /**
   * @brief объявлеят переменные
   * @param currentNode
   * @param parentFunction
   * @param bGlobal
   */
  void declareVariables(Node *currentNode, Function *parentFunction = nullptr, bool bGlobal = false) {
	// если текущая нода ноль, то делать с ней ничего нельзя
	// так что выходим из функции
	if (currentNode == nullptr)
	  return;

	if (currentNode->type == Node::nodeType::VARDECL) {
	  std::string varType = currentNode->op2->value;
	  for (auto &varName:currentNode->op1->list) {
		if (bGlobal) { // если глобальная переменная
		  globalVariables->addVar(new Variable(varName->value, Variable::determineVarType(varType)));
		} else if (parentFunction != nullptr) { // если переменная относится к функции
		  parentFunction->variables.addVar(new Variable(varName->value, Variable::determineVarType(varType)));
		  if (globalVariables->isVarDefined(parentFunction->getVariables().back()->getName())) {
			throw VariableShadowing(parentFunction->getVariables().back()->getName());
		  }
		} else { // если обычная переменная в программе
		  variables->addVar(new Variable(varName->value, Variable::determineVarType(varType)));
		  if (globalVariables->isVarDefined(variables->getVariables().back()->getName())) {
			throw VariableShadowing(variables->getVariables().back()->getName());
		  }
		}
	  }
	}

	declareVariables(currentNode->op1, parentFunction, bGlobal);
	declareVariables(currentNode->op2, parentFunction, bGlobal);
	declareVariables(currentNode->op3, parentFunction, bGlobal);
	declareVariables(currentNode->op4, parentFunction, bGlobal);
	for (auto &node :currentNode->list) {
	  declareVariables(node, parentFunction, bGlobal);
	}
  }

  /**
   * @brief проверяет была ли переменная объявлена перед использованием
   * @param currentNode
   * @param func
   */
  void checkVariableDeclaration(Node *currentNode, Function *func = nullptr) {
	if (currentNode == nullptr)
	  return;

	if (currentNode->type == Node::nodeType::VAR) {
	  if (func != nullptr) {
		if (!func->variables.isVarDefined(currentNode->value) &&
			!globalVariables->isVarDefined(currentNode->value) &&
			currentNode->value != func->getReturnVar()->getName()) {
		  throw VariableNotDefinedError(currentNode->value, func->getName());
		}
	  } else {
		if (!variables->isVarDefined(currentNode->value) &&
			!globalVariables->isVarDefined(currentNode->value)) {
		  throw VariableNotDefinedError(currentNode->value);
		}
	  }
	}
	checkVariableDeclaration(currentNode->op1, func);
	checkVariableDeclaration(currentNode->op2, func);
	checkVariableDeclaration(currentNode->op3, func);
	checkVariableDeclaration(currentNode->op4, func);
	for (auto &node: currentNode->list) {
	  checkVariableDeclaration(node, func);
	}
  }
/**
 * @brief проверят совместимы ли типы
 * @param currentNode
 * @param func
 */
  void checkTypeMismatch(Node *currentNode, Function *func = nullptr) {
	if (currentNode == nullptr)return;

	compareVariableTypes(currentNode, func);

	checkTypeMismatch(currentNode->op1, func);
	checkTypeMismatch(currentNode->op2, func);
	checkTypeMismatch(currentNode->op3, func);
	checkTypeMismatch(currentNode->op4, func);
	for (auto &node: currentNode->list) {
	  checkTypeMismatch(node, func);
	}
  }

  /**
   * @brief сравнимает типы переменных с эталоном
   * @param currentNode
   * @param func
   * @param ref
   */
  void compareVariableTypes(Node *currentNode, Function *func = nullptr, Variable::varType *ref = nullptr) {
	if (ref == nullptr) {
	  if (currentNode->type == Node::nodeType::BINOP) {
		if (currentNode->op1->type != Node::nodeType::BINOP &&
			currentNode->op2->type != Node::nodeType::BINOP && currentNode->op2->type != Node::nodeType::FACTOR) {
		  if (!Variable::areTypesCompatible(getVariableType(currentNode->op1, func),
											getVariableType(currentNode->op2, func))) {
			throw TypeMismatchError(Variable::varTypeToString(getVariableType(currentNode->op1, func)),
									Variable::varTypeToString(getVariableType(currentNode->op2, func)));
		  }
		} else if (currentNode->op1->type == Node::nodeType::BINOP) {
		  if (currentNode->op2->type != Node::nodeType::BINOP && currentNode->op2->type != Node::nodeType::FACTOR) {
			auto tmpRef = getVariableType(currentNode->op2, func);
			compareVariableTypes(currentNode->op1, func, &tmpRef);
		  } else { compareVariableTypes(currentNode->op1, func); }
		} else if (currentNode->op2->type == Node::nodeType::BINOP) {
		  if (currentNode->op1->type != Node::nodeType::BINOP) {
			auto tmpRef = getVariableType(currentNode->op1, func);
			compareVariableTypes(currentNode->op2, func, &tmpRef);
		  } else {
			compareVariableTypes(currentNode->op2, func);
		  }
		}
	  }
	} else {
	  if (currentNode->type == Node::nodeType::BINOP && currentNode->op2->type != Node::nodeType::FACTOR) {
		compareVariableTypes(currentNode->op1, func, ref);
		compareVariableTypes(currentNode->op2, func, ref);
	  } else if (currentNode->type == Node::nodeType::BINOP && currentNode->op2->type == Node::nodeType::FACTOR) {
		compareVariableTypes(currentNode->op1, func, ref);
		compareVariableTypes(currentNode->op2->op2, func, ref);
	  } else {
		if (!Variable::areTypesCompatible(*ref, getVariableType(currentNode, func))) {
		  throw TypeMismatchError(Variable::varTypeToString(*ref), Variable::varTypeToString(getVariableType(currentNode, func)));
		}
	  }
	}
  }
  /**
   * @brief проверяет правильность вызова функций
   * @param currentNode
   */
  void checkFunctionCalls(Node *currentNode) {
	if (currentNode == nullptr)
	  return;

	if (currentNode->type == Node::nodeType::FUNCCALL) {
	  Function *func = functions->getFuncByName(currentNode->value);
	  int i = 0;
	  for (auto &param:currentNode->op1->list) {
		if (i == func->getParams().size()) {
		  if (param->type == Node::nodeType::BINOP) {
			if (!Variable::areTypesCompatible(getVariableType(param->op1),
											  func->getReturnVar()->getType())) {
			  throw UnexpectedParameterType(func->getName(),
											Variable::varTypeToString(func->getReturnVar()->getType()),
											Variable::varTypeToString(getVariableType(param->op1)));
			}
		  } else if (!Variable::areTypesCompatible(getVariableType(param), func->getReturnVar()->getType())) {
			throw UnexpectedParameterType(func->getName(),
										  Variable::varTypeToString(func->getReturnVar()->getType()),
										  Variable::varTypeToString(getVariableType(param)));
		  }
		  i++;
		  continue;
		} else if (i > func->getParams().size()) {
		  i++;
		  continue;
		} else if (param->type == Node::nodeType::BINOP) {
		  if (!Variable::areTypesCompatible(func->getParams()[i]->getType(), getVariableType(param->op1))) {
			throw UnexpectedParameterType(func->getName(),
										  Variable::varTypeToString(func->getParams()[i]->getType()),
										  Variable::varTypeToString(getVariableType(param->op1)));
		  }
		} else if (!Variable::areTypesCompatible(func->getParams()[i]->getType(), getVariableType(param))) {
		  throw UnexpectedParameterType(func->getName(),
										Variable::varTypeToString(func->getParams()[i]->getType()),
										Variable::varTypeToString(getVariableType(param)));
		}
		i++;
	  }
	  std::cout << "Params in function: " << func->getParams().size() << std::endl;
	  if (i < func->getParams().size() + 1) {
		throw TooFewArgumentsException(func->getName(), func->getParams().size() + 1, i);
	  } else if (i > func->getParams().size() + 1) {
		throw TooManyArgumentsException(func->getName(), func->getParams().size() + 1, i);
	  }
	}
	checkFunctionCalls(currentNode->op1);
	checkFunctionCalls(currentNode->op2);
	checkFunctionCalls(currentNode->op3);
	checkFunctionCalls(currentNode->op4);
	for (auto &node: currentNode->list) {
	  checkFunctionCalls(node);
	}
  }

};

#endif //SPO_COMPILER_SRC_SEMANTIC_ANALYZER_HPP_
