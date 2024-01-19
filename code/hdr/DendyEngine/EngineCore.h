#pragma once

#include <PixPhetamine/InputHandler.h>
#include <DendyEngine/GameObject.h>
#include <DendyEngine/GameSystems.h>
#include <DendyEngine/Terrain.h>

#include <PixPhetamine/RenderingSystem.h>

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

    
    std::unique_ptr<PixPhetamine::CInputHandler> m_pInputHandler;

    std::unordered_map<size_t,std::unique_ptr<CGameObject>> m_pOwnedGameObjectsMap;

    // std::unique_ptr<CTerrain> m_pTerrain;
    PixPhetamine::IMesh* m_pTerrain;



    std::unique_ptr<PixPhetamine::CRenderingSystem> m_pRenderingSystem;
    std::unique_ptr<DendyEngine::CGameSystem<DendyEngine::CSpatialNavigationComponent>> m_pSystemSpatialNavigation;

private:
    void _InitialiseRendering();
    void _InitialiseInputManager();
    void _InitialiseGameSystems();
    void _InitialiseGameObjects();
    


public:
    CEngineCore(bool isInDebugState);
    ~CEngineCore();

    bool GetRunningState() const { return m_IsRunning; }

    void Update();
};

}