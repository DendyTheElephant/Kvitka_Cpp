#include "DendyEngine/EngineCore.h"
#include "DendyCommon/Logger.h"

#include "DendyEngine/Actor.h"

#include <iostream>

DendyEngine::CEngineCore::CEngineCore(bool isInDebugState):
m_IsInDebugState(isInDebugState),
m_IsRunning(true)
{
    LOG_CALLSTACK_PUSH(__FILE__,__LINE__,__PRETTY_FUNCTION__);

    m_pGameObjectsMapById[IGameObject::GetGameObjectIdIncrement()] = new CActor("Cossack01");
    m_pGameObjectsMapById[IGameObject::GetGameObjectIdIncrement()] = new CActor("Cossack02");

    _InitialiseRendering();
    _InitialiseInputManager();

    LOG_CALLSTACK_POP();
}

DendyEngine::CEngineCore::~CEngineCore()
{
    LOG_CALLSTACK_PUSH(__FILE__,__LINE__,__PRETTY_FUNCTION__);

    delete m_pRenderingEngineInstance;

    LOG_CALLSTACK_POP();
}

void DendyEngine::CEngineCore::_InitialiseRendering()
{
    LOG_CALLSTACK_PUSH(__FILE__,__LINE__,__PRETTY_FUNCTION__);

    m_pRenderingEngineInstance = new PixPhetamine::CRenderingCore(m_IsInDebugState);

    LOG_CALLSTACK_POP();
}

void DendyEngine::CEngineCore::_InitialiseInputManager()
{
    LOG_CALLSTACK_PUSH(__FILE__,__LINE__,__PRETTY_FUNCTION__);

    m_pInputHandler = new PixPhetamine::CInputHandler(m_pRenderingEngineInstance->GetGLFWWindow());

    LOG_CALLSTACK_POP();
}

void DendyEngine::CEngineCore::Update()
{
    LOG_CALLSTACK_PUSH(__FILE__,__LINE__,__PRETTY_FUNCTION__);

    m_pInputHandler->UpdateInputs();
    if (m_pInputHandler->GetWindowClosedState())
    {
        m_IsRunning = false;
        LOG_CALLSTACK_POP();
        return;
    }
    
    for (auto& [GameObjectId, pGameObject]: m_pGameObjectsMapById)
    {
        pGameObject->Update();
    }

    for (auto& [GameObjectId, pGameObject]: m_pGameObjectsMapById)
    {
        pGameObject->GetComponent("Mesh");
        
    }

    m_pRenderingEngineInstance->Render();


    LOG_CALLSTACK_POP();
}