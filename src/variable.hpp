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
  Variable(std::string _name, varType _type, bool bIsParam = false) : name(std::move(_name)), type(_type), isParam(bIsParam) {}

 private:
  std::string name;
  varType type;
  bool isParam = false;
 public:
  bool getIsParam() const {
	return isParam;
  }
 public:
  [[nodiscard]] const std::string &getName() const {
	return name;
  }
  [[nodiscard]] varType getType() const {
	return type;
  }
  static std::string toLowerCase(std::string str) {
	for (auto &c: str) c = tolower(c);
	return str;
  }
  static varType determineVarType(std::string _value) {
	_value = toLowerCase(_value);
	if (_value == "integer")
	  return varType::INTEGER;
	else if (_value == "double")
	  return varType::DOUBLE;
	else if (_value == "real")
	  return varType::REAL;
	else if (_value == "shortint")
	  return varType::SHORTINT;
	else if (_value == "smallint")
	  return varType::SMALLINT;
	else if (_value == "byte")
	  return varType::BYTE;
	else if (_value == "longint")
	  return varType::LONGINT;
	else if (_value == "char")
	  return varType::CHAR;
	else if (_value == "string")
	  return varType::STRING;
	else if (_value == "widechar")
	  return varType::WIDECHAR;
	else if (_value == "num")
	  return varType::REAL;
	else if (_value == "const") {
	  return varType::INTEGER;
	} else return varType::UNKNOWN;
  }
  static std::string varTypeToString(varType type) {
	switch (type) {
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
	return "Undefined";
  }
  static bool areTypesCompatible(varType type1, varType type2) {
	if (type1 == varType::INTEGER ||
		type1 == varType::DOUBLE ||
		type1 == varType::REAL ||
		type1 == varType::SHORTINT ||
		type1 == varType::SMALLINT ||
		type1 == varType::LONGINT ||
		type1 == varType::BYTE) {
	  return type2 == varType::INTEGER ||
		  type2 == varType::DOUBLE ||
		  type2 == varType::REAL ||
		  type2 == varType::SHORTINT ||
		  type2 == varType::SMALLINT ||
		  type2 == varType::LONGINT ||
		  type2 == varType::BYTE;
	}
	if (type1 == varType::STRING) {
	  return type2 == varType::STRING || type2 == varType::CHAR || type2 == varType::WIDECHAR || type2 == varType::WORD;
	}
	if (type1 == varType::CHAR || type1 == varType::WIDECHAR) {
	  return type2 == varType::CHAR || type2 == varType::WIDECHAR;
	}
	return false;
  }

};

#endif //SPO_COMPILER_SRC_VARIABLE_HPP_
