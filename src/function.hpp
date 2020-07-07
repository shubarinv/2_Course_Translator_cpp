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
  /**
   * @brief Возвращает имя функции
   * @return
   */
  [[nodiscard]] const std::string &getName() const {
	return name;
  }
  /**
   * @brief адрес функции
   */
  Node *funcAddr{};

  /**
   * @brief Возвращает адресс функции
   * @return
   */
  [[nodiscard]] Node *getFuncAddr() const {
	return funcAddr;
  }
 private:
  /**
   * @brief Возвращаемая переменная
   */
  Variable *returnVar{};
 public:
  /**
   * @brief возвращает переменную которую надо вернуть
   * @return
   */
  [[nodiscard]] Variable *getReturnVar() const {
	return returnVar;
  }
 public:
  /**
   * @brief позволяет задать возвращаемую переменную
   * @param return_var
   */
  void setReturnVar(Variable *return_var) {
	returnVar = return_var;
  }
  /**
   * @brief Возвращет переменные которые принадлежат функции
   * @return
   */
  [[nodiscard]] std::vector<Variable *> getVariables() const {
	return variables.getVariables();
  }
  /**
   * @brief Возвращает параметры функции
   * @return
   */
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
