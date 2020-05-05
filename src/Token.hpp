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
        Logic,
        Comparison,
        Semicolon,
        DataType,
        Assignment,
        Id,
        Num,
        Undefined,
        String,
        Colon,
        COMMA,
        DOT,
        LPAR,
        LBRACE,
        RPAR,
        RBRACE, MathDiv, MathMult, MathMinus, MathPlus, IF_Keyword,
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

    static bool isNumber(const std::string &str) {
        return !str.empty() &&
               std::find_if(str.begin(), str.end(), [](unsigned char c) { return !std::isdigit(c); }) == str.end();
    }

public:
    static tokenType determineTokenType(const std::string &str) {
        using namespace std;
        boost::regex IdRegex("^[a-zA-Z_$][a-zA-Z_$0-9]*$");
        boost::smatch what;

        try { // checking if string is double
            double value = std::stod(str);
            return tokenType::Num;
        }
        catch (std::exception &e) {}

        if ((str[0] == '\'' || str[0] == '\"') && str[str.size() - 1] == '\'' || str[str.size() - 1] == '\"')
            return tokenType::String;

        if (isNumber(str))
            return tokenType::Num;
        else if (str == ":=") return tokenType::Assignment;
        else if (str == "==") return tokenType::Comparison;

        else if (str == "+") return tokenType::MathPlus;
        else if (str == "-") return tokenType::MathMinus;
        else if (str == "*") return tokenType::MathMult;
        else if (str == "/") return tokenType::MathDiv;

        else if (str == "if") return tokenType::IF_Keyword;
        else if (str == "then") return tokenType::Keyword;
        else if (str == "else") return tokenType::Keyword;
        else if (str == "for") return tokenType::Keyword;
        else if (str == "while") return tokenType::Keyword;
        else if (str == "var") return tokenType::Keyword;
        else if (str == "procedure") return tokenType::Keyword;
        else if (str == "type") return tokenType::Keyword;
        else if (str == "begin") return tokenType::Keyword;
        else if (str == "end") return tokenType::Keyword;
        else if (str == "downto") return tokenType::Keyword;
        else if (str == "do") return tokenType::Keyword;
        else if (str == "Continue") return tokenType::Keyword;
        else if (str == "readln") return tokenType::Keyword;
        else if (str == "writeln") return tokenType::Keyword;
        else if (str == "write") return tokenType::Keyword;
        else if (str == "read") return tokenType::Keyword;
        else if (str == "in") return tokenType::Keyword;
        else if (str == "as") return tokenType::Keyword;
        else if (str == "is") return tokenType::Keyword;

        else if (str == "(") return tokenType::LPAR;
        else if (str == ")") return tokenType::RPAR;
        else if (str == "{") return tokenType::LBRACE;
        else if (str == "}") return tokenType::RBRACE;
        else if (str == ";") return tokenType::Semicolon;
        else if (str == ":") return tokenType::Colon;
        else if (str == ",") return tokenType::COMMA;
        else if (str == ".") return tokenType::DOT;

        else if (str == ">") return tokenType::Comparison;
        else if (str == "<") return tokenType::Comparison;
        else if (str == ">=") return tokenType::Comparison;
        else if (str == "<=") return tokenType::Comparison;


        else if (str == "Integer") return tokenType::DataType;
        else if (str == "LongInt") return tokenType::DataType;
        else if (str == "Byte") return tokenType::DataType;

        else if (boost::regex_match(str, what, IdRegex))
            return tokenType::Id;
        else
            return tokenType::Undefined;
    }

public:

    explicit Token(std::string _val) {
        text = std::move(_val);
        type = determineTokenType(text);
    }

    static std::string typeToString(tokenType _type) {
        switch (_type) {
            case (tokenType::Assignment):
                return "Assignment";
            case (tokenType::Comparison):
                return "Comparison";
            case tokenType::Keyword:
                return "Keyword";
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
            case tokenType::String:
                return "String";
            case tokenType::Colon:
                return "Colon";
            case tokenType::COMMA:
                return "COMMA";
            case tokenType::DOT:
                return "DOT";
            case tokenType::LPAR:
                return "LPAR";
            case tokenType::LBRACE:
                return "LBRACE";
            case tokenType::RPAR:
                return "RPAR";
            case tokenType::RBRACE:
                return "RBRACE";
            case tokenType::MathDiv:
                return "MathDiv";
            case tokenType::MathMult:
                return "MathMult";
            case tokenType::MathMinus:
                return "MathMinus";
            case tokenType::MathPlus:
                return "MathPlus";
        }
        return "ERROR";
    }
};


#endif //SPO_COMPILER_TOKEN_HPP
