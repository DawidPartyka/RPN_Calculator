//
// Created by dawpa on 01.10.2021.
//

#include <Symbol.hpp>
#include <Element.hpp>

Element::Element(Symbol symbol, std::string value) : symbol(symbol), value(std::move(value)) {}

Symbol Element::getSymbol() const {
    return symbol;
}

std::string Element::getValue() const {
    return value;
}