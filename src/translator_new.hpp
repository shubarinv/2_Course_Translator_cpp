//
// Created by Vladimir Shubarin on 04.07.2020.
//

#ifndef SPO_COMPILER_SRC_TRANSLATOR_NEW_HPP_
#define SPO_COMPILER_SRC_TRANSLATOR_NEW_HPP_

#include "variable_table.hpp"
#include "function_table.hpp"
#include "Node.hpp"
#include "semantic_analyzer.hpp"
class Translator_new {
  Node *tree{};
  VariableTable *localVariables;
  VariableTable *globalVariables;
  FunctionTable *functions;
  std::string program;
  std::string asmHeader;
  std::string asmData;
  std::string asmCode;
  std::string asmBSS;
  std::string afterMain;

 public:
  explicit Translator_new(SemanticAnalyzer *_semanticAnalyzer) {
	if (_semanticAnalyzer == nullptr) {
	  throw std::runtime_error("ERROR: Translator::Translator(SemanticAnalyzer*) SemanticAnalyzer is NULL");
	}
	localVariables = _semanticAnalyzer->getVariables();
	globalVariables = _semanticAnalyzer->getGlobalVariables();
	functions = _semanticAnalyzer->getFunctions();
	tree = _semanticAnalyzer->getTree();
	translate();
  }

  void writeHeader() {

  }
  void translate() {
	writeHeader();
	writeVariables();
	goThroughTree(tree);
	program += asmHeader;
	if (!asmData.empty()) {
	  program += ".data\n";
	  program += asmData;
	}
	program += "global _main\n";
	program += "section .text\n";
	program += "_main:\n";
	program += asmCode;
	program += "ret\n";
	if (!afterMain.empty()) {
	  program += afterMain;
	}
	if (!asmBSS.empty()) {
	  program += "section .bss\n";
	  program += asmBSS;
	}

	std::ofstream outputFile;
	outputFile.open("output.asm");
	outputFile << program;
	outputFile.close();
  }

  void goThroughTree(Node *currentNode) {
	if (currentNode == nullptr)return;
	if (currentNode->type == Node::nodeType::FUNCCALL) {
	  throw NotImplementedException("Sorry functions are not supported yet");
	}
	if (currentNode->type == Node::nodeType::OUTPUT) { throw NotImplementedException(); }
	if (currentNode->type == Node::nodeType::INPUT) { throw NotImplementedException(); }
	if (currentNode->type == Node::nodeType::FOR_LOOP) { throw NotImplementedException(); }
	if (currentNode->type == Node::nodeType::PROCEDURE) { throw NotImplementedException(); }
	if (currentNode->type == Node::nodeType::FUNCTION) { throw NotImplementedException(); }
	if (currentNode->type == Node::nodeType::IF) { throw NotImplementedException(); }
	if (currentNode->type == Node::nodeType::BINOP) {
	  writeBinOP(currentNode);
	}
	goThroughTree(currentNode->op1);
	goThroughTree(currentNode->op2);
	goThroughTree(currentNode->op3);
	goThroughTree(currentNode->op4);
  }

  void writeVariables() {
	for (auto &var:localVariables->getVariables()) {
	  asmBSS += var->getName() + ":\t";
	  switch (var->getType()) {
		case Variable::INTEGER:asmBSS += "RESD\t 1";
		  break;
		case Variable::DOUBLE:
		case Variable::REAL:asmBSS += "RESQ\t 1";
		  break;
		case Variable::SHORTINT:
		case Variable::SMALLINT:
		case Variable::BYTE:asmBSS += "RESB\t 1";
		  break;
		case Variable::LONGINT:asmBSS += "RESQ\t 1";
		  break;
		case Variable::WORD:asmBSS += "RESW\t 1";
		  break;
		case Variable::CHAR:asmBSS += "RESB\t 1";
		  break;
		case Variable::WIDECHAR:asmBSS += "RESD\t 1";
		  break;
		case Variable::STRING:asmBSS += "RESB\t 1; might need to reallocate later";
		case Variable::UNKNOWN:throw std::runtime_error("Got var of unknown type");
	  }
	  asmBSS += "\n";
	}
  }
  void writeBinOP(Node *currentNode) {
	if (currentNode->value == ":=") {
	  asmCode += "mov r8,";
	  if (currentNode->op2->type != Node::nodeType::BINOP) {
		asmCode += writeValue(currentNode->op2) + "\n";
	  }
	  asmCode += "mov [rel " + writeValue(currentNode->op1) + "] ,r8\n";
	}
  }

  /**
 * @brief This func will put val in [] if it is a var, will paste val w\o
 * them otherwise
 * @param currentNode node with value
 */
  std::string writeValue(Node *currentNode) {
	if (currentNode->type == Node::nodeType::VAR) {
	  return "" + currentNode->value + "";
	} else if (currentNode->type == Node::nodeType::CONSTANT || currentNode->type == Node::nodeType::STR) {
	  return currentNode->value;
	}
  }
};

#endif //SPO_COMPILER_SRC_TRANSLATOR_NEW_HPP_
