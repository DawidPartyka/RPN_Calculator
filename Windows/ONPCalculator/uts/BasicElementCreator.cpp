//
// createElementd by dawpa on 01.10.2021.
//

#include "gtest/gtest.h"
#include <BasicElementCreator.hpp>

class BasicElementCreatorTest : public ::testing::Test {
protected:
    static constexpr std::pair add{"+", Symbol::Add};
    static constexpr std::pair subtract{"-", Symbol::Subtract};
    static constexpr std::pair divide{"/", Symbol::Divide};
    static constexpr std::pair multiply{"*", Symbol::Multiply};
    static constexpr std::pair parenthesisOpen{"(", Symbol::ParenthesisOpen};
    static constexpr std::pair parenthesisClose{")", Symbol::ParenthesisClose};
    std::map<std::string, Symbol> basicMap {
        {"+", Symbol::Add},
        {"-", Symbol::Subtract},
        {"*", Symbol::Multiply},
        {"/", Symbol::Divide},
        {"(", Symbol::ParenthesisOpen},
        {")", Symbol::ParenthesisClose}
    };
    BasicElementCreator creator{basicMap};
    BasicElementCreatorTest()= default;
    ~BasicElementCreatorTest() override = default;
    void SetUp() override {};
    void TearDown() override {};
};

TEST_F(BasicElementCreatorTest, GivenPlusSignCreateElementReturnsElementWithEnumAddAndValuePlusSign) {
    auto expected = add;

    auto createElement = creator.createElement(expected.first);
    EXPECT_EQ(createElement.getSymbol(), expected.second);
    EXPECT_EQ(createElement.getValue(), expected.first);
}

TEST_F(BasicElementCreatorTest, GivenMinusSignCreateElementReturnsElementWithEnumSubtractAndValueMinusSign) {
    auto expected = subtract;

    auto createElement = creator.createElement(expected.first);
    EXPECT_EQ(createElement.getSymbol(), expected.second);
    EXPECT_EQ(createElement.getValue(), expected.first);
}

TEST_F(BasicElementCreatorTest, GivenDivideSignCreateElementReturnsElementWithEnumDivideAndValueDivideSign) {
    auto expected = divide;

    auto createElement = creator.createElement(expected.first);
    EXPECT_EQ(createElement.getSymbol(), expected.second);
    EXPECT_EQ(createElement.getValue(), expected.first);
}

TEST_F(BasicElementCreatorTest, GivenMultiplySignCreateElementReturnsElementWithEnumMultiplyAndValueMultiplySign) {
    auto expected = multiply;

    auto createElement = creator.createElement(expected.first);
    EXPECT_EQ(createElement.getSymbol(), expected.second);
    EXPECT_EQ(createElement.getValue(), expected.first);
}

TEST_F(BasicElementCreatorTest, GivenParenthesisOpenSignCreateElementReturnsElementWithEnumMultiplyAndValueMultiplySign) {
    auto expected = parenthesisOpen;

    auto createElement = creator.createElement(expected.first);
    EXPECT_EQ(createElement.getSymbol(), expected.second);
    EXPECT_EQ(createElement.getValue(), expected.first);
}

TEST_F(BasicElementCreatorTest, GivenParenthesisCloseSignCreateElementReturnsElementWithEnumMultiplyAndValueMultiplySign) {
    auto expected = parenthesisClose;

    auto createElement = creator.createElement(expected.first);
    EXPECT_EQ(createElement.getSymbol(), expected.second);
    EXPECT_EQ(createElement.getValue(), expected.first);
}

TEST_F(BasicElementCreatorTest, GivenStringVarCreateElementReturnsElementWithEnumVariableAndValueVar) {
    auto expectedValue{"Var"};
    auto expectedSymbol{Symbol::Variable};
    auto createElement = creator.createElement(expectedValue);
    EXPECT_EQ(createElement.getSymbol(), expectedSymbol);
    EXPECT_EQ(createElement.getValue(), expectedValue);
}

TEST_F(BasicElementCreatorTest, GivenString2point37CreateElementReturnsElementWithEnumNumberAndValue2point37) {
    auto expectedValue{"2.37"};
    auto expectedSymbol{Symbol::Number};
    auto createElement = creator.createElement(expectedValue);
    EXPECT_EQ(createElement.getSymbol(), expectedSymbol);
    EXPECT_EQ(createElement.getValue(), expectedValue);
}

TEST_F(BasicElementCreatorTest, GivenString43CreateElementReturnsElementWithEnumNumberAndValue43) {
    auto expectedValue{"43"};
    auto expectedSymbol{Symbol::Number};
    auto createElement = creator.createElement(expectedValue);
    EXPECT_EQ(createElement.getSymbol(), expectedSymbol);
    EXPECT_EQ(createElement.getValue(), expectedValue);
}

TEST_F(BasicElementCreatorTest, GivenString43ABCCreateElementThrowsInvalidArgumentException) {
    auto invalid{"43ABC"};
    EXPECT_THROW(creator.createElement(invalid), std::invalid_argument);
}

TEST_F(BasicElementCreatorTest, GivenString43period56period3ABCCreateElementThrowsInvalidArgumentException) {
    auto invalid{"43.56.3"};
    EXPECT_THROW(creator.createElement(invalid), std::invalid_argument);
}

TEST_F(BasicElementCreatorTest, GivenString43periodW6ABCCreateElementThrowsInvalidArgumentException) {
    auto invalid{"43.W6"};
    EXPECT_THROW(creator.createElement(invalid), std::invalid_argument);
}