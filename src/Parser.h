//
// Created by vhundef on 27.04.2020.
//
#ifndef SPO_COMPILER_PARSER_H
#define SPO_COMPILER_PARSER_H

#include <stdexcept>

#if __APPLE__
#include "StackTraceGenerator.hpp"
#include <execinfo.h>
#endif

#include "Lexer.h"
#include "Node.hpp"
#include "exceptions.cpp"

class Parser {
 private:
  Lexer *lexer;
  Node *tree;

 public:
  void parse() {
	lexer->tokenize();
	lexer->printAllTokens();
	tree = Goal();
	printRecursive(tree, 0);
  }

  explicit Parser(const std::string &_filename) {

	lexer = new Lexer(_filename);
	parse();
  }
 private:
  void expect(Token::tokenType tokenType) {

	if (lexer->getCurrentToken()->getType() == tokenType) {
	  lexer->nextToken();
	} else {
#if __APPLE__
	  StackTraceGenerator::printStack();
#endif
	  std::string error = "Expected token of type: ";
	  error += Token::typeToString(tokenType);
	  error += ", but got: ";
	  error += Token::typeToString(lexer->getCurrentToken()->getType());
	  error += "(" + lexer->getCurrentToken()->getText() + ")";
	  printRecursive(tree, 0);
	  throw ParsingError(error);
	}
  }

  void printRecursive(Node *currentNode, size_t level) {
	using namespace std;
	bool bAddNewline = true;
	// если узел не существует, то просто выходим из функции
	if (currentNode == nullptr)
	  return;

	for (int i = 0; i < level; ++i) {
	  // выводим level раз отступ в два пробела
	  cout << "|  ";
	}

	// выводим обозначение узла
	cout << "+-";

	// в зависимости от типа узла
	// выводим нужное
	switch (currentNode->type) {
	  case Node::nodeType::START:cout << "START ";
		break;
	  case Node::nodeType::EXPR:cout << "EXPR ";
		break;
	  case Node::nodeType::CONSTANT:cout << "CONST ";
		// так как имеет значение, то выводим его в скобках
		cout << "(" << currentNode->value << ")";
		break;
	  case Node::nodeType::ADD:cout << "ADD ";
		break;
	  case Node::nodeType::AND:cout << "AND";
		break;
	  case Node::nodeType::SUB:cout << "SUB ";
		break;
	  case Node::nodeType::MUL:cout << "MUL ";
		break;
	  case Node::nodeType::DIV:cout << "DIV ";
		break;
	  case Node::nodeType::STATEMENT:cout << "STATEMENT ";
		break;
	  case Node::nodeType::IF:cout << "IF ";
		break;
	  case Node::nodeType::ELSE:cout << "ELSE ";
		break;
	  case Node::nodeType::VAR:cout << "VAR ";
		cout << "(" << currentNode->value << ")";
		break;
	  case Node::nodeType::BINOP:cout << "BINOP ";
		cout << "(" << currentNode->value << ")";
		break;
	  case Node::nodeType::STR:cout << "STR ";
		cout << "(" << currentNode->value << ")";
		break;
	  case Node::nodeType::FACTOR:cout << "FACTOR ";
		break;
	  case Node::nodeType::PAR:cout << "PAR";
		cout << " ('" << currentNode->value << "')";
		break;
	  case Node::nodeType::TERM:cout << "TERM ";
		break;
	  case Node::nodeType::RESERVED:cout << "RESERVED ";
		break;
	  case Node::nodeType::NODE:cout << "NODE";
		break;
	  case Node::nodeType::COMP:cout << "COMP";
		break;
	  case Node::nodeType::VARDECL:cout << "VARDECL";
		break;
	  case Node::nodeType::VARTYPE:cout << "VARTYPE";
		cout << " ('" << currentNode->value << "')";
		break;
	  case Node::nodeType::DECL:cout << "DECL ";
		break;
	  case Node::nodeType::VARLIST:cout << "VARLIST ";
		break;
	  case Node::nodeType::DESIGNATOR:cout << "DESIGNATOR ";
		break;
	  case Node::nodeType::PROG:cout << "PROG ";
		break;
	  case Node::nodeType::UNIT:cout << "UNIT ";
		break;
	  case Node::nodeType::PROGRAM_BLOCK:cout << "PROGRAM_BLOCK ";
		break;
	  case Node::nodeType::USES:cout << "USES ";
		break;
	  case Node::nodeType::INTERFACE_SECTION:cout << "INTERFACE_SECTION ";
		break;
	  case Node::nodeType::BLOCK:cout << "BLOCK ";
		break;
	  case Node::nodeType::SECTION:cout << "SECTION ";
		break;
	  case Node::nodeType::TYPE:cout << "TYPE ";
		break;
	  case Node::nodeType::ARRAY_CONST:cout << "ARRAY_CONST ";
		break;
	  case Node::nodeType::SUBRANGE_EXPR:cout << "SUBRANGE_EXPR ";
		break;
	  case Node::nodeType::INPUT:cout << "INPUT ";
		break;
	  case Node::nodeType::OUTPUT:cout << "OUTPUT ";
		break;
	  case Node::nodeType::FOR_LOOP:cout << "FOR_LOOP ";
		break;
	  case Node::nodeType::STATEMENT_LIST: {
		cout << "STATEMENT_LIST ";
		break;
	  }
	  case Node::nodeType::VAR_SECTION:cout << "VAR_SECTION";
		break;
	  case Node::nodeType::WITH:cout << "WITH";
		break;
	  case Node::nodeType::WHILE:cout << "WHILE";
		break;
	  case Node::nodeType::ARRAY_TYPE:cout << "Array_TYPE";
		break;
	  case Node::nodeType::PROCEDURE:cout << "PROCEDURE";
		break;
	  case Node::nodeType::PARAMS:cout << "PARAMS";
		break;
	  case Node::nodeType::PARAM:cout << "PARAM";
		break;
	  case Node::nodeType::FUNCTION:cout << "FUNCTION";
		break;
	}
	if (bAddNewline)
	  cout << endl;

	// т.к рекурсивная
	printRecursive(currentNode->op1, level + 1);
	printRecursive(currentNode->op2, level + 1);
	printRecursive(currentNode->op3, level + 1);
	printRecursive(currentNode->op4, level + 1);
	for (auto &node : currentNode->list) {
	  printRecursive(node, level + 1);
	}
  }

  void recursiveTraversal(Node *currentNode) {
	// если текущая нода ноль, то делать с ней ничего нельзя
	// так что выходим из функции
	if (currentNode == nullptr)
	  return;

	recursiveTraversal(currentNode->op1);
	recursiveTraversal(currentNode->op2);
	recursiveTraversal(currentNode->op3);
	recursiveTraversal(currentNode->op4);
	for (auto &node :currentNode->list) {
	  recursiveTraversal(node);
	}
  }

  Node *Goal() {
	/*
	 * Goal -> (Program | Package  | Library  | Unit)
	 */

	Node *node = new Node(Node::nodeType::START);
	node->op1 = Program();
	node->op2 = Package();
	node->op3 = Library();
	node->op4 = Unit();
	return node;
  }

  Node *Program() {
	/*
	 * Program -> [PROGRAM Ident ['(' IdentList ')'] ';']
							  ProgramBlock '.'
	 */

	Node *node = new Node(Node::nodeType::PROG);
	if (lexer->getCurrentToken()->getType() ==
		Token::tokenType::PROGRAM_Keyword) {
	  lexer->nextToken();
	  node->op1 = Ident(true);
	  if (lexer->getCurrentToken()->getType() == Token::tokenType::LPAR) {
		lexer->nextToken();
		node->op2 = IdentList();
		expect(Token::tokenType::RPAR);
	  }
	  expect(Token::tokenType::Semicolon);
	}
	if (node->op1 == nullptr) {
	  node->op1 = ProgramBlock();
	} else if (node->op2 == nullptr) {
	  node->op2 = ProgramBlock();
	} else {
	  node->op3 = ProgramBlock();
	}
	expect(Token::tokenType::DOT);
	return node;
  }

