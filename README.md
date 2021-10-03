# RPN (pl. ONP) Calculator
Calculator can convert and evaluate postfix and infix equation passed to it as strings.  
Adds, subtracts, multiplies, divides and exponentiates.  
Available operations are extendable.   
The same goes for notations.  
Variables can be used and calculated after using ```calculatorInstance.setVar("a", 1);```  
Calculator operates on double types.  
Spaces are ignored.  
Separate build for Windows and Linux  
100+  GTest tests so far

## Requirements:
* CMake 3.10+
* Compiler compatible with CMake (gcc, clang, etc...)
* Git

## Compilation and test run on Linux:
```bash
$ cd cmake
$ cmake .. 
$ make
$ cd ONPCalculator
$ ./tests
````
## Basically how it works
### [Equation parser](Linux/ONPCalculator/include/EquationParser.hpp)
Parsers string into "tokens" creating [Element](Linux/ONPCalculator/include/Element.hpp) returning vector of Elements

### [BasicElementCreator](Linux/ONPCalculator/include/BasicElementCreator.hpp)
- basically just creates elements with correct [Symbol](Linux/ONPCalculator/include/Symbol.hpp) according to the
sign passed to it
- takes ``std::map<std::string, Symbol>`` in constructor to create basic elements and the rest of the
logic  
handling numbers and variables is in the ``createElement(const std::string&)`` method

### [Basic operations](Linux/ONPCalculator/include/BasicOperations.hpp)
- registers operations through ``registerOperation`` method with argument of Symbol  
and ``const std::function<double(const double& a, const double& b)>`` that can take lambda to perform an operation
- removes operations through ``removeOperations`` method with argument of Symbol
- performs operations through ``operation`` method with 3 elements as arguments in order ``a operand b``  
returning new Element with value after the operation

### [PostfixConverter](Linux/ONPCalculator/include/PostfixConverter.hpp) and [InfixConverter](Linux/ONPCalculator/include/InfixConverter.hpp)
- ``check()`` if given equation ``std::vector<Element>`` is specific notation returning 
[NotationEnum](Linux/ONPCalculator/include/NotationEnum.hpp)   
- ``checksNotation()`` returns NotationEnum of which Notation it checks the equation to be
- ``convertsToNotation()`` returns NotationEnum to which Notation it converts the equation
- ``convertToQueue()`` converts the equation to ``std::queue``
- ``convertToString()`` converts the equation to ``std::string``
- ``static unsigned int getPriority()`` returns priority of signs
  
(InfixConverter takes ``std::shared_ptr<IEquationParser>`` as it converts equations to string, so returning std::queue  
requires another parser run to tokenize it again) 

### [InfixNotation](Linux/ONPCalculator/include/InfixNotation.hpp) and [PostfixNotation](Linux/ONPCalculator/include/PostfixNotation.hpp)
- ``calculate()`` returns evaluated value of the equation as double
- contains NotationEnum of what notation it evaluates

### [Calculator](Linux/ONPCalculator/include/Calculator.hpp)
- ``Calculator(std::shared_ptr<IEquationParser>, std::shared_ptr<BasicOperations>)`` takes shared pointers  
to EquationParser and BasicOperations
- ``load(const std::string&)`` simply stores the equation string
- ``setNotation(const std::shared_ptr<INotation>&, std::shared_ptr<INotationConverter>)`` registers notation  
storing it in a pair with a converter that can convert opposite notation to the one on which it can perform calculation  
- ``unsetNotation(NotationEnum)`` removes INotation and associated converter in which the INotation holds the same NotationEnum
- ``setVar(const std::string&, double)`` associates variable name with a value
- ``unsetVariable(const std::string&)`` removes variable
- ``get(NotationEnum)`` returns loaded equation as string in requested notation while still displaying variables instead of their values
- ``result()`` returns evaluated equation in form of a double
