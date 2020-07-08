//
// Created by Vladimir Shubarin on 04.07.2020.
//

#ifndef SPO_COMPILER_SRC_TRANSLATOR_HPP_
#define SPO_COMPILER_SRC_TRANSLATOR_HPP_

#include "variable_table.hpp"
#include "function_table.hpp"
#include "Node.hpp"
#include "semantic_analyzer.hpp"
class Translator {
  Node *tree{};
  VariableTable *localVariables;
  VariableTable *globalVariables;
  FunctionTable *functions;
  std::string program;
  std::string asmHeader;
  std::string asmData;
  std::string asmBeforeMain;
  std::string asmCode;
  std::string asmBSS;
  std::string afterMain;
 private:
  int inputFmtsNum = 0;
  int outputFmtsNum = 0;
  int msgNum = 0;
  int loopsNum = 0;
  int ifsNum = 0;
 public:
  explicit Translator(SemanticAnalyzer *_semanticAnalyzer) {
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

  /*
 * Erase all Occurrences of given substring from main string.
 */
  static void eraseAllSubStr(std::string &mainStr, const std::string &toErase) {
	size_t pos = std::string::npos;
	// Search for the substring in string in a loop untill nothing is found
	while ((pos = mainStr.find(toErase)) != std::string::npos) {
	  // If found then erase it from string
	  mainStr.erase(pos, toErase.length());
	}
  }

  /**
 * @brief определяет тип переменной
 * @param currentNode
 * @param func функция в которой переменная находится
 * @return
 */
  Variable::varType getVariableType(Node *currentNode, Function *func = nullptr) {
	if (func != nullptr) {
	  if (func->variables.isVarDefined(currentNode->value)) {
		return func->variables.getVarType(currentNode->value);
	  }
	  if (func->getReturnVar()->getName() == currentNode->value) {
		return func->getReturnVar()->getType();
	  }
	} else {
	  if (localVariables->isVarDefined(currentNode->value)) { return localVariables->getVarType(currentNode->value); }
	}
	if (globalVariables->isVarDefined(currentNode->value)) {
	  return globalVariables->getVarType(currentNode->value);
	}
	if (Token::determineTokenType(currentNode->value) != Token::tokenType::Id) {
	  return Variable::determineVarType(Token::typeToString(Token::determineTokenType(currentNode->value)));
	}
	return Variable::varType::UNKNOWN;
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
	program += "global _main\ndefault rel\n";
	program += "section .text\n";
	program += asmBeforeMain;
	program += "_main:\n";
	program += asmCode;
	program += "end:\nmov rax,0\nret\n";
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
		asmHeader += "extern\t_printf,_scanf\t\t; the C function, to be called\n";
	  }
	  asmData += "fmt" + std::to_string(outputFmtsNum) + " db \"" + printfFormatGenerator(currentNode->op1) + "\",10,0\n";
	  if (currentNode->op1->type != Node::nodeType::EXPR) {
		asmCode += "push rbp\n";
		asmCode += "mov rdi,fmt" + std::to_string(outputFmtsNum) + "\n";
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
	  } else {
		// Todo add support for writeln(a,b);
	  }
	  outputFmtsNum++;
	  msgNum++;
	}
	if (currentNode->type == Node::nodeType::INPUT) {
	  if (asmHeader.empty()) {
		asmHeader += "extern\t_printf,_scanf\t\t; the C function, to be called\n";
	  }
	  asmData += "inpFmt" + std::to_string(inputFmtsNum) + " db \"" + printfFormatGenerator(currentNode->op1) + "\",0\n";

	  asmCode += "push rbp\nlea rdi, [rel inpFmt" + std::to_string(inputFmtsNum) + "]\n";
	  asmCode += "lea rsi, " + writeValue(currentNode->op1) + "\n";
	  asmCode += "xor rax, rax\n";
	  asmCode += "call _scanf\n";
	  asmCode += "pop rbp\n";;

	  inputFmtsNum++;
	}
	if (currentNode->type == Node::nodeType::FOR_LOOP) {
	  asmCode += "mov rcx," + writeValue(currentNode->op2) + "\n";
	  asmCode += "mov " + writeValue(currentNode->op1) + ",rcx" + "\n";
	  asmCode += "loop" + std::to_string(loopsNum) + ":\n";
	  asmCode += "mov r12,rcx\n";
	  for (auto &node : currentNode->list) {
		goThroughTree(node);
	  }
	  asmCode += "mov rcx,r12\n";
	  asmCode += "inc rcx\n";
	  asmCode += "mov " + writeValue(currentNode->op1) + ",rcx" + "\n";
	  asmCode += "cmp rcx," + writeValue(currentNode->op4) + "\n";
	  asmCode += "jle loop" + std::to_string(loopsNum) + "\n";
	  loopsNum++;
	  return;
	}
	if (currentNode->type == Node::nodeType::PROCEDURE) { throw NotImplementedException(); }
	if (currentNode->type == Node::nodeType::FUNCTION) { throw NotImplementedException(); }
	if (currentNode->type == Node::nodeType::IF) {
	  if (currentNode->op1->op2->type == Node::nodeType::BINOP) {
		writeBinOP(currentNode->op1->op2);
		if (currentNode->op1->op2->value == ">") {
		  asmCode += "jg";
		}
		if (currentNode->op1->op2->value == ">=") {
		  asmCode += "lge";
		}
		if (currentNode->op1->op2->value == "<") {
		  asmCode += "jl";
		}
		if (currentNode->op1->op2->value == "<=") {
		  asmCode += "jle";
		}
		if (currentNode->op1->op2->value == "=") {
		  asmCode += "je";
		}
		asmCode += " cond" + std::to_string(ifsNum) + "\n";
		asmCode += "jmp afterCond" + std::to_string(ifsNum) + "\n";
		asmCode += "cond" + std::to_string(ifsNum) + ":\n";
		goThroughTree(currentNode->op2);
		asmCode += "afterCond" + std::to_string(ifsNum) + ":\n";
		ifsNum++;

		return;
	  }
	}
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
	  if (!currentNode->list.empty() && currentNode->op1 == nullptr) {
		for (auto &param : currentNode->list) {
		  tmp += printfFormatGenerator(param) + " ";
		}
	  } else {
		for (auto &param : currentNode->op1->list) {
		  tmp += printfFormatGenerator(param) + " ";
		}
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
		  break;
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
		writeBinOPs_test(currentNode->op2);
	  }
	  if (currentNode->op2->type != Node::nodeType::STR)
		asmCode += "mov " + writeValue(currentNode->op1) + " ,r8\n";
	  else {
		asmData += currentNode->op1->value + " db " + currentNode->op2->value + ",0xa\n";
		eraseAllSubStr(asmBSS, currentNode->op1->value + ":\tRESB\t 1; might need to reallocate later\n");
	  }
	} else if (currentNode->value == ">" || currentNode->value == "<" || currentNode->value == ">=" || currentNode->value == "<="
		|| currentNode->value == "=") {
	  asmCode += "mov r8,";
	  if (currentNode->op1->type != Node::nodeType::BINOP) {
		asmCode += writeValue(currentNode->op1) + "\n";
	  } else { writeBinOPs_test(currentNode->op1); }
	  asmCode += "mov r9,";
	  if (currentNode->op2->type != Node::nodeType::BINOP) {
		asmCode += writeValue(currentNode->op2) + "\n";
	  } else { writeBinOPs_test(currentNode->op2); }
	  asmCode += "cmp r8,r9\n";
	} else
	  writeBinOPs_test(currentNode);
  }
  void writeBinOPs(Node *currentNode) {
	if (currentNode->op1 == nullptr || currentNode->op2 == nullptr) {
	  asmCode += "\n";
	  return;
	}
	//if(currentNode->value==">"||currentNode->value=="<"||currentNode->value==">="||currentNode->value=="<=")return;

	if (currentNode->op1->type == Node::nodeType::BINOP) {
	  writeBinOPs(currentNode->op1);
	} else {
	  asmCode += writeValue(currentNode->op1) + "\n";
	}
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
	} else if (currentNode->op2->type == Node::nodeType::FACTOR) {
	  writeBinOPs(currentNode->op2->op2);
	} else {
	  asmCode += writeValue(currentNode->op2) + "\n";
	}
	if (currentNode->value == "*") { asmCode += "imul r8,r9\n"; }
	if (currentNode->value == "/") { asmCode += "\nxor rdx,rdx\nmov rax,r8\nidiv r9\nmov r8,rax\n"; }
  }
  void writeBinOPs_test(Node *currentNode, std::string writeAfterOP1 = "", std::string writeAfterOP2 = "") {
	if (currentNode->op1 == nullptr || currentNode->op2 == nullptr) {
	  asmCode += "\n";
	  return;
	}
	//if(currentNode->value==">"||currentNode->value=="<"||currentNode->value==">="||currentNode->value=="<=")return;

	if (currentNode->op1->type == Node::nodeType::BINOP) {
	  writeBinOPs_test(currentNode->op1);
	} else {
	  asmCode += writeValue(currentNode->op1) + "\n";
	  asmCode += writeAfterOP1;
	}
	if (currentNode->value == "+") {
	  if (currentNode->op2->value == "*" || currentNode->op2->value == "/") {
		asmCode += "mov r10,";
		writeBinOPs(currentNode->op2);
		asmCode += "add r8,r10\n";
		return;
	  } else {
		asmCode += "add r8, ";
	  }
	}
	if (currentNode->value == "-") {
	  asmCode += "sub r8, ";
	}
	if (currentNode->value == "*") {
	  asmCode += "mov r9, ";
	  writeAfterOP1 = "imul r8,r9\n";
	  writeAfterOP2 = writeAfterOP1;
	}
	if (currentNode->value == "/") {
	  asmCode += "mov r9, ";
	  writeAfterOP1 = "xor rdx,rdx\nmov rax,r8\nidiv r9\nmov r8,rax\n";
	  writeAfterOP2 = writeAfterOP1;
	}
	if (currentNode->op2->type == Node::nodeType::BINOP) {
	  writeBinOPs_test(currentNode->op2, writeAfterOP1, writeAfterOP2);
	} else if (currentNode->op2->type == Node::nodeType::FACTOR) {
	  writeBinOPs_test(currentNode->op2->op2, writeAfterOP1, writeAfterOP2);
	} else {
	  asmCode += writeValue(currentNode->op2) + "\n";
	  asmCode += writeAfterOP2;
	}
  }
  /**
 * @brief This func will put val in [] if it is a var, will paste val w\o
 * them otherwise
 * @param currentNode node with value
 */
  std::string writeValue(Node *currentNode) {
	if (currentNode->type == Node::nodeType::VAR) {
	  if (getVariableType(currentNode) == Variable::varType::REAL || getVariableType(currentNode) == Variable::varType::DOUBLE) {
		throw NotImplementedException("Sorry fractions are not supported in this version :(\n");
	  } else if (getVariableType(currentNode) == Variable::varType::STRING) return currentNode->value;
	  return "[rel " + currentNode->value + "]";
	} else if (currentNode->type == Node::nodeType::CONSTANT || currentNode->type == Node::nodeType::STR) {
	  return currentNode->value;
	}
  }
};

#endif //SPO_COMPILER_SRC_TRANSLATOR_HPP_
