#pragma once

//#include <string>
//#include <iostream>
#include <vector>
#include <memory>
#include <unordered_map>

namespace DendyEngine::ECS
{

enum class EGameComponentType
{
    Transform,
    SimpleMesh
};

class IGameComponent
{
public:
    virtual ~IGameComponent() = default;
};

template<typename TGameComponent, EGameComponentType TGameComponentType>
class CGameComponent : public IGameComponent
{
public:
    static constexpr EGameComponentType Type = TGameComponentType;
};





struct STransform : CGameComponent<STransform,EGameComponentType::Transform>
{
    float X{0.0f};
};

struct SSimpleMesh : CGameComponent<SSimpleMesh,EGameComponentType::SimpleMesh>
{

};


class CECSEngine;

class CGameObject
{
protected:
    //friend class std::hash<DendyEngine::CGameObject>;
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


class CECSEngine
{
private:
    inline static constexpr uint32_t c_MaxGameObjectCount{2048};
    inline static constexpr uint32_t c_MaxGameComponentCount{4096};
    std::array<std::unique_ptr<CGameObject>,c_MaxGameObjectCount> m_OwnedGameObjectArray;
    std::array<std::unique_ptr<IGameComponent>,c_MaxGameComponentCount> m_OwnedGameComponentsArray;
    uint32_t m_GameComponentCount = 0;
    uint32_t m_GameObjectCount = 0;



public:
    CECSEngine(){}

    CGameObject* AddGameObject(std::string name)
    {
        auto pGameObject = std::make_unique<CGameObject>(name);
        CGameObject* pRefGameObject = pGameObject.get();
        m_GameObjectCount++;
        pRefGameObject->m_Id = m_GameObjectCount;
        m_OwnedGameObjectArray[m_GameObjectCount] = std::move(pGameObject);
        return pRefGameObject;
    }

    template<class TGameComponent>
    TGameComponent* AddComponent(CGameObject* pGameObject)
    {
        auto pComponent = std::make_unique<TGameComponent>();
        TGameComponent* pRefComponent = pComponent.get();
        pGameObject->m_GameComponentIdMapByGameComponentType.insert({TGameComponent::Type,m_GameComponentCount});
        m_OwnedGameComponentsArray[m_GameComponentCount++] = std::move(pComponent);
        return pRefComponent;
    }

    template<class TGameComponent>
    TGameComponent* GetComponent(CGameObject* pGameObject)
    {
        uint32_t ComponentId = pGameObject->m_GameComponentIdMapByGameComponentType.find(TGameComponent::Type)->second;
        TGameComponent* pComponent = static_cast<TGameComponent*>(m_OwnedGameComponentsArray[ComponentId].get());
        return pComponent;
    }

    //template<EGameComponentType... TComponentTypes>
    template<class... TGameComponents>
    std::vector<CGameObject*> GetGameObjectsVecWithComponents()
    {
        std::vector<CGameObject*> Result;
        std::vector<EGameComponentType> RequiredComponentTypesVec;
        ((RequiredComponentTypesVec.push_back(TGameComponents::Type)),...);
        for (uint32_t iGameObject=1; iGameObject<m_GameObjectCount+1; iGameObject++)
        {
            bool IsSuitable = true;
            for (auto RequiredComponent : RequiredComponentTypesVec)
            {
                auto ComponentsMapByType = m_OwnedGameObjectArray[iGameObject]->m_GameComponentIdMapByGameComponentType;
                if ( ComponentsMapByType.find(RequiredComponent) == ComponentsMapByType.end() )
                {
                    IsSuitable = false;
                    break;
                }
            }
            if (IsSuitable)
            {
                Result.push_back( m_OwnedGameObjectArray[iGameObject].get() );
            }
        }
        return Result;
    }
};

}