#pragma once

// GLFW (Graphics Library Framework)
// https://www.glfw.org/
#include <GLFW/glfw3.h>

#include <vector>

namespace PixPhetamine
{

class CInputHandler
{
private:
    std::vector<int> m_ConnectedGamepadsVec;
    GLFWwindow* m_pMainWindow;
    bool m_IsWindowClosed{false};

private:
    void _UpdateGamepads();

public:
    CInputHandler(GLFWwindow* pGLFWindow);
    ~CInputHandler();

    void UpdateInputs();
    inline bool GetWindowClosedState() const { return m_IsWindowClosed; }
};

}