#pragma once

#include <DendyEngine/Components/GameComponents.h>
#include <DendyEngine/Scene.h>

#include <glm/glm.hpp>


namespace DendyEngine
{


struct SShape
{
    enum class EShapeType
    {
        Circle,
        AABBox,
        Convex
    };

    std::vector<glm::vec2> ConvexPositionsVec;
    glm::vec2 AABBoxMin;
    glm::vec2 AABBoxMax;
    float OuterRadius{1.5};
    
    SShape() {}
};


class CCollisionSystem
{
protected:
    CScene* m_pScene{nullptr};

public:
    CCollisionSystem(CScene* pScene): m_pScene(pScene) {}
    ~CCollisionSystem() {}

    void IsColliding(glm::vec2 const& segmentStart, glm::vec2 const& segmentEnd, SShape* pShape) {}
    // void IsColliding(CGameObject* pGameObject) const;
    // void ResolveCollision(CGameObject* pGameObject) const;
};

}