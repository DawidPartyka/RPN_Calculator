#pragma once


#include <vector>
#include <map>
#include <memory>
#include "INotation.hpp"
#include "IEquationParser.hpp"
#include "INotationConverter.hpp"

class Calculator {
    std::vector< std::pair< std::shared_ptr<INotation>, std::shared_ptr<INotationConverter> > > notations{};
    typedef std::vector< std::pair< std::shared_ptr<INotation>, std::shared_ptr<INotationConverter> > >::iterator iterator;
    std::map<std::string, double>variables{};
    std::shared_ptr<IEquationParser> parser;
    std::shared_ptr<BasicOperations> operations;
    std::vector<Element> equation;
    NotationEnum properConverter{};
    NotationEnum equationNotationCheck();
    iterator findNotationInVec(NotationEnum);
    std::vector<Element> changedVariables();
public:
    explicit Calculator(std::shared_ptr<IEquationParser>, std::shared_ptr<BasicOperations>);
    unsigned int setNotation(const std::shared_ptr<INotation>&, std::shared_ptr<INotationConverter>);
    unsigned int unsetNotation(NotationEnum);
    unsigned int setVar(const std::string&, double);
    unsigned int unsetVariable(const std::string&);
    void load(const std::string&);
    std::string get(NotationEnum);
    double result();
};