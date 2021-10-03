//
// Created by dawpa on 02.10.2021.
//
#include <gtest/gtest.h>
#include <BasicElementCreator.hpp>
#include "InfixConverter.hpp"
#include "EquationParser.hpp"

class InfixConverterTest : public ::testing::Test {
protected:
    static constexpr std::pair add{"+", Symbol::Add};
    static constexpr std::pair subtract{"-", Symbol::Subtract};
    static constexpr std::pair divide{"/", Symbol::Divide};
    static constexpr std::pair multiply{"*", Symbol::Multiply};
    static constexpr std::pair power{"^", Symbol::Power};
    static constexpr std::pair parenthesisOpen{"(", Symbol::ParenthesisOpen};
    static constexpr std::pair parenthesisClose{")", Symbol::ParenthesisClose};
    BasicElementCreator creator{
        {
            add,
            subtract,
            multiply,
            divide,
            power,
            parenthesisOpen,
            parenthesisClose
        }
    };
    EquationParser parser{std::make_shared<BasicElementCreator>(creator)};
    InfixConverter converter{std::make_shared<EquationParser>(parser)};
    static bool compareQueues(std::queue<Element> expected, std::queue<Element> actual){
        if(expected.size() != actual.size()){
            return false;
        }

        while(!expected.empty()){
            const auto& left = expected.front();
            const auto& right = actual.front();

            if(left.getValue() != right.getValue() || left.getSymbol() != right.getSymbol()){
                return false;
            }

            expected.pop();
            actual.pop();
        }

        return true;
    };

    static std::queue<Element> queueFromVector(const std::vector<Element>& vec){
        std::queue<Element> result;

        for(const auto& elem : vec) {
            result.push(elem);
        }

        return result;
    }

    InfixConverterTest()= default;
    ~InfixConverterTest() override = default;
    void SetUp() override {};
    void TearDown() override {};
};

TEST_F(InfixConverterTest, smoke) {
    std::vector<Element> equation = { creator.createElement("2") };
    auto expected{queueFromVector(equation)};

    EXPECT_TRUE(compareQueues(expected, converter.convertToQueue(equation)));
}

TEST_F(InfixConverterTest, GivenPostfixEquation22PlusConvertToQueueReturns2Plus2) {
    std::vector<Element> equation{
            creator.createElement("2"),
            creator.createElement("2"),
            creator.createElement("+")
    };

    std::queue<Element> expected{ queueFromVector({
            creator.createElement("("),
            creator.createElement("2"),
            creator.createElement("+"),
            creator.createElement("2"),
            creator.createElement(")")
    })};

    EXPECT_TRUE(compareQueues(expected, converter.convertToQueue(equation)));
}

TEST_F(InfixConverterTest, GivenInfixEquationConvertToQueueReturnsLogicError) {
    std::vector<Element> equation{
        creator.createElement("2"),
        creator.createElement("+"),
        creator.createElement("2")
    };

    EXPECT_THROW(converter.convertToQueue(equation), std::logic_error);
}

TEST_F(InfixConverterTest, GivenInfixEquationCheckReturnsFalse) {
    std::vector<Element> equation{
        creator.createElement("2"),
        creator.createElement("+"),
        creator.createElement("2")
    };

    EXPECT_FALSE(converter.check(equation));
}

TEST_F(InfixConverterTest, GivenPostfixEquationCheckReturnsTrue) {
    std::vector<Element> equation{
        creator.createElement("2"),
        creator.createElement("2"),
        creator.createElement("+")
    };

    EXPECT_TRUE(converter.check(equation));
}

TEST_F(InfixConverterTest, Given22PlusEquationConvertToStringReturns2Plus2InsideParentheses) {
    std::vector<Element> equation{
        creator.createElement("2"),
        creator.createElement("2"),
        creator.createElement("+")
    };

    auto expected{"(2+2)"};

    EXPECT_EQ(converter.convertToString(equation), expected);
}

TEST_F(InfixConverterTest, GivenEquationWithoutOperatorBetweenTwoParenthesesCheckReturnsFalse){
    std::vector<Element> equation{
        creator.createElement("1.55"), creator.createElement("+"),
        creator.createElement("("), creator.createElement("3.75"),
        creator.createElement(")"), creator.createElement("("),
        creator.createElement("("), creator.createElement("8.23"),
        creator.createElement("+"), creator.createElement("1"),
        creator.createElement(")"), creator.createElement("/"),
        creator.createElement("3"), creator.createElement(")")
    };

    auto actual(converter.check(equation));
    EXPECT_FALSE(actual);
}

TEST_F(InfixConverterTest, GivenPostfixEquation22PowerConvertToQueueReturns2ToThePowerOf2) {
    std::vector<Element> equation{
        creator.createElement("2"),
        creator.createElement("2"),
        creator.createElement("^")
    };

    std::queue<Element> expected{ queueFromVector({
        creator.createElement("("),
        creator.createElement("2"),
        creator.createElement("^"),
        creator.createElement("2"),
        creator.createElement(")")
    })};

    EXPECT_TRUE(compareQueues(expected, converter.convertToQueue(equation)));
}

TEST_F(InfixConverterTest, GivenComplexPostfixEquationWithPowersConvertToQueueReturnsCorrectEquation) {
    // 9 2 ^ 5 12 * / 10 24 13 2 + / 1 - ^ +
    std::vector<Element> equation{
        creator.createElement("9"), creator.createElement("2"),
        creator.createElement("^"), creator.createElement("5"),
        creator.createElement("12"), creator.createElement("*"),
        creator.createElement("/"), creator.createElement("10"),
        creator.createElement("24"), creator.createElement("13"),
        creator.createElement("2"), creator.createElement("+"),
        creator.createElement("/"), creator.createElement("1"),
        creator.createElement("-"), creator.createElement("^"),
        creator.createElement("+")
    };

    // (((9 ^ 2) / (5 * 12)) + (10 ^ ((24 / (13 + 2)) - 1)))
    std::queue<Element> expected{ queueFromVector({
        creator.createElement("("), creator.createElement("("),
        creator.createElement("("), creator.createElement("9"),
        creator.createElement("^"), creator.createElement("2"),
        creator.createElement(")"), creator.createElement("/"),
        creator.createElement("("), creator.createElement("5"),
        creator.createElement("*"), creator.createElement("12"),
        creator.createElement(")"), creator.createElement(")"),
        creator.createElement("+"), creator.createElement("("),
        creator.createElement("10"), creator.createElement("^"),
        creator.createElement("("), creator.createElement("("),
        creator.createElement("24"), creator.createElement("/"),
        creator.createElement("("), creator.createElement("13"),
        creator.createElement("+"), creator.createElement("2"),
        creator.createElement(")"), creator.createElement(")"),
        creator.createElement("-"), creator.createElement("1"),
        creator.createElement(")"), creator.createElement(")"),
        creator.createElement(")")
    })};

    EXPECT_TRUE(compareQueues(expected, converter.convertToQueue(equation)));
}