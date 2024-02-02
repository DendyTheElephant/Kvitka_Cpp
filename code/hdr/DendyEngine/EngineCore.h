#pragma once

#include <PixPhetamine/InputHandler.h>
#include <PixPhetamine/RenderingSystem.h>

#include <DendyEngine/TerrainSystem.h>
#include <DendyEngine/Components/GameComponents.h>
#include <DendyEngine/Scene.h>


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
    //std::unique_ptr<Components::CComponentsEngine> m_pOwnedComponentsEngine;
    std::unique_ptr<CScene> m_pOwnedScene;

    //std::unordered_map<size_t,std::unique_ptr<CGameObject>> m_pOwnedGameObjectsMap;

    std::unique_ptr<CTerrainSystem> m_pOwnedTerrainSystem;
    //std::unique_ptr<CTerrain> m_pOwnedTerrainChunks;



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
    void SetWindowCaption(std::string str) { glfwSetWindowTitle(m_pOwnedRenderingSystem->GetGLFWWindow(),str.c_str());}
};

}