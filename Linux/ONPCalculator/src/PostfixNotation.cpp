//
// Created by dawpa on 02.10.2021.
//

#include <BasicOperations.hpp>
#include <stdexcept>
#include <vector>
#include "PostfixNotation.hpp"

PostfixNotation::PostfixNotation(NotationEnum type) : INotation(type) {}

double PostfixNotation::calculate(std::shared_ptr<BasicOperations>& operations, const std::vector<Element> & equation) {
    std::stack<Element> result{};

    for(const auto & element : equation) {
        auto symbol = element.getSymbol();
        if(symbol == Symbol::Number){
            result.push(element);
        }

        else if(result.size() < 2){
            throw std::logic_error{"PostfixNotation.calculate: Invalid equation"};
        }

        else {
            const Element a = result.top();
            result.pop();
            const Element b = result.top();
            result.pop();

            result.push(operations->operation(a, element, b));
        }
    }

    auto res = result.top().getValue();
    return std::stod(res);
}
