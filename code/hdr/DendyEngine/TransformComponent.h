#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>

namespace DendyEngine
{

class CTransformComponent
{
protected:
    glm::mat4 m_TranformMatrix{1};

protected:

public:
    CTransformComponent() {};
    ~CTransformComponent();

    const glm::mat4 GetTransformMatrix() const { return m_TranformMatrix; }

    static const std::string GetComponentTypeName() { return "Transform"; }
};

}