  Node *Unit() {
	/*
	 * Unit -> UNIT Ident ';'
					   InterfaceSection
					   ImplementationSection
					   InitSection '.'
	 */

	Node *node{nullptr};
	if (lexer->getCurrentToken()->getType() == Token::tokenType::UNIT_Keyword) {
	  node = new Node(Node::nodeType::UNIT);
	  lexer->nextToken();
	  node->op1 = Ident(false);
	  expect(Token::tokenType::Semicolon);
	  node->op2 = InterfaceSection();
	  node->op3 = ImplementationSection();
	  node->op4 = InitSection();
	  expect(Token::tokenType::DOT);
	  return node;
	} else
	  return nullptr;
  }

  Node *Package() {
	/*
	 * Package -> PACKAGE Ident ';'
							  [RequiresClause]
							  [ContainsClause]
							  END '.'
	 */

	std::cout << "Package() is not implemented and will return nullptr"
			  << std::endl;
	return nullptr;
	// throw NotImplementedException("Package()");
  }

  Node *Library() {
	/*
	 * Library -> LIBRARY Ident ';'
							  ProgramBlock '.'
	 */

	std::cout << "Library() is not implemented and will return nullptr"
			  << std::endl;
	return nullptr;
	// throw NotImplementedException("Library()");
  }

  Node *ProgramBlock() {
	/*
	 * ProgramBlock -> [UsesClause]
									   Block
	*/

	Node *node = new Node(Node::nodeType::PROGRAM_BLOCK);
	node->op1 = UsesClause();
	if (node->op1 == nullptr) {
	  node->op1 = Block();
	} else {
	  node->op2 = Block();
	}
	return node;
  }

  Node *UsesClause() {
	/*
	 * UsesClause -> USES IdentList ';'
	 */

	Node *node = nullptr;
	if (lexer->getCurrentToken()->getType() == Token::tokenType::USES_Keyword) {
	  node = new Node(Node::nodeType::USES);
	  lexer->nextToken();
	  node->op1 = IdentList();
	  expect(Token::tokenType::Semicolon);
	}

	return node;
  }

  Node *InterfaceSection() {
	/*
	 * InterfaceSection -> INTERFACE
											   [UsesClause]
											   [InterfaceDecl]...
	 */
	Node *node = new Node(Node::nodeType::INTERFACE_SECTION);
	expect(Token::tokenType::INTERFACE_Keyword);
	node->op1 = UsesClause();
	node->op2 = InterfaceDecl();
	return node;
  }

  Node *InterfaceDecl() {
	/*
	 * InterfaceDecl -> ConstSection
									 -> TypeSection
									 -> VarSection
									 -> ExportedHeading
	 */
	Node *node = ConstSection();
	if (node != nullptr)
	  return node;
	else if ((node = TypeSection()) != nullptr)
	  return node;
	else if ((node = VarSection()) != nullptr)
	  return node;
	node = ExportedHeading();
	return node;
  }

  Node *ExportedHeading() {
	/*
	 * ExportedHeading -> ProcedureHeading ';' [Directive]
									   -> FunctionHeading ';' [Directive]
	 */
	Node *node{nullptr};
	if ((node = ProcedureHeading()) != nullptr) {
	  expect(Token::tokenType::Semicolon);
	  node->op2 = Directive();
	} else {
	  node = FunctionHeading();
	  expect(Token::tokenType::Semicolon);
	  node->op2 = Directive();
	}
	return node;
  }

  Node *ImplementationSection() {
	/*
	 * ImplementationSection -> IMPLEMENTATION
															[UsesClause]
															[DeclSection]...
	 */
	throw NotImplementedException("ImplementationSection()");
  }

  Node *Block() {
	/*
	 * Block -> [DeclSection]
							CompoundStmt
	 */

	Node *node = new Node(Node::nodeType::BLOCK);
	node->op1 = DeclSection();
	if (node->op1 != nullptr)
	  node->op2 = CompoundStmt();
	else {
	  node->op1 = CompoundStmt();
	}
	return node;
  }

  Node *DeclSection() {
	/*
	 * DeclSection -> LabelDeclSection
							   -> ConstSection
							   -> TypeSection
							   -> VarSection
							   -> ProcedureDeclSection
	 */
	Node *node = new Node(Node::nodeType::DECL);
	Node *tmp;
	while (true) {
	  tmp = LabelDeclSection();
	  if (tmp == nullptr) {
		tmp = ConstSection();
		if (tmp == nullptr) {
		  tmp = TypeSection();
		  if (tmp == nullptr) {
			tmp = VarSection();
			if (tmp == nullptr) {
			  tmp = ProcedureDeclSection();
			}
		  }
		}
	  }
	  if (tmp == nullptr)
		break;
	  else {
		node->list.push_back(tmp);
	  }
	}
	return node;
  }

  Node *LabelDeclSection() {
	/*
	 * LabelDeclSection -> LABEL LabelId
	 */
	if (lexer->getCurrentToken()->getType() ==
		Token::tokenType::LABEL_Keyword) {
	  lexer->nextToken();
	  return LabelId();
	} else
	  return nullptr;
  }

  Node *ConstSection() {
	/*
	 * ConstSection -> CONST (ConstantDecl ';')...
	 */
	Node *node{nullptr};
	if (lexer->getCurrentToken()->getType() ==
		Token::tokenType::CONST_Keyword) {
	  node = new Node(Node::nodeType::SECTION);
	  lexer->nextToken();
	  node->list.push_back(ConstantDecl());
	  expect(Token::tokenType::Semicolon);
	  while (node->list.back() != nullptr) {
		node->list.push_back(ConstantDecl());
		if (node->list.back() != nullptr)
		  expect(Token::tokenType::Semicolon);
	  }
	}
	return node;
  }

  Node *TypedConstant() {
	/*
	 * TypedConstant -> (ConstExpr | ArrayConstant | RecordConstant)
	 */
	Node *node = ConstExpr();
	if (node == nullptr) {
	  node = ArrayConstant();
	  if (node == nullptr) {
		node = RecordConstant();
	  }
	}
	return node;
  }

  Node *ConstantDecl() {
	/*
	 * ConstantDecl -> Ident '=' ConstExpr
									-> Ident ':' TypeId '=' TypedConstant
	 */
	Node *node = new Node(Node::nodeType::CONSTANT);
	node->op1 = Ident();
	if (lexer->getCurrentToken()->getType() == Token::tokenType::EQUAL) {
	  lexer->nextToken();
	  node->op2 = new StringNode("=");
	  node->op3 = ConstExpr();
	} else if (lexer->getCurrentToken()->getType() == Token::tokenType::Colon) {
	  lexer->nextToken();
	  node->op2 = TypeId();
	  expect(Token::tokenType::EQUAL);
	  node->op3 = TypedConstant();
	}

	throw NotImplementedException("ConstantDecl()");
  }

  Node *TypeSection() {
	/*
	 * TypeSection -> TYPE (TypeDecl ';')...
	 */
	Node *node{nullptr};
	if (lexer->getCurrentToken()->getType() == Token::tokenType::TYPE_Keyword) {
	  node = new Node(Node::nodeType::SECTION);
	  lexer->nextToken();
	  node->list.push_back(TypeDecl());
	  expect(Token::tokenType::Semicolon);
	  while (node->list.back() != nullptr) {
		node->list.push_back(TypeDecl());
		if (node->list.back() != nullptr)
		  expect(Token::tokenType::Semicolon);
	  }
	}
	return node;
  }

