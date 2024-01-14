#include "DendyEngine/Actor.h"

DendyEngine::CActor::CActor(std::string name):
IGameObject(name)
{
    m_Name = name;
}

DendyEngine::CActor::~CActor()
{
    IGameObject::~IGameObject();
}

void DendyEngine::CActor::Update()
{
    
}