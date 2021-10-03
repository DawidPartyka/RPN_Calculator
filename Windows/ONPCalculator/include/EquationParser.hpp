#pragma once
#include <memory>
#include <vector>
#include <iterator>
#include "IEquationParser.hpp"
#include "IElementCreator.hpp"

class EquationParser : public IEquationParser {
private:
    std::shared_ptr<IElementCreator> elementCreator;
public:
    explicit EquationParser(std::shared_ptr<IElementCreator> creator) : elementCreator(std::move(creator)){}
    std::vector<Element> read(const std::string& equation) override;
};
