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
        std::make_pair(std::string("ressources/images/terrain-1_0.png"),glm::vec2(   -1,    0)),
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
    m_pOwnedMovementSystem = std::make_unique<CMovementSystem>(m_pOwnedScene.get());

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

        m_pCamera = pGameObject;
    }


    // Create Player Kvitka
    {
        CGameObject* pGameObject = m_pOwnedScene->AddGameObject("Kvitka",{0,0});
        pGameObject->AddComponent<DendyEngine::Components::SVisibility>();
        Components::SStaticMesh* pMesh = pGameObject->AddComponent<DendyEngine::Components::SStaticMesh>();
        pGameObject->AddComponent<DendyEngine::Components::STransform>();
        Components::SWalkingCharacter* pWalkingCharacter = pGameObject->AddComponent<DendyEngine::Components::SWalkingCharacter>();
        pGameObject->AddComponent<DendyEngine::Components::SMovementTarget>();

        pWalkingCharacter->Acceleration *= 2.0f;
        pWalkingCharacter->MaxVelocity *= 4.0f;
        pMesh->Color = {0.6f,0.0f,0.0f};
        pMesh->MeshName = "kvitka";

        m_pKvitka = pGameObject;
    }


    // Create Cossack
    {
        CGameObject* pGameObject = m_pOwnedScene->AddGameObject("KossackYellow",{-2,-2});
        Components::SKossack* pKossack = pGameObject->AddComponent<DendyEngine::Components::SKossack>();
        pGameObject->AddComponent<DendyEngine::Components::SVisibility>();
        pGameObject->AddComponent<DendyEngine::Components::STransform>();
        pGameObject->AddComponent<DendyEngine::Components::SWalkingCharacter>();
        pGameObject->AddComponent<DendyEngine::Components::SMovementTarget>();

        pKossack->Color = {1,1,0};
    }
    {
        CGameObject* pGameObject = m_pOwnedScene->AddGameObject("KossackGreen",{2,2});
        Components::SKossack* pKossack = pGameObject->AddComponent<DendyEngine::Components::SKossack>();
        pGameObject->AddComponent<DendyEngine::Components::SVisibility>();
        pGameObject->AddComponent<DendyEngine::Components::STransform>();
        pGameObject->AddComponent<DendyEngine::Components::SWalkingCharacter>();
        pGameObject->AddComponent<DendyEngine::Components::SMovementTarget>();

        pKossack->Color = {0,1,0};
    }
    {
        CGameObject* pGameObject = m_pOwnedScene->AddGameObject("KossackBlue",{8,-14});
        pGameObject->AddComponent<DendyEngine::Components::SVision>();
        Components::SKossack* pKossack = pGameObject->AddComponent<DendyEngine::Components::SKossack>();
        pGameObject->AddComponent<DendyEngine::Components::STransform>();

        pKossack->Color = {0,0,1};
    }



    // Create static meshes

    // Hata
    {
        CGameObject* pGameObject = m_pOwnedScene->AddGameObject("Hata001",{15,-15},{1,0});
        auto pStaticMesh = pGameObject->AddComponent<DendyEngine::Components::SStaticMesh>();
        auto pCollider = pGameObject->AddComponent<DendyEngine::Components::SCollider>();
        pGameObject->AddComponent<DendyEngine::Components::STransform>();

        pStaticMesh->MeshName = "hata";
        pStaticMesh->Color = {1.0f, 1.0f, 1.0f};
        pCollider->PositionsVec = {{12,-21},{12,-9},{18,-9},{18,-21}};
    }

    // Sich
    {
        CGameObject* pGameObject = m_pOwnedScene->AddGameObject("Sich001",{-8,-10});
        Components::SStaticMesh* pStaticMesh = pGameObject->AddComponent<DendyEngine::Components::SStaticMesh>();
        pGameObject->AddComponent<DendyEngine::Components::STransform>();

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

    glm::vec2 TargetForYellowKossack{0,0};
    bool OrderMoveForYellowKossack{false};
    // Player movements with inputs
    {
        glm::vec2 LeftStickValue = m_pOwnedInputHandler->GetLeftStickValue();

        if (LeftStickValue.x != 0.0f || LeftStickValue.y != 0.0f)
        {
            m_pKvitka->GetComponent<Components::SMovementTarget>()->MovementType = Components::SMovementTarget::EMovementTargetType::MoveToDirection;
            m_pKvitka->GetComponent<Components::SMovementTarget>()->TargetDirection = glm::vec2(LeftStickValue.x,LeftStickValue.y);
        }
        else
        {
            m_pKvitka->GetComponent<Components::SMovementTarget>()->MovementType = Components::SMovementTarget::EMovementTargetType::None;
        }

        if (m_pOwnedInputHandler->GetKeySpaceReleased())
        {
            TargetForYellowKossack = m_pKvitka->GetScenePosition();
            OrderMoveForYellowKossack = true;
        }
    }
    if (OrderMoveForYellowKossack)
    {
        for (auto pGameObject : m_pOwnedScene->GetGameObjectsSetWithComponents<Components::SKossack>())
        {
            if (pGameObject->GetName() == "KossackYellow")
            {
                pGameObject->GetComponent<Components::SMovementTarget>()->MovementType = Components::SMovementTarget::EMovementTargetType::ReachPosition;
                pGameObject->GetComponent<Components::SMovementTarget>()->TargetPosition = TargetForYellowKossack;
            }
        }
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
            if (pVisibleGameObject->GetId() == m_pKvitka->GetId())
                WorldPosition.y += 1.75f;
            glm::mat4 TranslateMatrix = glm::translate(glm::mat4{1}, WorldPosition);
            glm::mat4 ScaleMatrix = glm::scale(glm::mat4{1}, {0.3f, 0.3f, 0.3f});
            glm::mat4 TransformMatrix = TranslateMatrix * ScaleMatrix;
            m_pOwnedRenderingSystem->AddPawnInstance(TransformMatrix, {1.0f, 0.5f, 0.0f});
        }
    }


    // Update Pose : basically movement
    for (auto pGameObject : m_pOwnedScene->GetGameObjectsSetWithComponents<Components::SWalkingCharacter,Components::SMovementTarget>())
    {
        m_pOwnedMovementSystem->UpdatePositionOfMovingGameObject(pGameObject, deltaTime);
    }


    // Camera movements: follow player
    {
        float ZoomValue = m_pOwnedInputHandler->GetZoomValue();
        glm::vec2 CameraToPlayer = m_pKvitka->GetScenePosition() - m_pCamera->GetScenePosition();
        auto pCameraComponent = m_pCamera->GetComponent<Components::SCamera>();

        if (CameraToPlayer.x != 0.0f || CameraToPlayer.y != 0.0f || ZoomValue != 0.0f)
        {
            glm::vec2 MovementDirection = glm::normalize(CameraToPlayer);
            glm::vec2 Movement{0};

            pCameraComponent->ArmTranslationMagnitude += ZoomValue * pCameraComponent->SpeedArm * deltaTime;
            pCameraComponent->ArmTranslationMagnitude = glm::clamp(pCameraComponent->ArmTranslationMagnitude, pCameraComponent->ArmTranslationMagnitudeMinValue, pCameraComponent->ArmTranslationMagnitudeMaxValue);
            if (ZoomValue != 0.0) std::cout << "ArmMag: " << std::to_string(pCameraComponent->ArmTranslationMagnitude) << std::endl;
            
            if (DendyCommon::Math::FastCompareMagnitude(CameraToPlayer, 1.0f) > 0)
            {
                Movement = MovementDirection * pCameraComponent->Speed * deltaTime;
            }
            

            m_pCamera->GetScenePose()->Position = m_pCamera->GetScenePosition() + Movement;
            auto pTerrainChunk = m_pOwnedScene->GetTerrainChunkAtScenePosition(m_pCamera->GetScenePosition());
            pCameraComponent->TargetPosition = m_pOwnedTerrainSystem->GetWorldPositionFromScenePosition(pTerrainChunk, m_pCamera->GetScenePosition());


            m_pOwnedRenderingSystem->SetCameraLookAt(pCameraComponent->TargetPosition);
            glm::vec3 ArmTranslation = glm::normalize(pCameraComponent->ArmTranslationDirection);
        
            m_pOwnedRenderingSystem->SetCameraArmTranslation(ArmTranslation*pCameraComponent->ArmTranslationMagnitude);
        }
    }


    // Compute Transform matrix : based on pose in scene, and terrain
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