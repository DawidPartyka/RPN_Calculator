//
// Created by dawpa on 02.10.2021.
//

#include <gtest/gtest.h>
#include <BasicElementCreator.hpp>
#include <BasicOperations.hpp>
#include <InfixNotation.hpp>
#include <EquationParser.hpp>

class InfixNotationTest : public ::testing::Test {
protected:
    static constexpr auto add{[](const auto& a, const auto& b) -> double { return a + b; }};
    static constexpr auto subtract{[](const auto& a, const auto& b) -> double { return a - b; }};
    static constexpr auto multiply{[](const auto& a, const auto& b) -> double { return a * b; }};
    static constexpr auto divide{[](const auto& a, const auto& b) -> double { return a / b; }};
    static constexpr auto floatingPointTolerance{0.00001};
    const std::shared_ptr<BasicElementCreator> creator{std::make_shared<BasicElementCreator>(BasicElementCreator{{
        std::pair{"+", Symbol::Add},
        std::pair{"-", Symbol::Subtract},
        std::pair{"/", Symbol::Divide},
        std::pair{"*", Symbol::Multiply},
        std::pair{"(", Symbol::ParenthesisOpen},
        std::pair{")", Symbol::ParenthesisClose}
    }})};

    InfixNotation infix{};
    std::vector<Element> getElementVector(const std::vector<std::string>& values) {
        std::vector<Element> result{};

        std::for_each(values.begin(), values.end(), [this, &result](const auto& val) {
            std::fill_n(std::back_inserter(result), 1, creator->createElement(val));
        });
        return result;
    };

    std::shared_ptr<BasicOperations> operations{ std::make_shared<BasicOperations>(BasicOperations{}) };

    InfixNotationTest()= default;
    ~InfixNotationTest() override = default;
    void SetUp() override {
        operations->registerOperation(Symbol::Add, add);
        operations->registerOperation(Symbol::Subtract, subtract);
        operations->registerOperation(Symbol::Divide, divide);
        operations->registerOperation(Symbol::Multiply, multiply);
    };
    void TearDown() override {};
};

TEST_F(InfixNotationTest, smoke){
    auto equation{getElementVector({"2"})};
    auto expected{2};
    auto actual{infix.calculate(operations, equation)};
    EXPECT_EQ(expected, actual);
}

TEST_F(InfixNotationTest, Given2plus2InInfixNotationCalculateReturns4){
    auto equation{getElementVector({"2", "+", "2"})};
    auto expected{4};
    auto actual{infix.calculate(operations, equation)};
    EXPECT_EQ(expected, actual);
}

TEST_F(InfixNotationTest, GivenComplexEquationInInfixNotationCalculateReturnsCorrectValue){
    auto equation{getElementVector({"12", "+", "2", "*", "(", "3", "*", "4", "+", "10", "/", "5", ")"})};
    auto expected{40};
    auto actual{infix.calculate(operations, equation)};
    EXPECT_EQ(expected, actual);
}

TEST_F(InfixNotationTest, GivenTrickyEquationInInfixNotationCalculateReturnsCorrectValue){
    auto equation{getElementVector({"(", "1", "+", "2", ")", "*", "4", "+", "5", "-", "3"})};
    auto expected{14};
    auto actual{infix.calculate(operations, equation)};
    EXPECT_EQ(expected, actual);
}

TEST_F(InfixNotationTest, GivenComplexEquationWithFloatingPointNumbersInInfixNotationCalculateReturnsCorrectValueUpTo5DecimalPoints){
    auto equation{getElementVector({"(", "1.55", "+", "3.75", ")", "*", "(", "(", "8.23", "+", "1", ")", "/", "3", ")"})};
    auto expected{16.3063333};
    auto actual{infix.calculate(operations, equation)};
    EXPECT_NEAR(actual, expected, floatingPointTolerance);
}

TEST_F(InfixNotationTest, GivenEquationWithIncorrectlySetParenthesesCalculateThrowsLogicErrorException){
    auto equation{getElementVector({"(", "1.55", "+", "3.75", ")", "*", "(", "(", "8.23", "+", "1", ")", "/", "3"})};
    EXPECT_THROW(infix.calculate(operations, equation), std::logic_error);
}