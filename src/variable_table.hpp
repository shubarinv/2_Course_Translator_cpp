//
// Created by Vladimir Shubarin on 17.05.2020.
//

#ifndef SPO_COMPILER_SRC_VARIABLE_TABLE_HPP_
#define SPO_COMPILER_SRC_VARIABLE_TABLE_HPP_

#include <vector>
#include "variable.hpp"
#include "exceptions.cpp"
class VariableTable {
 public:
  ~VariableTable() { variables.clear(); }

  void addVar(Variable *var) { variables.push_back(var); }

  Variable *getVarByName(const std::string &_name) {
	for (auto &var : variables) {
	  if (var->getName() == _name) {
		return var;
	  }
	}
	throw VariableNotDefinedError(_name);
  }

  bool isVarDefined(const std::string &_name) {
	for (auto &var : variables) {
	  if (var->getName() == _name) {
		return true;
	  }
	}
	return false;
  }

 private:
  std::vector<Variable *> variables;
};

#endif //SPO_COMPILER_SRC_VARIABLE_TABLE_HPP_
