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

    // Create Terrain Chunks
    std::array<std::pair<std::string,glm::vec2>,9> TerrainConfigArray =
    {
        std::make_pair(std::string("ressources/images/terrain.png"),glm::vec2(   -1,   -1)),
        std::make_pair(std::string("ressources/images/terrain.png"),glm::vec2(    0,   -1)),
        std::make_pair(std::string("ressources/images/terrain.png"),glm::vec2(    1,   -1)),
        std::make_pair(std::string("ressources/images/terrain.png"),glm::vec2(   -1,    0)),
        std::make_pair(std::string("ressources/images/terrain0_0.png"),glm::vec2(    0,    0)),
        std::make_pair(std::string("ressources/images/terrain1_0.png"),glm::vec2(    1,    0)),
        std::make_pair(std::string("ressources/images/terrain.png"),glm::vec2(   -1,    1)),
        std::make_pair(std::string("ressources/images/terrain.png"),glm::vec2(    0,    1)),
        std::make_pair(std::string("ressources/images/terrain.png"),glm::vec2(    1,    1))
    };
    for (auto [HeighmapName,ChunkLocation] : TerrainConfigArray)
    {
        glm::vec2 ChunkWorldPosition = ChunkLocation;
        ChunkWorldPosition *= Definitions::c_TerrainScale*Definitions::c_TerrainSize;
        
        std::string TerrainChunkName = "TerrainChunk_" + std::to_string(ChunkLocation.x) + "_" + std::to_string(ChunkLocation.y);
        auto pTerrainChunk = m_pOwnedScene->AddTerrainChunk(TerrainChunkName,ChunkWorldPosition);
        pTerrainChunk->Translation = ChunkWorldPosition;

        m_pOwnedTerrainSystem->InitialiseTerrainChunkFromHeighmap(pTerrainChunk, HeighmapName);
        m_pOwnedRenderingSystem->AddTerrainChunk(pTerrainChunk);
    }

    //m_pOwnedTerrain = std::make_unique<DendyEngine::CTerrain>();
    //m_pOwnedRenderingSystem->InitialiseTerrain(m_pOwnedTerrain->GetTerrainSize(), m_pOwnedTerrain->GetScale(), m_pOwnedTerrain->GetMaxHeight(), m_pOwnedTerrain->GetData());

    LOG_CALLSTACK_POP();
}

