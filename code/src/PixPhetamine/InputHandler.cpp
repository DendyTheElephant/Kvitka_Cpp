#include "PixPhetamine/InputHandler.h"

#include <iostream>

PixPhetamine::CInputHandler::CInputHandler(GLFWwindow* pGLFWindow)
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

    // glfwSetScrollCallback(pGLFWindow, [](GLFWwindow* window, double xOffset, double yOffset)
    // {
    //     std::cout << "Mouse scrolled!" << std::endl;
    // });

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
            

            std::cout<<std::endl;
        }
    }
}

void PixPhetamine::CInputHandler::UpdateInputs()
{
    glfwPollEvents();
    _UpdateGamepads();
}
