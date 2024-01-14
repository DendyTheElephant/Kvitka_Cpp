#include "DendyEngine/EngineCore.h"
#include "DendyCommon/Logger.h"

#include "DendyEngine/Actor.h"

#include <iostream>

DendyEngine::CEngineCore::CEngineCore(bool isInDebugState):
m_IsInDebugState(isInDebugState)
{
    LOG_CALLSTACK_PUSH(__FILE__,__LINE__,__PRETTY_FUNCTION__);

    m_pGameObjectsMapById[IGameObject::GetGameObjectIdIncrement()] = new CActor("Cossack01");
    m_pGameObjectsMapById[IGameObject::GetGameObjectIdIncrement()] = new CActor("Cossack02");

    LOG_CALLSTACK_POP();
}

DendyEngine::CEngineCore::~CEngineCore()
{

}

void DendyEngine::CEngineCore::SetRenderingCoreInstance(PixPhetamine::CRenderingCore* pRenderingCoreInstance)
{
    m_pRenderingEngineInstance = pRenderingCoreInstance;
}

void DendyEngine::CEngineCore::Update()
{
    LOG_CALLSTACK_PUSH(__FILE__,__LINE__,__PRETTY_FUNCTION__);

    for (auto& [GameObjectId, pGameObject]: m_pGameObjectsMapById)
    {
        pGameObject->Update();
    }

    for (auto& [GameObjectId, pGameObject]: m_pGameObjectsMapById)
    {
        pGameObject->GetComponent("Mesh");
        
    }


    LOG_CALLSTACK_POP();
}