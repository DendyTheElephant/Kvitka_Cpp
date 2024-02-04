#include <DendyEngine/VisionSystem.h>
#include <DendyCommon/Logger.h>
//#include <DendyCommon/Math.h>
#include <DendyEngine/Definitions.h>


void DendyEngine::CVisionSystem::UpdateVisibleGameObjectsVec(CGameObject* pGameObjectSeeing) const
{
    LOG_CALLSTACK_PUSH(__FILE__,__LINE__,__PRETTY_FUNCTION__);

    Components::SVision* pVision = pGameObjectSeeing->GetComponent<Components::SVision>();
    Components::SScenePose* pPose = pGameObjectSeeing->GetScenePose();

    pVision->VisibleGameObjectsVec.clear();

    for (auto pOtherGameObject : m_pScene->GetGameObjectsSetNearScenePositionWithComponents<Components::SVisibility>(pGameObjectSeeing->GetScenePosition()))
    {
        // Not seeing self
        if (pOtherGameObject == pGameObjectSeeing)
            continue;

        Components::SVisibility* pOthersVisibility = pOtherGameObject->GetComponent<Components::SVisibility>();
        Components::SScenePose* pOthersPose = pOtherGameObject->GetScenePose();

        glm::vec2 RelativeToTarget = pOthersPose->Position - pPose->Position;

        if (DendyCommon::Math::FastCompareDistance(pPose->Position, pOthersPose->Position, pVision->Radius+pOthersVisibility->Radius) < 0)
        {
            //std::cout << glm::dot(pPose->Orientation, RelativeToTarget) << std::endl;
            if (glm::dot(pPose->Orientation, RelativeToTarget) > 0)
            {
                bool Obstructed = false;
                for (auto pColliderGameObject : m_pScene->GetGameObjectsSetNearScenePositionWithComponents<Components::SCollider>(pGameObjectSeeing->GetScenePosition()))
                {
                    if ( DendyCommon::Math::IsCollisionEdgeWithConvexShape( pPose->Position, pOthersPose->Position, pColliderGameObject->GetComponent<Components::SCollider>()->PositionsVec ) )
                    {
                        Obstructed = true;
                        break;
                    }
                }

                if (Obstructed == false)
                    pVision->VisibleGameObjectsVec.push_back(pOtherGameObject);
            }
        }
    }

    LOG_CALLSTACK_POP();
}
