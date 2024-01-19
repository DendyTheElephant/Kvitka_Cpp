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
    virtual ~IGameComponent() = default;

    static const std::string GetComponentTypeName() { return ""; }
};

}