  Node *TypeDecl() {
	/*
	 * TypeDecl -> Ident '=' Type
							-> Ident '=' RestrictedType
	 */
	Node *node{nullptr};
	Node *tmp = Ident(true);

	if (lexer->getCurrentToken()->getType() == Token::tokenType::EQUAL &&
		tmp != nullptr) {
	  node = new Node(Node::nodeType::TYPE);
	  node->op1 = tmp;
	  lexer->nextToken();
	  node->op2 = new StringNode("=");
	  node->op3 = Type();
	  if (node->op3 == nullptr) {
		node->op3 = RestrictedType();
	  }
	}
	return node;
  }

  Node *ArrayConstant() {
	/*
	 * ArrayConstant -> '(' TypedConstant/','... ')'
	 */
	Node *node = new Node(Node::nodeType::ARRAY_CONST);
	if (lexer->getCurrentToken()->getType() == Token::tokenType::LPAR) {
	  lexer->nextToken();
	  node->list.push_back(TypedConstant());
	  if (lexer->getCurrentToken()->getType() == Token::tokenType::COMMA) {
		lexer->nextToken();
		while (node->list.back() != nullptr) {
		  node->list.push_back(TypedConstant());
		  if (node->list.back() != nullptr)
			expect(Token::tokenType::COMMA);
		}
	  }
	  expect(Token::tokenType::RPAR);
	}
	return node;
  }

  Node *RecordConstant() {
	/*
	 * RecordConstant -> '(' RecordFieldConstant/';'... ')'
	 */
	std::cout << "RecordConstant will NOT be implemented" << std::endl;
	return nullptr;
  }

  Node *RecordFieldConstant() {
	/*
	 * RecordFieldConstant -> Ident ':' TypedConstant
	 */
	Node *node = new Node(Node::nodeType::NODE);
	node->op1 = Ident();
	expect(Token::tokenType::Colon);
	node->op2 = new StringNode(":");
	node->op3 = TypedConstant();
	return node;
  }

  Node *Type() {
	/*
	 * Type -> TypeId
					-> SimpleType
					-> StrucType
					-> PointerType
					-> StringType
					-> ProcedureType
					-> VariantType
					-> ClassRefType
	 */
	Node *node;
	node = TypeId(true);
	if (node == nullptr) {
	  node = SimpleType();
	  if (node == nullptr) {
		node = StrucType();
		if (node == nullptr) {
		  node = PointerType();
		  if (node == nullptr) {
			node = StringType();
			if (node == nullptr) {
			  node = ProcedureType();
			  if (node == nullptr) {
				node = VariantType();
				if (node == nullptr) {
				  node = ClassRefType();
				}
			  }
			}
		  }
		}
	  }
	}
	return node;
  }

  Node *RestrictedType() {
	/*
	 * RestrictedType -> ObjectType
									  -> ClassType
									  -> InterfaceType
	 */
	Node *node = ObjectType();
	if (node == nullptr) {
	  node = ClassType();
	  if (node == nullptr) {
		node = InterfaceType();
	  }
	}
	return node;
  }

  Node *ClassRefType() {
	/*
	 * ClassRefType -> CLASS OF TypeId
	 */
	Node *node{nullptr};
	if (lexer->getCurrentToken()->getType() ==
		Token::tokenType::CLASS_Keyword) {
	  lexer->nextToken();
	  expect(Token::tokenType::OF_Keyword);
	  node = TypeId();
	}
	return node;
  }

  Node *SimpleType() {
	/*
	 * SimpleType -> (OrdinalType | RealType)
	 */
	Node *node = OrdinalType();
	if (node == nullptr) {
	  node = RealType();
	}
	return node;
  }

  Node *RealType() {
	/*
	 * RealType -> REAL48
							-> REAL
							-> SINGLE
							-> DOUBLE
							-> EXTENDED
							-> CURRENCY
							-> COMP
	 */
	Node *node{nullptr};
	if (lexer->getCurrentToken()->getType() == Token::tokenType::REAL48_Type ||
		lexer->getCurrentToken()->getType() == Token::tokenType::REAL_Type ||
		lexer->getCurrentToken()->getType() == Token::tokenType::SINGLE_Type ||
		lexer->getCurrentToken()->getType() == Token::tokenType::DOUBLE_Type ||
		lexer->getCurrentToken()->getType() ==
			Token::tokenType::EXTENDED_Type ||
		lexer->getCurrentToken()->getType() ==
			Token::tokenType::CURRENCY_Type ||
		lexer->getCurrentToken()->getType() == Token::tokenType::COMP_Type) {
	  node = new Node(Node::nodeType::TYPE);
	  node->value = lexer->getCurrentToken()->getText();
	  lexer->nextToken();
	} else
	  return nullptr;

	return node;
  }

  Node *OrdinalType() {
	/*
	 * OrdinalType -> (SubrangeType | EnumeratedType | OrdIdent)
	 */
	Node *node = SubrangeType();
	if (node == nullptr) {
	  node = EnumeratedType();
	  if (node == nullptr) {
		node = OrdIdent();
	  }
	}
	return node;
  }

  Node *OrdIdent() {
	/*
	 * OrdIdent -> SHORTINT
							-> SMALLINT
							-> INTEGER
							-> BYTE
							-> LONGINT
							-> INT64
							-> WORD
							-> BOOLEAN
							-> CHAR
							-> WIDECHAR
							-> LONGWORD
							-> PCHAR
	 */
	Node *node{nullptr};
	if (lexer->getCurrentToken()->getType() ==
		Token::tokenType::SHORTINT_Type ||
		lexer->getCurrentToken()->getType() ==
			Token::tokenType::SMALLINT_Type ||
		lexer->getCurrentToken()->getType() == Token::tokenType::INTEGER_Type ||
		lexer->getCurrentToken()->getType() == Token::tokenType::BYTE_Type ||
		lexer->getCurrentToken()->getType() == Token::tokenType::LONGINT_Type ||
		lexer->getCurrentToken()->getType() == Token::tokenType::INT64_Type ||
		lexer->getCurrentToken()->getType() == Token::tokenType::WORD_Type ||
		lexer->getCurrentToken()->getType() == Token::tokenType::BOOLEAN_Type ||
		lexer->getCurrentToken()->getType() == Token::tokenType::CHAR_Type ||
		lexer->getCurrentToken()->getType() ==
			Token::tokenType::WIDECHAR_Type ||
		lexer->getCurrentToken()->getType() ==
			Token::tokenType::LONGWORD_Type ||
		lexer->getCurrentToken()->getType() == Token::tokenType::PCHAR_Type) {
	  node = new Node(Node::nodeType::TYPE);
	  node->value = lexer->getCurrentToken()->getText();
	  lexer->nextToken();
	} else
	  return nullptr;
	return node;
  }

  Node *VariantType() {
	/*
	 * VariantType -> VARIANT
							   -> OLEVARIANT
	 */
	Node *node{nullptr};
	if (lexer->getCurrentToken()->getType() == Token::tokenType::VARIANT_Type ||
		lexer->getCurrentToken()->getType() ==
			Token::tokenType::OLEVARIANT_Type) {
	  node = new Node(Node::nodeType::TYPE);
	  node->value = lexer->getCurrentToken()->getText();
	  lexer->nextToken();
	} else
	  return nullptr;
	return node;
  }

