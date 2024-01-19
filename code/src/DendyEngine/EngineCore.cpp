#include <DendyEngine/EngineCore.h>
#include <DendyCommon/Logger.h>

#include <iostream>

DendyEngine::CEngineCore::CEngineCore(bool isInDebugState):
m_IsInDebugState(isInDebugState),
m_IsRunning(true)
{
    LOG_CALLSTACK_PUSH(__FILE__,__LINE__,__PRETTY_FUNCTION__);

    _InitialiseRendering();
    _InitialiseInputManager();
    _InitialiseGameSystems();
    _InitialiseGameObjects();

    LOG_CALLSTACK_POP();
}

DendyEngine::CEngineCore::~CEngineCore()
{
    LOG_CALLSTACK_PUSH(__FILE__,__LINE__,__PRETTY_FUNCTION__);

    m_pOwnedGameObjectsMap.clear();

    m_pInputHandler.release();
    m_pRenderingSystem.release();

    LOG_CALLSTACK_POP();
}

void DendyEngine::CEngineCore::_InitialiseRendering()
{
    LOG_CALLSTACK_PUSH(__FILE__,__LINE__,__PRETTY_FUNCTION__);

    m_pRenderingSystem = std::make_unique<PixPhetamine::CRenderingSystem>(m_IsInDebugState);

    LOG_CALLSTACK_POP();
}

void DendyEngine::CEngineCore::_InitialiseInputManager()
{
    LOG_CALLSTACK_PUSH(__FILE__,__LINE__,__PRETTY_FUNCTION__);

    m_pInputHandler = std::make_unique<PixPhetamine::CInputHandler>(m_pRenderingSystem->GetGLFWWindow());

    LOG_CALLSTACK_POP();
}

void DendyEngine::CEngineCore::_InitialiseGameObjects()
{
    LOG_CALLSTACK_PUSH(__FILE__,__LINE__,__PRETTY_FUNCTION__);

    // Create Cossack
    {
        std::unique_ptr<DendyEngine::CGameObject> pGameObject = std::make_unique<CGameObject>("Cossack01");

        // Create components
        { // SpatialNavigation
            CSpatialNavigationComponent* pComponent = m_pSystemSpatialNavigation->CreateComponent(pGameObject.get());
            pComponent->Step.x = 0.001f;
        }
        { // Renderable Pawn
            CRenderablePawnComponent* pComponent = m_pSystemRenderablePawn->CreateComponent(pGameObject.get());
            pComponent->Height = 2.0f;
        }        

        // Insert and hold in Map    
        size_t GameObjectHandle = pGameObject->GetUID();
        m_pOwnedGameObjectsMap.insert( { GameObjectHandle, std::move(pGameObject) } );
    }
    

    m_pTerrain = new CTerrain(4.0f);


    LOG_CALLSTACK_POP();
}

void DendyEngine::CEngineCore::_InitialiseGameSystems()
{
    LOG_CALLSTACK_PUSH(__FILE__,__LINE__,__PRETTY_FUNCTION__);

    m_pRenderingSystem = std::make_unique<PixPhetamine::CRenderingSystem>(m_IsInDebugState);

    m_pSystemSpatialNavigation = std::make_unique<CGameSystem<CSpatialNavigationComponent>>();
    m_pSystemRenderablePawn = std::make_unique<CRenderablePawn>(m_pRenderingSystem.get());

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
    

    if (m_pTerrain->GetIsLoadedInGPUState() == false)
        m_pTerrain->LoadToGPU();

    m_pSystemSpatialNavigation->Update();
    m_pSystemRenderablePawn->Update();

    //m_pRenderingSystem->Render(m_pTerrain);

    //glm::vec4 Position4{0,0,0,1};
    //glm::mat4 Matrix = m_pOwnedGameObjectsMap.at(1).get()->GetComponent<CTransformComponent>()->TranformMatrix;
    //Position4 = Matrix * Position4;
    //std::cout << "(" << Position4.x << ", " << Position4.y << ", " << Position4.z << ")" << std::endl;
    //std::cout << "(" << Matrix[3][1] << ", " << Position4.y << ", " << Matrix[3][3] << ")" << std::endl;



    LOG_CALLSTACK_POP();
}