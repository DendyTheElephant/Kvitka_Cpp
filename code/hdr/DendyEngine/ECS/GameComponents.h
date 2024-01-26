#pragma once

#include <DendyEngine/ECS/GameComponentBase.h>
#include <DendyEngine/ECS/GameObject.h>

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
    ScenePose,
    Transform,
    WalkingCharacter,
    Vision,
    Visibility,
    RenderablePawn,
    StaticMesh,
    Camera
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// Define Data
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct SScenePose : CGameComponent<SScenePose,EGameComponentType::ScenePose>
{
    glm::vec2 Position{0.0f};
    glm::vec2 Orientation{0.0f, 1.0f};
};

struct STransform : CGameComponent<STransform,EGameComponentType::Transform>
{
    glm::mat4 TransformMatrix{1};
};

struct SWalkingCharacter : CGameComponent<SWalkingCharacter,EGameComponentType::WalkingCharacter>
{
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

struct SVision : CGameComponent<SVision,EGameComponentType::Vision>
{
    float Radius{50.0f};
    float AngleInDegrees{180.0};
    std::vector<CGameObject*> VisibleGameObjectsVec;
};

struct SVisibility : CGameComponent<SVisibility,EGameComponentType::Visibility>
{
    float Radius{1.0f}; // Can be negative!
};

// struct SMaslowNeeds : CGameComponent<SMaslowNeeds,EGameComponentType::MaslowNeeds>
// {
//     // Physiological needs
//     float WaterSatisfaction{1.0};
//     float FoodSatisfaction{1.0};
//     float HeatSatisfaction{1.0};
//     float ClothesSatisfaction{1.0};
//     float ShelterSatisfaction{1.0};
//     float SleepSatisfaction{1.0};

//     // Safety needs
//     float HealthSatisfaction{1.0};
//     float SecuritySatisfaction{1.0};
//     float EmotionalSatisfaction{1.0};
//     float FinancialSatisfaction{1.0};

//     // Love and social needs
//     float FamilySatisfaction{1.0};
//     float FriendshipSatisfaction{1.0};
//     float IntimacySatisfaction{1.0};
//     float LoveSatisfaction{1.0};

//     // Esteem needs
//     float PrestigeSatisfaction{1.0};
//     float AccomplishementSatisfaction{1.0};
//     float CreativeSatisfaction{1.0};
// };

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