#pragma once

#include "INotation.hpp"
#include "PostfixConverter.hpp"
#include "NotationEnum.hpp"

class PostfixNotation : public INotation {
public:
    explicit PostfixNotation(NotationEnum type = NotationEnum::Postfix);
    double calculate(std::shared_ptr<BasicOperations>& operations, const std::vector<Element>&) override;
};