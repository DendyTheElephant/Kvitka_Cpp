#pragma once

#include <PixPhetamine/InputHandler.h>
#include <DendyEngine/Terrain.h>
#include <DendyEngine/ECS/ECSEngine.h>

#include <PixPhetamine/RenderingSystem.h>
#include <DendyEngine/Terrain.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <memory>

namespace DendyEngine
{

class CEngineCore
{
private:

    bool m_IsRunning {false};
    bool m_IsInDebugState{false};

    
    std::unique_ptr<PixPhetamine::CInputHandler> m_pOwnedInputHandler;
    std::unique_ptr<ECS::CECSEngine> m_pOwnedECSEngine;

    //std::unordered_map<size_t,std::unique_ptr<CGameObject>> m_pOwnedGameObjectsMap;

    // std::unique_ptr<CTerrain> m_pTerrain;
    std::unique_ptr<CTerrain> m_pOwnedTerrain;



    std::unique_ptr<PixPhetamine::CRenderingSystem> m_pOwnedRenderingSystem;
    //std::unique_ptr<DendyEngine::CGameSystem<DendyEngine::CSpatialNavigationComponent>> m_pSystemSpatialNavigation;
    //std::unique_ptr<DendyEngine::CRenderablePawn> m_pSystemRenderablePawn;

private:
    void _InitialiseRendering();
    void _InitialiseInputManager();
    void _InitialiseTerrain();
    void _InitialiseGameSystems();
    void _InitialiseGameObjects();


public:
    CEngineCore(bool isInDebugState);
    ~CEngineCore();

    bool GetRunningState() const { return m_IsRunning; }

    void Update(float deltaTime);
};

}