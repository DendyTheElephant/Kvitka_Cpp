#pragma once

#include "DendyEngine/GameComponent.h"
#include "DendyCommon/Serial.h"

#include <string>
#include <unordered_map>

namespace DendyEngine
{

class CGameObject
{
protected:
    DendyCommon::CSerial m_Serial;
    std::unordered_map<std::string,IGameComponent*> m_GameComponentsMapByComponentType;

public:
    CGameObject(CGameObject& original);
    CGameObject(std::string name);
    ~CGameObject();

    std::string GetName() const {return m_Serial.GetName();}
    DendyCommon::CSerial GetSerial() const {return m_Serial;}

    void Update();

    void AddComponent(IGameComponent* pComponent) { m_GameComponentsMapByComponentType[pComponent->GetComponentTypeName()] = pComponent; }
    IGameComponent* GetComponent(std::string componentTypeName);
};

}