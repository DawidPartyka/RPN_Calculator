#pragma once

#include "INotation.hpp"
#include "InfixConverter.hpp"
#include "BasicOperations.hpp"

class InfixNotation : public INotation {
public:
    explicit InfixNotation(NotationEnum type = NotationEnum::Infix);
    double calculate(std::shared_ptr<BasicOperations>& operations, const std::vector<Element>&) override;
};
