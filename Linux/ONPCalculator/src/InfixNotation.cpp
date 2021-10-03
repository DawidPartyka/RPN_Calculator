//
// Created by dawpa on 02.10.2021.
//
#include "InfixNotation.hpp"
#include <stdexcept>
#include <iostream>

InfixNotation::InfixNotation(NotationEnum type) : INotation(type){}

double InfixNotation::calculate(std::shared_ptr<BasicOperations> &operations, const std::vector<Element> & equation) {
    std::stack<Element> output{};
    std::stack<Element> operators{};

    auto performOperation = [&output, &operators, &operations](){
        const auto a{output.top()};
        output.pop();
        const auto b{output.top()};
        output.pop();
        const auto op{operators.top()};
        operators.pop();

        output.push(operations->operation(a, op, b));
    };

    for(const auto& elem : equation) {
        auto symbol{elem.getSymbol()};

        switch (symbol) {
            case Symbol::ParenthesisOpen:
                operators.push(elem);
                break;
            case Symbol::Number:
                output.push(elem);
                break;
            case Symbol::ParenthesisClose:
                while(!operators.empty() && operators.top().getSymbol() != Symbol::ParenthesisOpen){
                    performOperation();
                }

                if(operators.empty() || operators.top().getSymbol() != Symbol::ParenthesisOpen){
                    throw std::logic_error{"InfixNotation.calculate: Incorrect equation"};
                }

                operators.pop();
                break;
            default:
                while(!operators.empty() && INotationConverter::getPriority(operators.top().getSymbol()) >= INotationConverter::getPriority(symbol) && operators.top().getSymbol() != Symbol::ParenthesisOpen){
                    performOperation();
                }
                operators.push(elem);
                break;
        }
    }

    while(!operators.empty()){
        performOperation();
    }

    return std::stod(output.top().getValue());
}
