//
// Created by dawpa on 02.10.2021.
//
#include <stdexcept>
#include <algorithm>
#include "InfixConverter.hpp"

std::string InfixConverter::convertToString(const std::vector<Element>& equation) {
    std::stack<std::string> out{};

    for(const auto& elem : equation){
        auto symbol{elem.getSymbol()};

        if(symbol == Symbol::Number || symbol == Symbol::Variable) {
            out.push(elem.getValue());
        }

        else if(isOperator(symbol)) {
            if(out.size() < minimalStackSize){
                throw std::logic_error{"InfixConverter.convertToString: Incorrect postfix equation"};
            }

            auto a{out.top()};
            out.pop();
            auto b{out.top()};
            out.pop();
            std::string toPush{"("};
            toPush.append(b).append(elem.getValue()).append(a).append(")");

            out.push(toPush);
        }
    }

    return out.top();
}

std::queue<Element> InfixConverter::convertToQueue(const std::vector<Element> & equation) {
    std::queue<Element> result{};

    auto elementVector{parser->read(convertToString(equation))};
    for(const auto& a : elementVector){
        result.push(a);
    }

    return result;
}

bool InfixConverter::check(const std::vector<Element> & equation) {
    auto it = std::adjacent_find(equation.begin(), equation.end(), [](const auto& left, const auto& right){
        return (left.getSymbol() == Symbol::ParenthesisOpen && right.getSymbol() == Symbol::ParenthesisClose);
    });

    if(it != equation.end()){
        return false;
    }

    try{
        auto res{convertToString(equation)};
        return true;
    }
    catch (std::logic_error&) {
        return false;
    }
}

NotationEnum InfixConverter::checksNotation() {
    return NotationEnum::Postfix;
}

NotationEnum InfixConverter::convertsToNotation() {
    return NotationEnum::Infix;
}

