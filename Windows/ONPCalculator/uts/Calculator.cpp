//
// Created by dawpa on 03.10.2021.
//

#include <gtest/gtest.h>
#include <cmath>
#include <BasicElementCreator.hpp>
#include <BasicOperations.hpp>
#include <InfixNotation.hpp>
#include <PostfixNotation.hpp>
#include <EquationParser.hpp>
#include <Calculator.hpp>

class CalculatorTest : public ::testing::Test {
protected:
    static constexpr auto add{[](const auto& a, const auto& b) -> double { return a + b; }};
    static constexpr auto subtract{[](const auto& a, const auto& b) -> double { return a - b; }};
    static constexpr auto multiply{[](const auto& a, const auto& b) -> double { return a * b; }};
    static constexpr auto divide{[](const auto& a, const auto& b) -> double { return a / b; }};
    static constexpr auto power{[](const auto& a, const auto& b) -> double { return std::pow(a, b); }};
    static constexpr auto floatingPointTolerance{0.00001};

    std::map<std::string, Symbol> basicMap {
        {"+", Symbol::Add},
        {"-", Symbol::Subtract},
        {"*", Symbol::Multiply},
        {"/", Symbol::Divide},
        {"^", Symbol::Power},
        {"(", Symbol::ParenthesisOpen},
        {")", Symbol::ParenthesisClose}
    };
    BasicElementCreator creator{basicMap};

    std::shared_ptr<BasicOperations> operations{std::make_shared<BasicOperations>(BasicOperations{})};
    std::shared_ptr<EquationParser> parser{std::make_shared<EquationParser>(EquationParser{std::make_shared<BasicElementCreator>(creator)})};

    InfixConverter infixConverter{parser};
    InfixNotation infix{};
    PostfixConverter postfixConverter{};
    PostfixNotation postfix{};

    Calculator calculator{parser, operations};

    CalculatorTest()= default;
    ~CalculatorTest() override = default;
    void SetUp() override {
        operations->registerOperation(Symbol::Add, add);
        operations->registerOperation(Symbol::Subtract, subtract);
        operations->registerOperation(Symbol::Divide, divide);
        operations->registerOperation(Symbol::Multiply, multiply);
        operations->registerOperation(Symbol::Power, power);

        calculator.setNotation(std::make_shared<PostfixNotation>(postfix), std::make_shared<PostfixConverter>(postfixConverter));
        calculator.setNotation(std::make_shared<InfixNotation>(infix), std::make_shared<InfixConverter>(infixConverter));
    };
    void TearDown() override {};
};

TEST_F(CalculatorTest, SmokeGet){
    calculator.load("2");
    auto actualPostfix{calculator.get(NotationEnum::Postfix)};
    auto actualInfix{calculator.get(NotationEnum::Infix)};

    EXPECT_EQ(actualPostfix, "2");
    EXPECT_EQ(actualInfix, "2");
}

TEST_F(CalculatorTest, Given2plus2InInfixNotationGetReturnsCorrectEquationsAsStringsForInfixAndPostfixNotation){
    calculator.load("2+2");
    auto actualPostfix{calculator.get(NotationEnum::Postfix)};
    auto actualInfix{calculator.get(NotationEnum::Infix)};
    EXPECT_EQ(actualPostfix, "2 2 +");
    EXPECT_EQ(actualInfix, "2 + 2");
}

TEST_F(CalculatorTest, Given2plus2InPostfixNotationGetReturnsCorrectEquationsAsStringsForInfixAndPostfixNotation){
    calculator.load("2 2 +");
    auto actualPostfix{calculator.get(NotationEnum::Postfix)};
    auto actualInfix{calculator.get(NotationEnum::Infix)};
    EXPECT_EQ(actualPostfix, "2 2 +");
    EXPECT_EQ(actualInfix, "(2+2)");
}

TEST_F(CalculatorTest, GivenComplexInfixEquationGetReturnsCorrectEquationsAsStringsForInfixAndPostfixNotation){
    calculator.load("((2+7)/3+(14-3)*4)/2");
    auto actualPostfix{calculator.get(NotationEnum::Postfix)};
    auto actualInfix{calculator.get(NotationEnum::Infix)};
    EXPECT_EQ(actualPostfix, "2 7 + 3 / 14 3 - 4 * + 2 /");
    EXPECT_EQ(actualInfix, "( ( 2 + 7 ) / 3 + ( 14 - 3 ) * 4 ) / 2");
}

