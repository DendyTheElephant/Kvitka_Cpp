#pragma once

#include "DendyEngine/GameComponent.h"

#include <string>
#include <unordered_map>

namespace DendyEngine
{

class IGameObject
{
protected:
    unsigned int m_Id{0};
    std::string m_Name;
    std::unordered_map<std::string,IGameComponent*> m_GameComponentsMapByComponentType;
    static unsigned int s_GameObjectIdIncrement;

public:
    IGameObject(std::string name);
    ~IGameObject();

    virtual void Update() = 0;

    void AddComponent(IGameComponent* pComponent) { m_GameComponentsMapByComponentType[pComponent->GetComponentTypeName()] = pComponent; }
    IGameComponent* GetComponent(std::string componentTypeName);
    static const unsigned int GetGameObjectIdIncrement() { return s_GameObjectIdIncrement; }
};

}