  Node *SubrangeType() {
	/*
	 * SubrangeType -> ConstExpr '..' ConstExpr
	 */

	Node *node = new Node(Node::nodeType::SUBRANGE_EXPR);
	node->op1 = ConstExpr();
	if (node->op1 == nullptr)
	  return nullptr;
	expect(Token::tokenType::DOT);
	expect(Token::tokenType::DOT);
	node->value = "..";
	node->op2 = ConstExpr();
	if (node->op1 == nullptr || node->op2 == nullptr) {
	  return nullptr;
	}
	return node;
  }

  Node *EnumeratedType() {
	/*
	 * EnumeratedType -> '(' IdentList ')'
	 */
	Node *node{nullptr};
	if (lexer->getCurrentToken()->getType() == Token::tokenType::LPAR) {
	  lexer->nextToken();
	  node = IdentList();
	} else
	  return nullptr;
	return node;
  }

  Node *StringType() {
	/*
	 * StringType -> STRING
							  -> ANSISTRING
							  -> WIDESTRING
							  -> STRING '[' ConstExpr ']'
	 */

	if (lexer->getCurrentToken()->getType() == Token::tokenType::ANSISTRING ||
		lexer->getCurrentToken()->getType() == Token::tokenType::WIDESTRING) {
	  Node *node = new Node(Node::nodeType::TYPE);
	  node->value = lexer->getCurrentToken()->getText();
	  lexer->nextToken();
	  return node;
	} else if (lexer->getCurrentToken()->getType() ==
		Token::tokenType::STRING) {
	  Node *node = new Node(Node::nodeType::TYPE);
	  node->op1 = new StringNode(lexer->getCurrentToken()->getText());
	  lexer->nextToken();
	  if (lexer->getCurrentToken()->getType() == Token::tokenType::LBRACKET) {
		expect(Token::tokenType::LBRACKET);
		node->op2 = new StringNode("[");
		node->op3 = ConstExpr();
		expect(Token::tokenType::RBRACKET);
		node->op4 = new StringNode("]");
	  } else
		return node;
	} else
	  return nullptr;
	return nullptr;
  }

  Node *StrucType() {
	/*
	 * StrucType -> [PACKED] (ArrayType | SetType | FileType | RecType)
	 */
	return ArrayType();
  }

  Node *ArrayType() {
	/*
	 * ArrayType -> ARRAY ['[' OrdinalType/','... ']'] OF Type
	 */
	Node *node{nullptr};
	if (lexer->getCurrentToken()->getType() ==
		Token::tokenType::ARRAY_Keyword) {
	  lexer->nextToken();
	  node = new Node(Node::nodeType::ARRAY_TYPE);
	  if (lexer->getCurrentToken()->getType() == Token::tokenType::LBRACKET) {
		lexer->nextToken();
		node->list.push_back(OrdinalType());
		if (lexer->getCurrentToken()->getType() == Token::tokenType::COMMA) {
		  expect(Token::tokenType::COMMA);
		  while (node->list.back() != nullptr) {
			node->list.push_back(OrdinalType());
			if (lexer->getCurrentToken()->getType() == Token::tokenType::COMMA)
			  expect(Token::tokenType::COMMA);
			else {
			  break;
			}
		  }
		}
		expect(Token::tokenType::RBRACKET);
	  }
	  expect(Token::tokenType::OF_Keyword);
	  node->op1 = Type();
	}
	return node;
  }

  Node *RecType() {
	/*
	 * RecType -> RECORD [FieldList] END
	 */
	throw NotImplementedException("RecType()");
  }

  Node *FieldList() {
	/*
	 * FieldList ->  FieldDecl/';'... [VariantSection] [';']
	 */
	throw NotImplementedException("FieldList()");
  }

  Node *FieldDecl() {
	/*
	 * FieldDecl -> IdentList ':' Type
	 */
	throw NotImplementedException("FieldDecl()");
  }

  Node *VariantSection() {
	/*
	 * VariantSection -> CASE [Ident ':'] TypeId OF RecVariant/';'...
	 */
	throw NotImplementedException("VariantSection()");
  }

  Node *RecVariant() {
	/*
	 * RecVariant -> ConstExpr/','...  ':' '(' [FieldList] ')'
	 */
	throw NotImplementedException("RecVariant()");
  }

  Node *SetType() {
	/*
	 * SetType -> SET OF OrdinalType
	 */
	throw NotImplementedException("SetType()");
  }

  Node *FileType() {
	/*
	 * FileType -> FILE OF TypeId
	 */
	throw NotImplementedException("FileType()");
  }

  Node *PointerType() {
	/*
	 * PointerType -> '^' TypeId
	 */
	return nullptr;
  }

  Node *ProcedureType() {
	/*
	 * ProcedureType -> (ProcedureHeading | FunctionHeading) [OF OBJECT]
	 */
	Node *node = new Node(Node::nodeType::TYPE);
	node->op1 = ProcedureHeading();
	if (node == nullptr)
	  node->op1 = FunctionHeading();
	if (node->op1 == nullptr)
	  return nullptr;
	if (lexer->getCurrentToken()->getType() == Token::tokenType::OF_Keyword) {
	  lexer->nextToken();
	  expect(Token::tokenType::OBJECT_Keyword);
	}
	return node;
  }

  Node *VarSection() {
	/*
	 * VarSection -> VAR (VarDecl ';')...
	 */
	if (lexer->getCurrentToken()->getType() ==
		Token::tokenType::VAR_Keyword) { /// если есть ключевое слово var
	  lexer->nextToken();
	  Node *node = new Node(Node::nodeType::VAR_SECTION);
	  Node *tmp = VarDecl();
	  if (tmp == nullptr)
		return nullptr; // если нету переменной
	  node->list.push_back(tmp);
	  expect(Token::tokenType::Semicolon);
	  while (node->list.back() != nullptr) {
		node->list.push_back(VarDecl());
		if (node->list.back() != nullptr)
		  expect(Token::tokenType::Semicolon);
	  }
	  return node;
	} else
	  return nullptr;
  }

  Node *VarDecl() {
	/*
	 * VarDecl -> IdentList ':' Type [(ABSOLUTE (Ident | ConstExpr)) | '='
	 * ConstExpr]
	 */
	Node *node = new Node(Node::nodeType::VARDECL);
	node->op1 = IdentList();
	if (node->op1 == nullptr) {
	  return nullptr;
	}
	expect(Token::tokenType::Colon);
	node->op2 = Type();
	if (lexer->getCurrentToken()->getType() == Token::tokenType::EQUAL) {
	  lexer->nextToken();
	  node->op3 = new StringNode("=");
	  node->op4 = ConstExpr();
	}
	return node;
  }

  Node *Expression() {
	/*
	 * Expression -> SimpleExpression [RelOp SimpleExpression]...
	 */
	Node *node = new Node(Node::nodeType::EXPR);
	Node *tmp = SimpleExpression();
	if (tmp == nullptr)
	  return nullptr;
	node->op1 = tmp;
	node->op2 = RelOp();
	if (node->op2 == nullptr) {
	  return node->op1;
	}
	node->op3 = SimpleExpression();
	node = new binOpNode(node->op1, node->op2->value, node->op3);
	return node;
  }