TEST_F(CalculatorTest, GivenComplexPostfixEquationGetReturnsCorrectEquationsAsStringsForInfixAndPostfixNotation){
    calculator.load("2 7 + 3 / 14 3 - 4 * + 2 /");
    auto actualPostfix{calculator.get(NotationEnum::Postfix)};
    auto actualInfix{calculator.get(NotationEnum::Infix)};

    EXPECT_EQ(actualPostfix, "2 7 + 3 / 14 3 - 4 * + 2 /");
    EXPECT_EQ(actualInfix, "((((2+7)/3)+((14-3)*4))/2)");
}

TEST_F(CalculatorTest, GivenBadlyFormatedComplexInfixEquationGetReturnsCorrectEquationsAsStringsForInfixAndPostfixNotation){
    calculator.load("12 2 3 4*  10 5/+* +");
    auto actualPostfix{calculator.get(NotationEnum::Postfix)};
    auto actualInfix{calculator.get(NotationEnum::Infix)};

    EXPECT_EQ(actualPostfix, "12 2 3 4 * 10 5 / + * +");
    EXPECT_EQ(actualInfix, "(12+(2*((3*4)+(10/5))))");
}

TEST_F(CalculatorTest, GivenBadlyFormatedComplexPostfixEquationGetReturnsCorrectEquationsAsStringsForInfixAndPostfixNotation){
    calculator.load("12    + 2*( 3*    4  +  (10/5     ))");
    auto actualPostfix{calculator.get(NotationEnum::Postfix)};
    auto actualInfix{calculator.get(NotationEnum::Infix)};

    EXPECT_EQ(actualPostfix, "12 2 3 4 * 10 5 / + * +");
    EXPECT_EQ(actualInfix, "12 + 2 * ( 3 * 4 + ( 10 / 5 ) )");
}

TEST_F(CalculatorTest, GivenPostfixEquationWithOneVariableGetReturnsCorrectEquationAsStringsForInfixAndPostfixNotation){
    calculator.load("a + 2");
    auto actualPostfix{calculator.get(NotationEnum::Postfix)};
    auto actualInfix{calculator.get(NotationEnum::Infix)};

    EXPECT_EQ(actualPostfix, "a 2 +");
    EXPECT_EQ(actualInfix, "a + 2");
}

TEST_F(CalculatorTest, GivenInfixEquationWithOneVariableGetReturnsCorrectEquationAsStringsForInfixAndPostfixNotation){
    calculator.load("a 2 +");
    auto actualPostfix{calculator.get(NotationEnum::Postfix)};
    auto actualInfix{calculator.get(NotationEnum::Infix)};

    EXPECT_EQ(actualPostfix, "a 2 +");
    EXPECT_EQ(actualInfix, "(a+2)");
}

TEST_F(CalculatorTest, GivenPostfixEquationWithTwoDifferentVariablesGetReturnsCorrectEquationAsStringsForInfixAndPostfixNotation){
    calculator.load("a + b");
    auto actualPostfix{calculator.get(NotationEnum::Postfix)};
    auto actualInfix{calculator.get(NotationEnum::Infix)};

    EXPECT_EQ(actualPostfix, "a b +");
    EXPECT_EQ(actualInfix, "a + b");
}

TEST_F(CalculatorTest, GivenPostfixEquationWithTwoSameVariablesGetReturnsCorrectEquationAsStringsForInfixAndPostfixNotation){
    calculator.load("a + a");
    auto actualPostfix{calculator.get(NotationEnum::Postfix)};
    auto actualInfix{calculator.get(NotationEnum::Infix)};

    EXPECT_EQ(actualPostfix, "a a +");
    EXPECT_EQ(actualInfix, "a + a");
}

TEST_F(CalculatorTest, GivenInfixEquationWithTwoDifferentVariablesGetReturnsCorrectEquationAsStringsForInfixAndPostfixNotation){
    calculator.load("a b +");
    auto actualPostfix{calculator.get(NotationEnum::Postfix)};
    auto actualInfix{calculator.get(NotationEnum::Infix)};

    EXPECT_EQ(actualPostfix, "a b +");
    EXPECT_EQ(actualInfix, "(a+b)");
}

TEST_F(CalculatorTest, GivenInfixEquationWithTwoSameVariablesGetReturnsCorrectEquationAsStringsForInfixAndPostfixNotation){
    calculator.load("a a +");
    auto actualPostfix{calculator.get(NotationEnum::Postfix)};
    auto actualInfix{calculator.get(NotationEnum::Infix)};

    EXPECT_EQ(actualPostfix, "a a +");
    EXPECT_EQ(actualInfix, "(a+a)");
}

