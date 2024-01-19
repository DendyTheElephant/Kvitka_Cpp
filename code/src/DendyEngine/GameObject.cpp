#include "DendyEngine/GameObject.h"

# include <iostream>

DendyEngine::CGameObject::CGameObject(CGameObject& original)
{
    std::cout << "CopyCtor of CGameObject("<< m_Serial << ") called." << std::endl;
}

DendyEngine::CGameObject::CGameObject(std::string name)
{
    m_Serial.SetName(name);
    std::cout << "Ctor of CGameObject("<< m_Serial.GetName() << m_Serial << ") called." << std::endl;
}

DendyEngine::CGameObject::~CGameObject()
{
    std::cout << "Detructor of CGameObject("<< m_Serial << ") called." << std::endl;
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