//
// Created by Vladimir Shubarin on 17.05.2020.
//

#ifndef SPO_COMPILER_SRC_VARIABLE_HPP_
#define SPO_COMPILER_SRC_VARIABLE_HPP_

#include <string>
#include <utility>
class Variable {
 public:
  enum varType {
	INTEGER,
	DOUBLE,
	REAL,
	SHORTINT,
	SMALLINT,
	BYTE,
	LONGINT,
	WORD,
	CHAR,
	WIDECHAR,
	STRING,
  };
  Variable(std::string _name, varType _type) : name(std::move(_name)), type(_type) {}

 private:
  std::string name;
  varType type;
 public:
  [[nodiscard]] const std::string &getName() const {
	return name;
  }
  [[nodiscard]] varType getType() const {
	return type;
  }

};

#endif //SPO_COMPILER_SRC_VARIABLE_HPP_
