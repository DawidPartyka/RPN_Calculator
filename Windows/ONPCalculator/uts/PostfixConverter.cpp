//
// Created by dawpa on 02.10.2021.
//

#include <gtest/gtest.h>
#include <BasicElementCreator.hpp>
#include <PostfixConverter.hpp>
#include <queue>

class PostfixConverterTest : public ::testing::Test {
protected:
    static constexpr std::pair add{"+", Symbol::Add};
    static constexpr std::pair subtract{"-", Symbol::Subtract};
    static constexpr std::pair divide{"/", Symbol::Divide};
    static constexpr std::pair multiply{"*", Symbol::Multiply};
    static constexpr std::pair power{"^", Symbol::Power};
    static constexpr std::pair parenthesisOpen{"(", Symbol::ParenthesisOpen};
    static constexpr std::pair parenthesisClose{")", Symbol::ParenthesisClose};

    PostfixConverter converter{};

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

    PostfixConverterTest()= default;
    ~PostfixConverterTest() override = default;
    void SetUp() override {};
    void TearDown() override {};
};

TEST_F(PostfixConverterTest, smoke) {
    std::vector<Element> equation = { creator.createElement("2") };
    auto expected = queueFromVector({ creator.createElement("2") });

    EXPECT_TRUE(compareQueues(expected, converter.convertToQueue(equation)));
}

TEST_F(PostfixConverterTest, GivenTwoPlusTwoInfixConvertToQueueReturnsCorrectEquationOrderInQueue) {
    std::vector<Element> equation{
            creator.createElement("2"),
            creator.createElement("+"),
            creator.createElement("2")
    };

    std::queue<Element> expected = queueFromVector({
            creator.createElement("2"),
            creator.createElement("2"),
            creator.createElement("+")
    });

    EXPECT_TRUE(compareQueues(expected, converter.convertToQueue(equation)));
}

TEST_F(PostfixConverterTest, GivenComplexInfixEquationConvertToQueueReturnsCorrectEquationOrderInQueue) {
    std::vector<Element> equation{
            creator.createElement("12"), creator.createElement("+"),
            creator.createElement("2"), creator.createElement("*"),
            creator.createElement("("), creator.createElement("3"),
            creator.createElement("*"), creator.createElement("4"),
            creator.createElement("+"), creator.createElement("10"),
            creator.createElement("/"), creator.createElement("5"),
            creator.createElement(")")
    };

    std::queue<Element> expected{queueFromVector({
            creator.createElement("12"), creator.createElement("2"),
            creator.createElement("3"), creator.createElement("4"),
            creator.createElement("*"), creator.createElement("10"),
            creator.createElement("5"), creator.createElement("/"),
            creator.createElement("+"), creator.createElement("*"),
            creator.createElement("+")
    })};

    EXPECT_TRUE(compareQueues(expected, converter.convertToQueue(equation)));
}

TEST_F(PostfixConverterTest, GivenTrickyInfixEquationFromWikipediaConvertToQueueReturnsCorrectEquationOrderInQueue) {
    std::vector<Element> equation{
            creator.createElement("("), creator.createElement("1"),
            creator.createElement("+"), creator.createElement("2"),
            creator.createElement(")"), creator.createElement("*"),
            creator.createElement("4"), creator.createElement("+"),
            creator.createElement("5"), creator.createElement("-"),
            creator.createElement("3")
    };

    std::queue<Element> expected{queueFromVector({
            creator.createElement("1"), creator.createElement("2"),
            creator.createElement("+"), creator.createElement("4"),
            creator.createElement("*"), creator.createElement("5"),
            creator.createElement("+"), creator.createElement("3"),
            creator.createElement("-")
    })};

    EXPECT_TRUE(compareQueues(expected, converter.convertToQueue(equation)));
}

TEST_F(PostfixConverterTest, GivenChangedInfixEquationFromWikipediaConvertToQueueReturnsCorrectEquationOrderInQueue) {
    std::vector<Element> equation{
            creator.createElement("("), creator.createElement("1"),
            creator.createElement("+"), creator.createElement("2"),
            creator.createElement(")"), creator.createElement("/"),
            creator.createElement("4"), creator.createElement("+"),
            creator.createElement("5"), creator.createElement("-"),
            creator.createElement("3")
    };

    std::queue<Element> expected{queueFromVector({
        creator.createElement("1"), creator.createElement("2"),
        creator.createElement("+"), creator.createElement("4"),
        creator.createElement("/"), creator.createElement("5"),
        creator.createElement("+"), creator.createElement("3"),
        creator.createElement("-")
    })};

    EXPECT_TRUE(compareQueues(expected, converter.convertToQueue(equation)));
}

TEST_F(PostfixConverterTest, GivenInfixEquationWithManyParenthesesConvertToQueueReturnsCorrectEquationOrderInQueue) {
    std::vector<Element> equation{
            creator.createElement("("), creator.createElement("("),
            creator.createElement("2"), creator.createElement("+"),
            creator.createElement("7"), creator.createElement(")"),
            creator.createElement("/"), creator.createElement("3"),
            creator.createElement("+"), creator.createElement("("),
            creator.createElement("14"), creator.createElement("-"),
            creator.createElement("3"), creator.createElement(")"),
            creator.createElement("*"), creator.createElement("4"),
            creator.createElement(")"), creator.createElement("/"),
            creator.createElement("2")
    };

    std::queue<Element> expected{queueFromVector({
        creator.createElement("2"), creator.createElement("7"),
        creator.createElement("+"), creator.createElement("3"),
        creator.createElement("/"), creator.createElement("14"),
        creator.createElement("3"), creator.createElement("-"),
        creator.createElement("4"), creator.createElement("*"),
        creator.createElement("+"), creator.createElement("2"),
        creator.createElement("/")
    })};

    EXPECT_TRUE(compareQueues(expected, converter.convertToQueue(equation)));
}

