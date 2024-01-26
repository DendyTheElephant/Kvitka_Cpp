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

void DendyEngine::CEngineCore::_InitialiseGameSystems()
{
    LOG_CALLSTACK_PUSH(__FILE__,__LINE__,__PRETTY_FUNCTION__);

    m_pOwnedECSEngine = std::make_unique<DendyEngine::ECS::CECSEngine>();

    LOG_CALLSTACK_POP();
}


// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// -=-=-=- CREATE OBJECTS
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
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
        ECS::SScenePose* pPose = m_pOwnedECSEngine->AddComponent<DendyEngine::ECS::SScenePose>(pGameObject);
        ECS::SVision* pVision = m_pOwnedECSEngine->AddComponent<DendyEngine::ECS::SVision>(pGameObject);
        ECS::SRenderablePawn* pRenderablePawn = m_pOwnedECSEngine->AddComponent<DendyEngine::ECS::SRenderablePawn>(pGameObject);

        ECS::STransform* pTransform = m_pOwnedECSEngine->AddComponent<DendyEngine::ECS::STransform>(pGameObject);
        ECS::SWalkingCharacter* pWalkingCharacter = m_pOwnedECSEngine->AddComponent<DendyEngine::ECS::SWalkingCharacter>(pGameObject);
        

        pPose->Position = {-2,-2};
        pPose->Orientation = {0.0f, 1.0f};
        pRenderablePawn->Color = glm::vec3(1.0f, 1.0f, 0.0f);
        pVision->Radius = 5.0f;
    }
    {
        ECS::CGameObject* pGameObject = m_pOwnedECSEngine->AddGameObject("Kossack002");
        ECS::SScenePose* pPose = m_pOwnedECSEngine->AddComponent<DendyEngine::ECS::SScenePose>(pGameObject);
        ECS::SRenderablePawn* pRenderablePawn = m_pOwnedECSEngine->AddComponent<DendyEngine::ECS::SRenderablePawn>(pGameObject);

        ECS::STransform* pTransform = m_pOwnedECSEngine->AddComponent<DendyEngine::ECS::STransform>(pGameObject);
        ECS::SWalkingCharacter* pWalkingCharacter = m_pOwnedECSEngine->AddComponent<DendyEngine::ECS::SWalkingCharacter>(pGameObject);
        

        pPose->Position = {2,2};
        pPose->Orientation = {0.0f, 1.0f};
        pRenderablePawn->Color = glm::vec3(0.0f, 1.0f, 0.0f);
    }



    // Create static meshes
    {
        ECS::CGameObject* pGameObject = m_pOwnedECSEngine->AddGameObject("Hata001");
        ECS::SScenePose* pPose = m_pOwnedECSEngine->AddComponent<DendyEngine::ECS::SScenePose>(pGameObject);
        ECS::SVisibility* pVisibility = m_pOwnedECSEngine->AddComponent<DendyEngine::ECS::SVisibility>(pGameObject);
        ECS::SStaticMesh* pStaticMesh = m_pOwnedECSEngine->AddComponent<DendyEngine::ECS::SStaticMesh>(pGameObject);

        ECS::STransform* pTransform = m_pOwnedECSEngine->AddComponent<DendyEngine::ECS::STransform>(pGameObject);

        pPose->Position = {8,-20};
        pPose->Orientation = {0.0f, 1.0f};
        pStaticMesh->MeshName = "hata";
        pStaticMesh->Color = {1.0f, 1.0f, 1.0f};
        pVisibility->Radius = 6.0f;
    }
    {
        ECS::CGameObject* pGameObject = m_pOwnedECSEngine->AddGameObject("Sich001");
        ECS::SScenePose* pPose = m_pOwnedECSEngine->AddComponent<DendyEngine::ECS::SScenePose>(pGameObject);
        ECS::SStaticMesh* pStaticMesh = m_pOwnedECSEngine->AddComponent<DendyEngine::ECS::SStaticMesh>(pGameObject);

        ECS::STransform* pTransform = m_pOwnedECSEngine->AddComponent<DendyEngine::ECS::STransform>(pGameObject);

        pPose->Position = {-8,-20};
        pPose->Orientation = {0.0f, 1.0f};
        pStaticMesh->MeshName = "sich";
        pStaticMesh->Color = {0.35f, 0.25f, 0.2f};
    }

    LOG_CALLSTACK_POP();
}


// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// -=-=-=- UPDATE
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
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


    // Update Vision
    for (auto pGameObject : m_pOwnedECSEngine->GetGameObjectsVecWithComponents<ECS::SVision,ECS::SScenePose>())
    {
        ECS::SVision* pVision = m_pOwnedECSEngine->GetComponent<ECS::SVision>(pGameObject);
        ECS::SScenePose* pPose = m_pOwnedECSEngine->GetComponent<ECS::SScenePose>(pGameObject);

        pVision->VisibleGameObjectsVec.clear();

        for (auto pOtherGameObject : m_pOwnedECSEngine->GetGameObjectsVecWithComponents<ECS::SVisibility,ECS::SScenePose>())
        {
            ECS::SVisibility* pOthersVisibility = m_pOwnedECSEngine->GetComponent<ECS::SVisibility>(pOtherGameObject);
            ECS::SScenePose* pOthersPose = m_pOwnedECSEngine->GetComponent<ECS::SScenePose>(pOtherGameObject);

            glm::vec2 RelativeToTarget = pOthersPose->Position - pPose->Position;
            if (glm::dot(pPose->Orientation, RelativeToTarget) > -pOthersVisibility->Radius)
            {
                float Distance = glm::length(RelativeToTarget);
                if (Distance <= pVision->Radius + pOthersVisibility->Radius)
                {
                    pVision->VisibleGameObjectsVec.push_back(pOtherGameObject);
                    std::cout << *pOtherGameObject << std::endl;
                }
            }
        }
    }


    // Update Pose
    for (auto pGameObject : m_pOwnedECSEngine->GetGameObjectsVecWithComponents<ECS::SWalkingCharacter, ECS::SScenePose>())
    {
        ECS::SWalkingCharacter* pWalkingCharacter = m_pOwnedECSEngine->GetComponent<ECS::SWalkingCharacter>(pGameObject);
        ECS::SScenePose* pPose = m_pOwnedECSEngine->GetComponent<ECS::SScenePose>(pGameObject);

        glm::vec2 Target{CameraTargetPosition.x, CameraTargetPosition.z};

        glm::vec2 RelativeToTarget = Target - pPose->Position;
        float DistanceToTarget = glm::length(RelativeToTarget);


        pPose->Orientation = RelativeToTarget;

        if (DistanceToTarget > 1.0)
        {
            pPose->Position += pPose->Orientation * pWalkingCharacter->MaxVelocity * deltaTime;
        }
    }


    // Compute Transform matrix
    for (auto pGameObject : m_pOwnedECSEngine->GetGameObjectsVecWithComponents<ECS::SScenePose, ECS::STransform>())
    {
        ECS::SScenePose* pPose = m_pOwnedECSEngine->GetComponent<ECS::SScenePose>(pGameObject);
        ECS::STransform* pTransform = m_pOwnedECSEngine->GetComponent<ECS::STransform>(pGameObject);
        
        glm::vec3 WorldPosition = {pPose->Position.x, 0, pPose->Position.y};

        WorldPosition.y = m_pOwnedTerrain->GetHeightAtPosition(pPose->Position);

        float OrientationAngle = atan2f( -pPose->Orientation.x, -pPose->Orientation.y );
        glm::quat OrientationQuaternion{ cosf( OrientationAngle/2.0f ), 0, sinf( OrientationAngle/2.0f ), 0 };

        glm::mat4 RotateMatrix = glm::mat4_cast(OrientationQuaternion);
        glm::mat4 TranslateMatrix = glm::translate(glm::mat4{1}, WorldPosition);
        glm::mat4 ScaleMatrix{1};

        pTransform->TransformMatrix = TranslateMatrix * RotateMatrix * ScaleMatrix;
    }
    

    // Pawn rendering
    for (auto pGameObject : m_pOwnedECSEngine->GetGameObjectsVecWithComponents<ECS::STransform, ECS::SRenderablePawn>())
    {
        ECS::STransform* pTransform = m_pOwnedECSEngine->GetComponent<ECS::STransform>(pGameObject);
        ECS::SRenderablePawn* pRenderablePawn = m_pOwnedECSEngine->GetComponent<ECS::SRenderablePawn>(pGameObject);

        m_pOwnedRenderingSystem->AddPawnInstance(pTransform->TransformMatrix, pRenderablePawn->Color);
    }

    // Static Meshes rendering
    for (auto pGameObject : m_pOwnedECSEngine->GetGameObjectsVecWithComponents<ECS::STransform, ECS::SStaticMesh>())
    {
        ECS::SStaticMesh* pMesh = m_pOwnedECSEngine->GetComponent<ECS::SStaticMesh>(pGameObject);
        ECS::STransform* pTransform = m_pOwnedECSEngine->GetComponent<ECS::STransform>(pGameObject);

        m_pOwnedRenderingSystem->AddStaticMesh(pMesh->MeshName, pTransform->TransformMatrix, pMesh->Color);
    }






    m_pOwnedRenderingSystem->RenderScene();

    LOG_CALLSTACK_POP();
}