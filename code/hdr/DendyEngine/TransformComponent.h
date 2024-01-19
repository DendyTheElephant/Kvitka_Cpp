#pragma once

#include "DendyEngine/GameObject.h"
#include "DendyEngine/GameComponent.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <iostream>

namespace DendyEngine
{

class CTransformComponent: public IGameComponent
{
protected:
    glm::mat4 m_TranformMatrix{1};

public:
    CTransformComponent(CGameObject* pOwner):
    IGameComponent(pOwner)
    {
        std::cout << "Constructor of ["<<GetComponentTypeName()<<"] of "<< *m_pGameObjectOwner << std::endl;
    }
    virtual ~CTransformComponent() { std::cout << "Destructor of Component ["<<GetComponentTypeName()<<"] of "<< *m_pGameObjectOwner << std::endl; };

    const glm::mat4 GetTransformMatrix() const { return m_TranformMatrix; }

    static const std::string GetComponentTypeName() { return "CTransformComponent"; }
};

}