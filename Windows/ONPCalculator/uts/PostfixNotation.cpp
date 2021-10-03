//
// Created by dawpa on 02.10.2021.
//

#include <BasicOperations.hpp>
#include "gtest/gtest.h"
#include "PostfixNotation.hpp"
#include "BasicElementCreator.hpp"


class PostfixNotationTest : public ::testing::Test {
protected:
    static constexpr auto add{[](const auto& a, const auto& b) -> double { return a + b; }};
    static constexpr auto subtract{[](const auto& a, const auto& b) -> double { return a - b; }};
    static constexpr auto multiply{[](const auto& a, const auto& b) -> double { return a * b; }};
    static constexpr auto divide{[](const auto& a, const auto& b) -> double { return a / b; }};
    std::map<std::string, Symbol> basicMap {
        {"+", Symbol::Add},
        {"-", Symbol::Subtract},
        {"*", Symbol::Multiply},
        {"/", Symbol::Divide},
        {"(", Symbol::ParenthesisOpen},
        {")", Symbol::ParenthesisClose}
    };

    BasicElementCreator creator{basicMap};

    std::vector<Element> getElementVector(const std::vector<std::string>& values) {
        std::vector<Element> result{};

        std::for_each(values.begin(), values.end(), [this, &result](const auto& val) {
            std::fill_n(std::back_inserter(result), 1, creator.createElement(val));
        });
        return result;
    }
    PostfixNotation postfix{};
    std::shared_ptr<BasicOperations> operations{ std::make_shared<BasicOperations>(BasicOperations{}) };
    PostfixNotationTest()= default;
    ~PostfixNotationTest() override = default;
    void SetUp() override {
        operations->registerOperation(Symbol::Add, add);
        operations->registerOperation(Symbol::Subtract, subtract);
        operations->registerOperation(Symbol::Divide, divide);
        operations->registerOperation(Symbol::Multiply, multiply);
    };
    void TearDown() override {};
};

TEST_F(PostfixNotationTest, smoke){
    auto equation{getElementVector({"2"})};
    auto expected{2};
    auto actual{postfix.calculate(operations, equation)};
    EXPECT_EQ(actual, expected);
}

TEST_F(PostfixNotationTest, GivenInfixEquationTwoPlusTwoCalculateReturnsFour){
    auto equation{getElementVector({"2", "2", "+"})};
    auto expected{4};
    auto actual{postfix.calculate(operations, equation)};
    EXPECT_EQ(actual, expected);
}

TEST_F(PostfixNotationTest, GivenShortInfixEquationCalculateReturnsCorrectResult){
    auto equation{getElementVector({"2", "3", "+", "5", "*"})};
    auto expected{25};
    auto actual{postfix.calculate(operations, equation)};
    EXPECT_EQ(actual, expected);
}

TEST_F(PostfixNotationTest, GivenComplexInfixEquationWhichResultIsAFloatingPointNumberCalculateReturnsCorrectResult){
    auto equation{getElementVector({"2", "7", "+", "3", "/", "14", "3", "-", "4", "*", "+", "2", "/"})};
    auto expected{23.5};
    auto actual{postfix.calculate(operations, equation)};
    EXPECT_EQ(actual, expected);
}

TEST_F(PostfixNotationTest, GivenComplexInfixEquationWhichResultIsAnIntegerCalculateReturnsCorrectResult){
    auto equation{getElementVector({"12", "2", "3", "4", "*", "10", "5", "/", "+", "*", "+"})};
    auto expected{40};
    auto actual{postfix.calculate(operations, equation)};
    EXPECT_EQ(actual, expected);
}

TEST_F(PostfixNotationTest, GivenIncorrectInfixEquationCalculateThrowsLogicExceptionException){
    auto equation{getElementVector({"2", "+"})};
    EXPECT_THROW(postfix.calculate(operations, equation), std::logic_error);
}