void DendyEngine::CEngineCore::_InitialiseGameSystems()
{
    LOG_CALLSTACK_PUSH(__FILE__,__LINE__,__PRETTY_FUNCTION__);

    //m_pOwnedComponentsEngine = std::make_unique<DendyEngine::Components::CComponentsEngine>();
    m_pOwnedTerrainSystem = std::make_unique<CTerrainSystem>();
    m_pOwnedVisionSystem = std::make_unique<CVisionSystem>(m_pOwnedScene.get());

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
        Components::SVisibility* pVisibility = pGameObject->AddComponent<DendyEngine::Components::SVisibility>();
        pVisibility->Radius = 0.0f;

        m_pCamera = pGameObject;
    }


    {
        CGameObject* pGameObject = m_pOwnedScene->AddGameObject("Kvitka",{0,0});
        Components::SVisibility* pVisibility = pGameObject->AddComponent<DendyEngine::Components::SVisibility>();
        Components::SKossack* pKossack = pGameObject->AddComponent<DendyEngine::Components::SKossack>();
        Components::STransform* pTransform = pGameObject->AddComponent<DendyEngine::Components::STransform>();
        Components::SWalkingCharacter* pWalkingCharacter = pGameObject->AddComponent<DendyEngine::Components::SWalkingCharacter>();
        
        pVisibility->Radius = 0.0f;
        pWalkingCharacter->IsSprinting = true;

        m_pKvitka = pGameObject;
    }


    // Create Cossack
    {
        CGameObject* pGameObject = m_pOwnedScene->AddGameObject("KossackYellow",{-2,-2});
        //Components::SVision* pVision = pGameObject->AddComponent<DendyEngine::Components::SVision>();
        Components::SKossack* pKossack = pGameObject->AddComponent<DendyEngine::Components::SKossack>();
        Components::SVisibility* pVisibility = pGameObject->AddComponent<DendyEngine::Components::SVisibility>();
        Components::STransform* pTransform = pGameObject->AddComponent<DendyEngine::Components::STransform>();
        Components::SWalkingCharacter* pWalkingCharacter = pGameObject->AddComponent<DendyEngine::Components::SWalkingCharacter>();

        pKossack->Color = glm::vec3(1.0f, 1.0f, 0.0f);
        pVisibility->Radius = 0.0f;
    }
    {
        CGameObject* pGameObject = m_pOwnedScene->AddGameObject("KossackGreen",{2,2});
        Components::SKossack* pKossack = pGameObject->AddComponent<DendyEngine::Components::SKossack>();
        Components::SVisibility* pVisibility = pGameObject->AddComponent<DendyEngine::Components::SVisibility>();
        Components::STransform* pTransform = pGameObject->AddComponent<DendyEngine::Components::STransform>();
        Components::SWalkingCharacter* pWalkingCharacter = pGameObject->AddComponent<DendyEngine::Components::SWalkingCharacter>();

        pKossack->Color = glm::vec3(0.0f, 1.0f, 0.0f);
        pVisibility->Radius = 0.0f;
    }
    {
        CGameObject* pGameObject = m_pOwnedScene->AddGameObject("KossackRed",{8,-14});
        Components::SVision* pVision = pGameObject->AddComponent<DendyEngine::Components::SVision>();
        Components::SKossack* pKossack = pGameObject->AddComponent<DendyEngine::Components::SKossack>();
        
        pGameObject->AddComponent<DendyEngine::Components::STransform>();

        pKossack->Color = glm::vec3(1.0f, 0.0f, 0.0f);
        pVision->Radius = 9.0f;
    }



    // Create static meshes
    {
        CGameObject* pGameObject = m_pOwnedScene->AddGameObject("Hata001",{10,-15},{1,0});
        auto pStaticMesh = pGameObject->AddComponent<DendyEngine::Components::SStaticMesh>();
        pGameObject->AddComponent<DendyEngine::Components::STransform>();
        auto pCollider = pGameObject->AddComponent<DendyEngine::Components::SCollider>();
        

        pStaticMesh->MeshName = "hata";
        pStaticMesh->Color = {1.0f, 1.0f, 1.0f};
        pCollider->PositionsVec = {{9,-18},{9,-12},{11,-12},{11,-18}};
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

    //glm::vec2 PlayerScenePosition{0,0};

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
        //m_pOwnedTerrain->LoadFromFiles("ressources/images/ruggedTerrainHeightmap.png");
        //m_pOwnedRenderingSystem->InitialiseTerrain(m_pOwnedTerrain->GetTerrainSize(), m_pOwnedTerrain->GetScale(), m_pOwnedTerrain->GetMaxHeight(), m_pOwnedTerrain->GetData());
        m_pOwnedRenderingSystem->ReloadShaders();
    }


    // Player movements with inputs
    {
        glm::vec2 LeftStickValue = m_pOwnedInputHandler->GetLeftStickValue();

        if (LeftStickValue.x != 0.0f || LeftStickValue.y != 0.0f)
        {
            glm::vec2 Movement = LeftStickValue * m_pKvitka->GetComponent<Components::SWalkingCharacter>()->SprintMaxVelocity * deltaTime;

            m_pKvitka->GetComponent<Components::SWalkingCharacter>()->Movement = Movement;
        }
    }

    // Camera movements with inputs
    {
        Components::SCamera* pCamera = m_pCamera->GetComponent<Components::SCamera>();

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


            // Move = glm::normalize(Move);
            // float AngleBetweenTargetAndCurrentOrientation = atan2f( -Move.x, -Move.z );
            // glm::quat OrientationQuaternion{ cosf( AngleBetweenTargetAndCurrentOrientation/2.0f ), 0, sinf( AngleBetweenTargetAndCurrentOrientation/2.0f ), 0 };
            // RotateMatrix = glm::mat4_cast(OrientationQuaternion);
        }

        m_pCamera->GetScenePose()->Position = {pCamera->TargetPosition.x, pCamera->TargetPosition.z};
        m_pOwnedRenderingSystem->SetCameraLookAt(pCamera->TargetPosition);
        glm::vec3 ArmTranslation = glm::normalize(pCamera->ArmTranslationDirection);
        
        m_pOwnedRenderingSystem->SetCameraArmTranslation(ArmTranslation*pCamera->ArmTranslationMagnitude);

        // Debug
        auto pTerrainChunk = m_pOwnedScene->GetTerrainChunkAtScenePosition(m_pCamera->GetScenePosition());
        glm::vec3 CameraWorldTargetPosition = m_pOwnedTerrainSystem->GetWorldPositionFromScenePosition(pTerrainChunk, m_pCamera->GetScenePosition());
        glm::mat4 TranslateMatrix = glm::translate(glm::mat4{1}, CameraWorldTargetPosition);


        //glm::mat4 TransformMatrix = TranslateMatrix * RotateMatrix;// * ScaleMatrix;
        glm::mat4 TransformMatrix = TranslateMatrix;// * ScaleMatrix;
        m_pOwnedRenderingSystem->AddPawnInstance(TransformMatrix, {0.0f, 0.5f, 1.0f});
    }


    // Update Vision
    for (auto pGameObject : m_pOwnedScene->GetGameObjectsSetNearScenePositionWithComponents<Components::SVision>(m_pCamera->GetScenePosition()))
    {
        m_pOwnedVisionSystem->UpdateVisibleGameObjectsVec(pGameObject);

        // Debug
        for (auto pVisibleGameObject : pGameObject->GetComponent<Components::SVision>()->VisibleGameObjectsVec)
        {
            auto pTerrainChunk = m_pOwnedScene->GetTerrainChunkAtScenePosition(pVisibleGameObject->GetScenePosition());
            glm::vec3 WorldPosition = m_pOwnedTerrainSystem->GetWorldPositionFromScenePosition(pTerrainChunk, pVisibleGameObject->GetScenePosition());
            WorldPosition.y += 1.75f;
            glm::mat4 TranslateMatrix = glm::translate(glm::mat4{1}, WorldPosition);
            glm::mat4 ScaleMatrix = glm::scale(glm::mat4{1}, {0.3f, 0.3f, 0.3f});
            glm::mat4 TransformMatrix = TranslateMatrix * ScaleMatrix;
            m_pOwnedRenderingSystem->AddPawnInstance(TransformMatrix, {1.0f, 0.5f, 0.0f});
        }
    }


    // Update Pose
    for (auto pGameObject : m_pOwnedScene->GetGameObjectsSetWithComponents<Components::SWalkingCharacter>())
    {
        Components::SWalkingCharacter* pWalkingCharacter = pGameObject->GetComponent<Components::SWalkingCharacter>();
        Components::SScenePose* pPose = pGameObject->GetScenePose();

        glm::vec2 Target = m_pCamera->GetScenePosition();

        pPose->Orientation = glm::normalize(Target - pPose->Position);

        if (DendyCommon::Math::FastCompareDistance(pPose->Position, Target, pWalkingCharacter->ArrivalEpsilon) > 0)
        {
            glm::vec2 NewPosition = pPose->Position + pPose->Orientation * pWalkingCharacter->MaxVelocity * deltaTime;
            m_pOwnedScene->UpdateGameObjectScenePosition(pGameObject, NewPosition);
        }
    }


    // Compute Transform matrix
    for (auto pGameObject : m_pOwnedScene->GetGameObjectsSetNearScenePositionWithComponents<Components::STransform>(m_pCamera->GetScenePosition()))
    {
        Components::SScenePose* pPose = pGameObject->GetScenePose();
        Components::STransform* pTransform = pGameObject->GetComponent<Components::STransform>();
        

        auto pTerrainChunk = m_pOwnedScene->GetTerrainChunkAtScenePosition(pPose->Position);
        glm::vec3 WorldPosition = m_pOwnedTerrainSystem->GetWorldPositionFromScenePosition(pTerrainChunk, pPose->Position);

        glm::mat4 RotateMatrix = DendyCommon::Math::GetRotationMatrixFromOrientation(pPose->Orientation);
        glm::mat4 TranslateMatrix = glm::translate(glm::mat4{1}, WorldPosition);
        glm::mat4 ScaleMatrix{1};

        pTransform->TransformMatrix = TranslateMatrix * RotateMatrix * ScaleMatrix;
    }
    

    // Pawn rendering
    for (auto pGameObject : m_pOwnedScene->GetGameObjectsSetNearScenePositionWithComponents<Components::STransform,Components::SRenderablePawn>(m_pCamera->GetScenePosition()))
    {
        Components::STransform* pTransform = pGameObject->GetComponent<Components::STransform>();
        Components::SRenderablePawn* pRenderablePawn = pGameObject->GetComponent<Components::SRenderablePawn>();

        m_pOwnedRenderingSystem->AddPawnInstance(pTransform->TransformMatrix, pRenderablePawn->Color);
    }

    // Kossack rendering
    for (auto pGameObject : m_pOwnedScene->GetGameObjectsSetNearScenePositionWithComponents<Components::STransform,Components::SKossack>(m_pCamera->GetScenePosition()))
    {
        Components::STransform* pTransform = pGameObject->GetComponent<Components::STransform>();
        Components::SKossack* pRenderableKossack = pGameObject->GetComponent<Components::SKossack>();

        m_pOwnedRenderingSystem->AddKossackInstance(pTransform->TransformMatrix, pRenderableKossack->Color);
    }

    // Static Meshes rendering
    for (auto pGameObject : m_pOwnedScene->GetGameObjectsSetNearScenePositionWithComponents<Components::STransform,Components::SStaticMesh>(m_pCamera->GetScenePosition()))
    {
        Components::SStaticMesh* pMesh = pGameObject->GetComponent<Components::SStaticMesh>();
        Components::STransform* pTransform = pGameObject->GetComponent<Components::STransform>();

        m_pOwnedRenderingSystem->AddStaticMesh(pMesh->MeshName, pTransform->TransformMatrix, pMesh->Color);
    }

    // Terrain Chunks rendering
    for (auto pTerrainChunk : m_pOwnedScene->GetTerrainChunksSetNearScenePosition(m_pCamera->GetScenePosition()))
    {
        m_pOwnedRenderingSystem->AddTerrainIdInstanceToRender(pTerrainChunk->TerrainId);
    }



    m_pOwnedRenderingSystem->RenderScene();

    LOG_CALLSTACK_POP();
}