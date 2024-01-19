#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <iostream>

namespace DendyEngine
{

class CTransformComponent: IGameComponent
{
protected:
    glm::mat4 m_TranformMatrix{1};

protected:

public:
    CTransformComponent(std::string const& name) { std::cout << "Ctor of Component ["<<GetComponentTypeName()<<"]["<<name<<"]" << std::endl; }
    ~CTransformComponent();

    const glm::mat4 GetTransformMatrix() const { return m_TranformMatrix; }

    static const std::string GetComponentTypeName() { return "Transform"; }
};

}