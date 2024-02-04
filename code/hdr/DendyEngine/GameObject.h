#pragma once

#include <DendyEngine/Components/GameComponentBase.h>
#include <DendyCommon/Logger.h>
//#include <DendyEngine/Components/GameComponents.h>

#include <unordered_map>
#include <memory>

namespace DendyEngine
{

class CGameObject
{
protected:
    uint32_t m_Id;
    const std::string m_Name{""};
    std::unordered_map<Components::EGameComponentType, std::unique_ptr<Components::IGameComponent>> m_pOwnedGameComponentsMapByGameComponentType;
    Components::SScenePose m_ScenePose;

public:
    // Forbid contructor by copy
    CGameObject(CGameObject& original) = delete;
    CGameObject& operator=(CGameObject const&) = delete;

    CGameObject(uint32_t id, std::string name, glm::vec2 scenePosition, glm::vec2 orientation): m_Id(id), m_Name(name), m_ScenePose(scenePosition,orientation) {}
    ~CGameObject() {}

    inline uint32_t const GetId() const { return m_Id; }
    inline Components::SScenePose* GetScenePose() { return &m_ScenePose; }
    inline glm::vec2 GetScenePosition() { return m_ScenePose.Position; }
    inline bool HasComponent(Components::EGameComponentType componentType) const { return m_pOwnedGameComponentsMapByGameComponentType.count(componentType) != 0; }

    template<class TGameComponent>
    TGameComponent* AddComponent()
    {
        auto pComponent = std::make_unique<TGameComponent>();
        TGameComponent* pRefComponent = pComponent.get();
        m_pOwnedGameComponentsMapByGameComponentType.insert({TGameComponent::Type,std::move(pComponent)});
        return pRefComponent;
    }

    template<class TGameComponent>
    TGameComponent* GetComponent()
    {
        LOG_CALLSTACK_PUSH(__FILE__,__LINE__,__PRETTY_FUNCTION__);
        if (m_pOwnedGameComponentsMapByGameComponentType.count(TGameComponent::Type) == 0)
            //LOG_CRITICAL_ERROR("GetComponent Enum Type " + std::string(TGameComponent::Type) + " of object " + std::string(*this) + " not found!");
            LOG_CRITICAL_ERROR("GetComponent not found");
        TGameComponent* pComponent = static_cast<TGameComponent*>(m_pOwnedGameComponentsMapByGameComponentType.at(TGameComponent::Type).get());
        LOG_CALLSTACK_POP();
        return pComponent;
    }

    template<class TGameComponent>
    TGameComponent* RemoveComponent()
    {
        m_pOwnedGameComponentsMapByGameComponentType.erase(TGameComponent::Type);
    }

    // For Hashable functions
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
    friend std::string operator+(std::string const& str, CGameObject const& gameObject)
    {
        return str + std::string("[CGameObject]<") + gameObject.m_Name + std::string("|") + std::to_string(gameObject.m_Id) + std::string(">");
    }
    friend std::string operator+(CGameObject const& gameObject, std::string const& str)
    {
        return std::string("[CGameObject]<") + gameObject.m_Name + std::string("|") + std::to_string(gameObject.m_Id) + std::string(">") + str;
    }
};

}

// For Hashable functions
namespace std
{
    template <>
    struct hash<DendyEngine::CGameObject>
    {
        auto operator()(DendyEngine::CGameObject const& key) const -> size_t {return hash<size_t>()(key.GetId());}
    };
} 