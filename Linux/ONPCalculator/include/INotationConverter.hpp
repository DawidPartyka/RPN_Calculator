#pragma once

#include <queue>
#include <stack>
#include "Element.hpp"
#include "NotationEnum.hpp"

class INotationConverter {
protected:
    static bool isOperator(Symbol symbol) { return symbol >= Symbol::Add; }
public:
    virtual ~INotationConverter() = default;
    virtual std::string convertToString(const std::vector<Element>&) = 0;
    virtual std::queue<Element> convertToQueue(const std::vector<Element>&) = 0;
    virtual bool check(const std::vector<Element>&) = 0;
    virtual NotationEnum convertsToNotation() = 0;
    virtual NotationEnum checksNotation() = 0;
    static unsigned int getPriority(Symbol symbol) {
        if(symbol == Symbol::Add || symbol == Symbol::Subtract){
            return 1;
        }
        else if(symbol == Symbol::Multiply || symbol == Symbol::Divide){
            return 2;
        }
        return 3;
    }
};