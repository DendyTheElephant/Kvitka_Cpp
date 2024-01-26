#include <PixPhetamine/InputHandler.h>

#include <iostream>

PixPhetamine::CInputHandler::CInputHandler(GLFWwindow* pGLFWindow):
m_pMainWindow(pGLFWindow)
{
    // // Set GLFW callbacks
    // glfwSetWindowSizeCallback(pGLFWindow, [](GLFWwindow* window, int width, int height)
    // {
    //     std::cout << "Window resized!" << std::endl;
    // });

    // glfwSetWindowCloseCallback(pGLFWindow, [](GLFWwindow* window)
    // {
    //     std::cout << "Window closed!" << std::endl;
    // });

    // glfwSetKeyCallback(pGLFWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods)
    // {
    //     switch (action)
    //     {
    //         case GLFW_PRESS:
    //         {
    //             std::cout << "Key pressed!" << std::endl;
    //             break;
    //         }
    //         case GLFW_RELEASE:
    //         {
    //             std::cout << "Key released!" << std::endl;
    //             break;
    //         }
    //         case GLFW_REPEAT:
    //         {
    //             std::cout << "Key repeat!" << std::endl;
    //             break;
    //         }
    //     }
    // });

    // glfwSetCharCallback(pGLFWindow, [](GLFWwindow* window, unsigned int keycode)
    // {
    //     std::cout << "Char typed!" << std::endl;
    // });

    // glfwSetMouseButtonCallback(pGLFWindow, [](GLFWwindow* window, int button, int action, int mods)
    // {
    //     switch (action)
    //     {
    //         case GLFW_PRESS:
    //         {
    //             std::cout << "Mouse "<< button <<" pressed!" << std::endl;
    //             break;
    //         }
    //         case GLFW_RELEASE:
    //         {
    //             std::cout << "Mouse "<< button <<" released!" << std::endl;
    //             break;
    //         }
    //     }
    // });

    glfwSetScrollCallback(pGLFWindow, [](GLFWwindow* window, double xOffset, double yOffset)
    {
        if (yOffset != 0.0)
        {
            // m_IsZoomValueChanged = true;
            // m_ZoomValue = yOffset;
            std::cout << "Mouse scrolled! x:" << xOffset << " y:" << yOffset << std::endl;
        }
    });

    // glfwSetCursorPosCallback(pGLFWindow, [](GLFWwindow* window, double xPos, double yPos)
    // {
    //     std::cout << "Mouse moved ("<< xPos <<',' << yPos <<")!" << std::endl;
    // });

    // glfwSetJoystickCallback([](int joystickId, int event)
    // {
    //     switch (event)
    //     {
    //         case GLFW_CONNECTED:
    //         {
    //             std::cout << "Gamepad Id:" << joystickId << " Name:\'" << glfwGetJoystickName(joystickId) << "\' connected!" << std::endl;
    //             break;
    //         }
    //         case GLFW_DISCONNECTED:
    //         {
    //             std::cout << "Gamepad "<< joystickId <<" disconnected!" << std::endl;
    //             break;
    //         }
    //     }
    // });
}

PixPhetamine::CInputHandler::~CInputHandler()
{
    
}

void PixPhetamine::CInputHandler::_OnMouseScroll(GLFWwindow* pGLFWindow, double xOffset, double yOffset)
{

}

