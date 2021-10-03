add_library(implementations src/Element.cpp src/BasicElementCreator.cpp src/PostfixConverter.cpp
        src/InfixConverter.cpp src/EquationParser.cpp src/PostfixNotation.cpp src/BasicOperations.cpp
        src/BasicOperations.cpp src/InfixNotation.cpp src/Calculator.cpp)
target_include_directories(implementations PUBLIC include)

add_executable(tests uts/Element.cpp uts/BasicElementCreator.cpp uts/EquationParser.cpp
        uts/PostfixConverter.cpp uts/InfixConverter.cpp uts/BasicOperations.cpp
        uts/PostfixNotation.cpp uts/InfixNotation.cpp uts/Calculator.cpp)

target_link_libraries(tests gmock_main gtest gmock implementations)