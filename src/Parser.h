//
// Created by vhundef on 27.04.2020.
//
#ifndef SPO_COMPILER_PARSER_H
#define SPO_COMPILER_PARSER_H

#include <stdexcept>
#include "Lexer.h"
#include "Node.hpp"
#include "exceptions.cpp"

class Parser {
 private:
  Lexer *lexer;

 public:
  void parse() {
	lexer->tokenize();
	lexer->printAllTokens();
	Node *ast = Goal();
	printRecursive(ast, 0);
  }

  explicit Parser(const std::string &_filename) {
	lexer = new Lexer(_filename);
	parse();
  }

  void expect(Token::tokenType tokenType) {
	if (lexer->getCurrentToken()->getType() == tokenType) {
	  lexer->nextToken();
	} else {
	  std::string error = "Expected token of type: ";
	  error += Token::typeToString(tokenType);
	  error += ", but got: ";
	  error += Token::typeToString(lexer->getCurrentToken()->getType());
	  throw ParsingError(error);
	}
  }

  void printRecursive(Node *currentNode, size_t level) {
	using namespace std;
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
	  case Node::nodeType::CONST:cout << "CONST ";
		// так как имеет значение, то выводим его в скобках
		cout << "(" << currentNode->value << ")";
		break;
	  case Node::nodeType::ADD:cout << "ADD ";
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
		break;
	  case Node::nodeType::BINOP:cout << "BINOP ";
		cout << "(" << currentNode->value << ")";
		break;
	  case Node::nodeType::STR:cout << "STR ";
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
	}

	cout << endl;

	// т.к рекурсивная
	printRecursive(currentNode->op1, level + 1);
	printRecursive(currentNode->op2, level + 1);
	printRecursive(currentNode->op3, level + 1);
	printRecursive(currentNode->op4, level + 1);
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
	if (lexer->getCurrentToken()->getType() == Token::tokenType::PROGRAM_Keyword) {
	  lexer->nextToken();
	  node->op1 = Ident();
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
	Node *node = new Node(Node::nodeType::UNIT);
	node->op1 = Ident();
	expect(Token::tokenType::Semicolon);
	node->op2 = InterfaceSection();
	node->op3 = ImplementationSection();
	node->op4 = InitSection();
	expect(Token::tokenType::DOT);
	return node;
  }

  Node *Package() {
	/*
	 * Package -> PACKAGE Ident ';'
				  [RequiresClause]
				  [ContainsClause]
				  END '.'
	 */
	throw NotImplementedException("Package()");
  }

  Node *Library() {
	/*
	 * Library -> LIBRARY Ident ';'
				  ProgramBlock '.'
	 */
	throw NotImplementedException("Library()");
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
	Node *node = new Node(Node::nodeType::NODE);
	if ((node->op1 = ProcedureHeading()) != nullptr) {
	  expect(Token::tokenType::Semicolon);
	  node->op2 = Directive();
	}
	throw NotImplementedException("ExportedHeading()");
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
	Node *node = LabelDeclSection();
	if (node == nullptr) {
	  node = ConstSection();
	  if (node == nullptr) {
		node = TypeSection();
		if (node == nullptr) {
		  node = VarSection();
		  if (node == nullptr) {
			node = ProcedureDeclSection();
		  }
		}
	  }
	}
	return node;
  }

  Node *LabelDeclSection() {
	/*
	 * LabelDeclSection -> LABEL LabelId
	 */
	if (lexer->getCurrentToken()->getType() == Token::tokenType::LABEL_Keyword) {
	  lexer->nextToken();
	  return LabelId();
	} else return nullptr;
  }

