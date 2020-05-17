//
// Created by Vladimir Shubarin on 17.05.2020.
//

#ifndef SPO_COMPILER_SRC_SEMANTIC_ANALYSER_HPP_
#define SPO_COMPILER_SRC_SEMANTIC_ANALYSER_HPP_

#include "Node.hpp"
#include "Parser.h"
class SemanticAnalyser {
 private:
  Node*tree{};
  Parser*parser{};
 public:
  explicit SemanticAnalyser(const std::string &_filename) {
	parser = new Parser(_filename);
	//parser->parse();
	tree=parser->GetTree();
  }
};

#endif //SPO_COMPILER_SRC_SEMANTIC_ANALYSER_HPP_
