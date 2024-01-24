#pragma once

#include <vector>
#include <iostream>

// OpenGL context GLAD
// https://glad.dav1d.de/
// OpenGL Core V4.6 no extensions
#include <glad/glad.h>

// GLFW (Graphics Library Framework)
// https://www.glfw.org/
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/* Internal headers includes */
// #include <CStaticMesh.h>
// #include <CShader.h>
// #include <CCamera.h>
// #include <UErrorHandler.h>
// #include <Display.h>
// #include <CFrameBuffer.h>
// #include <CPostProcessPass.h>
// #include <CSkybox.h>

#include <PixPhetamine/Shader.h>
#include <PixPhetamine/Mesh.h>
#include <PixPhetamine/Camera.h>
#include <PixPhetamine/InputHandler.h>

#include <memory>
#include <unordered_map>

namespace PixPhetamine
{

class CRenderingSystem
{
private:
    bool m_IsRunning {false};
    bool m_IsInDebugState{false};
    bool m_IsInWireframe{false};

    //int m_Frame{0};
    //unsigned long int m_ElapsedTime{0};
    //CTimer m_secondTimer; // Timer reset on each second for FPS computation
    //char m_windowCaption[64]; // Window caption (updated on each frame to show FPS)

    GLFWwindow* m_pMainWindow{nullptr}; // Our window handle
    CCamera* m_pMainCamera{nullptr}; // Camera for the player one

    std::vector<std::string> m_ShaderNamesVec;
    std::vector<std::string> m_MeshNamesVec;

    std::unordered_map<std::string, std::unique_ptr<PixPhetamine::CShader>> m_ShaderMapByName; // List of the shaders used in the game
    std::unordered_map<std::string, std::unique_ptr<PixPhetamine::CMesh>> m_MeshMapByName;

    //uint16_t m_TerrainSubdivisions{4}
    std::unordered_map<std::string, std::unique_ptr<PixPhetamine::CMesh>> m_MeshTerrainMapBy;

    std::vector<std::pair<glm::mat4,glm::vec3>> m_PawnIntanceDataVec;
    glm::vec3 m_CameraLookAtPosition{0.0f};

    // PixPhetamine::PostProcess::CFrameBuffer* m_GBufferMS;
    // PixPhetamine::PostProcess::CFrameBuffer* m_GBufferAA;
    // PixPhetamine::PostProcess::CFrameBuffer* m_DownSampled;
    // PixPhetamine::PostProcess::CFrameBuffer* m_RGBSplitted;
    // PixPhetamine::PostProcess::CFrameBuffer* m_BufferBlurPartial;
    // PixPhetamine::PostProcess::CFrameBuffer* m_BufferBlur;

    // PixPhetamine::PostProcess::CPostProcessPass* m_DownSamplingPass;
    // PixPhetamine::PostProcess::CPostProcessPass* m_BlurPassPartI;
    // PixPhetamine::PostProcess::CPostProcessPass* m_BlurPassPartII;
    // PixPhetamine::PostProcess::CPostProcessPass* m_RGBSplitPass;
    // PixPhetamine::PostProcess::CPostProcessPass* m_DeferredShadingPass;

    // PixPhetamine::SceneRendering::CSkybox* m_skyBox;


private:
    void _LoadShaders();
    void _LoadMeshes();

public:
    CRenderingSystem(bool isInDebugState);
    ~CRenderingSystem();

    static void AssertOpenGLErrors();

    void InitialiseTerrain(size_t terrainSize, float scale, const float* pHeightsVec);
    void AddPawnInstance(glm::mat4 const& transformMatrix, glm::vec3 const& color);
    void RenderScene();
    void ReloadShaders();
    void SetCameraLookAt(glm::vec3 const& targetPosition) {m_CameraLookAtPosition = targetPosition;}
    inline GLFWwindow* GetGLFWWindow() const {return m_pMainWindow;}
};

}