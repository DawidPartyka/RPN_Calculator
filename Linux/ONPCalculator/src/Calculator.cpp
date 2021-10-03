//
// Created by dawpa on 03.10.2021.
//

#include <sstream>
#include <algorithm>
#include "Calculator.hpp"


Calculator::Calculator(std::shared_ptr<IEquationParser> parser, std::shared_ptr<BasicOperations> operations)
    : parser(std::move(parser)), operations(std::move(operations)){}

Calculator::iterator Calculator::findNotationInVec(NotationEnum symbol) {
    auto it = std::find_if(notations.begin(), notations.end(), [&symbol](const auto& notation){
        return notation.first->getType() == symbol;
    });
    return it;
}

NotationEnum Calculator::equationNotationCheck() {
    if(equation.empty()){
        throw std::logic_error{"Calculator.equationNotationCheck: Equation is empty, no check allowed"};
    }

    auto it = std::find_if(notations.begin(), notations.end(), [this](const auto& elem){
        return elem.second->check(equation);
    });

    if(it == notations.end()){
        throw std::logic_error{"Calculator.equationNotationCheck: No registered notation can recognize this equation"};
    }

    return it->second->checksNotation();
}

unsigned int Calculator::setNotation(const std::shared_ptr<INotation>& newNotation, std::shared_ptr<INotationConverter> newConverter) {
    if(findNotationInVec(newNotation->getType()) == notations.end()){
        notations.emplace_back(newNotation, std::move(newConverter));
    }

    return notations.size();
}


unsigned int Calculator::unsetNotation(NotationEnum symbol) {
    auto it = findNotationInVec(symbol);
    if(it != notations.end()){
        notations.erase(it);
        return true;
    }

    return notations.size();
}

unsigned int Calculator::setVar(const std::string& varName, double varVal) {
    variables[varName] = varVal;
    return variables.size();
}

unsigned int Calculator::unsetVariable(const std::string & key) {
    variables.erase(key);
    return variables.size();
}

void Calculator::load(const std::string & equationString) {
    equation = parser->read(equationString);
    equationNotationCheck();
}

std::string Calculator::get(NotationEnum getInNotation) {
    auto currentEquationNotation{equationNotationCheck()};

    if(getInNotation == currentEquationNotation){
        std::stringstream output{};
        std::for_each(equation.begin(), equation.end(),
                      [&output](const auto& elem){ output << elem.getValue()<<" "; });

        std::string out = output.str();
        out.pop_back();
        return out;
    }

    auto it{std::find_if(notations.begin(), notations.end(), [&getInNotation](const auto& n){
        return n.second->convertsToNotation() == getInNotation;
    })};

    return it->second->convertToString(equation);
}

std::vector<Element> Calculator::changedVariables() {
    std::vector<Element> output{};
    std::for_each(equation.begin(), equation.end(), [this, &output](const auto& elem){
       if(elem.getSymbol() != Symbol::Variable){
           output.emplace_back(elem);
       }
       else{
           if(!variables.count(elem.getValue())){
               throw std::logic_error{"Calculator.changedVariables: No variable set for - " + elem.getValue()};
           }
           Element changed{Symbol::Number, std::to_string(variables.at(elem.getValue()))};
           output.push_back(changed);
       }
    });
    return output;
}

double Calculator::result() {
    auto currentEquationNotation{equationNotationCheck()};

    auto it{std::find_if(notations.begin(), notations.end(), [this, &currentEquationNotation](const auto& n){
        return n.first->getType() == currentEquationNotation;
    })};

    return it->first->calculate(operations, changedVariables());
}



