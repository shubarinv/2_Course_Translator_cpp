//
// Created by vhundef on 27.04.2020.
//
#include <stdexcept>
#include <string>
#include <utility>
#include "Token.hpp"

class NotImplementedException : public std::logic_error {
 private:

  std::string _text;

 public:
  NotImplementedException() : NotImplementedException("Not Implemented", __FUNCTION__) {
  }
  explicit NotImplementedException(const char *message) : NotImplementedException(message, __FUNCTION__) {}

  virtual const char *what() const noexcept {
	return _text.c_str();
  }

  NotImplementedException(const char *message, const char *function) : std::logic_error("Not Implemented") {
	_text = message;
	_text += " : ";
	_text += function;
  }
};

class ParsingError : public std::exception {
 private:
  std::string message;
  std::string got;
 public:
  ParsingError(std::string expected, std::string _got) : message(std::move(expected)), got(std::move(_got)) {
	std::string error = "Expected token of type: ";
	error += message;
	error += ", but got: ";
	error += Token::typeToString(Token::determineTokenType(got));
	error += "(" + got + ")";
	message = error;
  }
  virtual const char *what() const throw() {
	return message.c_str();
  }
};

class VariableNotDefinedError : public std::exception {
 private:
  std::string message;
 public:
  explicit VariableNotDefinedError(std::string expected, const std::string &funcName = "") : message(std::move(expected)) {
	std::string error = "Variable with name: \'";
	error += message;
	error += "\' was not defined";
	if (!funcName.empty()) {
	  error += " in function named: " + funcName;
	}
	message = error;
  }
  virtual const char *what() const throw() {
	return message.c_str();
  }
};


class FunctionNotDefinedError : public std::exception {
 private:
  std::string message;
 public:
  explicit FunctionNotDefinedError(std::string expected) : message(std::move(expected)) {
	std::string error = "Variable with name: \'";
	error += message;
	error += "\' was not defined";
	message = error;
  }
  virtual const char *what() const throw() {
	return message.c_str();
  }
};

class TypeMismatchError : public std::exception {
 private:
  std::string message;
 public:
  explicit TypeMismatchError(const std::string &type1, const std::string &type2) : message(" ") {
	std::string error = "Var type mismatch: expected " + type1 + " got "+ type2;
	message = error;
  }
  virtual const char *what() const throw() {
	return message.c_str();
  }
};

class VariableShadowing : public std::exception {
 private:
  std::string message;
 public:
  explicit VariableShadowing(const std::string &type1) : message(type1) {
	std::string error = "Var shadows previous declaration: " + type1;
	message = error;
  }
  virtual const char *what() const throw() {
	return message.c_str();
  }
};

class UnexpectedParameterType : public std::exception {
 private:
  std::string message;
 public:
  explicit UnexpectedParameterType(const std::string &funcName, const std::string &type1, const std::string &type2) : message(" ") {
	std::string error = "In function " + funcName + ". Param type mismatch: expected " + type1 + " got " + type2;
	message = error;
  }
  virtual const char *what() const throw() {
	return message.c_str();
  }
};

class TooManyArgumentsException : public std::exception {
 private:
  std::string message;
 public:
  explicit TooManyArgumentsException(const std::string &funcName, int expected, int got) : message(" ") {
	std::string error = "In function " + funcName + " expected " + std::to_string(expected) + " parameters, but got " + std::to_string(got);
	message = error;
  }
  virtual const char *what() const throw() {
	return message.c_str();
  }
};

class TooFewArgumentsException : public std::exception {
 private:
  std::string message;
 public:
  explicit TooFewArgumentsException(const std::string &funcName, int expected, int got) : message(" ") {
	std::string error = "In function " + funcName + " expected " + std::to_string(expected) + " parameters, but got " + std::to_string(got);
	message = error;
  }
  virtual const char *what() const throw() {
	return message.c_str();
  }
};