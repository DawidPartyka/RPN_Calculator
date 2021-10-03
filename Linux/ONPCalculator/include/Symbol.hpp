#pragma once

enum class Symbol : int {
    ParenthesisOpen = 0,
    ParenthesisClose = 1,
    Number = 2,
    Variable = 3,
    Add = 4,
    Subtract = 5,
    Multiply = 6,
    Divide = 7,
    Power = 8
};