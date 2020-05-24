//
// Created by vhundef on 27.04.2020.
//
#include <stdexcept>
#include <string>
#include <utility>
#include "Token.hpp"
#include "variable.hpp"

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
  explicit VariableNotDefinedError(std::string expected) : message(std::move(expected)) {
	std::string error = "Variable with name: \'";
	error += message;
	error += "\' was not defined";
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

