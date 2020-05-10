//
// Created by vhundef on 27.04.2020.
//
#ifndef SPO_COMPILER_LEXER_H
#define SPO_COMPILER_LEXER_H

#include <boost/algorithm/string.hpp>
#include <boost/range/algorithm/remove_if.hpp>
#include <boost/algorithm/string/split.hpp>

#include <iostream>
#include <utility>
#include <vector>
#include <sstream>
#include <fstream>
#include <queue>
#include "Token.hpp"

class Lexer {
 private:
  std::queue<Token> tokens;
  std::string program;
  void emplaceToTokens(const std::string &str) {
	emplaceToTokens(str);
	if (tokens.back().getType() == Token::tokenType::Undefined)
	  tokens.pop();
  }
 public:
  void nextToken() {
	tokens.pop();
  }

  Token *getCurrentToken() {
	return &tokens.front();
  }

  [[nodiscard]] std::queue<Token> getTokens() const {
	return tokens;
  }

  void printAllTokens() {
	using namespace std;
	std::queue<Token> tmp = tokens;
	cout << "=========================" << endl;
	int i = 0;
	while (!tmp.empty()) {
	  cout << "[" << i << "] Type: " << Token::typeToString(tmp.front().getType()) << " Value: "
		   << tmp.front().getText()
		   << endl;
	  i++;
	  tmp.pop();
	}
  }

 private:

  static std::string getString(char x) {
	// string class has a constructor
	// that allows us to specify size of
	// string as first parameter and character
	// to be filled in given size as second
	// parameter.
	std::string s(1, x);

	return s;
  }

  void loadFile(const std::string &_filename) {
	std::ifstream file(_filename);
	if (file.is_open()) {
	  std::string fileContent;
	  program = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	}
  }

 public:
  explicit Lexer(const std::string &_filename) {
	loadFile(_filename);
  }

  void insertText(std::string _program) {
	program = std::move(_program);
  }

  void tokenize() {
	using namespace std;
	string lexeme;
	for (int i = 0; i < program.size(); ++i) {

	}
  }
  void printToFile() {
	using namespace std;
	ofstream out("output.txt");
	out << "# Этот файл используется только для демонстрации работоспособности лексического анализатора\n";
	out << "# lexer.printToFiles SHOULD BE REMOVED\n";
	int i = 0;
	std::queue<Token> tmp = tokens;
	while (!tmp.empty()) {
	  out << "[" << i << "] Тип: " << Token::typeToString(tmp.front().getType()) << " Значение: "
		  << tmp.front().getText()
		  << endl;
	  i++;
	  tmp.pop();
	}
	out.close();
  }
};

#endif // SPO_COMPILER_LEXER_H
