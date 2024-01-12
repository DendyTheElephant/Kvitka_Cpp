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

private:
    void _UpdateGamepads();

public:

    CInputHandler(GLFWwindow* pGLFWindow);

    void UpdateInputs();
};

}