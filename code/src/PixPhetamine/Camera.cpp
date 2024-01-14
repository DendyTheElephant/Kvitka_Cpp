#include "PixPhetamine/Camera.h"
#include <math.h>

PixPhetamine::CCamera::CCamera()
{
    int width, height;
    //SDL_GetWindowSize(a_window, &width, &height);
    m_Width = static_cast<float>(width);
    m_Height = static_cast<float>(height);

    m_Direction = normalize(m_Direction);

    m_Strafe = cross(Y_AXIS, m_Direction);
    m_Strafe = normalize(m_Strafe);
}

PixPhetamine::CCamera::~CCamera()
{
    
}


void PixPhetamine::CCamera::SetFOV(float const& a_newAngle)
{
    m_ViewAngle = a_newAngle;
}


void PixPhetamine::CCamera::MoveCameraForward(float const& a_distance)
{
    m_Position	= m_Position + m_Direction * a_distance;
    m_Sight		= m_Position + m_Direction;
}


void PixPhetamine::CCamera::MoveCameraBackward(float const& a_distance)
{
    m_Position	= m_Position - m_Direction * a_distance;
    m_Sight		= m_Position + m_Direction;
}


void PixPhetamine::CCamera::MoveCameraLeft(float const& a_distance)
{
    m_Position	= m_Position + m_Strafe * a_distance;
    m_Sight		= m_Position + m_Direction;
}


void PixPhetamine::CCamera::MoveCameraRight(float const& a_distance)
{
    m_Position	= m_Position - m_Strafe * a_distance;
    m_Sight		= m_Position + m_Direction;
}


void PixPhetamine::CCamera::MoveCameraUp(float const& a_distance)
{
    m_Position = m_Position + Y_AXIS * a_distance;
    m_Sight = m_Position + m_Direction;
}


void PixPhetamine::CCamera::MoveCameraDown(float const& a_distance)
{
    m_Position = m_Position - Y_AXIS * a_distance;
    m_Sight = m_Position + m_Direction;
}


void PixPhetamine::CCamera::SetPosition(glm::vec3 const& a_newPosition)
{
    m_Position	= a_newPosition;
    m_Sight		= m_Position + m_Sight;
}

#include <algorithm>
#include <cmath>
void PixPhetamine::CCamera::MoveView(float const& a_dx, float const& a_dy)
{
    m_AngleX -= a_dx / 5.0f;
    m_AngleY -= a_dy / 5.0f;

    m_AngleX = std::fmod(m_AngleX, 360.0f);
    m_AngleY = std::fmod(m_AngleY, 360.0f);

    //std::cout << "Angle X = " << m_AngleX << "   Angle Y = " << m_AngleY << std::endl;

    // Limitation Y
    m_AngleY = std::clamp(m_AngleY, -89.0f, 89.0f);

    float radAngleX = glm::radians(m_AngleX);
    float radAngleY = glm::radians(m_AngleY);

    // Spherical resolution
    m_Direction.x = glm::cos(radAngleY) * glm::sin(radAngleX);
    m_Direction.y = glm::sin(radAngleY);
    m_Direction.z = glm::cos(radAngleY) * glm::cos(radAngleX);


    // Sight is :
    m_Sight = m_Position + m_Direction;

    m_Strafe = glm::cross(Y_AXIS, m_Direction);
    m_Strafe = glm::normalize(m_Strafe);
}


glm::mat4 PixPhetamine::CCamera::GetViewProjectionMatrix()
{
    m_ProjectionMatrix = glm::perspective((m_ViewAngle), m_Width / m_Height, MIN_DEPTH_OF_FIELD, MAX_DEPTH_OF_FIELD);
    m_ViewMatrix = glm::lookAt(m_Position, m_Sight, Y_AXIS);
    return m_ProjectionMatrix * m_ViewMatrix;
}