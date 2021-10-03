//
// Created by dawpa on 02.10.2021.
//
#include <sstream>
#include <algorithm>
#include "BasicOperations.hpp"

Element BasicOperations::operation(const Element& a, const Element& operand, const Element& b)  {
    double aVal = std::stod(a.getValue());
    double bVal = std::stod(b.getValue());

    auto found{std::find_if(BasicOperations::operations.begin(), BasicOperations::operations.end(), [&operand](const auto& action){
        return action.first == operand.getSymbol();
    })};

    if(found == BasicOperations::operations.end()){
        throw std::invalid_argument{"BasicOperations.operation: Invalid operand argument - " + operand.getValue()};
    }

    //string cleanup
    std::string outValue = std::to_string(found->second(bVal, aVal));
    outValue.erase ( outValue.find_last_not_of('0') + 1, std::string::npos );
    if(*(--outValue.end()) == '.')
        outValue.pop_back();

    return Element{Symbol::Number, outValue};
}

void BasicOperations::registerOperation(Symbol symbol, const std::function<double(const double &, const double &)> & action) {
    operations.emplace_back(symbol, action);
}

void BasicOperations::removeOperations(Symbol symbol) {
    auto it = std::find_if(operations.begin(), operations.end(), [&symbol](const auto& elem) { return elem.first == symbol; });
    if(it != operations.end()){
        operations.erase(it);
    }
}
