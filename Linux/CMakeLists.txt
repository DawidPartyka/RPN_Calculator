cmake_minimum_required(VERSION 3.10)
project(ONPCalculator CXX)

set(CMAKE_CXX_STANDARD 17)

if (MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

include(cmake/GTest.cmake)

add_subdirectory(ONPCalculator)

enable_testing()

add_test(tests ONPCalculator/tests)

add_custom_target(testsONP
        COMMAND "./ONPCalculator/tests"
        DEPENDS tests)