TEST_F(CalculatorTest, GivenNoEquationStringGetThrowsLogicErrorException){
    EXPECT_THROW(calculator.get(NotationEnum::Postfix), std::logic_error);
}

TEST_F(CalculatorTest, GivenCalculatorSetWithTwoNotationsAfterUnsetNotationReturns1AndAfterSettingItAgainItReturns2){
    auto actual{calculator.unsetNotation(NotationEnum::Infix)};
    unsigned int expected{1};
    EXPECT_EQ(actual, expected);
    auto actual2{calculator.setNotation(std::make_shared<InfixNotation>(infix), std::make_shared<InfixConverter>(infixConverter))};
    unsigned int expected2{2};
    EXPECT_EQ(actual2, expected2);
}

TEST_F(CalculatorTest, GivenPostfixEquationPassingNotationEnumInfixToGetMethodAfterRemovingInfixConverterFromCalculatorGetThrowsLogicErrorException){
    calculator.load("2 2 +");
    calculator.unsetNotation(NotationEnum::Infix);
    EXPECT_THROW(calculator.get(NotationEnum::Infix), std::logic_error);
    calculator.setNotation(std::make_shared<InfixNotation>(infix), std::make_shared<InfixConverter>(infixConverter));
}

TEST_F(CalculatorTest, SmokeResult){
    calculator.load("2");
    auto expected{2};
    auto actual{calculator.result()};
    EXPECT_EQ(expected, actual);
}

TEST_F(CalculatorTest, Given2Plus2InInfixNotationResultReturns4){
    calculator.load("2+2");
    auto expected{4};
    auto actual{calculator.result()};
    EXPECT_EQ(expected, actual);
}

TEST_F(CalculatorTest, Given2Plus2InPostfixNotationResultReturns4){
    calculator.load("2 2+");
    auto expected{4};
    auto actual{calculator.result()};
    EXPECT_EQ(expected, actual);
}

TEST_F(CalculatorTest, GivenComplexEquationWithFloatingPointResultInInfixNotationResultReturnsCorrectValue){
    calculator.load("((2+7)/3+(14-3)*4)/2");
    auto expected{23.5};
    auto actual{calculator.result()};
    EXPECT_EQ(expected, actual);
}

TEST_F(CalculatorTest, GivenComplexEquationWithFloatingPointResultInPostfixNotationResultReturnsCorrectValue){
    calculator.load("2 7 + 3/ 14 3 - 4 * + 2 /");
    auto expected{23.5};
    auto actual{calculator.result()};
    EXPECT_EQ(expected, actual);
}

TEST_F(CalculatorTest, GivenTrickyEquationFromWikipediaWithFloatingPointResultInInfixNotationResultReturnsCorrectValue){
    calculator.load("(1+2)*4+5-3");
    auto expected{14};
    auto actual{calculator.result()};
    EXPECT_EQ(expected, actual);
}

TEST_F(CalculatorTest, GivenTrickyEquationFromWikipediaWithFloatingPointResultInPostfixNotationResultReturnsCorrectValue){
    calculator.load("5 1 2 + 4 * + 3 -");
    auto expected{14};
    auto actual{calculator.result()};
    EXPECT_EQ(expected, actual);
}

TEST_F(CalculatorTest, GivenTrickyEquationInInfixConvertedToPostfixByCalculatorResultReturnsCorrectValue){
    calculator.load("(1+2)*4+5-3");
    calculator.load(calculator.get(NotationEnum::Postfix));
    auto expected{14};
    auto actual{calculator.result()};
    EXPECT_EQ(expected, actual);
}

TEST_F(CalculatorTest, GivenTrickyEquationInPostfixConvertedToPostfixByCalculatorResultReturnsCorrectValue){
    calculator.load("5 1 2 + 4 * + 3 -");
    calculator.load(calculator.get(NotationEnum::Infix));
    auto expected{14};
    auto actual{calculator.result()};
    EXPECT_EQ(expected, actual);
}

TEST_F(CalculatorTest, VariableResultSmoke) {
    calculator.load("a");
    calculator.setVar("a", 2);
    auto expected{2};
    auto actual{calculator.result()};
    EXPECT_EQ(expected, actual);
}

TEST_F(CalculatorTest,GivenInfixEquationWithTwoDifferentVariablesResultReturnCorrectValue) {
    calculator.load("a + b");
    calculator.setVar("a", 2);
    calculator.setVar("b", 4);
    auto expected{6};
    auto actual{calculator.result()};
    EXPECT_EQ(expected, actual);
}

