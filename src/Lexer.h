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
  std::queue<Token> tokens;
  std::string program;
  void emplaceToTokens(const std::string &str) {
	emplaceToTokens(str);
	if (tokens.back().getType() == Token::tokenType::Undefined)
	  tokens.pop();
  }

 public:
  void nextToken() { tokens.pop(); }

  Token *getCurrentToken() { return &tokens.front(); }

  [[nodiscard]] std::queue<Token> getTokens() const { return tokens; }

  void printAllTokens() {
	using namespace std;
	std::queue<Token> tmp = tokens;
	cout << "=========================" << endl;
	int i = 0;
	while (!tmp.empty()) {
	  cout << "[" << i
		   << "] Type: " << Token::typeToString(tmp.front().getType())
		   << " Value: " << tmp.front().getText() << endl;
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
	  program = std::string((std::istreambuf_iterator<char>(file)),
							std::istreambuf_iterator<char>());
	}
  }

 public:
  explicit Lexer(const std::string &_filename) { loadFile(_filename); }

  void insertText(std::string _program) { program = std::move(_program); }

  void tokenize() {
	using namespace std;
	string lexeme;
	for (int i = 0; i < program.size(); ++i) {
	  if (Token::determineTokenType(lexeme) != Token::tokenType::Undefined &&
		  Token::determineTokenType(lexeme) != Token::tokenType::Id &&
		  Token::determineTokenType(lexeme) !=
			  Token::tokenType::Num) { ///< if lexeme is known by Token
		tokens.emplace(lexeme);
		lexeme = "";
	  } else if (program[i] == '\r' || program[i + 1] == '\t' || program[i + 1] == '\0') //игнорируем всякую ерунду
		continue;
	  else if (program[i] == '\n' || program[i] == ' ') { // если перенос строки или пробел добавляем лексему
		if (!lexeme.empty() &&
			Token::determineTokenType(lexeme) != Token::tokenType::Undefined) {
		  tokens.emplace(lexeme);
		  lexeme = "";
		}
	  } else if (Token::determineTokenType(getString(program[i])) != Token::tokenType::Undefined && ///< если текущий символ мб ток.
		  Token::determineTokenType(getString(program[i])) != Token::tokenType::Id &&
		  Token::determineTokenType(getString(program[i])) != Token::tokenType::Num) {
		if (Token::determineTokenType(getString(program[i]) + getString(program[i + 1])) != Token::tokenType::Undefined &&
			Token::determineTokenType(getString(program[i]) + getString(program[i + 1])) != Token::tokenType::Id &&
			Token::determineTokenType(getString(program[i]) + getString(program[i + 1])) != Token::tokenType::Num) {
		  if (!lexeme.empty()) {
			tokens.emplace(lexeme);
			lexeme = "";
		  }
		  tokens.emplace(getString(program[i]) + getString(program[i + 1]));
		} else {
		  if (!lexeme.empty()) {
			tokens.emplace(lexeme);
			lexeme = "";
		  }
		  tokens.emplace(getString(program[i]));
		}
	  } else if (program[i] == '\'' || program[i] == '"') { ///<if string
		if (!lexeme.empty()) {
		  tokens.emplace(lexeme);
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
		  tokens.emplace(lexeme);
		  lexeme = "";
		}
	  } else {
		lexeme += getString(program[i]);
	  }
	}
	if (!lexeme.empty() && Token::determineTokenType(lexeme) != Token::tokenType::Undefined) {
	  tokens.emplace(lexeme);
	  lexeme = "";
	}
  }
  void printToFile() {
	using namespace std;
	ofstream out("output.txt");
	out << "# Этот файл используется только для демонстрации работоспособности "
		   "лексического анализатора\n";
	out << "# lexer.printToFiles SHOULD BE REMOVED\n";
	int i = 0;
	std::queue<Token> tmp = tokens;
	while (!tmp.empty()) {
	  out << "[" << i << "] Тип: " << Token::typeToString(tmp.front().getType())
		  << " Значение: " << tmp.front().getText() << endl;
	  i++;
	  tmp.pop();
	}
	out.close();
  }
};

#endif // SPO_COMPILER_LEXER_H
