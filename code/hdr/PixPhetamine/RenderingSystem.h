#pragma once


#include <PixPhetamine/Shader.h>
#include <PixPhetamine/Mesh.h>
#include <PixPhetamine/Camera.h>
#include <PixPhetamine/InputHandler.h>

#include <DendyEngine/Components/GameComponents.h>

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

#include <memory>
#include <unordered_map>
#include <vector>
#include <iostream>


namespace PixPhetamine
{

class CRenderingSystem
{
public:
    struct SMeshInstance { std::string Name; glm::mat4 TransformMatrix; glm::vec3 Color; };

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

    std::unordered_map<uint32_t, std::unique_ptr<PixPhetamine::CMesh>> m_MeshTerrainMapById;
    std::vector<uint32_t> m_TerrainIdsToRender;

    std::vector<std::pair<glm::mat4,glm::vec3>> m_PawnInstanceDataVec;
    std::vector<std::pair<glm::mat4,glm::vec3>> m_KossackInstancesDataVec;

    std::vector<SMeshInstance> m_StaticMeshInstanceDataVec;
    glm::vec3 m_CameraLookAtPosition{0.0f};
    glm::vec3 m_CameraArmTranslation{0.0f};


private:
    void _LoadShaders();
    void _LoadMeshes();

public:
    CRenderingSystem(bool isInDebugState);
    ~CRenderingSystem();

    static void AssertOpenGLErrors();

    void AddTerrainChunk(DendyEngine::Components::STerrainChunk* pTerrainChunk);

    void AddTerrainIdInstanceToRender(uint32_t const& id);
    void AddPawnInstance(glm::mat4 const& transformMatrix, glm::vec3 const& color);
    void AddKossackInstance(glm::mat4 const& transformMatrix, glm::vec3 const& color);
    void AddStaticMesh(std::string const& name, glm::mat4 const& transformMatrix, glm::vec3 const& color);
    void RenderScene();
    void ReloadShaders();

    void SetCameraLookAt(glm::vec3 const& targetPosition) {m_CameraLookAtPosition = targetPosition;}
    void SetCameraArmTranslation(glm::vec3 const& armTranslation) {m_CameraArmTranslation = armTranslation;}

    inline GLFWwindow* GetGLFWWindow() const {return m_pMainWindow;}
};

}