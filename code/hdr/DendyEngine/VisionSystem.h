#pragma once

#include <DendyEngine/Components/GameComponents.h>
#include <DendyEngine/Scene.h>

#include <glm/glm.hpp>


namespace DendyEngine
{

class CVisionSystem
{
protected:
    CScene* m_pScene{nullptr};

public:
    CVisionSystem(CScene* pScene): m_pScene(pScene) {}
    ~CVisionSystem() {}

    void UpdateVisibleGameObjectsVec(CGameObject* pGameObjectSeeing) const;

};

}