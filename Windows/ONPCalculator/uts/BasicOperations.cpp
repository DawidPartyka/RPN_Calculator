//
// Created by dawpa on 02.10.2021.
//

#include <gtest/gtest.h>
#include <cmath>
#include <BasicElementCreator.hpp>
#include <BasicOperations.hpp>

class BasicOperationsTest : public ::testing::Test {
protected:
    static constexpr auto add{[](const auto& a, const auto& b) -> double { return a + b; }};
    static constexpr auto subtract{[](const auto& a, const auto& b) -> double { return a - b; }};
    static constexpr auto multiply{[](const auto& a, const auto& b) -> double { return a * b; }};
    static constexpr auto divide{[](const auto& a, const auto& b) -> double { return a / b; }};
    static constexpr auto power{[](const auto& a, const auto& b) -> double { return std::pow(a, b); }};

    std::map<std::string, Symbol> basicMap {
        {"+", Symbol::Add},
        {"-", Symbol::Subtract},
        {"*", Symbol::Multiply},
        {"/", Symbol::Divide},
        {"(", Symbol::ParenthesisOpen},
        {")", Symbol::ParenthesisClose},
        {"^", Symbol::Power}
    };

    BasicElementCreator creator{basicMap};

    BasicOperations operations{};

    Element zero{Symbol::Number, "0"};
    Element one{Symbol::Number, "1"};
    Element two{Symbol::Number, "2"};
    Element four{Symbol::Number, "4"};
    Element opAdd{Symbol::Add, "+"};
    Element opSubtract{Symbol::Subtract, "+"};
    Element opDivide{Symbol::Divide, "+"};
    Element opMultiply{Symbol::Multiply, "+"};
    Element opPower{Symbol::Power, "^"};
    Element opInvalid{Symbol::Variable, "a^&"};

    BasicOperationsTest()= default;
    ~BasicOperationsTest() override = default;
    void SetUp() override {
        operations.registerOperation(Symbol::Add, add);
        operations.registerOperation(Symbol::Subtract, subtract);
        operations.registerOperation(Symbol::Divide, divide);
        operations.registerOperation(Symbol::Multiply, multiply);
        operations.registerOperation(Symbol::Power, power);
    };
    void TearDown() override {};
};

TEST_F(BasicOperationsTest, Given2Plus2OperationReturnsElementWithValue4){
    auto expected{four};
    auto actual{operations.operation(two, opAdd, two)};

    EXPECT_TRUE(actual == expected);
}

TEST_F(BasicOperationsTest, Given2Times2OperationReturnsElementWithValue4){
    auto expected{four};
    auto actual{operations.operation(two, opMultiply, two)};

    EXPECT_TRUE(actual == expected);
}

TEST_F(BasicOperationsTest, Given2Minus2OperationReturnsElementWithValue0){
    auto expected{zero};
    auto actual{operations.operation(two, opSubtract, two)};

    EXPECT_TRUE(actual == expected);
}

TEST_F(BasicOperationsTest, Given2DivededBy2OperationReturnsElementWithValue1){
    auto expected{one};
    auto actual{operations.operation(two, opDivide, two)};

    EXPECT_TRUE(actual == expected);
}

TEST_F(BasicOperationsTest, Given2ToThePowerOf2OperationReturnsElementWithValue4){
    auto expected{four};
    auto actual{operations.operation(two, opPower, two)};
    EXPECT_TRUE(actual == expected);
}

TEST_F(BasicOperationsTest, GivenInvalidOperatorOperationReturnsInvalidArgumentException){
    EXPECT_THROW(operations.operation(two, opInvalid, two), std::invalid_argument);
}

TEST_F(BasicOperationsTest, GivenRemovedOperatorOperationReturnsInvalidArgumentException){
    operations.removeOperations(Symbol::Add);

    EXPECT_THROW(operations.operation(two, opAdd, two), std::invalid_argument);
}

TEST_F(BasicOperationsTest, GivenInvalidOperandOperandReturnsInvalidArgumentException){
    EXPECT_THROW(operations.operation(opInvalid, opDivide, two), std::invalid_argument);
}

