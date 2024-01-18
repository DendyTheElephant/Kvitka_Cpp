#include "DendyEngine/GameObject.h"

DendyEngine::IGameObject::IGameObject(std::string name)
{
    m_Serial.SetName(name);
}

DendyEngine::IGameObject::~IGameObject()
{
    
}

DendyEngine::IGameComponent* DendyEngine::IGameObject::GetComponent(std::string componentTypeName)
{
    auto it_Component = m_GameComponentsMapByComponentType.find(componentTypeName);
    if ( it_Component == m_GameComponentsMapByComponentType.end() )
    {
        return nullptr;
    }
    else
    {
        return it_Component->second;
    }
}