  Node *SimpleExpression() {
	/*
	 * SimpleExpression -> ['+' | '-'] Term [AddOp Term]...
	 */
	Node *node{nullptr};
	if (lexer->getCurrentToken()->getType() == Token::tokenType::MathPlus) {
	  node = new Node(Node::nodeType::EXPR);
	  node->op1 = new Node(Node::nodeType::ADD, "+");
	  lexer->nextToken();
	} else if (lexer->getCurrentToken()->getType() ==
		Token::tokenType::MathMinus) {
	  node = new Node(Node::nodeType::EXPR);
	  node->op1 = new Node(Node::nodeType::SUB, "-");
	  lexer->nextToken();
	}
	if (node == nullptr) {
	  node = new Node(Node::nodeType::EXPR);
	  node->op1 = Term();
	  if (node->op1 == nullptr) {
		return nullptr;
	  }
	  node->op2 = AddOp();
	  if (node->op2 == nullptr) {
		return node->op1;
	  }
	  node->op3 = SimpleExpression();

	  node = new binOpNode(node->op1, node->op2->value, node->op3);
	  return node;
	} else {
	  node->op2 = Term();
	  node->op3 = AddOp();
	  if (node->op3 == nullptr) {
		return node->op2;
	  }
	  node->op4 = SimpleExpression();

	  return node;
	}
  }

  Node *Term() {
	/*
	 * Term -> Factor [MulOp Factor]...
	 */
	Node *node = new Node(Node::nodeType::TERM);
	node->op1 = Factor();
	node->op2 = MulOp();
	if (node->op2 == nullptr) {
	  return node->op1;
	}
	node->op3 = Term();
	node = new binOpNode(node->op1, node->op2->value, node->op3);
	return node;
  }

  Node *Factor() {
	/*
	 * Factor -> Designator ['(' ExprList ')']
					  -> '' Designator
					  -> Number
					  -> String
					  -> NIL
					  -> '(' Expression ')'
					  -> NOT Factor
					  -> SetConstructor
					  -> TypeId '(' Expression ')'
	 */
	Node *node;
	switch (lexer->getCurrentToken()->getType()) {
	  case Token::tokenType::Num: ///< factor() -> Number
		node = new NumberNode(lexer->getCurrentToken()->getText());
		lexer->nextToken();
		return node;
	  case Token::tokenType::STRING: ///< factor() -> String
		node = new StringNode(lexer->getCurrentToken()->getText());
		lexer->nextToken();
		return node;
	  case Token::tokenType::LPAR: ///< factor() -> '(' Expression ')'
		expect(Token::tokenType::LPAR);
		node = new Node(Node::nodeType::FACTOR);
		node->op1 = new ParenNode("(");
		node->op2 = Expression();
		expect(Token::tokenType::RPAR);
		node->op3 = new ParenNode(")");
		return node;
	  default:break;
	}
	node = Designator();
	return node;
  }

  Node *RelOp() {
	/*
	 * RelOp -> '>'
					 -> '<'
					 -> '<='
					 -> '>='
					 -> '<>'
					 -> IN
					 -> IS
					 -> AS
	 */
	Node *node;
	if (lexer->getCurrentToken()->getType() == Token::tokenType::Comparison ||
		lexer->getCurrentToken()->getType() == Token::tokenType::EQUAL) {
	  node =
		  new Node(Node::nodeType::COMP, lexer->getCurrentToken()->getText());
	  lexer->nextToken();
	  return node;
	}
	return nullptr;
  }

  Node *AddOp() {
	/*
	 * AddOp -> '+'
					 -> '-'
					 -> OR
					 -> XOR
	 */
	Node *node;
	switch (lexer->getCurrentToken()->getType()) {
	  case Token::tokenType::MathPlus:node = new Node(Node::nodeType::ADD, "+");
		lexer->nextToken();
		return node;
	  case Token::tokenType::MathMinus:node = new Node(Node::nodeType::SUB, "-");
		lexer->nextToken();
		return node;
	  default:return nullptr;
	}
  }

  Node *MulOp() {
	/*
	 * MulOp -> '*'
					 -> '/'
					 -> DIV
					 -> MOD
					 -> AND
					 -> SHL
					 -> SHR
	 */
	Node *node;
	switch (lexer->getCurrentToken()->getType()) {
	  case Token::tokenType::MathMult:node = new Node(Node::nodeType::MUL, "*");
		lexer->nextToken();
		return node;
	  case Token::tokenType::MathDiv:node = new Node(Node::nodeType::DIV, "/");
		lexer->nextToken();
		return node;
	  case Token::tokenType::AND_Keyword:node = new Node(Node::nodeType::AND, "AND");
		lexer->nextToken();
		return node;
	  default:return nullptr;
	}
	return nullptr;
  }

  Node *Designator() {
	/*
	 * Designator -> QualId ['.' Ident | '[' ExprList ']' | '^']...
	 */
	Node *node = new Node(Node::nodeType::DESIGNATOR);
	Node *tmp = QualId();
	if (tmp == nullptr) {
	  return nullptr;
	}
	node->op1 = tmp;
	if (lexer->getCurrentToken()->getType() == Token::tokenType::DOT) {
	  lexer->nextToken();
	  node->op2 = new StringNode(".");
	  node->op3 = Ident(true);
	} else if (lexer->getCurrentToken()->getType() ==
		Token::tokenType::LBRACKET) {
	  lexer->nextToken();
	  node->op2 = new StringNode("[");
	  node->op3 = ExprList();
	  expect(Token::tokenType::RBRACKET);
	  node->op4 = new StringNode("]");
	}
	if (node->op2 == nullptr)
	  return node->op1;
	return node;
  }

  Node *SetConstructor() {
	/*
	 * SetConstructor -> '[' [SetElement/','...] ']'
	 */
	throw NotImplementedException("SetConstructor()");
  }

  Node *SetElement() {
	/*
	 * SetElement -> Expression ['..' Expression]
	 */
	throw NotImplementedException("SetElement()");
  }

  Node *ExprList() {
	/*
	 * ExprList -> Expression/','...
	 */
	Node *node = new Node(Node::nodeType::EXPR);;
	node->list.push_back(Expression());
	if (lexer->getCurrentToken()->getType() != Token::tokenType::COMMA) {
	  return node;
	}
	lexer->nextToken();
	while (node->list.back() != nullptr) {
	  node->list.push_back(Expression());
	  if (lexer->getCurrentToken()->getType() == Token::tokenType::COMMA) {
		expect(Token::tokenType::COMMA);
	  } else
		break;
	}
	return node;
  }

  Node *Statement() {
	/*
	 * Statement -> [LabelId ':'] [SimpleStatement | StructStmt]
	 */
	Node *node = Write();
	if (node == nullptr) {
	  node = Read();
	  if (node == nullptr) {
		node = SimpleStatement();
		if (node == nullptr) {
		  node = StructStmt();
		}
	  }
	}
	return node;
  }

  Node *StmtList() {
	/*
	 * StmtList -> Statement/';'...
	 */
	Node *node = new Node(Node::nodeType::STATEMENT_LIST);
	Node *tmp = Statement();
	if (tmp == nullptr) {
	  return nullptr;
	}
	node->list.push_back(tmp);
	if (lexer->getCurrentToken()->getType() == Token::tokenType::Semicolon) {
	  lexer->nextToken();
	}
	while (node->list.back() != nullptr) {
	  tmp = Statement();
	  if (tmp == nullptr) {
		if (node->list.size() == 1) {
		  return node->list.front();
		}
		return node;
	  }
	  node->list.push_back(tmp);
	  if (node->list.back() != nullptr)
		expect(Token::tokenType::Semicolon);
	}
	return node;
  }

  Node *Read() {
	/*
	 * Read -> '(' ident ')' ';'
	 * Read -> ';'
	 */
	Node *node{nullptr};
	if (lexer->getCurrentToken()->getType() == Token::tokenType::READ_Keyword) {
	  node = new Node(Node::nodeType::INPUT);
	  node->value = lexer->getCurrentToken()->getText();
	  lexer->nextToken();
	  if (lexer->getCurrentToken()->getType() == Token::tokenType::LPAR) {
		expect(Token::tokenType::LPAR);
		node->op1 = ExprList();
		expect(Token::tokenType::RPAR);
	  }
	} else
	  return nullptr;
	return node;
  }