TEST_F(PostfixConverterTest, GivenOnlyParenthesesConvertToQueueReturnsEmptyQueue) {
    std::vector<Element> equation{
            creator.createElement("("),
            creator.createElement(")")
    };

    std::queue<Element> expected = queueFromVector({});

    EXPECT_TRUE(compareQueues(expected, converter.convertToQueue(equation)));
}

TEST_F(PostfixConverterTest, GivenOnlyMultipleParenthesesConvertToQueueReturnsEmptyQueue) {
    std::vector<Element> equation{
            creator.createElement("("),
            creator.createElement(")"),
            creator.createElement("("),
            creator.createElement(")"),
            creator.createElement("("),
            creator.createElement(")")
    };

    std::queue<Element> expected = queueFromVector({});

    EXPECT_TRUE(compareQueues(expected, converter.convertToQueue(equation)));
}

TEST_F(PostfixConverterTest, GivenOnlyPlusSignConvertToQueueReturnsPlusSign) {
    std::vector<Element> equation{ creator.createElement("+") };
    std::queue<Element> expected{ queueFromVector({ creator.createElement("+") })};

    EXPECT_TRUE(compareQueues(expected, converter.convertToQueue(equation)));
}

TEST_F(PostfixConverterTest, GivenIncorrectEquationWithOnlyOpeningParenthesisConvertToQueueReturnsLogicErrorException) {
    std::vector<Element> equation{
            creator.createElement("("),
            creator.createElement("2"),
            creator.createElement("2"),
            creator.createElement("+")
    };

    EXPECT_THROW(converter.convertToQueue(equation), std::logic_error);
}

TEST_F(PostfixConverterTest, GivenIncorrectEquationWithOnlyClosingParenthesisConvertToQueueReturnsLogicErrorException) {
    std::vector<Element> equation{
            creator.createElement("2"),
            creator.createElement("2"),
            creator.createElement("+"),
            creator.createElement(")")
    };

    EXPECT_THROW(converter.convertToQueue(equation), std::logic_error);
}

TEST_F(PostfixConverterTest, GivenWeirdCaseThatImNotSureHowTheConvertToQueueShouldRespondToItReturnsPseudoCorrectQueue) {
    std::vector<Element> equation{ creator.createElement("2"), creator.createElement("+") };

    std::queue<Element> expected{ queueFromVector({ creator.createElement("2"), creator.createElement("+") }) };

    EXPECT_TRUE(compareQueues(expected, converter.convertToQueue(equation)));
}

TEST_F(PostfixConverterTest, GivenPostfixEquationCheckReturnsFalse) {
    std::vector<Element> equation{
        creator.createElement("2"),
        creator.createElement("2"),
        creator.createElement("+")
    };

    auto expected = queueFromVector(equation);

    EXPECT_FALSE(converter.check(equation));
}

TEST_F(PostfixConverterTest, GivenInfixEquationCheckReturnsTrue) {
    std::vector<Element> equation{
        creator.createElement("2"),
        creator.createElement("+"),
        creator.createElement("2")
    };

    auto expected = queueFromVector(equation);

    EXPECT_TRUE(converter.check(equation));
}

TEST_F(PostfixConverterTest, Given2Plus2EquationConvertToStringReturnsString22PlusSeparatedWithSpaces) {
    std::vector<Element> equation{
        creator.createElement("2"),
        creator.createElement("+"),
        creator.createElement("2")
    };

    EXPECT_EQ(converter.convertToString(equation), "2 2 +");
}

TEST_F(PostfixConverterTest, GivenComplexInfixEquationWithPowersConvertToQueueReturnsCorrectEquation) {

    //9 ^ 2 / (5 * 12) + 10 ^ (24 / (13 + 2) - 1)
    std::vector<Element> equation{
        creator.createElement("9"), creator.createElement("^"),
        creator.createElement("2"), creator.createElement("/"),
        creator.createElement("("), creator.createElement("5"),
        creator.createElement("*"), creator.createElement("12"),
        creator.createElement(")"), creator.createElement("+"),
        creator.createElement("10"), creator.createElement("^"),
        creator.createElement("("), creator.createElement("24"),
        creator.createElement("/"), creator.createElement("("),
        creator.createElement("13"), creator.createElement("+"),
        creator.createElement("2"), creator.createElement(")"),
        creator.createElement("-"), creator.createElement("1"),
        creator.createElement(")")
    };

    // 9 2 ^ 5 12 * / 10 24 13 2 + / 1 - ^ +
    std::queue<Element> expected{ queueFromVector({
        creator.createElement("9"), creator.createElement("2"),
        creator.createElement("^"), creator.createElement("5"),
        creator.createElement("12"), creator.createElement("*"),
        creator.createElement("/"), creator.createElement("10"),
        creator.createElement("24"), creator.createElement("13"),
        creator.createElement("2"), creator.createElement("+"),
        creator.createElement("/"), creator.createElement("1"),
        creator.createElement("-"), creator.createElement("^"),
        creator.createElement("+")
    })};
    EXPECT_TRUE(compareQueues(expected, converter.convertToQueue(equation)));
}