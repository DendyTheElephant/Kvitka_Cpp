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
    _InitialiseTerrain();
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

void DendyEngine::CEngineCore::_InitialiseTerrain()
{
    LOG_CALLSTACK_PUSH(__FILE__,__LINE__,__PRETTY_FUNCTION__);

    m_pOwnedTerrain = std::make_unique<DendyEngine::CTerrain>();
    m_pOwnedRenderingSystem->InitialiseTerrain(m_pOwnedTerrain->GetTerrainSize(), m_pOwnedTerrain->GetScale(), m_pOwnedTerrain->GetMaxHeight(), m_pOwnedTerrain->GetData());

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
        ECS::SWalkingCharacter* pWalkingCharacter = m_pOwnedECSEngine->AddComponent<DendyEngine::ECS::SWalkingCharacter>(pGameObject);

        pWalkingCharacter->Position = glm::vec3(0.0f, 0.0f, 0.0f);

        ECS::SRenderablePawn* pRenderablePawn = m_pOwnedECSEngine->AddComponent<DendyEngine::ECS::SRenderablePawn>(pGameObject);
        pRenderablePawn->Color = glm::vec3(1.0f, 0.0f, 0.0f);
    }



    {
        ECS::CGameObject* pGameObject = m_pOwnedECSEngine->AddGameObject("Kossack002");
        ECS::SWalkingCharacter* pWalkingCharacter = m_pOwnedECSEngine->AddComponent<DendyEngine::ECS::SWalkingCharacter>(pGameObject);

        pWalkingCharacter->Position = glm::vec3(2.0f, 0.0f, 0.0f);

        ECS::SRenderablePawn* pRenderablePawn = m_pOwnedECSEngine->AddComponent<DendyEngine::ECS::SRenderablePawn>(pGameObject);
        pRenderablePawn->Color = glm::vec3(1.0f, 1.0f, 0.0f);
    }



    // Create static meshes
    {
        ECS::CGameObject* pGameObject = m_pOwnedECSEngine->AddGameObject("Hata001");
        ECS::SPose* pPose = m_pOwnedECSEngine->AddComponent<DendyEngine::ECS::SPose>(pGameObject);
        ECS::SStaticMesh* pStaticMesh = m_pOwnedECSEngine->AddComponent<DendyEngine::ECS::SStaticMesh>(pGameObject);

        pStaticMesh->MeshName = "hata";
        pStaticMesh->Color = {1.0f, 1.0f, 1.0f};
        pPose->Position = {8.0f, 0.0f, -20.0f};
    }
    {
        ECS::CGameObject* pGameObject = m_pOwnedECSEngine->AddGameObject("Sich001");
        ECS::SPose* pPose = m_pOwnedECSEngine->AddComponent<DendyEngine::ECS::SPose>(pGameObject);
        ECS::SStaticMesh* pStaticMesh = m_pOwnedECSEngine->AddComponent<DendyEngine::ECS::SStaticMesh>(pGameObject);

        pStaticMesh->MeshName = "sich";
        pStaticMesh->Color = {0.35f, 0.25f, 0.2f};
        pPose->Position = {-8.0f, 0.0f, -20.0f};
    }

    


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
    if (m_pOwnedInputHandler->GetWindowClosedState() || m_pOwnedInputHandler->GetKeyEscapeReleased())
    {
        m_IsRunning = false;
        LOG_CALLSTACK_POP();
        return;
    }

    // Shaders / Terrain reloading
    if (m_pOwnedInputHandler->GetKeyRReleased())
    {
        m_pOwnedTerrain->LoadFromFiles("ressources/images/ruggedTerrainHeightmap.png");
        m_pOwnedRenderingSystem->InitialiseTerrain(m_pOwnedTerrain->GetTerrainSize(), m_pOwnedTerrain->GetScale(), m_pOwnedTerrain->GetMaxHeight(), m_pOwnedTerrain->GetData());
        m_pOwnedRenderingSystem->ReloadShaders();
    }


    glm::vec3 CameraTargetPosition;

    // Camera movements with inputs
    for (auto pGameObject : m_pOwnedECSEngine->GetGameObjectsVecWithComponents<ECS::SCamera>())
    {
        ECS::SCamera* pCamera = m_pOwnedECSEngine->GetComponent<ECS::SCamera>(pGameObject);

        glm::vec2 LeftStickValue = m_pOwnedInputHandler->GetLeftStickValue();
        float ZoomValue = m_pOwnedInputHandler->GetZoomValue();

        glm::mat4 RotateMatrix{1};
        if (LeftStickValue.x != 0.0f || LeftStickValue.y != 0.0f || ZoomValue != 0.0f)
        {
            glm::vec3 Move{LeftStickValue.x, 0.0f, LeftStickValue.y};

            pCamera->ArmTranslationMagnitude += ZoomValue * pCamera->Speed * deltaTime;
            pCamera->ArmTranslationMagnitude = glm::clamp(pCamera->ArmTranslationMagnitude, pCamera->ArmTranslationMagnitudeMinValue, pCamera->ArmTranslationMagnitudeMaxValue);
            //std::cout << "ArmMag: " << std::to_string(pCamera->ArmTranslationMagnitude) << std::endl;
            pCamera->TargetPosition += Move * pCamera->Speed * deltaTime;


            Move = glm::normalize(Move);
            float AngleBetweenTargetAndCurrentOrientation = atan2f( -Move.x, -Move.z );
            glm::quat OrientationQuaternion{ cosf( AngleBetweenTargetAndCurrentOrientation/2.0f ), 0, sinf( AngleBetweenTargetAndCurrentOrientation/2.0f ), 0 };
            RotateMatrix = glm::mat4_cast(OrientationQuaternion);
        }


        CameraTargetPosition = pCamera->TargetPosition;
        m_pOwnedRenderingSystem->SetCameraLookAt(pCamera->TargetPosition);
        glm::vec3 ArmTranslation = glm::normalize(pCamera->ArmTranslationDirection);
        
        m_pOwnedRenderingSystem->SetCameraArmTranslation(ArmTranslation*pCamera->ArmTranslationMagnitude);

        // Debug
        float TerrainHeight = m_pOwnedTerrain->GetHeightAtPosition(CameraTargetPosition);
        glm::mat4 TranslateMatrix = glm::translate(glm::mat4{1}, {CameraTargetPosition.x, TerrainHeight, CameraTargetPosition.z});


        glm::mat4 TransformMatrix = TranslateMatrix * RotateMatrix;// * ScaleMatrix;
        m_pOwnedRenderingSystem->AddPawnInstance(TransformMatrix, {0.0f, 0.5f, 1.0f});
    }


    // Pawns
    for (auto pGameObject : m_pOwnedECSEngine->GetGameObjectsVecWithComponents<ECS::SWalkingCharacter, ECS::SRenderablePawn>())
    {
        ECS::SWalkingCharacter* pWalkingCharacter = m_pOwnedECSEngine->GetComponent<ECS::SWalkingCharacter>(pGameObject);
        ECS::SRenderablePawn* pRenderablePawn = m_pOwnedECSEngine->GetComponent<ECS::SRenderablePawn>(pGameObject);

        //Target 
        //pWalkingCharacter->TargetPosition = 
        glm::vec2 GridTarget{CameraTargetPosition.x, CameraTargetPosition.z};
        glm::vec2 GridPosition{pWalkingCharacter->Position.x, pWalkingCharacter->Position.z};

        glm::vec2 RelativeToTarget = GridTarget - GridPosition;


        if (glm::length(RelativeToTarget) > 0.001)
        {
            pWalkingCharacter->TargetDirection = glm::normalize(glm::vec3{RelativeToTarget.x,0,RelativeToTarget.y});
            pWalkingCharacter->Direction = pWalkingCharacter->TargetDirection;
        }
        if (glm::length(RelativeToTarget) > 1.0)
        {
            pWalkingCharacter->Position.x += pWalkingCharacter->Direction.x * pWalkingCharacter->MaxVelocity * deltaTime;
            pWalkingCharacter->Position.z += pWalkingCharacter->Direction.z * pWalkingCharacter->MaxVelocity * deltaTime;
            float TerrainHeight = m_pOwnedTerrain->GetHeightAtPosition(pWalkingCharacter->Position);
            pWalkingCharacter->Position.y = TerrainHeight;
        }

        glm::mat4 TranslateMatrix{1};
        glm::mat4 RotateMatrix{1};


        float AngleBetweenTargetAndCurrentOrientation = atan2f( -pWalkingCharacter->Direction.x, -pWalkingCharacter->Direction.z );
        glm::quat OrientationQuaternion{ cosf( AngleBetweenTargetAndCurrentOrientation/2.0f ), 0, sinf( AngleBetweenTargetAndCurrentOrientation/2.0f ), 0 };
        RotateMatrix = glm::mat4_cast(OrientationQuaternion);
        TranslateMatrix = glm::translate(glm::mat4(1.0), pWalkingCharacter->Position);

        glm::mat4 TransformMatrix = TranslateMatrix * RotateMatrix;// * ScaleMatrix;
        m_pOwnedRenderingSystem->AddPawnInstance(TransformMatrix, pRenderablePawn->Color);
    }

    // Static Meshes
    for (auto pGameObject : m_pOwnedECSEngine->GetGameObjectsVecWithComponents<ECS::SPose, ECS::SStaticMesh>())
    {
        ECS::SStaticMesh* pMesh = m_pOwnedECSEngine->GetComponent<ECS::SStaticMesh>(pGameObject);
        ECS::SPose* pPose = m_pOwnedECSEngine->GetComponent<ECS::SPose>(pGameObject);
        
        // Fix Y
        float TerrainHeight = m_pOwnedTerrain->GetHeightAtPosition(pPose->Position);
        pPose->Position.y = TerrainHeight;

        // Compute matrix and add static mesh instance to renderer
        glm::mat4 TransformMatrix{1.0f};
        TransformMatrix = glm::translate(TransformMatrix, pPose->Position);
        m_pOwnedRenderingSystem->AddStaticMesh(pMesh->MeshName, TransformMatrix, pMesh->Color);
    }

    m_pOwnedRenderingSystem->RenderScene();

    LOG_CALLSTACK_POP();
}