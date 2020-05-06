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
            case Node::nodeType::START:
                cout << "START ";
                break;
            case Node::nodeType::EXPR:
                cout << "EXPR ";
                break;
            case Node::nodeType::CONST:
                cout << "CONST ";
                // так как имеет значение, то выводим его в скобках
                cout << "(" << currentNode->value << ")";
                break;
            case Node::nodeType::ADD:
                cout << "ADD ";
                break;
            case Node::nodeType::SUB:
                cout << "SUB ";
                break;
            case Node::nodeType::MUL:
                cout << "MUL ";
                break;
            case Node::nodeType::DIV:
                cout << "DIV ";
                break;
            case Node::nodeType::STATEMENT:
                cout << "STATEMENT ";
                break;
            case Node::nodeType::IF:
                cout << "IF ";
                break;
            case Node::nodeType::ELSE:
                cout << "ELSE ";
                break;
            case Node::nodeType::VAR:
                cout << "VAR ";
                break;
            case Node::nodeType::BINOP:
                cout << "BINOP ";
                cout << "(" << currentNode->value << ")";
                break;
            case Node::nodeType::STR:
                cout << "STR ";
                break;
            case Node::nodeType::FACTOR:
                cout << "FACTOR ";
                break;
            case Node::nodeType::PAR:
                cout << "PAR";
                cout << " ('" << currentNode->value << "')";
                break;
            case Node::nodeType::TERM:
                cout << "TERM ";
                break;
            case Node::nodeType::RESERVED:
                cout << "RESERVED ";
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
    Node*Goal(){
        /*
         * Goal -> (Program | Package  | Library  | Unit)
         */
        throw NotImplementedException("Goal()");
    }
    Node*Program(){
        /*
         * Program -> [PROGRAM Ident ['(' IdentList ')'] ';']
                      ProgramBlock '.'
         */
        throw NotImplementedException("Program()");
    }
    Node* Unit(){
        /*
         * Unit -> UNIT Ident ';'
                   InterfaceSection
                   ImplementationSection
                   InitSection '.'
         */
        throw NotImplementedException("Unit()");
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
        throw NotImplementedException("ProgramBlock()");
    }

    Node *UsesClause() {
        /*
         * UsesClause -> USES IdentList ';
         */
        throw NotImplementedException("UsesClause()");
    }

    Node *InterfaceSection() {
        /*
         * InterfaceSection -> INTERFACE
                               [UsesClause]
                               [InterfaceDecl]...
         */
        throw NotImplementedException("InterfaceSection()");
    }

    Node *InterfaceDecl() {
        /*
         * InterfaceDecl -> ConstSection
                         -> TypeSection
                         -> VarSection
                         -> ExportedHeading
         */
        throw NotImplementedException("InterfaceDecl()");
    }

    Node *ExportedHeading() {
        /*
         * ExportedHeading -> ProcedureHeading ';' [Directive]
                           -> FunctionHeading ';' [Directive]
         */
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
        throw NotImplementedException("Block()");
    }

    Node *DeclSection() {
        /*
         * DeclSection -> LabelDeclSection
                       -> ConstSection
                       -> TypeSection
                       -> VarSection
                       -> ProcedureDeclSection
         */
        throw NotImplementedException("DeclSection()");
    }

    Node *LabelDeclSection() {
        /*
         * LabelDeclSection -> LABEL LabelId
         */
        throw NotImplementedException("LabelDeclSection()");
    }

    Node *ConstSection() {
        /*
         * ConstSection -> CONST (ConstantDecl ';')...
         */
        throw NotImplementedException("ConstantSection()");
    }

    Node *ConstantDecl() {
        /*
         * ConstantDecl -> Ident '=' ConstExpr
                        -> Ident ':' TypeId '=' TypedConstant
         */
        throw NotImplementedException("ConstantDecl()");
    }

    Node *TypeSection() {
        /*
         * TypeSection -> TYPE (TypeDecl ';')...
         */
        throw NotImplementedException("TypeSection()");
    }

    Node *TypeDecl() {
        /*
         * TypeDecl -> Ident '=' Type
                    -> Ident '=' RestrictedType
         */
        throw NotImplementedException("TypeDecl()");
    }

    Node *ArrayConstant() {
        /*
         * ArrayConstant -> '(' TypedConstant/','... ')'
         */
        throw NotImplementedException("ArrayConstant()");
    }

    Node *RecordConstant() {
        /*
         * RecordConstant -> '(' RecordFieldConstant/';'... ')'
         */
        throw NotImplementedException("RecordConstant()");
    }

    Node *RecordFieldConstant() {
        /*
         * RecordFieldConstant -> Ident ':' TypedConstant
         */
        throw NotImplementedException("RecordFieldConstant()");
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
        throw NotImplementedException("Type()");
    }

    Node *RestrictedType() {
        /*
         * RestrictedType -> ObjectType
                          -> ClassType
                          -> InterfaceType
         */
        throw NotImplementedException("RestrictedType()");
    }

    Node *ClassRefType() {
        /*
         * ClassRefType -> CLASS OF TypeId
         */
        throw NotImplementedException("ClassRefType()");
    }

    Node *SimpleType() {
        /*
         * SimpleType -> (OrdinalType | RealType)
         */
        throw NotImplementedException("SimpleType()");
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
        throw NotImplementedException("RealType()");
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