#include <algorithm>
void PixPhetamine::CInputHandler::_UpdateGamepads()
{
    // Connexion:
    for (int iGamepadSlot=0; iGamepadSlot<GLFW_JOYSTICK_LAST; iGamepadSlot++)
    {
        int GamepadInSlotState = glfwJoystickPresent( iGamepadSlot );
        auto iConnectedGamepad = std::find(m_ConnectedGamepadsVec.begin(), m_ConnectedGamepadsVec.end(), iGamepadSlot);
        bool IsGamepadAlreadyConnected = iConnectedGamepad != m_ConnectedGamepadsVec.end();
        if ( GamepadInSlotState == GLFW_TRUE && IsGamepadAlreadyConnected == false )
        {
            m_ConnectedGamepadsVec.push_back(iGamepadSlot);
        }
        else if ( GamepadInSlotState == GLFW_FALSE && IsGamepadAlreadyConnected == true )
        {
            m_ConnectedGamepadsVec.erase(iConnectedGamepad);
        }
    }
    
    for ( auto iConnectedGamepad : m_ConnectedGamepadsVec )
    {
        if ( std::string( glfwGetJoystickName(iConnectedGamepad) ) ==  "Xbox Controller" )
        {
            std::cout << "Connected Gamepad ["<<iConnectedGamepad<<"]"<<" ["<<glfwGetJoystickName( iConnectedGamepad )<<"]"<<std::endl;

            int AxesCount;
            const float *AxisValuesArr = glfwGetJoystickAxes( iConnectedGamepad, &AxesCount );
            // std::cout << "Axes: "<< AxesCount << std::endl;

            int ButtonCount;
            const unsigned char *ButtonValuesArr = glfwGetJoystickButtons( iConnectedGamepad, &ButtonCount );
            //std::cout << "Buttons: "<< ButtonCount << std::endl;

            float LeftStickXValue = AxisValuesArr[0];
            float LeftStickYValue = AxisValuesArr[1];
            float RightStickXValue = AxisValuesArr[2];
            float RightStickYValue = AxisValuesArr[3];
            std::cout << "LX: " << LeftStickXValue << " LY: " << LeftStickYValue << " RX: " << RightStickXValue << " RY: " << RightStickYValue << std::endl;

            bool ButtonAPressed = static_cast<bool>(ButtonValuesArr[0]);
            bool ButtonBPressed = static_cast<bool>(ButtonValuesArr[1]);
            bool ButtonXPressed = static_cast<bool>(ButtonValuesArr[2]);
            bool ButtonYPressed = static_cast<bool>(ButtonValuesArr[3]);

            if (ButtonAPressed)
                std::cout << "A" << std::endl;
            if (ButtonBPressed)
                std::cout << "B" << std::endl;
            if (ButtonXPressed)
                std::cout << "X" << std::endl;
            if (ButtonYPressed)
                std::cout << "Y" << std::endl;

            // Dead zone
            if (abs(LeftStickXValue) < 0.1)
                LeftStickXValue = 0.0f;
            if (abs(LeftStickYValue) < 0.1)
                LeftStickYValue = 0.0f;

            // Inputs translation:
            if (LeftStickYValue != 0.0f || LeftStickXValue != 0.0f)
            {
                m_LeftStickValue = glm::normalize(glm::vec2(LeftStickXValue, LeftStickYValue));
            }
            
            if (ButtonYPressed)
            {
                m_ZoomValue = -1.0;
            }
            else if (ButtonXPressed)
            {
                m_ZoomValue = 1.0;
            }

            std::cout<<std::endl;
        }
    }
}

void PixPhetamine::CInputHandler::UpdateInputs()
{
    glfwPollEvents();
    if (glfwWindowShouldClose(m_pMainWindow))
        m_IsWindowClosed = true;
    
    // Parse Left Stick on Keyboard
    float LeftStickHorizontalValue{0.0f}, LeftStickVerticalValue{0.0f};
    if (glfwGetKey(m_pMainWindow, GLFW_KEY_A) == GLFW_PRESS && glfwGetKey(m_pMainWindow, GLFW_KEY_D) == GLFW_RELEASE)
        LeftStickHorizontalValue = -1.0;
    else if (glfwGetKey(m_pMainWindow, GLFW_KEY_D) == GLFW_PRESS && glfwGetKey(m_pMainWindow, GLFW_KEY_A) == GLFW_RELEASE)
        LeftStickHorizontalValue = 1.0;

    if (glfwGetKey(m_pMainWindow, GLFW_KEY_W) == GLFW_PRESS && glfwGetKey(m_pMainWindow, GLFW_KEY_S) == GLFW_RELEASE)
        LeftStickVerticalValue = -1.0;
    else if (glfwGetKey(m_pMainWindow, GLFW_KEY_S) == GLFW_PRESS && glfwGetKey(m_pMainWindow, GLFW_KEY_W) == GLFW_RELEASE)
        LeftStickVerticalValue = 1.0;

    if (LeftStickVerticalValue != 0.0f || LeftStickHorizontalValue != 0.0f)
    {
        m_LeftStickValue = glm::normalize(glm::vec2(LeftStickHorizontalValue, LeftStickVerticalValue));
    }
    else
    {
        m_LeftStickValue.x = 0;
        m_LeftStickValue.y = 0;
    }

    if (glfwGetKey(m_pMainWindow, GLFW_KEY_Q) == GLFW_PRESS)
    {
        m_ZoomValue = -1.0;
    }
    else if (glfwGetKey(m_pMainWindow, GLFW_KEY_E) == GLFW_PRESS)
    {
        m_ZoomValue = 1.0;
    }
    else
    {
        m_ZoomValue = 0.0;
    }

    m_KeyRReleased = false;
    if (glfwGetKey(m_pMainWindow, GLFW_KEY_R) == GLFW_PRESS)
    {
        m_KeyR = true;
    }
    else if (glfwGetKey(m_pMainWindow, GLFW_KEY_R) == GLFW_RELEASE)
    {
        if (m_KeyR)
            m_KeyRReleased = true;
        m_KeyR = false;
    }
    
    m_KeyEscapeReleased = false;
    if (glfwGetKey(m_pMainWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        m_KeyEscape = true;
    }
    else if (glfwGetKey(m_pMainWindow, GLFW_KEY_ESCAPE) == GLFW_RELEASE)
    {
        if (m_KeyEscape)
            m_KeyEscapeReleased = true;
        m_KeyEscape = false;
    }



    _UpdateGamepads();
}
