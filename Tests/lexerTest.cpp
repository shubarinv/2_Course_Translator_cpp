//
// Created by vhund on 28.04.2020.
//

#include "gtest/gtest.h"
#include "Lexer.h"

// Тест превода одной сторки в токены
TEST(lexer, tokenizeSingleLine) {
    Lexer lexer;
    std::string testingStr="int a = 5;";
    std::vector<std::string> expectedOutput={"int","a","=","5;"};
    EXPECT_EQ(lexer.tokenize(testingStr), expectedOutput);
}

// Тест превода нескольких строк в токены
TEST(lexer, tokenizeMultiplelines) {
    Lexer lexer;
    std::string testingStr="int a = 5;\ndouble b = 5.3;";
    std::vector<std::string> expectedOutput={"int","a","=","5;","double","b","=","5.3;"};
    EXPECT_EQ(lexer.tokenize(testingStr), expectedOutput);
}

// Тест превода сторки с мосором(несколько пробелов подряд с табуляцией и переносом строки)
TEST(lexer, tokenizeLinesWithTrash) {
    Lexer lexer;
    std::string testingStr="int a = 5;\n         \t\ndouble b = 5.3;";
    std::vector<std::string> expectedOutput={"int","a","=","5;","double","b","=","5.3;"};
    EXPECT_EQ(lexer.tokenize(testingStr), expectedOutput);
}

