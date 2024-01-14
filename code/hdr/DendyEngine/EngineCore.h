#pragma once

#include "PixPhetamine/RenderingCore.h"
#include "DendyEngine/GameObject.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <unordered_map>

namespace DendyEngine
{

class CEngineCore
{
private:

    bool m_IsRunning {false};
    bool m_IsInDebugState{false};

    PixPhetamine::CRenderingCore* m_pRenderingEngineInstance;
    std::unordered_map<unsigned int,IGameObject*> m_pGameObjectsMapById;

    std::vector<std::string> m_ShaderNamesVec;
    std::vector<std::string> m_MeshNamesVec;

    std::map<std::string, PixPhetamine::CShader*> m_ShaderMapByName; // List of the shaders used in the game
    std::map<std::string, PixPhetamine::CMesh*> m_MeshMapByName;

    glm::mat4 m_ViewProjectionMatrix;
    glm::mat4 m_ModelViewProjectionMatrix;

private:
    void _LoadShaders();
    void _ReloadShaders();
    void _LoadMeshes();
    void _UploadMeshesToGPU();

public:
    CEngineCore(bool isInDebugState);
    ~CEngineCore();

    void SetRenderingCoreInstance(PixPhetamine::CRenderingCore* pRenderingEngineInstance);
    bool GetRunningState() const { return m_IsRunning; }

    void Update();
};

}