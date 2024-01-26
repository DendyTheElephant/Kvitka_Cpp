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
    Pose,
    WalkingCharacter,
    RenderablePawn,
    StaticMesh,
    Camera,
    MovementDecision
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// Define Data
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct SPose : CGameComponent<SPose,EGameComponentType::Pose>
{
    glm::vec3 Position{0.0f};
    glm::vec3 Direction{1.0f, 0.0f, 0.0f};
};

struct SWalkingCharacter : CGameComponent<SWalkingCharacter,EGameComponentType::WalkingCharacter>
{
    glm::vec3 Position{0.0f};
    glm::vec3 Direction{1.0f, 0.0f, 0.0f};

    glm::vec3 TargetPosition{0.0f};
    glm::vec3 TargetDirection{1.0f, 0.0f, 0.0f};

    //float DirectionVelocity;

    bool IsSprinting{false};
    float Acceleration{0.0002f}; // 0.2m/s
    float SprintAcceleration{0.0005f}; // 0.5m/s
    float Velocity{0.0f};
    float MaxVelocity{0.00142f}; // 5.1km/h or 1.42m/s
    float SprintMaxVelocity{0.00362f}; // 13km/h or 3.62m/s
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

struct SStaticMesh : CGameComponent<SStaticMesh,EGameComponentType::StaticMesh>
{
    std::string MeshName{"Cube"};
    glm::vec3 Color{1.0f};
};

struct SCamera : CGameComponent<SCamera,EGameComponentType::Camera>
{
    glm::vec3 TargetPosition{0.0f, 0.0f, 0.0f};
    glm::vec3 ArmTranslationDirection{0.0f, 1.0f, 1.0f};
    float ArmTranslationMagnitude{20.0f};
    float ArmTranslationMagnitudeMinValue{5.0f};
    float ArmTranslationMagnitudeMaxValue{1000.0f};
    float Speed{0.00362f};
};

}