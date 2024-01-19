#include <DendyEngine/GameSystems.h>
#include <DendyEngine/GameComponents.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

template<>
void DendyEngine::CGameSystem<DendyEngine::CSpatialNavigationComponent>::Update()
{
    LOG_CALLSTACK_PUSH(__FILE__,__LINE__,__PRETTY_FUNCTION__);

    for (auto& pSpatialNavigationComponent : m_ComponentsSet)
    {
        CGameObject* pGameObject = pSpatialNavigationComponent->GetOwner();
        if (pGameObject->HasComponent<CTransformComponent>())
        {
            CTransformComponent* pTransformComponent = pGameObject->GetComponent<CTransformComponent>();
            glm::vec3 Step3D = glm::vec3(pSpatialNavigationComponent->Step.x, 0.0f, pSpatialNavigationComponent->Step.y);
            pTransformComponent->TranformMatrix = glm::translate(pTransformComponent->TranformMatrix, Step3D);
        }
    }

    LOG_CALLSTACK_POP();
}


// template<>
// DendyEngine::CGameSystem<DendyEngine::CRenderablePawn>::CGameSystem()
// {

// }


void DendyEngine::CRenderablePawn::Update()
{
    LOG_CALLSTACK_PUSH(__FILE__,__LINE__,__PRETTY_FUNCTION__);

    for (auto& pComponent : m_ComponentsSet)
    {
        CGameObject* pGameObject = pComponent->GetOwner();
        if (pGameObject->HasComponent<CTransformComponent>())
        {
            //CTransformComponent* pTransformComponent = pGameObject->GetComponent<CTransformComponent>();
            //glm::vec3 Step3D = glm::vec3(pSpatialNavigationComponent->Step.x, 0.0f, pSpatialNavigationComponent->Step.y);
            //pTransformComponent->TranformMatrix = glm::translate(pTransformComponent->TranformMatrix, Step3D);
        }
    }

    LOG_CALLSTACK_POP();
}