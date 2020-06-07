//
// Created by Vladimir Shubarin on 06.06.2020.
//

#ifndef SPO_COMPILER_SRC_TRANSLATOR_HPP_
#define SPO_COMPILER_SRC_TRANSLATOR_HPP_

#include "Parser.h"
class Translator {
  Node *tree{};
  VariableTable *localVariables;
  VariableTable *globalVariables;
  FunctionTable *functions;
  std::string program;
  std::ofstream *outputFile;
 public:
  explicit Translator(SemanticAnalyzer *_semanticAnalyzer) {
	if (_semanticAnalyzer == nullptr) {
	  throw std::runtime_error("ERROR: Translator::Translator(SemanticAnalyzer*) SemanticAnalyzer is NULL");
	}
	localVariables = _semanticAnalyzer->getVariables();
	globalVariables = _semanticAnalyzer->getGlobalVariables();
	functions = _semanticAnalyzer->getFunctions();
	tree = _semanticAnalyzer->getTree();
	outputFile = new std::ofstream;
	translate();
  }
  void translate() {
	outputFile->open("output.asm");
	*outputFile<<"global _start\n";
	writeFunctions();
	outputFile->close();
  }
  void writeFunctions(){
    for(auto &func:functions->getFunctions()){
      *outputFile<<func->getName()<<":\n";
    }
  }
  std::string getProgram() {
	return program;
  }
};

#endif //SPO_COMPILER_SRC_TRANSLATOR_HPP_
