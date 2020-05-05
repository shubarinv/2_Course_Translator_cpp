//
// Created by vhundef on 27.04.2020.
//
#ifndef SPO_COMPILER_PARSER_H
#define SPO_COMPILER_PARSER_H

#include "Lexer.h"
#include "exceptions.cpp"
#include "Node.hpp"

class Parser {
private:
    Lexer *lexer;


public:
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

    Node *identList() {
        Node *node = new VarList();
        node->op1 = ident();
        Node *prevNode = node->op1;
        while (lexer->getCurrentToken()->getType() == Token::tokenType::COMMA) {
            prevNode->op1 = ident();
            prevNode = prevNode->op1;
        }
        return node;
    }

    Node *ident() {
        if (lexer->getCurrentToken()->getType() == Token::tokenType::Id) {
            Node *node = new VarNode(lexer->getCurrentToken()->getText());
            lexer->nextToken();
            return node;
        } else {
            throw ParsingError("Expected identifier");
        }
    }

    Node *Type() {
        /*
         * -> TypeId
         * -> SimpleType      //Not implemented
         * -> StrucType       //Not implemented
         * -> PointerType     //Not implemented
         * -> StringType      //Not implemented
         * -> ProcedureType   //Not implemented
         * -> VariantType     //Not implemented
         * -> ClassRefType    //Not implemented
         */
        return typeId();
    }

    Node *typeId() {
        /*
         * [UnitId '.'] <type-identifier>
         * [UnitId '.'] //Not implemented
         */
        if (lexer->getCurrentToken()->getType() == Token::tokenType::Id) {
            Node *node = new Node(Node::nodeType::VARTYPE, lexer->getCurrentToken()->getText());
            lexer->nextToken();
            return node;
        }
        throw ParsingError("Expected TypeID");
    }

    Node *varDecl() {
        /*
         * IdentList ':' Type [(ABSOLUTE (Ident | ConstExpr)) | '=' ConstExpr]
         */
        Node *node = new Node(Node::nodeType::VARDECL);
        node->op1 = identList();
        expect(Token::tokenType::Colon);
        node->op2 = Type();
        return nullptr;
    }

    Node *program() {
        Node *node = new Node(Node::nodeType::START);
        return node;
    }

    Node *expr() {
        /*
         *  SimpleExpression [RelOp SimpleExpression]...
         */
        Node *node = new Node(Node::nodeType::EXPR);
        node->op1 = simpleExpression();
        node->op2 = relOp();
        if (node->op2 == nullptr) {
            return node->op1;
        }
        node->op3 = simpleExpression();
        node = new Node(node->op2->type, node->op2->value, node->op1, node->op3);
        return node;
    }

    Node *relOp() {
        /*
         *  -> '>'
         *  -> '<'
         *  -> '<='
         *  -> '>='
         *  -> '<>'
         *  -> IN
         *  -> IS
         *  -> AS
         */
        Node *node;
        if (lexer->getCurrentToken()->getType() == Token::tokenType::Comparison) {
            node = new Node(Node::nodeType::COMP, lexer->getCurrentToken()->getText());
            return node;
        }
        return nullptr;
    }

    Node *factor() {
        /*
         *   -> Number
         *   -> String
         *   -> NIL                         // not implemented
         *   -> '(' Expression ')'
         *   -> NOT Factor                  // not implemented
         *   -> TypeId '(' Expression ')'   // not implemented
         */
        Node *node;
        switch (lexer->getCurrentToken()->getType()) {
            case Token::tokenType::Num: ///< factor() -> Number
                node = new NumberNode(lexer->getCurrentToken()->getText());
                lexer->nextToken();
                return node;
            case Token::tokenType::String: ///< factor() -> String
                node = new StringNode(lexer->getCurrentToken()->getText());
                lexer->nextToken();
                return node;
            case Token::tokenType::LPAR: ///< factor() -> '(' Expression ')'
                expect(Token::tokenType::LPAR);
                node = new Node(Node::nodeType::FACTOR);
                node->op1 = new ParenNode("(");
                node->op2 = expr();
                expect(Token::tokenType::RPAR);
                node->op3 = new ParenNode(")");
                return node;
            default:
                throw ParsingError(
                        "ParsingError: was expecting a Left_Paren|Number|String, but got " +
                        lexer->getCurrentToken()->getText() + " , which is " +
                        Token::typeToString(lexer->getCurrentToken()->getType()));
        }
    }

    Node *multOp() {
        Node *node;
        /*
             * -> '*'
             * -> '/'
             * -> DIV  // not implemented
             * -> MOD  // not implemented
             * -> AND  // not implemented
         */
        switch (lexer->getCurrentToken()->getType()) {
            case Token::tokenType::MathMult:
                node = new Node(Node::nodeType::MUL, "*");
                lexer->nextToken();
                return node;
            case Token::tokenType::MathDiv:
                node = new Node(Node::nodeType::DIV, "/");
                lexer->nextToken();
                return node;
        }
        return nullptr;
    }

    Node *term() {
        /*
           * Factor [MulOp Factor]
        */
        Node *node = new Node(Node::nodeType::TERM);
        node->op1 = factor();
        node->op2 = multOp();
        if (node->op2 == nullptr) {
            return node->op1;
        }
        node->op3 = factor();
        node = new binOpNode(node->op1, node->op2->value, node->op3);
        return node;
    }

    Node *simpleExpression() {
        /*
           * SimpleExpression -> ['+' | '-'] Term [AddOp Term]
         */
        Node *node = new Node(Node::nodeType::EXPR);
        if (lexer->getCurrentToken()->getType() == Token::tokenType::MathPlus) {
            node->op1 = new Node(Node::nodeType::ADD, "+");
            lexer->nextToken();
        } else if (lexer->getCurrentToken()->getType() == Token::tokenType::MathMinus) {
            node->op1 = new Node(Node::nodeType::SUB, "-");
            lexer->nextToken();
        }
        if (node->op1 == nullptr) {
            node->op1 = term();
            node->op2 = addOp();
            if (node->op2 == nullptr) {
                return node->op1;
            }
            node->op3 = term();
            node = new Node(node->op2->type, node->op2->value, node->op1, node->op3);
            return node;
        } else {
            node->op2 = term();

            node->op3 = addOp();
            if (node->op3 == nullptr) {
                return node->op2;
            }
            node->op4 = term();
            return node;
        }
    }

    Node *statement() {
        /*
         * [LabelId ':'] [SimpleStatement | StructStmt]
         */
        /* [LabelId ':'] not implemented*/
        /* StructStmt not implemented*/

        return simpleExpression();
    }

    Node *addOp() {
        /*
             * -> '+'
             * -> '-'
             * -> OR  // not implemented
             * -> XOR // not implemented
        */
        Node *node;
        switch (lexer->getCurrentToken()->getType()) {
            case Token::tokenType::MathPlus:
                node = new Node(Node::nodeType::ADD, "+");
                lexer->nextToken();
                return node;
            case Token::tokenType::MathMinus:
                node = new Node(Node::nodeType::SUB, "-");
                lexer->nextToken();
                return node;
        }
        return nullptr;
    }

    void parse() {
        lexer->tokenize();
        Node *node = statement();
        printRecursive(node, 0);
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
            case Node::nodeType::COMP:
                cout << "COMP ";
                break;
            case Node::nodeType::VARDECL:
                cout << "VARDECL ";
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

    ~ Parser() {
        delete lexer;
    }
};


#endif //SPO_COMPILER_PARSER_H
