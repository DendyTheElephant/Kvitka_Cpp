#pragma once

namespace DendyEngine::ECS
{

enum class EGameComponentType;

class IGameComponent
{
public:
    virtual ~IGameComponent() = default;
};

template<typename TGameComponent, EGameComponentType TGameComponentType>
class CGameComponent : public IGameComponent
{
public:
    static constexpr EGameComponentType Type = TGameComponentType;
};

}