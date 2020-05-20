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
 private:
  Variable::varType returnType = Variable::varType::UNKNOWN;
 public:
  explicit Function(std::string _name) : name(std::move(_name)) {}
  VariableTable variables;
};

#endif //SPO_COMPILER_SRC_FUNCTION_HPP_
