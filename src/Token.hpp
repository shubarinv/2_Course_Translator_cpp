//
// Created by vhund on 28.04.2020.
//

#ifndef SPO_COMPILER_TOKEN_HPP
#define SPO_COMPILER_TOKEN_HPP

#include <string>
#include <utility>
#include <algorithm>
#include <iostream>
#include <boost/regex.hpp>


class Token {
public:
    enum class tokenType {
        Keyword,
        MathOp,
        Bracket,
        Logic,
        Comparison,
        Semicolon,
        DataType,
        Assignment,
        Id,
        Num,
        Undefined
    };
private:
    tokenType type = tokenType::Undefined;
public:
    [[nodiscard]] tokenType getType() const {
        return type;
    }

private:
    std::string text;
public:
    [[nodiscard]] const std::string &getText() const {
        return text;
    }

private:

    static bool isNumber(const std::string &s) {
        return !s.empty() &&
               std::find_if(s.begin(), s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
    }

public:
    static tokenType determineTokenType(const std::string &str) {
        using namespace std;
        boost::regex IdRegex("^[a-zA-Z_$][a-zA-Z_$0-9]*$");
        boost::smatch what;
        if (isNumber(str))
            return tokenType::Num;
        else if (str == ":=") return tokenType::Assignment;
        else if (str == "==") return tokenType::Comparison;

        else if (str == "+") return tokenType::MathOp;
        else if (str == "-") return tokenType::MathOp;
        else if (str == "*") return tokenType::MathOp;
        else if (str == "/") return tokenType::MathOp;

        else if (str == "if") return tokenType::Keyword;
        else if (str == "else") return tokenType::Keyword;
        else if (str == "for") return tokenType::Keyword;
        else if (str == "while") return tokenType::Keyword;

        else if (str == "(") return tokenType::Bracket;
        else if (str == ")") return tokenType::Bracket;
        else if (str == ";") return tokenType::Semicolon;

        else if (str == ">") return tokenType::Comparison;
        else if (str == "<") return tokenType::Comparison;
        else if (str == ">=") return tokenType::Comparison;
        else if (str == "<=") return tokenType::Comparison;

        else if (str == "int") return tokenType::DataType;

        else if (str == " ") {
            std::cout << "tokenValue is somehow whitespace" << endl;
            return tokenType::Undefined;
        } else if(boost::regex_match(str,what,IdRegex))
            return tokenType::Id;
        else
            return tokenType::Undefined;
    }

public:

    explicit Token(std::string _val) {
        text = std::move(_val);
        type= determineTokenType(text);
        //std::cout << "Token of value: " << text << " has type: " << typeToString(type) << std::endl;
    }

    static std::string typeToString(tokenType _type) {
        switch (_type) {
            case (tokenType::Assignment):
                return "Assignment";
            case (tokenType::Comparison):
                return "Comparison";
            case tokenType::Keyword:
                return "Keyword";
            case tokenType::MathOp:
                return "MathOp";
            case tokenType::Bracket:
                return "Bracket";
            case tokenType::Logic:
                return "Logic";
            case tokenType::Semicolon:
                return "Semicolon";
            case tokenType::DataType:
                return "DataType";
            case tokenType::Id:
                return "Id";
            case tokenType::Num:
                return "Num";
            case tokenType::Undefined:
                return "Undefined";
        }
        return "ERROR";
    }
};


#endif //SPO_COMPILER_TOKEN_HPP
