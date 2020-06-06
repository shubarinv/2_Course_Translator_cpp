//
// Created by Vladimir Shubarin on 06.06.2020.
//

#ifndef SPO_COMPILER_SRC_TRANSLATOR_HPP_
#define SPO_COMPILER_SRC_TRANSLATOR_HPP_

#include "Parser.h"
class Translator {
  Parser *parser{};
  Node *tree{};
  std::string program;
 public:
  explicit Translator(Parser *_parser) {
	parser = _parser;
	tree = parser->GetTree();
  }
  void translate() {

  }
  std::string getProgram() {
	return program;
  }
};

#endif //SPO_COMPILER_SRC_TRANSLATOR_HPP_
