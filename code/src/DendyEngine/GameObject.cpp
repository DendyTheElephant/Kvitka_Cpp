#include "DendyEngine/GameObject.h"

DendyEngine::CGameObject::CGameObject(std::string name)
{
    m_Serial.SetName(name);
}

DendyEngine::CGameObject::~CGameObject()
{
    
}

DendyEngine::IGameComponent* DendyEngine::CGameObject::GetComponent(std::string componentTypeName)
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