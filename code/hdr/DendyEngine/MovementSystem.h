#pragma once

#include <DendyEngine/Components/GameComponents.h>
#include <DendyEngine/Scene.h>

#include <glm/glm.hpp>


namespace DendyEngine
{

class CMovementSystem
{
protected:
    CScene* m_pScene{nullptr};

public:
    CMovementSystem(CScene* pScene): m_pScene(pScene) {}
    ~CMovementSystem() {}
    void MoveToOrientationWithVelocity(CGameObject* pGameObject, float const& deltaTime, bool isDeccelerating) const;

    void UpdatePositionOfMovingGameObject(CGameObject* pGameObject, float const& deltaTime) const;

};

}