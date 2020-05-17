//
// Created by Vladimir Shubarin on 17.05.2020.
//

#ifndef SPO_COMPILER_SRC_VARIABLE_TABLE_HPP_
#define SPO_COMPILER_SRC_VARIABLE_TABLE_HPP_

#include <vector>
#include "variable.hpp"
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

  void printToConsole(){
    for (auto &var : variables) {
      std::cout << var->getName() << ": " << Variable::varTypeToString(var->getType())<<std::endl;
    }
  }

 private:
  std::vector<Variable *> variables;
};

#endif //SPO_COMPILER_SRC_VARIABLE_TABLE_HPP_
