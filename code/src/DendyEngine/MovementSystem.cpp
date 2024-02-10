#include <DendyEngine/MovementSystem.h>
#include <DendyCommon/Logger.h>
//#include <DendyCommon/Math.h>
#include <DendyEngine/Definitions.h>


void DendyEngine::CMovementSystem::UpdatePositionOfMovingGameObject(CGameObject* pGameObject, float const& deltaTime) const
{
    LOG_CALLSTACK_PUSH(__FILE__,__LINE__,__PRETTY_FUNCTION__);

    Components::SWalkingCharacter* pWalkingCharacter = pGameObject->GetComponent<Components::SWalkingCharacter>();
    Components::SMovementTarget* pMovementTarget = pGameObject->GetComponent<Components::SMovementTarget>();
    
    Components::SScenePose* pPose = pGameObject->GetScenePose();
    glm::vec2 NewPosition;

    switch (pMovementTarget->MovementType)
    {
    case Components::SMovementTarget::EMovementTargetType::MoveToDirection:
        pPose->Orientation = pMovementTarget->TargetDirection;

        pWalkingCharacter->Velocity += pWalkingCharacter->Acceleration * deltaTime * 0.5f;
        pWalkingCharacter->Velocity = glm::clamp(pWalkingCharacter->Velocity, 0.0f, pWalkingCharacter->MaxVelocity);

        NewPosition = pPose->Position + pPose->Orientation * pWalkingCharacter->Velocity * deltaTime;
        std::cout << pWalkingCharacter->Velocity << std::endl;

        pWalkingCharacter->Velocity += pWalkingCharacter->Acceleration * deltaTime * 0.5f;
        pWalkingCharacter->Velocity = glm::clamp(pWalkingCharacter->Velocity, 0.0f, pWalkingCharacter->MaxVelocity);

        m_pScene->UpdateGameObjectScenePosition(pGameObject, NewPosition);
        break;
    case Components::SMovementTarget::EMovementTargetType::ReachPosition:
        //if (DendyCommon::Math::FastCompareMagnitude())
        break;
    case Components::SMovementTarget::EMovementTargetType::LookAtOrientation:
        
        break;
    
    case Components::SMovementTarget::EMovementTargetType::None:
        if ( pWalkingCharacter->Velocity > 0.0f )
        {
            pWalkingCharacter->Velocity -= pWalkingCharacter->Acceleration * deltaTime * 0.5f;
            pWalkingCharacter->Velocity = glm::clamp(pWalkingCharacter->Velocity, 0.0f, pWalkingCharacter->MaxVelocity);

            NewPosition = pPose->Position + pPose->Orientation * pWalkingCharacter->Velocity * deltaTime;

            pWalkingCharacter->Velocity -= pWalkingCharacter->Acceleration * deltaTime * 0.5f;
            pWalkingCharacter->Velocity = glm::clamp(pWalkingCharacter->Velocity, 0.0f, pWalkingCharacter->MaxVelocity);

            m_pScene->UpdateGameObjectScenePosition(pGameObject, NewPosition);
        }
        break;
    }

    LOG_CALLSTACK_POP();
}