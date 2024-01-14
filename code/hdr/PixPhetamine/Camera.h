#pragma once

/* Specific defines */
#define GLM_FORCE_RADIANS

/* External dependencies */
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace PixPhetamine
{

class CCamera
{

private:
    float m_ViewAngle{ 70.0f };
    const float MIN_DEPTH_OF_FIELD{ 0.1f };
    const float MAX_DEPTH_OF_FIELD{ 1000.0f };
    const glm::vec3 Y_AXIS{ 0.0f, 1.0f, 0.0f };
    float m_Width;
    float m_Height;

    float m_AngleX{ 180.0f };
    float m_AngleY{ 0.0f };
    glm::vec3 m_Position{ 0.0f, 0.0f, 5.0f };
    glm::vec3 m_Sight{ 0.0f, 0.0f, 4.0f };
    glm::vec3 m_Direction{ 0.0f, 0.0f, -1.0f };
    glm::vec3 m_Strafe;

    glm::mat4 m_ProjectionMatrix;
    glm::mat4 m_ViewMatrix;


public:
    CCamera();
    ~CCamera();

    glm::mat4 GetViewProjectionMatrix();
    glm::vec3 GetPosition() const { return m_Position; }
    glm::vec3 GetView() const { return m_Direction; }
    float GetNearDOF() const { return MIN_DEPTH_OF_FIELD; }
    float GetFarDOF() const { return MAX_DEPTH_OF_FIELD; }

    void SetFOV(float const& newAngle);
    void SetPosition(glm::vec3 const& newPosition);

    void MoveView(float const& dx, float const& dy);
    void MoveCameraForward(float const& distance);
    void MoveCameraBackward(float const& distance);
    void MoveCameraLeft(float const& distance);
    void MoveCameraRight(float const& distance);
    void MoveCameraUp(float const& distance);
    void MoveCameraDown(float const& distance);
};

}