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

    // m_pOwnedGameObjectsMap.clear();

    m_pOwnedInputHandler.release();
    m_pOwnedRenderingSystem.release();

    LOG_CALLSTACK_POP();
}

void DendyEngine::CEngineCore::_InitialiseRendering()
{
    LOG_CALLSTACK_PUSH(__FILE__,__LINE__,__PRETTY_FUNCTION__);

    m_pOwnedRenderingSystem = std::make_unique<PixPhetamine::CRenderingSystem>(m_IsInDebugState);

    LOG_CALLSTACK_POP();
}

void DendyEngine::CEngineCore::_InitialiseInputManager()
{
    LOG_CALLSTACK_PUSH(__FILE__,__LINE__,__PRETTY_FUNCTION__);

    m_pOwnedInputHandler = std::make_unique<PixPhetamine::CInputHandler>(m_pOwnedRenderingSystem->GetGLFWWindow());

    LOG_CALLSTACK_POP();
}

void DendyEngine::CEngineCore::_InitialiseGameObjects()
{
    LOG_CALLSTACK_PUSH(__FILE__,__LINE__,__PRETTY_FUNCTION__);


    // Create MainCamera
    {
        ECS::CGameObject* pGameObject = m_pOwnedECSEngine->AddGameObject("Camera");
        ECS::SCamera* pCamera = m_pOwnedECSEngine->AddComponent<DendyEngine::ECS::SCamera>(pGameObject);
    }



    // Create Cossack
    {
        ECS::CGameObject* pGameObject = m_pOwnedECSEngine->AddGameObject("Kossack001");
        ECS::STransform* pTransform = m_pOwnedECSEngine->AddComponent<DendyEngine::ECS::STransform>(pGameObject);

        float PositionX = 0.0f;
        float PositionY = 0.0f;
        float PositionZ = 0.0f;
        pTransform->TransformMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(PositionX, PositionY, PositionZ));

        ECS::SRenderablePawn* pRenderablePawn = m_pOwnedECSEngine->AddComponent<DendyEngine::ECS::SRenderablePawn>(pGameObject);
        pRenderablePawn->Color = glm::vec3(1.0f, 0.0f, 0.0f);
    }



    {
        ECS::CGameObject* pGameObject = m_pOwnedECSEngine->AddGameObject("Kossack002");
        ECS::STransform* pTransform = m_pOwnedECSEngine->AddComponent<DendyEngine::ECS::STransform>(pGameObject);

        float PositionX = 2.0f;
        float PositionY = 0.0f;
        float PositionZ = 0.0f;
        pTransform->TransformMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(PositionX, PositionY, PositionZ));

        ECS::SRenderablePawn* pRenderablePawn = m_pOwnedECSEngine->AddComponent<DendyEngine::ECS::SRenderablePawn>(pGameObject);
        pRenderablePawn->Color = glm::vec3(1.0f, 1.0f, 0.0f);
    }

    m_pTerrain = new CTerrain();


    LOG_CALLSTACK_POP();
}

void DendyEngine::CEngineCore::_InitialiseGameSystems()
{
    LOG_CALLSTACK_PUSH(__FILE__,__LINE__,__PRETTY_FUNCTION__);

    m_pOwnedECSEngine = std::make_unique<DendyEngine::ECS::CECSEngine>();


    LOG_CALLSTACK_POP();
}

void DendyEngine::CEngineCore::Update(float deltaTime)
{
    LOG_CALLSTACK_PUSH(__FILE__,__LINE__,__PRETTY_FUNCTION__);

    m_pOwnedInputHandler->UpdateInputs();
    if (m_pOwnedInputHandler->GetWindowClosedState())
    {
        m_IsRunning = false;
        LOG_CALLSTACK_POP();
        return;
    }
    

    if (m_pTerrain->GetIsLoadedInGPUState() == false)
        m_pTerrain->LoadToGPU();




    for (auto pGameObject : m_pOwnedECSEngine->GetGameObjectsVecWithComponents<ECS::SCamera>())
    {
        glm::vec2 LeftStickValue = m_pOwnedInputHandler->GetLeftStickValue();
        float ZoomValue = m_pOwnedInputHandler->GetZoomValue();
        if (LeftStickValue.x != 0.0f || LeftStickValue.y != 0.0f || ZoomValue != 0.0f)
        {
            glm::vec3 Move{LeftStickValue.x, ZoomValue, LeftStickValue.y};
            Move = Move * 10.0f * deltaTime;

            ECS::SCamera* pCamera = m_pOwnedECSEngine->GetComponent<ECS::SCamera>(pGameObject);
            pCamera->TargetPosition = pCamera->TargetPosition + Move;

            m_pOwnedRenderingSystem->SetCameraLookAt(pCamera->TargetPosition);
        }
    }





    static float s_PosX = 0.0f;
    static float s_PosZ = 0.0f;
    static bool s_State = false;
    if (s_State)
        if (s_PosX < 8.0f)
        {
            s_PosX += 0.1f * deltaTime;
            s_PosZ += 0.1f * deltaTime;
        }
        else
            s_State = false;
    else
        if (s_PosX > -8.0f)
        {
            s_PosX -= 0.025f * deltaTime;
            s_PosZ -= 0.025f * deltaTime;
        }
        else
            s_State = true;

    for (auto pGameObject : m_pOwnedECSEngine->GetGameObjectsVecWithComponents<ECS::STransform, ECS::SRenderablePawn>())
    {
        //std::cout << *pGameObject << std::endl;
        ECS::STransform* pTransform = m_pOwnedECSEngine->GetComponent<ECS::STransform>(pGameObject);
        ECS::SRenderablePawn* pRenderablePawn = m_pOwnedECSEngine->GetComponent<ECS::SRenderablePawn>(pGameObject);

        glm::mat4 TransformMatrix{1.0f};
        glm::vec4 Position{s_PosX,0.0f,s_PosZ,1.0f};
        Position = pTransform->TransformMatrix * Position;
        float TerrainHeight = m_pTerrain->GetHeightAtPosition(glm::vec3(Position));
        
        TransformMatrix = glm::translate(pTransform->TransformMatrix, glm::vec3(s_PosX, TerrainHeight*1.0f, s_PosZ));

        m_pOwnedRenderingSystem->AddPawnInstance(TransformMatrix, pRenderablePawn->Color);
    }

    // m_pSystemSpatialNavigation->Update();
    // m_pSystemRenderablePawn->Update();

    //m_pOwnedRenderingSystem->Render(m_pTerrain);
    m_pOwnedRenderingSystem->RenderScene(m_pTerrain);
    //m_pOwnedRenderingSystem->Render();

    //glm::vec4 Position4{0,0,0,1};
    //glm::mat4 Matrix = m_pOwnedGameObjectsMap.at(1).get()->GetComponent<CTransformComponent>()->TranformMatrix;
    //Position4 = Matrix * Position4;
    //std::cout << "(" << Position4.x << ", " << Position4.y << ", " << Position4.z << ")" << std::endl;
    //std::cout << "(" << Matrix[3][1] << ", " << Position4.y << ", " << Matrix[3][3] << ")" << std::endl;



    LOG_CALLSTACK_POP();
}