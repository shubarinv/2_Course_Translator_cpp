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

 public:
  /**
   * @brief Переходит к следущему токену
   */
  void nextToken() {
	tokens.pop_front();
	std::cout << "Current token: " << tokens.front().getText() << std::endl;
  }

  /**
   * @brief Возвращет текущий токен
   * @return возвращет переменную типа Token
   */
  Token *getCurrentToken() { return &tokens.front(); }

  /**
   * @brief Возвращет все токены
   * @return возвращет все токены в двусторонней очереди
   */
  [[nodiscard]] std::deque<Token> getTokens() const { return tokens; }

  /**
   * @brief Выводит все токены и их типы в консоль
   */
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
  /**
   * @brief делает из char string
   * @param x char
   * @return строка полученая из char
   */
  static std::string getString(char x) {
	// string class has a constructor
	// that allows us to specify size of
	// string as first parameter and character
	// to be filled in given size as second
	// parameter.
	std::string s(1, x);

	return s;
  }

  /**
   * @brief Читает указанный файл
   * @param _filename имя файла
   */
  void loadFile(const std::string &_filename) {
	std::ifstream file(_filename);
	if (file.is_open()) {
	  std::string fileContent;
	  program = std::string((std::istreambuf_iterator<char>(file)),
							std::istreambuf_iterator<char>());
	}
  }

  /**
   * @brief Проверяет является ли символ разделителем строки
   * @param character символ
   * @return true -если является, иначе false
   */
  static bool isWordSep(char character) {
	return character == '\0' || character == ' ' || character == '\t' || character == '\n' || character == '\r';
  }

 public:
  explicit Lexer(const std::string &_filename) { loadFile(_filename); }

  /**
   * @brief Позволяет переписать текст который был прочитан из файла, текстом который указан параметром этой функции
   * @param _program текст которым будет перезаписан прочитанный из файла текст
   */
  void insertText(std::string _program) { program = std::move(_program); }

  /**
   * @brief разбивает строку на токены
   */
  void tokenize() {
	using namespace std;
	string lexeme;
	for (int i = 0; i < program.size(); ++i) {
	  if (program[i] == '\r' || program[i] == '\t' || program[i] == '\0') //игнорируем всякую ерунду
		continue;
	  else if (program[i] == '\n' || program[i] == ' ') { // если перенос строки или пробел добавляем лексему
		if (!lexeme.empty() && Token::determineTokenType(lexeme) != Token::tokenType::Undefined) {
		  tokens.emplace_back(lexeme);
		  lexeme = "";
		}
	  } else if (program[i] == '\'' || program[i] == '"') { ///<if string
		if (!lexeme.empty()) {
		  tokens.emplace_back(lexeme);
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
		  tokens.emplace_back(lexeme);
		  lexeme = "";
		}
	  } else if (Token::determineTokenType(getString(program[i])) != Token::determineTokenType(getString(program[i + 1]))) {
		lexeme += program[i];
		if (Token::determineTokenType(lexeme + getString(program[i + 1])) == Token::tokenType::Id) { continue; }
		if ((Token::determineTokenType(getString(program[i]) + getString(program[i + 1])) == Token::tokenType::Assignment ||
			Token::determineTokenType(getString(program[i]) + getString(program[i + 1])) == Token::tokenType::Comparison) &&
			getString(program[i]) + getString(program[i + 1]) != "in") {

		  lexeme += program[i + 1];
		  i++;
		}
		tokens.emplace_back(lexeme);
		lexeme = "";
	  } else
		lexeme += program[i];
	}
	if (!lexeme.empty() && Token::determineTokenType(lexeme) != Token::tokenType::Undefined) {
	  tokens.emplace_back(lexeme);
	  lexeme = "";
	}
	cout << "\n\n---- TOKENIZATION DONE -----\n\n" << endl;
  }

  /**
   * @brief позволяет вставить токен вперед очереди
   * @param token токен который надо поставить в начало очереди
   */
  void pushToFront(const std::string &token) {
	tokens.emplace_front(token);
  }
  /**
   * @brief печатает токены в файл
   */
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
