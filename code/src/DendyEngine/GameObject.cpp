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