  Node *ConstSection() {
	/*
	 * ConstSection -> CONST (ConstantDecl ';')...
	 */
	Node *node = new Node(Node::nodeType::SECTION);
	if (lexer->getCurrentToken()->getType() == Token::tokenType::CONST_Keyword) {
	  lexer->nextToken();
	  node->list.push_back(ConstantDecl());
	  expect(Token::tokenType::Semicolon);
	  while (node->list.back() != nullptr) {
		node->list.push_back(ConstantDecl());
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
	Node *node = new Node(Node::nodeType::CONST);
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
	Node *node = new Node(Node::nodeType::SECTION);
	if (lexer->getCurrentToken()->getType() == Token::tokenType::TYPE_Keyword) {
	  lexer->nextToken();
	  node->list.push_back(TypeDecl());
	  expect(Token::tokenType::Semicolon);
	  while (node->list.back() != nullptr) {
		node->list.push_back(TypeDecl());
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
	Node *node = new Node(Node::nodeType::TYPE);
	node->op1 = Ident();
	if (lexer->getCurrentToken()->getType() == Token::tokenType::EQUAL) {
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
		  if (lexer->getCurrentToken()->getType() != Token::tokenType::COMMA)
			break;
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
	node = TypeId();
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
	if (lexer->getCurrentToken()->getType() == Token::tokenType::CLASS_Keyword) {
	  lexer->nextToken();
	  expect(Token::tokenType::OF);
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
	Node *node = new Node(Node::nodeType::TYPE);
	if (lexer->getCurrentToken()->getType() == Token::tokenType::REAL48_Type ||
		lexer->getCurrentToken()->getType() == Token::tokenType::REAL_Type ||
		lexer->getCurrentToken()->getType() == Token::tokenType::SINGLE_Type ||
		lexer->getCurrentToken()->getType() == Token::tokenType::DOUBLE_Type ||
		lexer->getCurrentToken()->getType() == Token::tokenType::EXTENDED_Type ||
		lexer->getCurrentToken()->getType() == Token::tokenType::CURRENCY_Type ||
		lexer->getCurrentToken()->getType() == Token::tokenType::COMP_Type) {
	  node->value = lexer->getCurrentToken()->getText();
	} else return nullptr;

	return node;
  }

  Node *OrdinalType() {
	/*
	 * OrdinalType -> (SubrangeType | EnumeratedType | OrdIdent)
	 */
	throw NotImplementedException("OrdinalType()");
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
	throw NotImplementedException("OrdIdent()");
  }

  Node *VariantType() {
	/*
	 * VariantType -> VARIANT
				   -> OLEVARIANT
	 */
	throw NotImplementedException("VariantType()");
  }

  Node *SubrangeType() {
	/*
	 * SubrangeType -> ConstExpr '..' ConstExpr
	 */
	throw NotImplementedException("SubrangeType()");
  }

  Node *EnumeratedType() {
	/*
	 * EnumeratedType -> '(' IdentList ')'
	 */
	throw NotImplementedException("EnumeratedType()");
  }

  Node *StringType() {
	/*
	 * StringType -> STRING
				  -> ANSISTRING
				  -> WIDESTRING
				  -> STRING '[' ConstExpr ']'
	 */
	throw NotImplementedException("StrucType()");
  }

  Node *StrucType() {
	/*
	 * StrucType -> [PACKED] (ArrayType | SetType | FileType | RecType)
	 */
	throw NotImplementedException("StrucType()");
  }

  Node *ArrayType() {
	/*
	 * ArrayType -> ARRAY ['[' OrdinalType/','... ']'] OF Type
	 */
	throw NotImplementedException("ArrayType()");
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
	throw NotImplementedException("ProcedureType()");
  }

  Node *ProcedureType() {
	/*
	 * ProcedureType -> (ProcedureHeading | FunctionHeading) [OF OBJECT]
	 */
	throw NotImplementedException("ProcedureType()");
  }

  Node *VarSection() {
	/*
	 * VarSection -> VAR (VarDecl ';')...
	 */
	throw NotImplementedException("VarSection()");
  }

  Node *VarDecl() {
	/*
	 * VarDecl -> IdentList ':' Type [(ABSOLUTE (Ident | ConstExpr)) | '=' ConstExpr]
	 */
	throw NotImplementedException("VarDecl()");
  }

  Node *Expression() {
	/*
	 * Expression -> SimpleExpression [RelOp SimpleExpression]...
	 */
	throw NotImplementedException("Expression()");
  }

  Node *SimpleExpression() {
	/*
	 * SimpleExpression -> ['+' | '-'] Term [AddOp Term]...
	 */
	throw NotImplementedException("SimpleExpression()");
  }

  Node *Term() {
	/*
	 * Term -> Factor [MulOp Factor]...
	 */
	throw NotImplementedException("Term()");
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
	throw NotImplementedException("Factor()");
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
	throw NotImplementedException("RelOp()");
  }

  Node *AddOp() {
	/*
	 * AddOp -> '+'
			 -> '-'
			 -> OR
			 -> XOR
	 */
	throw NotImplementedException("AddOp()");
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
	throw NotImplementedException("MulOp()");
  }

  Node *Designator() {
	/*
	 * Designator -> QualId ['.' Ident | '[' ExprList ']' | '^']...
	 */
	throw NotImplementedException("Designator()");
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
	throw NotImplementedException("ExprList()");
  }

  Node *Statement() {
	/*
	 * Statement -> [LabelId ':'] [SimpleStatement | StructStmt]
	 */
	throw NotImplementedException("Statement()");
  }

  Node *StmtList() {
	/*
	 * StmtList -> Statement/';'...
	 */
	throw NotImplementedException("StmtList()");
  }

  Node *SimpleStatement() {
	/*
	 * SimpleStatement -> Designator ['(' ExprList ')']
					   -> Designator ':=' Expression
					   -> INHERITED
					   -> GOTO LabelId
	 */
	throw NotImplementedException("SimpleStatement()");
  }

  Node *StructStmt() {
	/*
	 * StructStmt -> CompoundStmt
				  -> ConditionalStmt
				  -> LoopStmt
				  -> WithStmt
	 */
	throw NotImplementedException("StructStmt()");
  }

  Node *CompoundStmt() {
	/*
	 * CompoundStmt -> BEGIN StmtList END
	 */
	throw NotImplementedException("ConditionalStmt()");
  }

  Node *ConditionalStmt() {
	/*
	 * ConditionalStmt -> IfStmt
					   -> CaseStmt
	 */
	throw NotImplementedException("ConditionalStmt()");
  }

  Node *IfStmt() {
	/*
	 * IfStmt -> IF Expression THEN Statement [ELSE Statement]
	 */
	throw NotImplementedException("IfStmt()");
  }

  Node *CaseStmt() {
	/*
	 * CaseStmt -> CASE Expression OF CaseSelector/';'... [ELSE Statement] [';'] END
	 */
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
	throw NotImplementedException("WhileStmt()");
  }

  Node *ForStmt() {
	/*
	 * ForStmt -> FOR QualId ':=' Expression (TO | DOWNTO) Expression DO Statement
	 */
	throw NotImplementedException("ForStmt()");
  }

  Node *WithStmt() {
	/*
	 * WithStmt -> WITH IdentList DO Statement
	 */
	throw NotImplementedException("WithStmt()");
  }

  Node *ProcedureDeclSection() {
	/*
	 * ProcedureDeclSection -> ProcedureDecl
							-> FunctionDecl
	 */
	throw NotImplementedException("ProcedureDeclSection()");
  }

  Node *ProcedureDecl() {
	/*
	 * ProcedureDecl -> ProcedureHeading ';' [Directive]
			 Block ';'
	*/
	throw NotImplementedException("ProcedureDecl()");
  }

  Node *FunctionDecl() {
	/*
	 * FunctionDecl -> FunctionHeading ';' [Directive]
			Block ';'
	 */
	throw NotImplementedException("FunctionDecl()");
  }

  Node *FunctionHeading() {
	/*
	 * FunctionHeading -> FUNCTION Ident [FormalParameters] ':' (SimpleType | STRING)
	 */
	throw NotImplementedException("FunctionHeading()");
  }

  Node *ProcedureHeading() {
	/*
	 * ProcedureHeading -> PROCEDURE Ident [FormalParameters]
	 */
	throw NotImplementedException("ProcedureHeading()");
  }

  Node *FormalParameters() {
	/*
	 * FormalParameters -> '(' FormalParm/';'... ')'
	 */
	throw NotImplementedException("FormalParameters()");
  }

  Node *FormalParam() {
	/*
	 * FormalParam -> [VAR | CONST | OUT] Parameter
	 */
	throw NotImplementedException("FormalParam()");
  }

  Node *Parameter() {
	/*
	 * Parameter -> IdentList  [':' ([ARRAY OF] SimpleType | STRING | FILE)]
				 -> Ident ':' SimpleType '=' ConstExpr
	 */
	throw NotImplementedException("Parameter()");
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
	node->list.push_back(Ident());
	while (node->list.back() != nullptr) {
	  if (lexer->getCurrentToken()->getType() == Token::tokenType::COMMA) {
		lexer->nextToken();
		node->list.push_back(Ident());
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
	return Ident();
  }

  Node *TypeId(bool bCanFail = false) {
	/*
	 * TypeId -> [UnitId '.'] <type-identifier>
	 */
	Node *node = new Node(Node::nodeType::VARTYPE);
	if (lexer->getCurrentToken()->getType() == Token::tokenType::DataType) {
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
	throw NotImplementedException("Will not be implemented", "LabelId()");
	return nullptr;
  }

  Node *Number() {
	/*
	 *  Number -> <number>
	 */
	Node *node = nullptr;
	if (lexer->getCurrentToken()->getType() == Token::tokenType::Num) {
	  node = new NumberNode(lexer->getCurrentToken()->getText());
	}
	return node;
  };

  Node *String() {
	/*
	 * String -> <string>
	 */
	Node *node = nullptr;
	if (lexer->getCurrentToken()->getType() == Token::tokenType::String) {
	  node = new StringNode(lexer->getCurrentToken()->getText());
	}
	return node;
  }

  ~Parser() { delete lexer; }
};

#endif // SPO_COMPILER_PARSER_H
