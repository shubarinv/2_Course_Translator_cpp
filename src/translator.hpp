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
	*outputFile << "global _start\n";
	writeFunctions();
	writeVariableDeclaration();
	writeMain();
	outputFile->close();
  }

  void writeFunctions() {
	for (auto &func:functions->getFunctions()) {
	  *outputFile << func->getName() << ":\n";
	}
  }

  void writeVariableDeclaration() {
	*outputFile << "section .data\n";
	for (auto &var:localVariables->getVariables()) {
	  *outputFile << "\t" << var->getName() << "\t";
	  switch (var->getType()) {
		case Variable::INTEGER:*outputFile << "RESD\t 1";
		  break;
		case Variable::DOUBLE:
		case Variable::REAL:*outputFile << "RESQ\t 1";
		  break;
		case Variable::SHORTINT:
		case Variable::SMALLINT:
		case Variable::BYTE:*outputFile << "RESB\t 1";
		  break;
		case Variable::LONGINT:*outputFile << "RESQ\t 1";
		  break;
		case Variable::WORD:*outputFile << "RESW\t 1";
		  break;
		case Variable::CHAR:*outputFile << "RESB\t 1";
		  break;
		case Variable::WIDECHAR:*outputFile << "RESD\t 1";
		  break;
		case Variable::STRING:break;
		case Variable::UNKNOWN:throw std::runtime_error("Got var of unknown type");
	  }
	  *outputFile << "; Var of type: " << Variable::varTypeToString(var->getType()) << "\n";
	}
	for (auto &var:globalVariables->getVariables()) {
	  *outputFile << "global " << var->getName() << "\n";
	  *outputFile << var->getName() << ":\n\t";
	  switch (var->getType()) {
		case Variable::INTEGER:*outputFile << "RESD\t";
		  break;
		case Variable::DOUBLE:
		case Variable::REAL:*outputFile << "RESQ\t";
		  break;
		case Variable::SHORTINT:
		case Variable::SMALLINT:
		case Variable::BYTE:*outputFile << "RESB\t";
		  break;
		case Variable::LONGINT:*outputFile << "RESQ\t";
		  break;
		case Variable::WORD:*outputFile << "RESW\t";
		  break;
		case Variable::CHAR:*outputFile << "RESB\t";
		  break;
		case Variable::WIDECHAR:*outputFile << "RESD\t";
		  break;
		case Variable::STRING:break;
		case Variable::UNKNOWN:throw std::runtime_error("Got var of unknown type");
	  }
	  *outputFile << "; Global Var of type: " << Variable::varTypeToString(var->getType()) << "\n\n";
	}
  }

  void writeMain() {
	*outputFile << "_start:\n";

	goThroughTree(tree);
	*outputFile << "mov eax,1 ;1 is the number of the exit system call\n"
				   "mov ebx,0 ;Return code- 0 indicates success, if this is an error condition you might want to return something else\n"
				   "int 0x80 ;Transfer control to the kernel\n";
  }

  void goThroughTree(Node *currentNode) {
	if (currentNode == nullptr)return;

	if (currentNode->type == Node::nodeType::BINOP) {
	  writeBinOPs(currentNode);
	}

	goThroughTree(currentNode->op1);
	goThroughTree(currentNode->op2);
	goThroughTree(currentNode->op3);
	goThroughTree(currentNode->op4);
	for (auto &node : currentNode->list) {
	  goThroughTree(node);
	}
  }

  void writeBinOPs(Node *currentNode) {
	if (currentNode->value == ":=") {
	  *outputFile << "mov ";
	  writeValue(currentNode->op1);
	  *outputFile << ", ";
	  if (currentNode->op2->type != Node::nodeType::BINOP) {
		writeValue(currentNode->op2);
	  } else {
		writeMathOPs(currentNode->op2);
	  }
	  *outputFile << "\n";
	}
  }

  void writeMathOPs(Node *currentNode) {
	if (currentNode->op1 == nullptr || currentNode->op2 == nullptr) {
	  *outputFile << "\n";
	  return;
	}
	if (currentNode->op1->type == Node::nodeType::BINOP) {
	  writeMathOPs(currentNode->op1);
	} else { writeValue(currentNode->op1); }
	*outputFile << " " << currentNode->value << " ";
	if (currentNode->op2->type == Node::nodeType::BINOP) {
	  writeMathOPs(currentNode->op2);
	} else { writeValue(currentNode->op2); }
  }

  void writeConditionals(Node *currentNode) {
	throw NotImplementedException("writeConditionals()");
	if (currentNode == nullptr)return;

	writeConditionals(currentNode->op1);
	writeConditionals(currentNode->op2);
	writeConditionals(currentNode->op3);
	writeConditionals(currentNode->op4);

	for (auto &node : currentNode->list) {
	  writeConditionals(node);
	}
  }

  void writeLoops(Node *currentNode) {
	throw NotImplementedException("writeLoops()");
	if (currentNode == nullptr)return;

	writeLoops(currentNode->op1);
	writeLoops(currentNode->op2);
	writeLoops(currentNode->op3);
	writeLoops(currentNode->op4);

	for (auto &node : currentNode->list) {
	  writeLoops(node);
	}
  }

  void writeInputs(Node *currentNode) {
	throw NotImplementedException("writeInputs()");
	if (currentNode == nullptr)return;

	writeInputs(currentNode->op1);
	writeInputs(currentNode->op2);
	writeInputs(currentNode->op3);
	writeInputs(currentNode->op4);

	for (auto &node : currentNode->list) {
	  writeInputs(node);
	}
  }

  void writeOutputs(Node *currentNode) {
	throw NotImplementedException("writeOutputs()");
	if (currentNode == nullptr)return;

	writeOutputs(currentNode->op1);
	writeOutputs(currentNode->op2);
	writeOutputs(currentNode->op3);
	writeOutputs(currentNode->op4);

	for (auto &node : currentNode->list) {
	  writeOutputs(node);
	}
  }

  /**
   * @brief This func will put val in [] if it is a var, will paste val w\o
   * them otherwise
   * @param currentNode node with value
   */
  void writeValue(Node *currentNode) {
	if (currentNode->type == Node::nodeType::VAR) {
	  *outputFile << "[" << currentNode->value << "]";
	} else if (currentNode->type == Node::nodeType::CONSTANT) {
	  *outputFile << currentNode->value;
	} else if (currentNode->type == Node::nodeType::STR) {
	  throw NotImplementedException("writeValue: String values are not supported");
	}
  }

  /**
   * @brief Will construct string for output file(asm)
   * @param instruction, for example add
   * @param op1
   * @param op2
   * @param comment
   * @example outputConstructor("add","[a]","[b]","a+b") will give you "add [a] [b]; a+b"
   */
  void outputConstructor(const std::string &instruction = "",
						 const std::string &op1 = "", const std::string &op2 = "",
						 const std::string &comment = "") {
	if (!instruction.empty()) *outputFile << instruction << " ";
	if (!op1.empty()) *outputFile << op1 << ", ";
	if (!op2.empty()) *outputFile << op2;
	if (!comment.empty()) *outputFile << "\t;" << comment;
	*outputFile << "\n";
  }
};

#endif //SPO_COMPILER_SRC_TRANSLATOR_HPP_