  Node *Write() {
	/*
	 * Write -> '(' [ident|string] ')' ';'
	 * Write -> '(' [ident|string] ',' [ident|string] ')' ';'
	 */
	Node *node{nullptr};
	if (lexer->getCurrentToken()->getType() ==
		Token::tokenType::WRITE_Keyword) {
	  node = new Node(Node::nodeType::OUTPUT);
	  node->value = lexer->getCurrentToken()->getText();
	  lexer->nextToken();
	  expect(Token::tokenType::LPAR);
	  node->op1 = ExprList();
	  expect(Token::tokenType::RPAR);
	} else
	  return nullptr;
	return node;
  }

  Node *SimpleStatement() {
	/*
	 * SimpleStatement -> Designator ['(' ExprList ')']
									   -> Designator ':=' Expression
									   -> INHERITED
									   -> GOTO LabelId
	 */
	Node *node = {nullptr};
	Node *tmp = new Node(Node::nodeType::STATEMENT);
	tmp->op1 = Designator();
	if (tmp->op1 == nullptr) {
	  return nullptr;
	}
	node = new Node(Node::nodeType::STATEMENT, "", tmp->op1);
	if (lexer->getCurrentToken()->getType() == Token::tokenType::Assignment) {
	  lexer->nextToken();
	  node->op2 = Expression();
	  node = new binOpNode(node->op1, ":=", node->op2);
	} else if (lexer->getCurrentToken()->getType() == Token::tokenType::LPAR) {
	  lexer->nextToken();
	  node->op2 = ExprList();
	  expect(Token::tokenType::RPAR);
	}

	return node;
  }

  Node *StructStmt() {
	/*
	 * StructStmt -> CompoundStmt
							  -> ConditionalStmt
							  -> LoopStmt
							  -> WithStmt
	 */
	Node *node = CompoundStmt();
	if (node == nullptr) {
	  node = ConditionalStmt();
	  if (node == nullptr) {
		node = LoopStmt();
		if (node == nullptr) {
		  node = WithStmt();
		}
	  }
	}
	return node;
  }

  Node *CompoundStmt() {
	/*
	 * CompoundStmt -> BEGIN StmtList END
	 */
	Node *node{nullptr};
	if (lexer->getCurrentToken()->getType() ==
		Token::tokenType::BEGIN_Keyword) {
	  lexer->nextToken();
	  node = new Node(Node::nodeType::STATEMENT);
	  node->op1 = new StringNode("begin");
	  node->op2 = StmtList();
	  expect(Token::tokenType::END_Keyword);
	  node->op3 = new StringNode("end");
	} else
	  return nullptr;
	return node;
  }

  Node *ConditionalStmt() {
	/*
	 * ConditionalStmt -> IfStmt
									   -> CaseStmt
	 */
	Node *node = IfStmt();
	if (node == nullptr) {
	  node = CaseStmt();
	}
	return node;
  }

  Node *IfStmt() {
	/*
	 * IfStmt -> IF Expression THEN Statement [ELSE Statement]
	 */
	Node *node = {nullptr};
	if (lexer->getCurrentToken()->getType() == Token::tokenType::IF_Keyword) {
	  node = new Node(Node::nodeType::IF);
	  lexer->nextToken();
	  node->op1 = Expression();
	  expect(Token::tokenType::THEN_Keyword);
	  node->op2 = Statement();
	  if (lexer->getCurrentToken()->getType() ==
		  Token::tokenType::ELSE_Keyword) {
		lexer->nextToken();
		node->op3 = Statement();
	  }
	  node = new ifNode(node->op1, node->op2, node->op3);
	} else
	  return nullptr;
	return node;
  }

  Node *CaseStmt() {
	/*
	 * CaseStmt -> CASE Expression OF CaseSelector/';'... [ELSE Statement] [';']
	 * END
	 */
	return nullptr;
	throw NotImplementedException("CaseStmt()");
  }

  Node *CaseSelector() {
	/*
	 * CaseSelector -> CaseLabel/','... ':' Statement
	 */
	throw NotImplementedException("CaseSelector()");
  }

  Node *CaseLabel() {
	/*
	 * CaseLabel -> ConstExpr ['..' ConstExpr]
	 */
	throw NotImplementedException("CaseLabel()");
  }

  Node *LoopStmt() {
	/*
	 * LoopStmt -> RepeatStmt
							-> WhileStmt
							-> ForStmt
	 */
	Node *node = WhileStmt();
	if (node == nullptr) {
	  node = ForStmt();
	}
	return node;
	throw NotImplementedException("LoopStmt()");
  }

  Node *RepeatStmt() {
	/*
	 * RepeatStmt -> REPEAT Statement UNTIL Expression
	 */
	throw NotImplementedException("RepeatStmt()");
  }

  Node *WhileStmt() {
	/*
	 * WhileStmt -> WHILE Expression DO Statement
	 */
	Node *node{nullptr};
	if (lexer->getCurrentToken()->getType() ==
		Token::tokenType::WHILE_Keyword) {
	  lexer->nextToken();
	  node = new Node(Node::nodeType::WHILE);
	  node->op1 = Expression();
	  expect(Token::tokenType::DO_Keyword);
	  node->op2 = Statement();
	}
	return node;
  }

  Node *ForStmt() {
	/*
	 * ForStmt -> FOR QualId ':=' Expression (TO | DOWNTO) Expression DO
	 * Statement
	 */
	Node *node{nullptr};
	if (lexer->getCurrentToken()->getType() == Token::tokenType::FOR_Keyword) {
	  node = new Node(Node::nodeType::FOR_LOOP);
	  node->value = lexer->getCurrentToken()->getText();
	  lexer->nextToken();
	  node->op1 = QualId();
	  expect(Token::tokenType::Assignment);
	  node->op2 = Expression();
	  node->op3 = new StringNode(lexer->getCurrentToken()->getText());
	  lexer->nextToken();
	  node->op4 = Expression();
	  expect(Token::tokenType::DO_Keyword);
	  node->list.push_back(Statement());
	}
	return node;
  }

  Node *WithStmt() {
	/*
	 * WithStmt -> WITH IdentList DO Statement
	 */
	Node *node{nullptr};
	if (lexer->getCurrentToken()->getType() == Token::tokenType::WITH_Keyword) {
	  lexer->nextToken();
	  node = new Node(Node::nodeType::WITH);
	  node->op1 = IdentList();
	  expect(Token::tokenType::DO_Keyword);
	  node->op2 = Statement();
	}
	return node;
  }

  Node *ProcedureDeclSection() {
	/*
	 * ProcedureDeclSection -> ProcedureDecl
													-> FunctionDecl
	 */
	Node *node = ProcedureDecl();
	if (node == nullptr) {
	  node = FunctionDecl();
	}
	return node;
  }

  Node *ProcedureDecl() {
	/*
	 * ProcedureDecl -> ProcedureHeading ';' [Directive]
					 Block ';'
	*/
	Node *node = new Node(Node::nodeType::PROCEDURE);
	node->op1 = ProcedureHeading();
	if (node->op1 == nullptr)
	  return nullptr;
	expect(Token::tokenType::Semicolon);
	node->op2 = Directive();
	if (node->op2 == nullptr) {
	  node->op2 = Block();
	} else {
	  node->op3 = Block();
	}
	expect(Token::tokenType::Semicolon);
	return node;
  }

  Node *FunctionDecl() {
	/*
	 * FunctionDecl -> FunctionHeading ';' [Directive]
					Block ';'
	 */
	Node *node = new Node(Node::nodeType::FUNCTION);
	node->op1 = FunctionHeading();
	if (node->op1 == nullptr)
	  return nullptr;
	expect(Token::tokenType::Semicolon);
	node->op2 = Directive();
	if (node->op2 == nullptr) {
	  node->op2 = Block();
	} else {
	  node->op3 = Block();
	}
	expect(Token::tokenType::Semicolon);
	return node;
  }

