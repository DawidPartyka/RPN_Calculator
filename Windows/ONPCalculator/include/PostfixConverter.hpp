#pragma once

#include <string>
#include "INotationConverter.hpp"
#include "NotationEnum.hpp"


class PostfixConverter : public INotationConverter {
public:
    std::string convertToString(const std::vector<Element>&) override;
    std::queue<Element> convertToQueue(const std::vector<Element>&) override;
    bool check(const std::vector<Element>&) override;
    NotationEnum checksNotation() override;
    NotationEnum convertsToNotation() override;
};