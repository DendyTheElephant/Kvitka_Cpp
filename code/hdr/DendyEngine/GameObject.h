#pragma once

#include <string>

namespace DendyEngine
{

class CGameObject
{
protected:
    unsigned int m_Id;
    std::string m_Name;

protected:
    void _UpdateGamepads();

public:
    CGameObject(unsigned int id, std::string name);

    void UpdateInputs();
};

}