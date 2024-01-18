#pragma once

#include "DendyEngine/GameComponent.h"
#include "DendyCommon/Serial.h"

#include <string>
#include <unordered_map>

namespace DendyEngine
{

class IGameObject
{
protected:
    DendyCommon::CSerial m_Serial;
    std::unordered_map<std::string,IGameComponent*> m_GameComponentsMapByComponentType;

public:
    IGameObject(std::string name);
    ~IGameObject();

    virtual void Update() = 0;

    void AddComponent(IGameComponent* pComponent) { m_GameComponentsMapByComponentType[pComponent->GetComponentTypeName()] = pComponent; }
    IGameComponent* GetComponent(std::string componentTypeName);
};

}