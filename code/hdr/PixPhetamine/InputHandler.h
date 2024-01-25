#pragma once

// GLFW (Graphics Library Framework)
// https://www.glfw.org/
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <vector>

namespace PixPhetamine
{

class CInputHandler
{
private:
    std::vector<int> m_ConnectedGamepadsVec;
    GLFWwindow* m_pMainWindow;
    bool m_IsWindowClosed{false};

    glm::vec2 m_LeftStickValue{0.0f,0.0f};
    //bool m_IsZoomValueChanged{false};
    bool m_KeyR{false};
    bool m_KeyEscape{false};
    bool m_KeyRReleased{false};
    bool m_KeyEscapeReleased{false};
    float m_ZoomValue{0.0f};

private:
    void _UpdateGamepads();
    void _OnMouseScroll(GLFWwindow* pGLFWindow, double xOffset, double yOffset);

public:
    CInputHandler(GLFWwindow* pGLFWindow);
    ~CInputHandler();

    void UpdateInputs();
    inline bool GetWindowClosedState() const { return m_IsWindowClosed; }
    inline glm::vec2 GetLeftStickValue() const { return m_LeftStickValue; }
    inline float GetZoomValue() const { return m_ZoomValue; }
    inline bool GetKeyRReleased() const {return m_KeyRReleased;}
    inline bool GetKeyEscapeReleased() const {return m_KeyEscapeReleased;}
};

}