  Node *FunctionHeading() {
	/*
	 * FunctionHeading -> FUNCTION Ident [FormalParameters] ':' (SimpleType |
	 * STRING)
	 */
	Node *node{nullptr};
	if (lexer->getCurrentToken()->getType() ==
		Token::tokenType::FUNCTION_Keyword) {
	  lexer->nextToken();
	  node = new Node(Node::nodeType::FUNCTION);
	  node->op1 = Ident();
	  node->op2 = FormalParameters();
	  expect(Token::tokenType::Colon);
	  if (node->op2 == nullptr) {
		node->op2 = SimpleType();
		if (node->op2 == nullptr) {
		  if (lexer->getCurrentToken()->getType() ==
			  Token::tokenType::STRING_Keyword) {
			node->op2 = new StringNode(lexer->getCurrentToken()->getText());
			lexer->nextToken();
		  } else
			expect(Token::tokenType::STRING_Keyword);
		}
	  } else {
		node->op3 = SimpleType();
		if (node->op3 == nullptr) {
		  if (lexer->getCurrentToken()->getType() ==
			  Token::tokenType::STRING_Keyword) {
			node->op3 = new StringNode(lexer->getCurrentToken()->getText());
			lexer->nextToken();
		  } else
			expect(Token::tokenType::STRING_Keyword);
		}
	  }
	}
	return node;
  }

  Node *ProcedureHeading() {
	/*
	 * ProcedureHeading -> PROCEDURE Ident [FormalParameters]
	 */
	Node *node{nullptr};
	if (lexer->getCurrentToken()->getType() ==
		Token::tokenType::PROCEDURE_Keyword) {
	  lexer->nextToken();
	  node = new Node(Node::nodeType::PROCEDURE);
	  node->op1 = Ident();
	  node->op2 = FormalParameters();
	}
	return node;
  }

  Node *FormalParameters() {
	/*
	 * FormalParameters -> '(' FormalParm/';'... ')'
	 */
	Node *node{nullptr};
	Node *tmp;
	if (lexer->getCurrentToken()->getType() == Token::tokenType::LPAR) {
	  node = new Node(Node::nodeType::PARAMS);
	  lexer->nextToken();
	  tmp = FormalParam();
	  node->list.push_back(tmp);
	  if (lexer->getCurrentToken()->getType() == Token::tokenType::Semicolon) {
		lexer->nextToken();
		while (node->list.back() != nullptr) {
		  tmp = FormalParam();
		  if (tmp == nullptr) {
			if (node->list.size() == 1) {
			  return node->list.front();
			}
			return node;
		  }
		  node->list.push_back(tmp);
		  if (lexer->getCurrentToken()->getType() ==
			  Token::tokenType::Semicolon)
			expect(Token::tokenType::Semicolon);
		  else
			break;
		}
	  }
	  expect(Token::tokenType::RPAR);
	}
	return node;
  }

  Node *FormalParam() {
	/*
	 * FormalParam -> [VAR | CONST | OUT] Parameter
	 */
	Node *node = new Node(Node::nodeType::PARAM);
	switch (lexer->getCurrentToken()->getType()) {
	  case Token::tokenType::VAR_Keyword:
	  case Token::tokenType::CONST_Keyword:
	  case Token::tokenType::OUT:node->op1 = new StringNode(lexer->getCurrentToken()->getText());
		lexer->nextToken();
		break;
	  default:break;
	}
	node->op2 = Parameter();
	if (node->op1 == nullptr && node->op2 != nullptr)
	  return node->op2;
	return node;
  }

  Node *Parameter() {
	/*
	 * Parameter -> IdentList  [':' ([ARRAY OF] SimpleType | STRING | FILE)]
							 -> Ident ':' SimpleType '=' ConstExpr
	 */
	Node *node = IdentList();
	if (node != nullptr) {
	  if (lexer->getCurrentToken()->getType() == Token::tokenType::Colon) {
		lexer->nextToken();
		if (lexer->getCurrentToken()->getType() ==
			Token::tokenType::ARRAY_Keyword) {
		  expect(Token::tokenType::OF_Keyword);
		  node->op1 = new StringNode("array of");
		}
		if (node->op1 != nullptr) {
		  node->op2 = SimpleType();
		}
	  }
	  if (node->op1 == nullptr) {
		node->op1 = SimpleType();
		if (node->op1 == nullptr) {
		  if (lexer->getCurrentToken()->getType() ==
			  Token::tokenType::STRING_Keyword ||
			  lexer->getCurrentToken()->getType() ==
				  Token::tokenType::FILE_Keyword) {
			node->op1 = new StringNode(lexer->getCurrentToken()->getText());
			lexer->nextToken();
		  }
		}
	  }
	} else {
	  node = new Node(Node::nodeType::PARAM);
	  node->op1 = Ident();
	  expect(Token::tokenType::Colon);
	  node->op2 = SimpleType();
	  expect(Token::tokenType::EQUAL);
	  node->op3 = ConstExpr();
	}
	return node;
  }

  Node *Directive() {
	/*
	 * Directive -> CDECL
							 -> REGISTER
							 -> DYNAMIC
							 -> VIRTUAL
							 -> EXPORT
							 -> EXTERNAL
							 -> FAR
							 -> FORWARD
							 -> MESSAGE
							 -> OVERRIDE
							 -> OVERLOAD
							 -> PASCAL
							 -> REINTRODUCE
							 -> SAFECALL
							 -> STDCALL
	 */
	return nullptr;
	throw NotImplementedException("Directive()");
  }

  Node *ObjectType() {
	/*
	 * ObjectType -> OBJECT [ObjHeritage] [ObjFieldList] [MethodList] END
	 */
	throw NotImplementedException("ObjectType()");
  }

  Node *ObjHeritage() {
	/*
	 * ObjHeritage -> '(' QualId ')'
	 */
	throw NotImplementedException("ObjHeritage()");
  }

  Node *MethodList() {
	/*
	 * MethodList -> (MethodHeading [';' VIRTUAL])/';'...
	 */
	throw NotImplementedException("MethodList()");
  }

  Node *MethodHeading() {
	/*
	 * MethodHeading -> ProcedureHeading
			  -> FunctionHeading
			  -> ConstructorHeading
			  -> DestructorHeading
	 */
	throw NotImplementedException("MethodHeading()");
  }

  Node *ConstructorHeading() {
	/*
	 * ConstructorHeading -> CONSTRUCTOR Ident [FormalParameters]
	 */
	throw NotImplementedException("ConstructorHeading()");
  }

  Node *DestructorHeading() {
	/*
	 * DestructorHeading -> DESTRUCTOR Ident [FormalParameters]
	 */
	throw NotImplementedException("DestructorHeading()");
  }

  Node *objFieldList() {
	/*
	 * ObjFieldList -> (IdentList ':' Type)/';'...
	 */
	throw NotImplementedException("objFieldList()");
  }

  Node *InitSection() {
	/*
	 * InitSection -> INITIALIZATION StmtList [FINALIZATION StmtList] END
			-> BEGIN StmtList END
			-> END
	 */
	throw NotImplementedException("InitSection()");
  }

  Node *ClassType() {
	/*
	 * ClassType -> CLASS [ClassHeritage]
			 [ClassFieldList]
			 [ClassMethodList]
			 [ClassPropertyList]
			 END
	 */
	throw NotImplementedException("ClassType()");
  }

