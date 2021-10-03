//
// Created by dawpa on 02.10.2021.
//
#include <stdexcept>
#include <sstream>
#include <algorithm>
#include "PostfixConverter.hpp"

std::queue<Element> PostfixConverter::convertToQueue(const std::vector<Element>& equation) {
    std::stack<Element> operators{};
    std::queue<Element> output{};

    for(const auto& token : equation) {
        auto symbol = token.getSymbol();

        if(symbol == Symbol::ParenthesisOpen) {
            operators.push(token);
        }

        else if(symbol == Symbol::ParenthesisClose){
            while(!operators.empty() && operators.top().getSymbol() != Symbol::ParenthesisOpen){
                output.push(operators.top());
                operators.pop();
            }

            if(operators.empty()){
                throw std::logic_error{"PostfixConverter: Invalid equation parenthesis"};
            }

            operators.pop();
        }

        else if(isOperator(symbol)) {
            while(!operators.empty() && isOperator(operators.top().getSymbol()) && (getPriority(symbol) <= getPriority(operators.top().getSymbol()))){
                output.push(operators.top());
                operators.pop();
            }

            operators.push(token);
        }

        else{
            output.push(token);
        }
    }

    while(!operators.empty()){
        if(!isOperator(operators.top().getSymbol())){
            throw std::logic_error{"PostfixConverter: Invalid equation parenthesis"};
        }
        output.push(operators.top());
        operators.pop();
    }

    return output;
}

std::string PostfixConverter::convertToString(const std::vector<Element>& equation) {
    auto converted = convertToQueue(equation);
    std::stringstream appended;

    while(!converted.empty()){
        appended << converted.front().getValue() << " ";
        converted.pop();
    }

    std::string output{appended.str()};
    output.pop_back();

    return output;
}

bool PostfixConverter::check(const std::vector<Element> & equation) {
    auto adjacent = std::adjacent_find(equation.begin(), equation.end(), [](const auto& left, const auto& right){
        auto lSym{left.getSymbol()};
        auto rSym{right.getSymbol()};
        return (lSym == Symbol::Number || lSym == Symbol::Variable) &&
            (rSym == Symbol::Number || rSym == Symbol::Variable);
    });

    return adjacent == equation.end();
}

NotationEnum PostfixConverter::checksNotation() {
    return NotationEnum::Infix;
}

NotationEnum PostfixConverter::convertsToNotation() {
    return NotationEnum::Postfix;
}




