#pragma once

#include "DendyEngine/GameComponent.h"
#include "DendyCommon/Serial.h"

#include <string>
#include <unordered_map>
#include <memory>

namespace DendyEngine
{

class CGameObject
{
protected:
    DendyCommon::CSerial m_Serial;
    std::unordered_map<std::string,std::unique_ptr<IGameComponent>> m_GameComponentsMapByComponentType;

public:
    CGameObject(CGameObject& original);
    CGameObject(std::string name);
    ~CGameObject();

    std::string GetName() const {return m_Serial.GetName();}
    DendyCommon::CSerial GetSerial() const {return m_Serial;}

    void Update();

    template<class TGameComponent>
    void AddComponent(std::string const& name)
    {
        std::string ComponentTypeName = TGameComponent::GetComponentTypeName();

        std::unique_ptr<TGameComponent> pComponent = std::make_unique<TGameComponent>(name);
        m_GameComponentsMapByComponentType.insert(ComponentTypeName, std::move(pComponent));
    }



    template<class TGameComponent>
    TGameComponent* GetComponent(std::string const& componentTypeName)
    {
        return nullptr;
        // auto it_Component = m_GameComponentsMapByComponentType.find(componentTypeName);
        // if ( it_Component == m_GameComponentsMapByComponentType.end() )
        // {
        //     return nullptr;
        // }
        // else
        // {
        //     std::string ComponentTypeName = it_Component->second.get()->GetComponentTypeName();
        //     if (ComponentTypeName == "Transform")
        //         return static_cast<CTransformComponent*>(it_Component->second.get());
        // }
    }
};

}