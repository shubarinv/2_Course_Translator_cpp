//
// Created by Vladimir Shubarin on 20.05.2020.
//

#ifndef SPO_COMPILER_SRC_FUNCTION_HPP_
#define SPO_COMPILER_SRC_FUNCTION_HPP_

#include <string>
#include <utility>
#include "variable_table.hpp"
class Function {
  std::string name;
 public:
  [[nodiscard]] const std::string &getName() const {
	return name;
  }
  Node *funcAddr{};
  [[nodiscard]] Node *getFuncAddr() const {
	return funcAddr;
  }
 private:
  Variable *returnVar{};
 public:
  [[nodiscard]] Variable *getReturnVar() const {
	return returnVar;
  }
 public:
  void setReturnVar(Variable *return_var) {
	returnVar = return_var;
  }
  [[nodiscard]] std::vector<Variable *> getVariables() const {
	return variables.getVariables();
  }
  std::vector<Variable *> getParams() {
	std::vector<Variable *> params;
	for (auto &var :getVariables()) {
	  if (var->getIsParam()) {
		params.push_back(var);
	  }
	}
	return params;
  }
 public:
  explicit Function(std::string _name, Node *functionAddress) : name(std::move(_name)), funcAddr(functionAddress) {}
  VariableTable variables;
};

#endif //SPO_COMPILER_SRC_FUNCTION_HPP_
