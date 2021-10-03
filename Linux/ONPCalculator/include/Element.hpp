#pragma once

#include <string>
#include <Symbol.hpp>

class Element {
    const Symbol symbol;
    const std::string value;

public:
    Element(Symbol symbol, std::string value);

    [[nodiscard]] Symbol getSymbol() const;
    [[nodiscard]] std::string getValue() const;
    bool operator==(const Element& other){
        return symbol == other.getSymbol() && value == other.getValue();
    }
};