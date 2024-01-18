#include "DendyEngine/Actor.h"

DendyEngine::CActor::CActor(std::string name):
IGameObject(name)
{
    
}

DendyEngine::CActor::~CActor()
{
    IGameObject::~IGameObject();
}

void DendyEngine::CActor::Update()
{
    
}