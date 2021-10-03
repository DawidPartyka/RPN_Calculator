#pragma once

#include <algorithm>

class IElementCreator {
public:
    virtual ~IElementCreator() = default;
    virtual Element createElement (const std::string& sign) = 0;
};
