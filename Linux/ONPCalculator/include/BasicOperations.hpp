#pragma once

#include <functional>
#include <vector>
#include "Symbol.hpp"
#include "Element.hpp"

class BasicOperations {
    std::vector<std::pair<Symbol, std::function<double(const double& a, const double& b)>>> operations;
public:
    Element operation(const Element&, const Element&, const Element&);
    void registerOperation(Symbol, const std::function<double(const double& a, const double& b)>&);
    void removeOperations(Symbol);
};