TEST_F(CalculatorTest,GivenInfixEquationWithTwoDifferentVariablesWithFloatingPointValuesResultReturnCorrectValue) {
    calculator.load("a * b");
    calculator.setVar("a", 2.53);
    calculator.setVar("b", 4.899);
    auto expected{12.39447};
    auto actual{calculator.result()};
    EXPECT_EQ(expected, actual);
}

TEST_F(CalculatorTest,GivenInfixEquationWithTwoSameVariablesResultReturnCorrectValue) {
    calculator.load("a + a");
    calculator.setVar("a", 2);
    auto expected{4};
    auto actual{calculator.result()};
    EXPECT_EQ(expected, actual);
}

TEST_F(CalculatorTest,GivenInfixEquationWithTwoSameVariablesWithFloatingPointValuesResultReturnCorrectValue) {
    calculator.load("a * a");
    calculator.setVar("a", 2.53);
    auto expected{6.4009};
    auto actual{calculator.result()};
    EXPECT_EQ(expected, actual);
}

TEST_F(CalculatorTest,GivenPostfixEquationWithTwoDifferentVariablesResultReturnCorrectValue) {
    calculator.load("a b +");
    calculator.setVar("a", 2);
    calculator.setVar("b", 4);
    auto expected{6};
    auto actual{calculator.result()};
    EXPECT_EQ(expected, actual);
}

TEST_F(CalculatorTest,GivenPostfixEquationWithTwoDifferentVariablesWithFloatingPointValuesResultReturnCorrectValue) {
    calculator.load("a b *");
    calculator.setVar("a", 2.53);
    calculator.setVar("b", 4.899);
    auto expected{12.39447};
    auto actual{calculator.result()};
    EXPECT_EQ(expected, actual);
}

TEST_F(CalculatorTest,GivenPostfixEquationWithTwoSameVariablesResultReturnCorrectValue) {
    calculator.load("a a +");
    calculator.setVar("a", 2);
    auto expected{4};
    auto actual{calculator.result()};
    EXPECT_EQ(expected, actual);
    calculator.unsetVariable("a");
}

TEST_F(CalculatorTest,GivenPostfixEquationWithTwoSameVariablesWithFloatingPointValuesResultReturnCorrectValue) {
    calculator.load("a a *");
    calculator.setVar("a", 2.53);
    auto expected{6.4009};
    auto actual{calculator.result()};
    EXPECT_EQ(expected, actual);
    calculator.unsetVariable("a");
}

TEST_F(CalculatorTest,GivenPostfixEquationWithOneVariableReturnsCorrectAnswerAndAfterChangingTheVariableValueSecondCallToResultReturnsCorrectUpdatedValue) {
    calculator.load("a * 4 + 5 / 6");
    calculator.setVar("a", 2.53);
    auto expected{10.953333};
    auto actual{calculator.result()};
    EXPECT_NEAR(expected, actual, floatingPointTolerance);

    calculator.setVar("a", 3.35);
    auto expected2{14.233333};
    auto actual2{calculator.result()};
    EXPECT_NEAR(expected2, actual2, floatingPointTolerance);
}

TEST_F(CalculatorTest, GivenEquationWithVariableThatWasNotSetResultThrowsLogicErrorException) {
    calculator.load("a * 4 + 5 / 6");
    EXPECT_THROW(calculator.result(), std::logic_error);
}

TEST_F(CalculatorTest, GivenEquationWithVariableThatWasUnsetResultThrowsLogicErrorException) {
    calculator.load("a * 4 + 5 / 6");
    calculator.setVar("a", 2.53);
    calculator.unsetVariable("a");
    EXPECT_THROW(calculator.result(), std::logic_error);
}

TEST_F(CalculatorTest, GivenComplexInfixEquationWithVariablesPowersAndParenthesesResultReturnsCorrectValue) {
    // 2 ^ 2 / ( 5 * 5 ) + 8 ^ (9 / (2 + 1) - 1) == 4 / 25 + 64
    calculator.load("alfa ^ 2 / (5 * delta) + Qw ^ (beta / (b + gamma) - qw)");
    calculator.setVar("alfa", 2);
    calculator.setVar("delta", 5);
    calculator.setVar("Qw", 8);
    calculator.setVar("qw", 1);
    calculator.setVar("beta", 9);
    calculator.setVar("b", 2);
    calculator.setVar("gamma", 1);

    auto expected{64.16};
    auto actual{calculator.result()};
    EXPECT_EQ(actual, expected);
}