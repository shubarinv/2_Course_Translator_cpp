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
	UNKNOWN,
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
  static varType determineVarType(const std::string& _value){
    if(_value=="integer")
      return varType::INTEGER;
    else if(_value=="double")
      return varType::DOUBLE;
    else if(_value=="real")
      return varType::REAL;
    else if(_value=="shortint")
      return varType::SHORTINT;
    else if(_value=="smallint")
      return varType::SMALLINT;
    else if(_value=="byte")
      return varType::BYTE;
    else if(_value=="longint")
      return varType::LONGINT;
    else if(_value=="char")
      return varType::CHAR;
    else if(_value=="string")
      return varType::STRING;
    else if(_value=="widechar")
      return varType::WIDECHAR;
    else return varType::UNKNOWN;
  }
  static std::string varTypeToString(varType type){
    switch(type){
      case varType::INTEGER: return "integer";
      case varType::DOUBLE: return "double";
      case varType::REAL: return "real";
      case varType::SHORTINT: return "shortint";
      case varType::SMALLINT:return "smallint";
      case varType::BYTE: return "byte";
      case varType::LONGINT: return "longint";
      case varType::CHAR: return "char";
      case varType::STRING: return "string";
      case varType::WIDECHAR: return "widechar";
      case varType::UNKNOWN: return "unknown";
	  case WORD:return "word";
	}
  }

};

#endif //SPO_COMPILER_SRC_VARIABLE_HPP_
