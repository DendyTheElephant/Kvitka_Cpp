#pragma once

namespace DendyEngine::ECS
{

class CGameObject
{
protected:
    friend class CECSEngine;

    uint32_t m_Id{0};
    std::string m_Name{""};
    std::unordered_map<EGameComponentType,uint32_t> m_GameComponentIdMapByGameComponentType;

public:
    // Forbid contructor by copy
    CGameObject(CGameObject& original) = delete;
    CGameObject& operator=(CGameObject const&) = delete;

    CGameObject(std::string name): m_Name(name) {}
    ~CGameObject() {}

    size_t const GetId() const { return m_Id; }

    bool operator==(CGameObject const& other) const
    { 
        return (m_Id == other.m_Id);
    }

    // For debug
    friend std::ostream& operator<<(std::ostream& os, CGameObject const& gameObject) 
    {
        os << std::string("[CGameObject]<") << gameObject.m_Name << std::string("|") << std::to_string(gameObject.m_Id) << std::string(">");
        return os;
    }
};

}