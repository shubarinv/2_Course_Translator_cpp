//
// Created by vhundef on 27.04.2020.
//
#ifndef SPO_COMPILER_LEXER_H
#define SPO_COMPILER_LEXER_H

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/range/algorithm/remove_if.hpp>

#include "Token.hpp"
#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <utility>
#include <vector>

class Lexer {
 private:
  std::deque<Token> tokens;
  std::string program;

  static std::string toLowerCase(std::string str) {
	for (auto &c: str) c = tolower(c);
	return str;
  }
 public:
  void nextToken() {
	tokens.pop_front();
	std::cout << "Current token: " << tokens.front().getText() << std::endl;
  }

  Token *getCurrentToken() { return &tokens.front(); }

  [[nodiscard]] std::deque<Token> getTokens() const { return tokens; }

  void printAllTokens() {
	using namespace std;
	std::deque<Token> tmp = tokens;
	cout << "=========================" << endl;
	int i = 0;
	while (!tmp.empty()) {
	  cout << "[" << i
		   << "] Type: " << Token::typeToString(tmp.front().getType())
		   << " Value: " << tmp.front().getText() << endl;
	  i++;
	  tmp.pop_front();
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
	  program = std::string((std::istreambuf_iterator<char>(file)),
							std::istreambuf_iterator<char>());
	}
  }

  static bool isWordSep(char character) {
	return character == '\0' || character == ' ' || character == '\t' || character == '\n' || character == '\r';
  }

 public:
  explicit Lexer(const std::string &_filename) { loadFile(_filename); }

  void insertText(std::string _program) { program = std::move(_program); }

  void tokenize() {
	using namespace std;
	string lexeme;
	for (int i = 0; i < program.size(); ++i) {
	  if (program[i] == '\r' || program[i] == '\t' || program[i] == '\0') //игнорируем всякую ерунду
		continue;
	  else if (program[i] == '\n' || program[i] == ' ') { // если перенос строки или пробел добавляем лексему
		if (!lexeme.empty() && Token::determineTokenType(toLowerCase(lexeme)) != Token::tokenType::Undefined) {
		  tokens.emplace_back(toLowerCase(lexeme));
		  lexeme = "";
		}
	  } else if (program[i] == '\'' || program[i] == '"') { ///<if string
		if (!lexeme.empty()) {
		  tokens.emplace_back(toLowerCase(lexeme));
		  lexeme = "";
		}
		lexeme += program[i];
		i++;
		while (program.size() > i) {
		  if (program[i] == '\'' || program[i] == '"') break;
		  lexeme += program[i];
		  i++;
		}
		lexeme += program[i];
		if (!lexeme.empty()) {
		  tokens.emplace_back(toLowerCase(lexeme));
		  lexeme = "";
		}
	  } else if (Token::determineTokenType(getString(program[i])) != Token::determineTokenType(getString(program[i + 1]))) {
		lexeme += program[i];
		if ((Token::determineTokenType(getString(program[i]) + getString(program[i + 1])) == Token::tokenType::Assignment ||
			Token::determineTokenType(getString(program[i]) + getString(program[i + 1])) == Token::tokenType::Comparison) &&
			getString(program[i]) + getString(program[i + 1]) != "in") {
		  lexeme += program[i + 1];
		  i++;
		}
		tokens.emplace_back(toLowerCase(lexeme));
		lexeme = "";
	  } else
		lexeme += program[i];
	}
	if (!lexeme.empty() && Token::determineTokenType(toLowerCase(lexeme)) != Token::tokenType::Undefined) {
	  tokens.emplace_back(toLowerCase(lexeme));
	  lexeme = "";
	}
	printAllTokens();
	cout << "\n\n---- TOKENIZATION DONE -----\n\n" <<
		 endl;
  }
  void pushToFront(const std::string &token) {
	tokens.emplace_front(token);
  }
  void printToFile() {
	using namespace std;
	ofstream out("output.txt");
	out << "# Этот файл используется только для демонстрации работоспособности "
		   "лексического анализатора\n";
	out << "# lexer.printToFiles SHOULD BE REMOVED\n";
	int i = 0;
	std::deque<Token> tmp = tokens;
	while (!tmp.empty()) {
	  out << "[" << i << "] Тип: " << Token::typeToString(tmp.front().getType())
		  << " Значение: " << tmp.front().getText() << endl;
	  i++;
	  tmp.pop_front();
	}
	out.close();
  }
};

#endif // SPO_COMPILER_LEXER_H
