#pragma once

#include <string>

namespace DendyEngine
{

class IGameComponent
{
protected:

protected:

public:
    IGameComponent() {};
    virtual ~IGameComponent() = 0;

    static const std::string GetComponentTypeName() { return ""; }
};

}