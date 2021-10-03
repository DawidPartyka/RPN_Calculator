//
// Created by dawpa on 01.10.2021.
//

#include "gtest/gtest.h"
#include <Element.hpp>

class ElementTest : public ::testing::Test {
protected:
    static constexpr std::pair add{"+", Symbol::Add};
    static constexpr std::pair subtract{"-", Symbol::Subtract};
    static constexpr std::pair divide{"/", Symbol::Divide};
    static constexpr std::pair multiply{"*", Symbol::Multiply};
    static constexpr std::pair parenthesisOpen{"(", Symbol::ParenthesisOpen};
    static constexpr std::pair parenthesisClose{")", Symbol::ParenthesisClose};
    ElementTest()= default;
    ~ElementTest() override = default;
    void SetUp() override {};
    void TearDown() override{};
};

TEST_F(ElementTest, GivenElementValueIsPlusSignGetValueReturnsPlusSign) {
    auto expected = add;

    Element element{expected.second, expected.first};
    EXPECT_EQ(element.getValue(), expected.first);
}

TEST_F(ElementTest, GivenElementSymbolIsEnumAddGetSymbolReturnsEnumAdd) {
    auto expected = add;

    Element element{expected.second, expected.first};
    EXPECT_EQ(element.getSymbol(), expected.second);
}

TEST_F(ElementTest, GivenElementValueIsNumber43GetValueReturnsString43) {
    std::pair<std::string, Symbol> expected = {"43", Symbol::Number};

    Element element{expected.second, expected.first};
    EXPECT_EQ(element.getValue(), expected.first);
}

TEST_F(ElementTest, GivenElementValueIsMinusSignAndSymbolIsEnumSubtractGetSymbolReturnsEnumSubtractAndGetValueReturnsMinusSign) {
    auto expected = subtract;

    Element element{expected.second, expected.first};
    EXPECT_EQ(element.getValue(), expected.first);
    EXPECT_EQ(element.getSymbol(), expected.second);
}

TEST_F(ElementTest, GivenElementSymbolIsEnumVariableGetSymbolReturnsVariableEnum) {
    std::pair<std::string, Symbol> expected = {"Var", Symbol::Variable};

    Element element{expected.second, expected.first};
    EXPECT_EQ(element.getSymbol(), expected.second);
}

TEST_F(ElementTest, GivenElementSymbolIsEnumParenthesisOpenGetSymbolReturnsParenthesisOpenEnum) {
    std::pair<std::string, Symbol> expected = parenthesisOpen;

    Element element{expected.second, expected.first};
    EXPECT_EQ(element.getSymbol(), expected.second);
}

TEST_F(ElementTest, GivenElementSymbolIsEnumParenthesisCloseGetSymbolReturnsParenthesisCloseEnum) {
    std::pair<std::string, Symbol> expected = parenthesisClose;

    Element element{expected.second, expected.first};
    EXPECT_EQ(element.getSymbol(), expected.second);
}