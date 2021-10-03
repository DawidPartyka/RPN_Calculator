#pragma once

#include "INotationConverter.hpp"
#include "IEquationParser.hpp"
#include "NotationEnum.hpp"
#include <memory>
#include <utility>

class InfixConverter : public INotationConverter {
private:
    static constexpr unsigned int minimalStackSize{2};
    std::shared_ptr<IEquationParser> parser;
public:
    explicit InfixConverter(std::shared_ptr<IEquationParser> parser) : parser(std::move(parser)){};
    std::string convertToString(const std::vector<Element>&) override;
    std::queue<Element> convertToQueue(const std::vector<Element>&) override;
    bool check(const std::vector<Element>&) override;
    NotationEnum checksNotation() override;
    NotationEnum convertsToNotation() override;
};