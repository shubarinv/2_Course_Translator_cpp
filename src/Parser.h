//
// Created by vhundef on 27.04.2020.
//
#ifndef SPO_COMPILER_PARSER_H
#define SPO_COMPILER_PARSER_H


#include "Lexer.h"
#include "Ast.hpp"
#include "exceptions.cpp"

class Parser {
private:
    Lexer *lexer;
    Ast *ast;

    Node *program() {
        Node *statementNode = statement();
        return statementNode;
    }

    Node *statement() {
        // вызываем метод expression, согласно грамматике
        Node *expressionNode = expression();
        // возвращаем результат прошлой функции, согласно грамматике
        return expressionNode;
    }

    Node *expression() {
        // вызываем метод additive_expression_node, согласно грамматике
        Node *additiveExpressionNode = additiveExpression();
        // возвращаем результат прошлой функции, согласно грамматике
        return additiveExpressionNode;
    }

    Node *additiveExpression() {
        // согласно грамматике вызываем multiplicative_expression
        Node* multiplicative_node = multiplicativeExpression();

        // если текущий токен плюс
        if (lexer->getCurrentToken()->getType() == Token::tokenType::MathPlus)
        {
            // если мы вошли в условие, значит необходимо сдвинуть текущий токен
            lexer->nextToken();

            // вызываем метод согласно грамматике
            Node* additive_temp_node = additiveExpression();

            // создаем новый узел с типом сложение
            Node* new_node = new Node(Node::nodeType::ADD,"", multiplicative_node,
                                      additive_temp_node);

            // возвращаем полученный токен
            return new_node;
        }
            // если текущий токен минус
        else if (lexer->getCurrentToken()->getType() == Token::tokenType::MathMinus)
        {
            // если мы вошли в условие, значит необходимо сдвинуть текущий токен
            lexer->nextToken();

            // вызываем метод согласно грамматике
            Node* additive_temp_node = additiveExpression();

            // создаем новый узел с типом вычитание
            Node* new_node = new Node(Node::nodeType::SUB,"", multiplicative_node,
                                      additive_temp_node);

            // возвращаем полученный токен
            return new_node;
        }

        // если ни одно из условий не выполнено, то возвращаем результат
        // общего нетерминала
        return multiplicative_node;
    }

    Node *multiplicativeExpression() {
        // согласно грамматике вызываем primary_expression
        Node *primary_node = primaryExpression();

        // если текущий токен звездочка
        if (lexer->getCurrentToken()->getType() == Token::tokenType::MathMult) {
            // если мы вошли в условие, значит необходимо сдвинуть текущий токен
            lexer->nextToken();

            // вызываем метод согласно грамматике
            Node *multiplicative_temp_node = multiplicativeExpression();

            // создаем новый узел с типом умножение
            Node *new_node = new Node(Node::nodeType::MUL,"", primary_node, multiplicative_temp_node);

            // возвращаем полученный токен
            return new_node;
        }
            // если текущий токен слэш
        else if (lexer->getCurrentToken()->getType() == Token::tokenType::MathDiv) {
            // если мы вошли в условие, значит необходимо сдвинуть текущий токен
            lexer->nextToken();

            // вызываем метод согласно грамматике
            Node *multiplicative_temp_node = multiplicativeExpression();

            // создаем новый узел с типом деление, так как токен был знаком деления
            Node *new_node = new Node(Node::nodeType::DIV,"", primary_node, multiplicative_temp_node);

            // возвращаем полученный токен
            return new_node;
        }

        // если ни одно из условий не выполнено, то возвращаем результат
        // общего нетерминала
        return primary_node;

    }

    Node *primaryExpression() {
        // если тип текущего токена — число
        if (lexer->getCurrentToken()->getType() == Token::tokenType::Num) {
            // вызываем метод согласно грамматике
            Node *numberNode = number();
            // возвращаем полученный результат
            return numberNode;
        }
            // если — открывающаяся круглая скобка
        else if (lexer->getCurrentToken()->getType() == Token::tokenType::LPAR) {
            // вызываем метод согласно грамматике
            Node *parenExpressionNode = parenExpression();
            // возвращаем полученный результат
            return parenExpressionNode;
        }

        // а если ничего из выше описанного, то возвращаем nullptr
        return nullptr;
    }

    Node *parenExpression() {
        // если тип текущего токена не равен (
        if (lexer->getCurrentToken()->getType() != Token::tokenType::LPAR) {
            // то возбуждаем ошибку "Ожидалась '('"
            throw ParsingError(
                    "ParsingError: was expecting a Left_Paren, but got " + lexer->getCurrentToken()->getText() +
                    " , which is" + Token::typeToString(lexer->getCurrentToken()->getType()));
        }
        // если ошибки нет, то сдвигаем текущий токен
        lexer->nextToken();

        // вызываем метод expression, согласно грамматике
        Node *expressionNode = expression();

        // если тип текущего токена не равен )
        if (lexer->getCurrentToken()->getType() != Token::tokenType::RPAR) {
            // то возбуждаем ошибку "Ожидалась ')'"
            throw ParsingError(
                    "ParsingError: was expecting a Right_Paren, but got " + lexer->getCurrentToken()->getText() +
                    " , which is" + Token::typeToString(lexer->getCurrentToken()->getType()));
        }
        // если ошибки нет, то сдвигаем текущий токен
        lexer->nextToken();

        return expressionNode; // по-умолчанию возвращаем nullptr
    }

    Node *number() {
        if (lexer->getCurrentToken()->getType() != Token::tokenType::Num) {
            throw ParsingError("ParsingError: was expecting a number, but got " + lexer->getCurrentToken()->getText() +
                               " , which is" + Token::typeToString(lexer->getCurrentToken()->getType()));
        }

        // если же это число то
        // получаем значение текущего токена, которое равно числу
        std::string numberStr = lexer->getCurrentToken()->getText();

        // сдвигаем текущий токен
        lexer->nextToken();

        // создаем новый узел с типом CONSTANT и значением равным значению
        // из лексемы текущего токена
        Node *numberNode = new Node(Node::nodeType::CONST, numberStr);

        // возвращаем полученный узел
        return numberNode;
    }

public:
    explicit Parser(const std::string &_filename) {
        lexer = new Lexer(_filename);

        ast = new Ast();
    }

    void parse() {
        lexer->tokenize();

        ast->tree = program();

        ast->printRecursive(ast->tree, 0);
    }

    ~ Parser() {
        delete lexer;
        delete ast;
    }
};


#endif //SPO_COMPILER_PARSER_H
