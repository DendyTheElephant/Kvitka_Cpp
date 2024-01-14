#pragma once

#include "DendyEngine/GameObject.h"

namespace DendyEngine
{

class CActor: public IGameObject
{
protected:
    

public:
    CActor(std::string name);
    ~CActor();

    virtual void Update();
};

}