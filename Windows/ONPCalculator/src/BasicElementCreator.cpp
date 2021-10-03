//
// Created by dawpa on 01.10.2021.
//
#include <BasicElementCreator.hpp>
#include <stdexcept>

BasicElementCreator::BasicElementCreator(std::map<std::string, Symbol> symbolMap) : symbolMap(std::move(symbolMap)) {}

Element BasicElementCreator::createElement (const std::string& sign) {
    if(symbolMap.find(sign) != symbolMap.end()) {
        return Element{symbolMap.at(sign), sign};
    }
    else if(BasicElementCreator::isNumber(sign)) {
        return Element{Symbol::Number, sign};
    }
    else if(std::all_of(sign.begin(), sign.end(), ::isalpha)) {
        return Element{Symbol::Variable, sign};
    }

    throw std::invalid_argument{"BasicElementCreator: Invalid std::string argument supplied - " + sign};
}

bool BasicElementCreator::isNumber(const std::string& isNum) {
    auto invalidCount = [count = 0](const auto c) mutable {
        if(c == '.'){
            ++count;
        }
        else if(!::isdigit(c)){
            return true;
        }
        return count > 1;
    };
    if(std::any_of(isNum.begin(), isNum.end(), ::isalpha) || (std::count_if(isNum.begin(), isNum.end(), invalidCount) > 0)){
        return false;
    }

    try {
        std::stod(isNum);
    }
    catch(...) {
        return false;
    }
    return true;
}