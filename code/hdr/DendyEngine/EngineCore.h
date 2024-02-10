#pragma once

#include <PixPhetamine/InputHandler.h>
#include <PixPhetamine/RenderingSystem.h>

#include <DendyEngine/TerrainSystem.h>
#include <DendyEngine/VisionSystem.h>
#include <DendyEngine/MovementSystem.h>
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
    std::unique_ptr<CScene> m_pOwnedScene;


    std::unique_ptr<CTerrainSystem> m_pOwnedTerrainSystem;
    std::unique_ptr<CVisionSystem> m_pOwnedVisionSystem;
    std::unique_ptr<CMovementSystem> m_pOwnedMovementSystem;
    std::unique_ptr<PixPhetamine::CRenderingSystem> m_pOwnedRenderingSystem;

    CGameObject* m_pCamera{nullptr};
    CGameObject* m_pKvitka{nullptr};

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