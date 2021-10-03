#pragma once

#include <Element.hpp>
#include <IElementCreator.hpp>
#include <map>

class BasicElementCreator : public IElementCreator{
    const std::map<std::string, Symbol> symbolMap;
public:
    explicit BasicElementCreator(std::map<std::string, Symbol>);

    Element createElement(const std::string&) override;

    static bool isNumber(const std::string&);
};