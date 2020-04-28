//
// Created by vhundef on 28.04.2020.
//
#include <Token.hpp>
#include "gtest/gtest.h"

TEST(token,idToken) {
    EXPECT_EQ(Token::determineTokenType("i"), Token::tokenType::Id);
}
TEST(token, assignmentToken) {
    EXPECT_EQ(Token::determineTokenType(":="), Token::tokenType::Assignment);
}
TEST(token, equalityToken) {
    EXPECT_EQ(Token::determineTokenType("=="), Token::tokenType::Comparison);
}
TEST(token, keywordToken) {
    EXPECT_EQ(Token::determineTokenType("if"), Token::tokenType::Keyword);
}
TEST(token, mathOpToken) {
    EXPECT_EQ(Token::determineTokenType("+"), Token::tokenType::MathOp);
}