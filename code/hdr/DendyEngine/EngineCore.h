#pragma once

#include "PixPhetamine/RenderingCore.h"
#include "PixPhetamine/InputHandler.h"
#include "DendyEngine/GameObject.h"
#include "DendyEngine/Terrain.h"

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

    std::unique_ptr<PixPhetamine::CRenderingCore> m_pRenderingEngineInstance;
    std::unique_ptr<PixPhetamine::CInputHandler> m_pInputHandler;

    std::unordered_map<DendyCommon::CSerial,std::unique_ptr<CGameObject>> m_pGameObjectsOwnerMapBySerial;

    // std::unique_ptr<CTerrain> m_pTerrain;
    PixPhetamine::IMesh* m_pTerrain;

private:
    void _InitialiseRendering();
    void _InitialiseInputManager();

public:
    CEngineCore(bool isInDebugState);
    ~CEngineCore();

    bool GetRunningState() const { return m_IsRunning; }

    void Update();
};

}