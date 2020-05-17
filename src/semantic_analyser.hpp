//
// Created by Vladimir Shubarin on 17.05.2020.
//

#ifndef SPO_COMPILER_SRC_SEMANTIC_ANALYSER_HPP_
#define SPO_COMPILER_SRC_SEMANTIC_ANALYSER_HPP_

#include "Node.hpp"
#include "Parser.h"
#include "variable_table.hpp"
class SemanticAnalyser {
 private:
  Node*tree{};
  Parser*parser{};
  VariableTable*variables;
 public:
  explicit SemanticAnalyser(const std::string &_filename) {
	parser = new Parser(_filename);
	parser->parse();
	tree=parser->GetTree();
	variables=new VariableTable();
	analyze();
  }
 private:
  void analyze(){
	lookForVariableDeclaration(tree);
	variables->printToConsole();
  }
  void lookForVariableDeclaration(Node *currentNode){
	// если текущая нода ноль, то делать с ней ничего нельзя
	// так что выходим из функции
	if (currentNode == nullptr)
	  return;

	if(currentNode->type ==Node::nodeType::VARDECL){
	  std::string varType = currentNode->op2->value;
	  for(auto &varName:currentNode->op1->list){
	    variables->addVar(new Variable(varName->value,Variable::determineVarType(varType)));
	  }
	}
	else if(currentNode->type==Node::nodeType::VAR){
	  if(!variables->isVarDefined(currentNode->value)){
		throw VariableNotDefinedError(currentNode->value);
	  }
	}

	lookForVariableDeclaration(currentNode->op1);
	lookForVariableDeclaration(currentNode->op2);
	lookForVariableDeclaration(currentNode->op3);
	lookForVariableDeclaration(currentNode->op4);
	for (auto &node :currentNode->list) {
	  lookForVariableDeclaration(node);
	}
  }
};

#endif //SPO_COMPILER_SRC_SEMANTIC_ANALYSER_HPP_
