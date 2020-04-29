//
// Created by vhundef on 27.04.2020.
//
#ifndef SPO_COMPILER_LEXER_H
#define SPO_COMPILER_LEXER_H

#include <boost/algorithm/string.hpp>
#include <boost/range/algorithm/remove_if.hpp>
#include <boost/algorithm/string/split.hpp>

#include <iostream>
#include <utility>
#include <vector>
#include <sstream>
#include "Token.hpp"

class Lexer {
private:
    std::vector<Token> tokens;
    std::string program;
    int currentTokenIndex = 0;
    Token *currentToken{};
public:
    void nextToken() {
        if (currentTokenIndex < tokens.size() - 1) {
            currentToken = &tokens[currentTokenIndex];
            currentTokenIndex++;
        }
    }

    [[nodiscard]] Token *getCurrentToken() const {
        return currentToken;
    }

    [[nodiscard]] std::vector<Token> getTokens() const {
        return tokens;
    }

    void printAllTokens() {
        using namespace std;
        cout << "=========================" << endl;
        int i = 0;
        for (auto &token :tokens) {
            cout << "[" << i << "] Type: " << Token::typeToString(token.getType()) << " Value: " << token.getText()
                 << endl;
            i++;
        }
    }

private:
    static std::string singleSpace(std::string const &input) {
        std::istringstream buffer(input);
        std::ostringstream result;

        std::copy(std::istream_iterator<std::string>(buffer),
                  std::istream_iterator<std::string>(),
                  std::ostream_iterator<std::string>(result, " "));
        return result.str();
    }

    static std::string emplaceBefore(std::string str, char what, const std::string &with) {
        for (int i = 0; i < str.length(); i++) {
            if (str[i] == what) {
                str.insert(i++, with);
            }
        }
        return str;
    }

    static std::vector<std::string> splitString(std::string *str) {
        std::vector<std::string> tokens;
        std::string strToTokenize = *str;
        strToTokenize = singleSpace(strToTokenize);
        strToTokenize = emplaceBefore(strToTokenize, ';', " ");
        strToTokenize = emplaceBefore(strToTokenize, '(', " ");
        strToTokenize = emplaceBefore(strToTokenize, ')', " ");
        strToTokenize = emplaceBefore(strToTokenize, '+', " ");
        strToTokenize = emplaceBefore(strToTokenize, '/', " ");
        strToTokenize = emplaceBefore(strToTokenize, '-', " ");
        strToTokenize = emplaceBefore(strToTokenize, '*', " ");
        boost::split(tokens, strToTokenize, boost::is_any_of("\t,\n, "));
        return tokens;
    }

public:
    explicit Lexer(std::string _program) {
        program = std::move(_program);
    }

    void tokenize() {
        for (auto &token : splitString(&program)) {
            tokens.emplace_back(token);
        }
        tokens.pop_back();
    }
};

#endif // SPO_COMPILER_LEXER_H
