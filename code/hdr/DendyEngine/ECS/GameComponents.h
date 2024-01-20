#pragma once

#include <DendyEngine/ECS/GameObject.h>

#include <string>
#include <iostream>

#include <glm/glm.hpp>


namespace DendyEngine
{

class CGameObject;

class IGameComponent
{
protected:
    CGameObject* m_pGameObjectOwner;

public:
    IGameComponent(CGameObject* pOwner): m_pGameObjectOwner(pOwner) {}
    virtual ~IGameComponent() = default;

    CGameObject* GetOwner() const { return m_pGameObjectOwner; }
};


struct CTransformComponent: public IGameComponent { using IGameComponent::IGameComponent;
    glm::mat4 TranformMatrix{1};
};

struct CSpatialNavigationComponent: public IGameComponent { using IGameComponent::IGameComponent;
    glm::vec2 Step{0};
    float Speed{0.0f};
};

struct CRenderablePawnComponent: public IGameComponent { using IGameComponent::IGameComponent;
    glm::vec3 Color{1,1,1};
    float Radius{0.5f};
    float Height{1.0f};
};



}