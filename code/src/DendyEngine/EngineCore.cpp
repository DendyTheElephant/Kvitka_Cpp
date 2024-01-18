#include "DendyEngine/EngineCore.h"
#include "DendyCommon/Logger.h"

#include <iostream>

DendyEngine::CEngineCore::CEngineCore(bool isInDebugState):
m_IsInDebugState(isInDebugState),
m_IsRunning(true)
{
    LOG_CALLSTACK_PUSH(__FILE__,__LINE__,__PRETTY_FUNCTION__);

    //std::unique_ptr<CGameObject> pGameObject;
    std::unique_ptr<CEngineCore::CGameObject> pGameObject = std::make_unique<CGameObject>("Cossack01"); 
    
    m_pGameObjectsOwnerMapBySerial.insert(std::make_pair(pGameObject->GetSerial(), std::move(pGameObject)));

    //m_pGameObjectsOwnerMapBySerial.insert( {pGameObject->GetSerial(), std::move(pGameObject)} );
    //m_pGameObjectsMapById[] = new CGameObject("Cossack01");
    //m_pGameObjectsMapById[2] = new CGameObject("Cossack02");


    //m_pTerrain = std::make_unique<CTerrain>(1.0f);

    m_pTerrain = new CTerrain(4.0f);
    
    // std::cout << pTerrain->GetHeightAtPosition(glm::vec2(0.0f,0.0f)) << std::endl;
    // std::cout << pTerrain->GetHeightAtPosition(glm::vec2(0.5f,0.5f)) << std::endl;
    // std::cout << pTerrain->GetHeightAtPosition(glm::vec2(0.9f,0.8f)) << std::endl;

    _InitialiseRendering();
    _InitialiseInputManager();

    m_pTerrain->LoadToGPU();

    LOG_CALLSTACK_POP();
}

DendyEngine::CEngineCore::~CEngineCore()
{
    LOG_CALLSTACK_PUSH(__FILE__,__LINE__,__PRETTY_FUNCTION__);

    m_pInputHandler.release();
    m_pRenderingEngineInstance.release();

    LOG_CALLSTACK_POP();
}

void DendyEngine::CEngineCore::_InitialiseRendering()
{
    LOG_CALLSTACK_PUSH(__FILE__,__LINE__,__PRETTY_FUNCTION__);

    m_pRenderingEngineInstance = std::make_unique<PixPhetamine::CRenderingCore>(m_IsInDebugState);

    LOG_CALLSTACK_POP();
}

void DendyEngine::CEngineCore::_InitialiseInputManager()
{
    LOG_CALLSTACK_PUSH(__FILE__,__LINE__,__PRETTY_FUNCTION__);

    m_pInputHandler = std::make_unique<PixPhetamine::CInputHandler>(m_pRenderingEngineInstance->GetGLFWWindow());

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


    m_pRenderingEngineInstance->Render(m_pTerrain);



    LOG_CALLSTACK_POP();
}