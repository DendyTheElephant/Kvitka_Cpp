#include <DendyEngine/MovementSystem.h>
#include <DendyCommon/Logger.h>


 void DendyEngine::CMovementSystem::MoveToOrientationWithVelocity(CGameObject* pGameObject, float const& deltaTime, bool isDeccelerating) const
 {
    Components::SWalkingCharacter* pWalkingCharacter = pGameObject->GetComponent<Components::SWalkingCharacter>();

    if (isDeccelerating)
        pWalkingCharacter->Velocity -= pWalkingCharacter->Acceleration * deltaTime * 0.5f;
    else
        pWalkingCharacter->Velocity += pWalkingCharacter->Acceleration * deltaTime * 0.5f;
    pWalkingCharacter->Velocity = glm::clamp(pWalkingCharacter->Velocity, 0.0f, pWalkingCharacter->MaxVelocity);

    glm::vec2 NewPosition = pGameObject->GetScenePose()->Position + pGameObject->GetScenePose()->Orientation * pWalkingCharacter->Velocity * deltaTime;

    if (isDeccelerating)
        pWalkingCharacter->Velocity -= pWalkingCharacter->Acceleration * deltaTime * 0.5f;
    else
        pWalkingCharacter->Velocity += pWalkingCharacter->Acceleration * deltaTime * 0.5f;
    pWalkingCharacter->Velocity = glm::clamp(pWalkingCharacter->Velocity, 0.0f, pWalkingCharacter->MaxVelocity);

    m_pScene->UpdateGameObjectScenePosition(pGameObject, NewPosition);
 }

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
    {
        pPose->Orientation = pMovementTarget->TargetDirection;

        bool IsDeccelerating{false};
        MoveToOrientationWithVelocity(pGameObject, deltaTime, IsDeccelerating);
        break;
    }
    case Components::SMovementTarget::EMovementTargetType::ReachPosition:
    {
        glm::vec2 RelativeVector = pMovementTarget->TargetPosition - pPose->Position;
        bool IsDeccelerating{false};
        if (DendyCommon::Math::FastCompareMagnitude(RelativeVector, pMovementTarget->ArrivalAtPositionEpsilon) > 0)
        {
            pPose->Orientation = glm::normalize(RelativeVector);
            // Deccel ?
            float DistanceToStop = pWalkingCharacter->Velocity*pWalkingCharacter->Velocity / (2.0f*pWalkingCharacter->Acceleration) - pWalkingCharacter->Velocity/2.0f;
            if (DendyCommon::Math::FastCompareMagnitude(RelativeVector, DistanceToStop) <= 0)
            {
                IsDeccelerating = true;
                MoveToOrientationWithVelocity(pGameObject, deltaTime, IsDeccelerating);
            }
            else
            {
                MoveToOrientationWithVelocity(pGameObject, deltaTime, IsDeccelerating);
            }
        }
        break;
    }
    case Components::SMovementTarget::EMovementTargetType::LookAtOrientation:
    {
        break;
    }
    case Components::SMovementTarget::EMovementTargetType::None:
    {
        if ( pWalkingCharacter->Velocity > 0.0f )
        {
            bool IsDeccelerating{true};
            MoveToOrientationWithVelocity(pGameObject, deltaTime, IsDeccelerating);
        }
        break;
    }
    }

    LOG_CALLSTACK_POP();
}