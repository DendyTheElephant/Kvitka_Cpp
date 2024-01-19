#pragma once

#include <PixPhetamine/RenderingSystem.h>
#include <DendyEngine/GameObject.h>
#include <unordered_set>
#include <memory>

namespace DendyEngine
{

class CSpatialNavigationComponent;

template<class TGameComponent>
class CGameSystem
{
protected:
    std::unordered_set<std::unique_ptr<TGameComponent>> m_ComponentsSet;

public:
    TGameComponent* CreateComponent(CGameObject* pGameObject)
    {
        std::unique_ptr<TGameComponent> pComponent = std::make_unique<TGameComponent>(pGameObject);
        TGameComponent* pRefComponent = pComponent.get();

        pGameObject->AddComponent(typeid(TGameComponent).name(), pRefComponent);

        m_ComponentsSet.emplace( std::move(pComponent) );
        return pRefComponent;
    }
    

    void Update();
};



class CRenderablePawn: public CGameSystem<CRenderablePawnComponent>
{
protected:
    PixPhetamine::CRenderingSystem* m_pRenderingSystem;

public:
    CRenderablePawn(PixPhetamine::CRenderingSystem* pRendering):m_pRenderingSystem(pRendering) {}
    void Update();
};

}