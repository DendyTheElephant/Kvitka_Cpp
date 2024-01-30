#include <DendyEngine/EngineCore.h>
#include <DendyCommon/Logger.h>
#include <DendyCommon/Math.h>

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

    m_pOwnedScene = std::make_unique<DendyEngine::CScene>();
    m_pOwnedTerrain = std::make_unique<DendyEngine::CTerrain>();
    m_pOwnedRenderingSystem->InitialiseTerrain(m_pOwnedTerrain->GetTerrainSize(), m_pOwnedTerrain->GetScale(), m_pOwnedTerrain->GetMaxHeight(), m_pOwnedTerrain->GetData());

    LOG_CALLSTACK_POP();
}

void DendyEngine::CEngineCore::_InitialiseGameSystems()
{
    LOG_CALLSTACK_PUSH(__FILE__,__LINE__,__PRETTY_FUNCTION__);

    //m_pOwnedComponentsEngine = std::make_unique<DendyEngine::Components::CComponentsEngine>();

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
        CGameObject* pGameObject = m_pOwnedScene->AddGameObject("Camera",{0,0});
        Components::SCamera* pCamera = pGameObject->AddComponent<Components::SCamera>();
    }



    // Create Cossack
    {
        CGameObject* pGameObject = m_pOwnedScene->AddGameObject("Kossack001",{-2,-2});
        Components::SVision* pVision = pGameObject->AddComponent<DendyEngine::Components::SVision>();
        Components::SRenderablePawn* pRenderablePawn = pGameObject->AddComponent<DendyEngine::Components::SRenderablePawn>();

        Components::STransform* pTransform = pGameObject->AddComponent<DendyEngine::Components::STransform>();
        Components::SWalkingCharacter* pWalkingCharacter = pGameObject->AddComponent<DendyEngine::Components::SWalkingCharacter>();

        pRenderablePawn->Color = glm::vec3(1.0f, 1.0f, 0.0f);
        pVision->Radius = 5.0f;
    }
    {
        CGameObject* pGameObject = m_pOwnedScene->AddGameObject("Kossack002",{2,2});
        Components::SRenderablePawn* pRenderablePawn = pGameObject->AddComponent<DendyEngine::Components::SRenderablePawn>();

        Components::STransform* pTransform = pGameObject->AddComponent<DendyEngine::Components::STransform>();
        Components::SWalkingCharacter* pWalkingCharacter = pGameObject->AddComponent<DendyEngine::Components::SWalkingCharacter>();

        pRenderablePawn->Color = glm::vec3(0.0f, 1.0f, 0.0f);
    }



    // Create static meshes
    {
        CGameObject* pGameObject = m_pOwnedScene->AddGameObject("Hata001",{8,-10});
        Components::SVisibility* pVisibility = pGameObject->AddComponent<DendyEngine::Components::SVisibility>();
        Components::SStaticMesh* pStaticMesh = pGameObject->AddComponent<DendyEngine::Components::SStaticMesh>();

        Components::STransform* pTransform = pGameObject->AddComponent<DendyEngine::Components::STransform>();

        pStaticMesh->MeshName = "hata";
        pStaticMesh->Color = {1.0f, 1.0f, 1.0f};
        pVisibility->Radius = 6.0f;
    }
    {
        CGameObject* pGameObject = m_pOwnedScene->AddGameObject("Sich001",{-8,-10});
        Components::SStaticMesh* pStaticMesh = pGameObject->AddComponent<DendyEngine::Components::SStaticMesh>();

        Components::STransform* pTransform = pGameObject->AddComponent<DendyEngine::Components::STransform>();

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
    for (auto pGameObject : m_pOwnedScene->GetGameObjectsSetNearScenePositionWithComponents<Components::SCamera>({0,0}))
    {
        Components::SCamera* pCamera = pGameObject->GetComponent<Components::SCamera>();

        glm::vec2 LeftStickValue = m_pOwnedInputHandler->GetLeftStickValue();
        float ZoomValue = m_pOwnedInputHandler->GetZoomValue();

        glm::mat4 RotateMatrix{1};
        if (LeftStickValue.x != 0.0f || LeftStickValue.y != 0.0f || ZoomValue != 0.0f)
        {
            glm::vec3 Move{LeftStickValue.x, 0.0f, LeftStickValue.y};

            pCamera->ArmTranslationMagnitude += ZoomValue * pCamera->SpeedArm * deltaTime;
            pCamera->ArmTranslationMagnitude = glm::clamp(pCamera->ArmTranslationMagnitude, pCamera->ArmTranslationMagnitudeMinValue, pCamera->ArmTranslationMagnitudeMaxValue);
            if (ZoomValue != 0.0) std::cout << "ArmMag: " << std::to_string(pCamera->ArmTranslationMagnitude) << std::endl;
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
    for (auto pGameObject : m_pOwnedScene->GetGameObjectsSetNearScenePositionWithComponents<Components::SVision>({0,0}))
    {
        Components::SVision* pVision = pGameObject->GetComponent<Components::SVision>();
        Components::SScenePose* pPose = pGameObject->GetScenePose();

        pVision->VisibleGameObjectsVec.clear();

        for (auto pOtherGameObject : m_pOwnedScene->GetGameObjectsSetNearScenePositionWithComponents<Components::SVisibility>({0,0}))
        {
            Components::SVisibility* pOthersVisibility = pOtherGameObject->GetComponent<Components::SVisibility>();
            Components::SScenePose* pOthersPose = pOtherGameObject->GetScenePose();

            glm::vec2 RelativeToTarget = pOthersPose->Position - pPose->Position;
            if (glm::dot(pPose->Orientation, RelativeToTarget) > 0)
            {
                //float Distance = glm::length(RelativeToTarget);
                if (DendyCommon::Math::FastCompareDistance(pPose->Position, pOthersPose->Position, pVision->Radius) < 0)
                {
                    pVision->VisibleGameObjectsVec.push_back(pOtherGameObject);
                    //std::cout << *pOtherGameObject << std::endl;
                    std::cout << glm::dot(pPose->Orientation, RelativeToTarget) << std::endl;
                }
            }
        }
    }


    // Update Pose
    for (auto pGameObject : m_pOwnedScene->GetGameObjectsSetNearScenePositionWithComponents<Components::SWalkingCharacter>({0,0}))
    {
        Components::SWalkingCharacter* pWalkingCharacter = pGameObject->GetComponent<Components::SWalkingCharacter>();
        Components::SScenePose* pPose = pGameObject->GetScenePose();

        glm::vec2 Target = DendyCommon::Math::GetScenePositionFromWorldPosition(CameraTargetPosition);

        pPose->Orientation = Target - pPose->Position;

        if (DendyCommon::Math::FastCompareDistance(pPose->Position, Target, pWalkingCharacter->ArrivalEpsilon) > 0)
        {
            pPose->Position += pPose->Orientation * pWalkingCharacter->MaxVelocity * deltaTime;
        }
    }


    // Compute Transform matrix
    for (auto pGameObject : m_pOwnedScene->GetGameObjectsSetNearScenePositionWithComponents<Components::STransform>({0,0}))
    {
        Components::SScenePose* pPose = pGameObject->GetScenePose();
        Components::STransform* pTransform = pGameObject->GetComponent<Components::STransform>();
        
        glm::vec3 WorldPosition = DendyCommon::Math::GetWorldPositionFromScenePosition(pPose->Position);

        WorldPosition.y = m_pOwnedTerrain->GetHeightAtPosition(pPose->Position);

        glm::mat4 RotateMatrix = DendyCommon::Math::GetRotationMatrixFromOrientation(pPose->Orientation);
        glm::mat4 TranslateMatrix = glm::translate(glm::mat4{1}, WorldPosition);
        glm::mat4 ScaleMatrix{1};

        pTransform->TransformMatrix = TranslateMatrix * RotateMatrix * ScaleMatrix;
    }
    

    // Pawn rendering
    for (auto pGameObject : m_pOwnedScene->GetGameObjectsSetNearScenePositionWithComponents<Components::STransform,Components::SRenderablePawn>({0,0}))
    {
        Components::STransform* pTransform = pGameObject->GetComponent<Components::STransform>();
        Components::SRenderablePawn* pRenderablePawn = pGameObject->GetComponent<Components::SRenderablePawn>();

        m_pOwnedRenderingSystem->AddPawnInstance(pTransform->TransformMatrix, pRenderablePawn->Color);
    }

    // Static Meshes rendering
    for (auto pGameObject : m_pOwnedScene->GetGameObjectsSetNearScenePositionWithComponents<Components::STransform,Components::SStaticMesh>({0,0}))
    {
        Components::SStaticMesh* pMesh = pGameObject->GetComponent<Components::SStaticMesh>();
        Components::STransform* pTransform = pGameObject->GetComponent<Components::STransform>();

        m_pOwnedRenderingSystem->AddStaticMesh(pMesh->MeshName, pTransform->TransformMatrix, pMesh->Color);
    }




    m_pOwnedRenderingSystem->RenderScene();

    LOG_CALLSTACK_POP();
}