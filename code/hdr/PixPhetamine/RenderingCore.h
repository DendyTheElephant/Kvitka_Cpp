#pragma once

#include <vector>
#include <iostream>

// OpenGL context GLAD
// https://glad.dav1d.de/
// OpenGL Core V4.6 no extensions
#include <glad/glad.h>

// GLFW (Graphics Library Framework)
// https://www.glfw.org/
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/* Internal headers includes */
// #include "CStaticMesh.h"
// #include "CShader.h"
// #include "CCamera.h"
// #include "UErrorHandler.h"
// #include "Display.h"
// #include "CFrameBuffer.h"
// #include "CPostProcessPass.h"
// #include "CSkybox.h"

#include "PixPhetamine/Shader.h"
#include "PixPhetamine/Mesh.h"
#include "PixPhetamine/Camera.h"
#include "PixPhetamine/InputHandler.h"

namespace PixPhetamine
{

class CRenderingCore
{
private:

    const int VIEWPORT_WIDTH = 1280;
    const int VIEWPORT_HEIGHT = 720;

    bool m_IsRunning {false};
    bool m_IsInDebugState{false};
    std::vector<std::string> m_ErrorMessagesVec;

    int m_Frame{0};
    unsigned long int m_ElapsedTime{0};
    //CTimer m_secondTimer; // Timer reset on each second for FPS computation
    char m_windowCaption[64]; // Window caption (updated on each frame to show FPS)

    GLFWwindow* m_pMainWindow{nullptr}; // Our window handle
    CCamera* m_pMainCamera{nullptr}; // Camera for the player one

    std::vector<std::string> m_ShaderNamesVec;
    std::vector<std::string> m_MeshNamesVec;

    std::map<std::string, PixPhetamine::CShader*> m_ShaderMapByName; // List of the shaders used in the game
    std::map<std::string, PixPhetamine::CMesh*> m_MeshMapByName;

    glm::mat4 m_ViewProjectionMatrix;
    glm::mat4 m_ModelViewProjectionMatrix;

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
    //void _FramebufferSizeCallback(GLFWwindow* window, int width, int height);
    void _LoadShaders();
    void _ReloadShaders();
    void _LoadMeshes();
    void _UploadMeshesToGPU();

public:
    CRenderingCore(bool isInDebugState);
    ~CRenderingCore();

    static void AssertOpenGLErrors();

    void Render();
    inline GLFWwindow* GetGLFWWindow() const {return m_pMainWindow;}
};

}