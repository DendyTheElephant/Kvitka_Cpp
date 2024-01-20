#include <DendyEngine/ECS/GameObject.h>

# include <iostream>

DendyEngine::CGameObject::CGameObject(std::string name):
m_UID(s_UIDIncrement++),
m_Name(name)
{
    std::cout << "Constructor of "<< *this << std::endl;
}

DendyEngine::CGameObject::~CGameObject()
{
    m_GameComponentsMapByComponentTypeName.clear();
    std::cout << "Detructor of "<< *this << std::endl;
}

