#pragma once

#include "DendyEngine/GameObject.h"

#include <string>
#include <iostream>

namespace DendyEngine
{

class CGameObject;

class IGameComponent
{
protected:
    CGameObject* m_pGameObjectOwner;

public:
    IGameComponent(CGameObject* pOwner): m_pGameObjectOwner(pOwner) { std::cout << "Constructor of [IGameComponent] of " << m_pGameObjectOwner << std::endl; }
    virtual ~IGameComponent() = default;

    CGameObject* GetOwner() const { return m_pGameObjectOwner; }

    static const std::string GetComponentTypeName() = delete;
};

}