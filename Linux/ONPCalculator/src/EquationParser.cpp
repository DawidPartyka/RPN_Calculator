//
// Created by dawpa on 02.10.2021.
//

#include <EquationParser.hpp>

std::vector<Element> EquationParser::read(const std::string& equation) {
    std::vector<Element> result{};

    auto it{equation.begin()};
    auto last{equation.begin()};
    auto end{equation.cend()};

    auto stringCopy = [&](){
        std::string copied;
        std::copy(last, it, std::back_inserter(copied));

        if(copied.length() > 0)
            result.push_back(elementCreator->createElement(copied));
    };

    while((it = std::find_if_not(it, end, ::isalnum)) != end){
        stringCopy();

        if(*it != ' ')
            result.push_back(elementCreator->createElement(std::string(1, *it)));

        ++it;
        last = it;
    }

    stringCopy();

    return result;
}