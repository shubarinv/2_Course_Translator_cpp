//
// Created by vhund on 28.04.2020.
//

#include "gtest/gtest.h"
#include "Lexer.h"

// Тест превода одной сторки в токены
TEST(lexer, tokenizeSingleLine) {


    std::string testingStr = "int a = 5;";
    std::vector<std::string> expectedOutput = {"int", "a", "=", "5", ";"};
    Lexer lexer(testingStr);
    lexer.tokenize();
    auto tokens=lexer.getTokens();
    std::vector<std::string> actualOutput;
    while (!tokens.empty()) {
        actualOutput.push_back(tokens.front().getText());
        tokens.pop();
    }

    EXPECT_EQ(actualOutput, expectedOutput);
}

// Тест превода нескольких строк в токены
TEST(lexer, tokenizeMultiplelines) {
    std::string testingStr = "int a = 5;\ndouble b = 5.3;";
    std::vector<std::string> expectedOutput = {"int", "a", "=", "5", ";", "double", "b", "=", "5.3", ";"};
    Lexer lexer(testingStr);
    lexer.tokenize();
    auto tokens=lexer.getTokens();
    std::vector<std::string> actualOutput;
    while (!tokens.empty()) {
        actualOutput.push_back(tokens.front().getText());
        tokens.pop();
    }

    EXPECT_EQ(actualOutput, expectedOutput);
}

// Тест превода сторки с мосором(несколько пробелов подряд с табуляцией и переносом строки)
TEST(lexer, tokenizeLinesWithTrash) {
    std::string testingStr = "int a = 5;\n         \t\ndouble b = 5.3;";
    std::vector<std::string> expectedOutput = {"int", "a", "=", "5", ";", "double", "b", "=", "5.3", ";"};
    Lexer lexer(testingStr);
    lexer.tokenize();
    auto tokens=lexer.getTokens();
    std::vector<std::string> actualOutput;
    while (!tokens.empty()) {
        actualOutput.push_back(tokens.front().getText());
        tokens.pop();
    }

    EXPECT_EQ(actualOutput, expectedOutput);
}

