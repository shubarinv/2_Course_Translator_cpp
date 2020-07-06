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
 private:
  int fmtsNum = 0;
  int msgNum = 0;
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
	  program += "section .data\n";
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
	if (currentNode->type == Node::nodeType::OUTPUT) {
	  if (asmHeader.empty()) {
		asmHeader += "extern\t_printf\t\t; the C function, to be called\n";
	  }
	  asmData += "fmt" + std::to_string(fmtsNum) + " db \"" + printfFormatGenerator(currentNode->op1) + "\",10,0\n";
	  if (currentNode->op1->type != Node::nodeType::EXPR) {
		asmCode += "push rbp\n";
		asmCode += "mov rdi,fmt" + std::to_string(fmtsNum) + "\n";
		if (currentNode->op1->type != Node::nodeType::STR)
		  asmCode += "mov rsi," + writeValue(currentNode->op1) + "\n";
		else {
		  asmData += "msg" + std::to_string(msgNum) + " db " + currentNode->op1->value + ",0\n";
		  //asmData+="len"+std::to_string(msgNum)+" equ $ - msg"+std::to_string(msgNum)+"\n";
		  asmCode += "mov rsi, msg" + std::to_string(msgNum) + "\n";

		}
		asmCode += "mov rax,0\n";
		asmCode += "call _printf\n";
		asmCode += "pop rbp\n";
	  } else {}
	  fmtsNum++;
	}
	if (currentNode->type == Node::nodeType::INPUT) { throw NotImplementedException(); }
	if (currentNode->type == Node::nodeType::FOR_LOOP) { throw NotImplementedException(); }
	if (currentNode->type == Node::nodeType::PROCEDURE) { throw NotImplementedException(); }
	if (currentNode->type == Node::nodeType::FUNCTION) { throw NotImplementedException(); }
	if (currentNode->type == Node::nodeType::IF) { throw NotImplementedException(); }
	if (currentNode->type == Node::nodeType::BINOP) {
	  writeBinOP(currentNode);
	  return; // since this is a binary operation, we don't expect that there will be anything other than op1 and op2, which will already
	  // be written by the time program returns here
	}
	goThroughTree(currentNode->op1);
	goThroughTree(currentNode->op2);
	goThroughTree(currentNode->op3);
	goThroughTree(currentNode->op4);

	for (auto &node : currentNode->list) {
	  goThroughTree(node);
	}
  }

  std::string printfFormatGenerator(Node *currentNode, std::string *prevStr = nullptr) {
	std::string tmp;
	if (currentNode->type != Node::nodeType::EXPR) {
	  if (currentNode->type == Node::nodeType::VAR) {
		switch (localVariables->getVarByName(currentNode->value)->getType()) {
		  case Variable::INTEGER:
			if (prevStr == nullptr) tmp = "%i";
			else *prevStr += "%i";
			return tmp;
		  case Variable::DOUBLE:
		  case Variable::REAL:
			if (prevStr == nullptr) tmp = "%f";
			else *prevStr += "%f";
			return tmp;
		  case Variable::SHORTINT:
		  case Variable::SMALLINT:
		  case Variable::BYTE:
			if (prevStr == nullptr) tmp = "%hd";
			else *prevStr += "%hd";
			return tmp;
		  case Variable::LONGINT:
			if (prevStr == nullptr)tmp = "%lli";
			else *prevStr += "%lli";
			return tmp;
		  case Variable::WORD:
			if (prevStr == nullptr) tmp = "%s";
			else *prevStr += "%s";
			return tmp;
		  case Variable::CHAR:
			if (prevStr == nullptr)tmp = "%c";
			else *prevStr += "%c";
			return tmp;
		  case Variable::WIDECHAR:
			if (prevStr == nullptr)tmp = "%s";
			else *prevStr += "%s";
			return tmp;
		  case Variable::STRING:
			if (prevStr == nullptr)tmp = "%s";
			else *prevStr += "%s";
			return tmp;
		  case Variable::UNKNOWN:break;
		}
	  } else if (currentNode->type == Node::nodeType::CONSTANT) {
		if (prevStr == nullptr) tmp = "%i";
		else *prevStr += "%i";
		return tmp;
	  } else if (currentNode->type == Node::nodeType::STR) {
		if (prevStr == nullptr)tmp = "%s";
		else *prevStr += "%s";
		return tmp;
	  }
	} else {
	  for (auto &param : currentNode->op1->list) {
		tmp += printfFormatGenerator(param) + " ";
	  }
	}
	return tmp;
  }
  void writeVariables() {
	for (auto &var:localVariables->getVariables()) {
	  asmBSS += var->getName() + ":\t";
	  switch (var->getType()) {
		case Variable::INTEGER:asmBSS += "RESQ\t 1";
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
	  } else {
		writeBinOPs(currentNode->op2);
	  }
	  asmCode += "mov " + writeValue(currentNode->op1) + " ,r8\n";
	} else {
	  writeBinOPs(currentNode);
	}
  }
  void writeBinOPs(Node *currentNode) {
	if (currentNode->op1 == nullptr || currentNode->op2 == nullptr) {
	  asmCode += "\n";
	  return;
	}
	if (currentNode->op1->type == Node::nodeType::BINOP) {
	  writeBinOPs(currentNode->op1);
	} else { asmCode += writeValue(currentNode->op1) + "\n"; }
	if (currentNode->value == "+") {
	  asmCode += "add r8, ";
	}
	if (currentNode->value == "-") {
	  asmCode += "sub r8, ";
	}
	if (currentNode->value == "*") {
	  asmCode += "mov r9, ";
	}
	if (currentNode->value == "/") {
	  asmCode += "mov r9, ";
	}
	if (currentNode->op2->type == Node::nodeType::BINOP) {
	  writeBinOPs(currentNode->op2);
	} else { asmCode += writeValue(currentNode->op2) + "\n"; }
	if (currentNode->value == "*") { asmCode += "imul r8,r9\n"; }
	if (currentNode->value == "/") { asmCode += "\nxor rdx,rdx\nmov rax,r8\nidiv r9\nmov r8,rax\n"; }
  }
  /**
 * @brief This func will put val in [] if it is a var, will paste val w\o
 * them otherwise
 * @param currentNode node with value
 */
  std::string writeValue(Node *currentNode) {
	if (currentNode->type == Node::nodeType::VAR) {
	  return "[rel " + currentNode->value + "]";
	} else if (currentNode->type == Node::nodeType::CONSTANT || currentNode->type == Node::nodeType::STR) {
	  return currentNode->value;
	}
  }
};

#endif //SPO_COMPILER_SRC_TRANSLATOR_NEW_HPP_
