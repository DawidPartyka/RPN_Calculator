//
// Created by dawpa on 02.10.2021.
//

#include <BasicElementCreator.hpp>
#include "gtest/gtest.h"
#include "EquationParser.hpp"
#include "IElementCreator.hpp"

class EquationParserTest : public ::testing::Test {
protected:
    static constexpr std::pair add{"+", Symbol::Add};
    static constexpr std::pair subtract{"-", Symbol::Subtract};
    static constexpr std::pair divide{"/", Symbol::Divide};
    static constexpr std::pair multiply{"*", Symbol::Multiply};
    static constexpr std::pair parenthesisOpen{"(", Symbol::ParenthesisOpen};
    static constexpr std::pair parenthesisClose{")", Symbol::ParenthesisClose};
    BasicElementCreator creator{
        {
            add,
            subtract,
            multiply,
            divide,
            parenthesisOpen,
            parenthesisClose
        }
    };
    EquationParser parser{std::make_shared<BasicElementCreator>(creator)};
    static bool compare(const std::vector<Element>& expected, const std::vector<Element>& actual) {
        return std::equal(expected.begin(), expected.end(), actual.begin(), actual.end(), [](const auto& left, const auto& right){
            return left.getSymbol() == right.getSymbol() && left.getValue() == right.getValue();
        });
    };
    static Element elemNumber(const std::string& num) {
        return Element{Symbol::Number, num};
    }
    EquationParserTest()= default;
    ~EquationParserTest() override = default;
    void SetUp() override {};
    void TearDown() override{};
};

TEST_F(EquationParserTest, smoke) {
    std::vector<Element> expected{{Symbol::ParenthesisOpen, "("}};
    auto actual = parser.read("(");
    EXPECT_TRUE(compare(expected, parser.read("(")));
}

TEST_F(EquationParserTest, GivenEquation1WithoutSpacesReadReturnsVectorOfElementsCorrespondingToTheEquation){
    std::string equation{"12+2*(3*4+1/5)"};

    std::vector<Element> expected = {
            creator.createElement("12"),
            creator.createElement("+"),
            creator.createElement("2"),
            creator.createElement("*"),
            creator.createElement("("),
            creator.createElement("3"),
            creator.createElement("*"),
            creator.createElement("4"),
            creator.createElement("+"),
            creator.createElement("1"),
            creator.createElement("/"),
            creator.createElement("5"),
            creator.createElement(")")
    };

    EXPECT_TRUE(compare(expected, parser.read(equation)));
}

TEST_F(EquationParserTest, GivenEquation2WithoutSpacesReadReturnsVectorOfElementsCorrespondingToTheEquation){
    std::string equation{"12+2*(3*4+10/5)"};

    std::vector<Element> expected = {
            creator.createElement("12"),
            creator.createElement("+"),
            creator.createElement("2"),
            creator.createElement("*"),
            creator.createElement("("),
            creator.createElement("3"),
            creator.createElement("*"),
            creator.createElement("4"),
            creator.createElement("+"),
            creator.createElement("10"),
            creator.createElement("/"),
            creator.createElement("5"),
            creator.createElement(")")
    };

    EXPECT_TRUE(compare(expected, parser.read(equation)));
}

TEST_F(EquationParserTest, GivenEquation1WithSpacesReadReturnsVectorOfElementsCorrespondingToTheEquation){
    std::string equation{" 12 + 2 * ( 3 * 4 + 1 / 5 ) "};

    std::vector<Element> expected = {
            creator.createElement("12"),
            creator.createElement("+"),
            creator.createElement("2"),
            creator.createElement("*"),
            creator.createElement("("),
            creator.createElement("3"),
            creator.createElement("*"),
            creator.createElement("4"),
            creator.createElement("+"),
            creator.createElement("1"),
            creator.createElement("/"),
            creator.createElement("5"),
            creator.createElement(")")
    };

    EXPECT_TRUE(compare(expected, parser.read(equation)));
}

TEST_F(EquationParserTest, GivenEquationWithMultipleSpacesReadReturnsVectorOfElementsCorrespondingToTheEquation){
    std::string equation{" 56 *           543 / 234    +     8"};

    std::vector<Element> expected = {
            creator.createElement("56"),
            creator.createElement("*"),
            creator.createElement("543"),
            creator.createElement("/"),
            creator.createElement("234"),
            creator.createElement("+"),
            creator.createElement("8")
    };

    EXPECT_TRUE(compare(expected, parser.read(equation)));
}

TEST_F(EquationParserTest, IncorrectEquationReturnsExceptionInvalidArgument){
    std::string equation{" 56 *     a^%2      543 / 234    +     8"};

    EXPECT_THROW(parser.read(equation), std::invalid_argument);
}