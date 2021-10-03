#pragma once

#include <vector>
#include <memory>
#include "NotationEnum.hpp"
#include "Element.hpp"
#include "BasicOperations.hpp"

class INotation {
protected:
    const NotationEnum type{};
    explicit INotation(NotationEnum type) : type(type){};
public:
    virtual ~INotation() = default;
    virtual double calculate(std::shared_ptr<BasicOperations>& operations, const std::vector<Element>&) = 0;
    virtual NotationEnum getType(){ return type; }
};
