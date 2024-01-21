// #pragma once

// #include <DendyCommon/Logger.h>
// #include <DendyEngine/ECS/GameComponents.h>

// #include <string>
// #include <unordered_map>
// #include <memory>
// #include <sstream>


// namespace DendyEngine
// {

// class IGameComponent;

// template<class TGameComponent>
// class IGameSystem;

// class CGameObject
// {
// protected:
//     friend class std::hash<DendyEngine::CGameObject>;
//     inline static std::size_t s_UIDIncrement = 1;
    
//     const std::size_t m_UID{0};
//     std::string m_Name{""};
//     std::unordered_map<std::string,IGameComponent*> m_GameComponentsMapByComponentTypeName;

// public:
//     // Forbid contructor by copy
//     CGameObject(CGameObject& original) = delete;
//     CGameObject& operator=(CGameObject const&) = delete;

//     CGameObject(std::string name);
//     ~CGameObject();

//     size_t const GetUID() const { return m_UID; }

//     //void Update();

//     bool operator==(CGameObject const& other) const
//     { 
//         return (m_UID == other.m_UID);
//     }

//     // For debug
//     friend std::ostream& operator<<(std::ostream& os, CGameObject const& gameObject) 
//     {
//         os << std::string("[CGameObject]<") << gameObject.m_Name << std::string("|") << std::to_string(gameObject.m_UID) << std::string(">");
//         return os;
//     }
//     // friend std::string operator std::string() 
//     // {
//     //     return std::string("{") + gameObject.m_Name + std::string("|") + std::to_string(gameObject.m_UID) + std::string("}"; 
//     // }

//     void AddComponent(std::string const& componentTypeName, IGameComponent* pComponent)
//     {
//         LOG_CALLSTACK_PUSH(__FILE__,__LINE__,__PRETTY_FUNCTION__);

//         //std::string ComponentTypeName = typeid(pComponent).name();
//         if (m_GameComponentsMapByComponentTypeName.contains(componentTypeName))
//         {
//             std::ostringstream SS;
//             SS << "On " << *this << " try to AddComponent<" << componentTypeName << "> but insertion failed: component already there!";
//             LOG_CRITICAL_ERROR(SS.str());
//         }

//         //m_GameComponentsMapByComponentTypeName.insert({ComponentTypeName, std::move(pComponent)});
//         m_GameComponentsMapByComponentTypeName.insert({componentTypeName, pComponent});
        
//         LOG_CALLSTACK_POP();
//     }

//     template<class TGameComponent>
//     bool HasComponent() const
//     {
//         //std::string ComponentTypeName = TGameComponent::GetComponentTypeName();
//         std::string ComponentTypeName = typeid(TGameComponent).name();
//         auto it_Component = m_GameComponentsMapByComponentTypeName.find(ComponentTypeName);
//         if ( it_Component == m_GameComponentsMapByComponentTypeName.end() )
//             return false;
//         else
//             return true;
//     }

//     template<class TGameComponent>
//     TGameComponent* GetComponent()
//     {
//         LOG_CALLSTACK_PUSH(__FILE__,__LINE__,__PRETTY_FUNCTION__);

//         //std::string ComponentTypeName = TGameComponent::GetComponentTypeName();
//         std::string ComponentTypeName = typeid(TGameComponent).name();
//         auto it_Component = m_GameComponentsMapByComponentTypeName.find(ComponentTypeName);
//         if ( it_Component == m_GameComponentsMapByComponentTypeName.end() )
//         {
//             std::ostringstream SS;
//             SS << "On CGameObject" << *this << " try to GetComponent<" << ComponentTypeName << "> but doesn't exist!";
//             LOG_CRITICAL_ERROR(SS.str());
//             return nullptr;
//         }
//         else
//         {
//             LOG_CALLSTACK_POP();
//             return static_cast<TGameComponent*>(it_Component->second);
//         }
//     }
// };

// }

// // For Hashable functions
// namespace std
// {
// template <>
// struct hash<DendyEngine::CGameObject>
// {
//     auto operator()(const DendyEngine::CGameObject &gameObject) const -> size_t {return hash<size_t>()(gameObject.m_UID);}
// };

// }  // namespace std