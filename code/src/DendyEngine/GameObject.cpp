#include "DendyEngine/GameObject.h"

unsigned int DendyEngine::IGameObject::s_GameObjectIdIncrement = 1;

DendyEngine::IGameObject::IGameObject(std::string name):
m_Id(s_GameObjectIdIncrement++), m_Name(name)
{

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