  Node *ClassHeritage() {
	/*
	 * ClassHeritage -> '(' IdentList ')'
	 */
	throw NotImplementedException("ClassHeritage()");
  }

  Node *ClassVisibility() {
	/*
	 * ClassVisibility -> [PUBLIC | PROTECTED | PRIVATE | PUBLISHED]
	 */
	throw NotImplementedException("ClassVisibility()");
  }

  Node *ClassFieldList() {
	/*
	 * ClassFieldList -> (ClassVisibility ObjFieldList)/';'...
	 */
	throw NotImplementedException("ClassFieldList()");
  }

  Node *ClassMethodList() {
	/*
	 * ClassMethodList -> (ClassVisibility MethodList)/';'...
	 */
	throw NotImplementedException("ClassMethodList()");
  }

  Node *ClassPropertyList() {
	/*
	 * ClassPropertyList -> (ClassVisibility PropertyList ';')...
	 */
	throw NotImplementedException("ClassPropertyList()");
  }

  Node *PropertyList() {
	/*
	 * PropertyList -> PROPERTY  Ident [PropertyInterface]  PropertySpecifiers
	 */
	throw NotImplementedException("PropertyList()");
  }

  Node *PropertyInterface() {
	/*
	 * PropertyInterface -> [PropertyParameterList] ':' Ident
	 */
	throw NotImplementedException("PropertyInterface");
  }

  Node *PropertyParameterList() {
	/*
	 * PropertyParameterList -> '[' (IdentList ':' TypeId)/';'... ']'
	 */
	throw NotImplementedException("PropertyParameterList()");
  }

  Node *PropertySpecifiers() {
	/*
	 * PropertySpecifiers -> [INDEX ConstExpr]
							  [READ Ident]
							  [WRITE Ident]
							  [STORED (Ident | Constant)]
							  [(DEFAULT ConstExpr) | NODEFAULT]
							  [IMPLEMENTS TypeId]
	 */
	throw NotImplementedException("PropertySpecifiers()");
  }

  Node *InterfaceType() {
	/*
	 * InterfaceType -> INTERFACE [InterfaceHeritage]
					 [ClassMethodList]
					 [ClassPropertyList]
					 END
	 */
	throw NotImplementedException("InterfaceType()");
  }

  Node *InterfaceHeritage(bool bCanFail = false) {
	/*
	 * InterfaceHeritage -> '(' IdentList ')'
	 */
	Node *node;
	if (lexer->getCurrentToken()->getType() == Token::tokenType::LPAR) {
	  lexer->nextToken();
	  node = IdentList();
	  return node;
	} else if (!bCanFail) {
	  throw ParsingError("Expected LPAR");
	} else {
	  return nullptr;
	}
  }

  Node *RequiresClause() {
	/*
	 * RequiresClause -> REQUIRES IdentList... ';'
	 */
	throw NotImplementedException("RequiresClause()");
  }

  Node *ContainsClause() {
	/*
	 * ContainsClause -> CONTAINS IdentList... ';'
	 */
	throw NotImplementedException("ContainsClause()");
  }

  Node *IdentList() {
	/*
	 * IdentList -> Ident/','...
	 */
	Node *node = new VarList();
	Node *tmp = Ident(true);
	if (tmp == nullptr)
	  return nullptr;
	node->list.push_back(tmp);
	while (node->list.back() != nullptr) {
	  if (lexer->getCurrentToken()->getType() == Token::tokenType::COMMA) {
		lexer->nextToken();
		node->list.push_back(Ident(true));
	  } else {
		return node;
	  }
	}
	return node;
  }

  Node *QualId() {
	/*
	 * QualId -> [UnitId '.'] Ident
	 */
	return Ident(true);
  }

  Node *TypeId(bool bCanFail = false) {
	/*
	 * TypeId -> [UnitId '.'] <type-identifier>
	 */
	Node *node{nullptr};
	if (lexer->getCurrentToken()->getType() == Token::tokenType::DataType ||
		lexer->getCurrentToken()->getType() == Token::tokenType::INTEGER_Type ||
		lexer->getCurrentToken()->getType() == Token::tokenType::BYTE_Type ||
		lexer->getCurrentToken()->getType() == Token::tokenType::LONGINT_Type ||
		lexer->getCurrentToken()->getType() == Token::tokenType::BOOLEAN_Type ||
		lexer->getCurrentToken()->getType() == Token::tokenType::CHAR_Type ||
		lexer->getCurrentToken()->getType() == Token::tokenType::COMP_Type ||
		lexer->getCurrentToken()->getType() ==
			Token::tokenType::CURRENCY_Type ||
		lexer->getCurrentToken()->getType() == Token::tokenType::DOUBLE_Type ||
		lexer->getCurrentToken()->getType() ==
			Token::tokenType::EXTENDED_Type ||
		lexer->getCurrentToken()->getType() == Token::tokenType::INT64_Type ||
		lexer->getCurrentToken()->getType() ==
			Token::tokenType::LONGWORD_Type ||
		lexer->getCurrentToken()->getType() == Token::tokenType::PCHAR_Type ||
		lexer->getCurrentToken()->getType() == Token::tokenType::REAL_Type ||
		lexer->getCurrentToken()->getType() == Token::tokenType::REAL48_Type ||
		lexer->getCurrentToken()->getType() ==
			Token::tokenType::SHORTINT_Type ||
		lexer->getCurrentToken()->getType() == Token::tokenType::WORD_Type ||
		lexer->getCurrentToken()->getType() ==
			Token::tokenType::STRING_Keyword) {
	  node = new Node(Node::nodeType::VARTYPE);
	  node->value = lexer->getCurrentToken()->getText();
	  lexer->nextToken();
	  return node;
	} else if (!bCanFail) {
	  throw ParsingError("Expected type id");
	} else {
	  return nullptr;
	}
  }

  Node *Ident(bool bCanFail = false) {
	/*
	 * Ident -> <identifier>
	 */
	Node *node;
	if (lexer->getCurrentToken()->getType() == Token::tokenType::Id) {
	  node = new VarNode(lexer->getCurrentToken()->getText());
	  lexer->nextToken();
	  return node;
	} else if (!bCanFail) {
#if __APPLE__
	  StackTraceGenerator::printStack();
#endif
	  throw ParsingError("Expected id");
	} else {
	  return nullptr;
	}
  }

  Node *ConstExpr() {
	Node *node = nullptr;
	if (lexer->getCurrentToken()->getType() == Token::tokenType::Num) {
	  if (std::stod(lexer->getCurrentToken()->getText()) >= 0) {
		node = new NumberNode(lexer->getCurrentToken()->getText());
	  }
	  lexer->nextToken();
	}
	return node;
  }

  Node *UnitId() {
	/*
	 * UnitId -> <unit-identifier>
	 * IDK what this is
	 */
	return nullptr;
  }

  Node *LabelId() {
	/*
	 * LabelId -> <label-identifier>
	 */
	return nullptr;
	throw NotImplementedException("Will not be implemented", "LabelId()");
  }

  Node *Number() {
	/*
	 *  Number -> <number>
	 */
	Node *node = nullptr;
	if (lexer->getCurrentToken()->getType() == Token::tokenType::Num) {
	  node = new NumberNode(lexer->getCurrentToken()->getText());
	  lexer->nextToken();
	}
	return node;
  };

  Node *String() {
	/*
	 * String -> <string>
	 */
	Node *node = nullptr;
	if (lexer->getCurrentToken()->getType() == Token::tokenType::STRING) {
	  node = new StringNode(lexer->getCurrentToken()->getText());
	  lexer->nextToken();
	}
	return node;
  }

  ~Parser() { delete lexer; }
};

#endif // SPO_COMPILER_PARSER_H
