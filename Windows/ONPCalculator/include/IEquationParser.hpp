#pragma once
#include "Element.hpp"

class IEquationParser {
public:
    virtual ~IEquationParser() = default;
    virtual std::vector<Element> read(const std::string&) = 0;
};