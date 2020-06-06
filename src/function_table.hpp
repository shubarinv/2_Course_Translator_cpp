//
// Created by Vladimir Shubarin on 20.05.2020.
//

#ifndef SPO_COMPILER_SRC_FUNCTION_TABLE_HPP_
#define SPO_COMPILER_SRC_FUNCTION_TABLE_HPP_

#include <string>
#include <iostream>
#include "function.hpp"
class FunctionTable {
 public:
  ~FunctionTable() { functions.clear(); }

  Function *getFuncByName(const std::string &_name) {
	for (auto &func : functions) {
	  if (func->getName() == _name) {
		return func;
	  }
	}
	throw FunctionNotDefinedError(_name);
  }

  bool isFuncDefined(const std::string &_name) {
	for (auto &func : functions) {
	  if (func->getName() == _name) {
		return true;
	  }
	}
	return false;
  }

  void printToConsole() {
	for (auto &func : functions) {
	  std::cout << func->getName() << std::endl;
	  for (auto &var:func->variables.getVariables()) {
		std::cout << "+- " << var->getName() << " : " << Variable::varTypeToString(var->getType());
		if (var->getIsParam())
		  std::cout << " (Param)";
		std::cout << std::endl;
	  }
	  if (func->getReturnVar() != nullptr) {
		std::cout << "Returns: " << func->getReturnVar()->getName() << " : " << Variable::varTypeToString(func->getReturnVar()->getType()) << std::endl;
	  }
	  std::cout << std::endl << std::endl;
	}
  }
  void addFunction(Function *_func) {
	if (!isFuncDefined(_func->getName()))
	  functions.push_back(_func);
	else throw NotImplementedException("Redef of proc");
  }

 private:
  std::vector<Function *> functions;
 public:
  [[nodiscard]] const std::vector<Function *> &getFunctions() const {
      return functions;
  }
};

#endif //SPO_COMPILER_SRC_FUNCTION_TABLE_HPP_
