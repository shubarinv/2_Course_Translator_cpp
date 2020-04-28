//
// Created by vhund on 28.04.2020.
//

#ifndef SPO_COMPILER_TOKEN_HPP
#define SPO_COMPILER_TOKEN_HPP

#include <string>
#include <utility>
#include <algorithm>


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

    static bool isNumber(const std::string &s) {
        return !s.empty() &&
               std::find_if(s.begin(), s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
    }
public:
    static tokenType determineTokenType(const std::string &str) {
        using namespace std;
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
            std::cout << "tokenValue is somehow whitespace"<<endl;
            return tokenType::Undefined;
        }
        else
            return tokenType::Id;
    }

public:

    explicit Token(std::string _val) {
        text = std::move(_val);
        std::cout<<"Token of value: "<<text<<" has type: "<<typeToString(determineTokenType(text))<<std::endl;
    }
    static std::string typeToString(tokenType _type){
        switch(_type){
            case(tokenType::Assignment):
                return "Assignment";
                break;
            case(tokenType::Comparison):
                return "Comparison";
                break;
            case tokenType::Keyword:
                return "Keyword";
                break;
            case tokenType::MathOp:
                return "MathOp";
                break;
            case tokenType::Bracket:
                return "Bracket";
                break;
            case tokenType::Logic:
                return "Logic";
                break;
            case tokenType::Semicolon:
                return "Semicolon";
                break;
            case tokenType::DataType:
                return "DataType";
                break;
            case tokenType::Id:
                return "Id";
                break;
            case tokenType::Num:
                return "Num";
                break;
            case tokenType::Undefined:
                return "Undefined";
                break;
        }
    }
};


#endif //SPO_COMPILER_TOKEN_HPP
