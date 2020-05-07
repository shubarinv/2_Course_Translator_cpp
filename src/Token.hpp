//
// Created by vhund on 28.04.2020.
//

#ifndef SPO_COMPILER_TOKEN_HPP
#define SPO_COMPILER_TOKEN_HPP

#include <algorithm>
#include <boost/regex.hpp>
#include <iostream>
#include <string>
#include <utility>

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
    EQUAL,
    LBRACE,
    RPAR,
    RBRACE,
    MathDiv,
    MathMult,
    MathMinus,
    MathPlus,
    IF_Keyword,
    THEN_Keyword,
    ELSE_Keyword,
    LESS,
    RBRACKET,
    LBRACKET,
    PROGRAM_Keyword,
    FOR_Keyword,
    WHILE_Keyword,
    PROCEDURE_Keyword,
    TYPE_Keyword,
    VAR_Keyword,
    BEGIN_Keyword,
    END_Keyword,
    DOWNTO_Keyword,
    DO_Keyword,
    CONTINUE_Keyword,
  };

private:
  tokenType type = tokenType::Undefined;

public:
  [[nodiscard]] tokenType getType() const { return type; }

private:
  std::string text;

public:
  [[nodiscard]] const std::string &getText() const { return text; }

private:
  static bool isNumber(const std::string &str) {
    return !str.empty() &&
           std::find_if(str.begin(), str.end(), [](unsigned char c) {
             return !std::isdigit(c);
           }) == str.end();
  }

public:
  static tokenType determineTokenType(const std::string &str) {
    using namespace std;
    boost::regex IdRegex("^[a-zA-Z_$][a-zA-Z_$0-9]*$");
    boost::smatch what;

    try { // checking if string is double
      double value = std::stod(str);
      return tokenType::Num;
    } catch (std::exception &e) {
    }

    if ((str[0] == '\'' || str[0] == '\"') && str[str.size() - 1] == '\'' ||
        str[str.size() - 1] == '\"')
      return tokenType::String;

    if (isNumber(str))
      return tokenType::Num;
    else if (str == ":=")
      return tokenType::Assignment;
    else if (str == "==")
      return tokenType::Comparison;
    else if (str == "=")
      return tokenType::EQUAL;

    else if (str == "+")
      return tokenType::MathPlus;
    else if (str == "-")
      return tokenType::MathMinus;
    else if (str == "*")
      return tokenType::MathMult;
    else if (str == "/")
      return tokenType::MathDiv;

    else if (str == "program")
      return tokenType::PROGRAM_Keyword;
    else if (str == "if")
      return tokenType::IF_Keyword;
    else if (str == "then")
      return tokenType::THEN_Keyword;
    else if (str == "else")
      return tokenType::ELSE_Keyword;
    else if (str == "for")
      return tokenType::FOR_Keyword;
    else if (str == "while")
      return tokenType::WHILE_Keyword;
    else if (str == "var")
      return tokenType::VAR_Keyword;
    else if (str == "procedure")
      return tokenType::PROCEDURE_Keyword;
    else if (str == "type")
      return tokenType::TYPE_Keyword;
    else if (str == "begin")
      return tokenType::BEGIN_Keyword;
    else if (str == "end")
      return tokenType::END_Keyword;
    else if (str == "downto")
      return tokenType::DOWNTO_Keyword;
    else if (str == "do")
      return tokenType::DO_Keyword;
    else if (str == "Continue")
      return tokenType::CONTINUE_Keyword;
    else if (str == "readln")
      return tokenType::Keyword;
    else if (str == "writeln")
      return tokenType::Keyword;
    else if (str == "write")
      return tokenType::Keyword;
    else if (str == "read")
      return tokenType::Keyword;

    else if (str == "(")
      return tokenType::LPAR;
    else if (str == ")")
      return tokenType::RPAR;
    else if (str == "{")
      return tokenType::LBRACE;
    else if (str == "]")
      return tokenType::RBRACKET;
    else if (str == "[")
      return tokenType::LBRACKET;
    else if (str == "}")
      return tokenType::RBRACE;
    else if (str == ";")
      return tokenType::Semicolon;
    else if (str == ":")
      return tokenType::Colon;
    else if (str == ",")
      return tokenType::COMMA;
    else if (str == ".")
      return tokenType::DOT;

    else if (str == ">")
      return tokenType::LESS;
    else if (str == "<")
      return tokenType::Comparison;
    else if (str == ">=")
      return tokenType::Comparison;
    else if (str == "<=")
      return tokenType::Comparison;
    else if (str == "IN")
      return tokenType::Comparison;
    else if (str == "AS")
      return tokenType::Comparison;
    else if (str == "IS")
      return tokenType::Comparison;
    else if (str == "<>")
      return tokenType::Comparison;

    else if (str == "Integer")
      return tokenType::DataType;
    else if (str == "LongInt")
      return tokenType::DataType;
    else if (str == "Byte")
      return tokenType::DataType;

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
    case tokenType::IF_Keyword:
      return "IF_Keyword";
    case tokenType::THEN_Keyword:
      return "THEN_Keyword";
    case tokenType::ELSE_Keyword:
      return "ELSE_Keyword";
    case tokenType::LESS:
      return "LESS";
    case tokenType::EQUAL:
      return "EQUAL";
    case tokenType::RBRACKET:
      return "RBRACKET";
    case tokenType::LBRACKET:
      return "LBRACKET";
    case tokenType::PROGRAM_Keyword:
      return "PROGRAM_Keyword";
    case tokenType::FOR_Keyword:
      return "FOR_Keyword";
    case tokenType::WHILE_Keyword:
      return "WHILE_Keyword";
    case tokenType::PROCEDURE_Keyword:
      return "PROCEDURE_Keyword";
    case tokenType::TYPE_Keyword:
      return "TYPE_Keyword";
    case tokenType::VAR_Keyword:
      return "VAR_Keyword";
    case tokenType::BEGIN_Keyword:
      return "BEGIN_Keyword";
    case tokenType::END_Keyword:
      return "END_Keyword";
    case tokenType::DOWNTO_Keyword:
      return "DOWNTO_Keyword";
    case tokenType::DO_Keyword:
      return "DO_Keyword";
    case tokenType::CONTINUE_Keyword:
      return "Continue_Keyword";
    }
    return "ERROR";
  }
};

#endif // SPO_COMPILER_TOKEN_HPP
