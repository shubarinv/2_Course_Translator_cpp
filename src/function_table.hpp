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

  Function *getVarByName(const std::string &_name) {
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
	}
  }
  void addFunction(Function *_func) {
	if (!isFuncDefined(_func->getName()))
	  functions.push_back(_func);
	else throw NotImplementedException("Redef of proc");
  }

 private:
  std::vector<Function *> functions;
};

#endif //SPO_COMPILER_SRC_FUNCTION_TABLE_HPP_
