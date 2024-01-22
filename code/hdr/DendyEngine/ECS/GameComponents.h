#pragma once

#include <DendyEngine/ECS/GameComponentBase.h>

#include <glm/glm.hpp>

#include <vector>
#include <memory>
#include <unordered_map>

namespace DendyEngine::ECS
{

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// Define Enum
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum class EGameComponentType
{
    Transform,
    RenderablePawn,
    Camera,
    MovementDecision
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// Define Data
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct STransform : CGameComponent<STransform,EGameComponentType::Transform>
{
    //STransform(float posX, float posZ)
    glm::mat4 TransformMatrix{1.0f};
};

struct SMovementDecision : CGameComponent<SMovementDecision,EGameComponentType::MovementDecision>
{
    glm::vec2 Direction;
    float MaxDistance;
};

struct SRenderablePawn : CGameComponent<SRenderablePawn,EGameComponentType::RenderablePawn>
{
    glm::vec3 Color{1.0f};
};

struct SCamera : CGameComponent<SCamera,EGameComponentType::Camera>
{
    glm::vec3 TargetPosition{0.0f, 0.0f, 0.0f};
    glm::vec3 ArmTranslation{0.0f,20.0f,10.0f};
};

}