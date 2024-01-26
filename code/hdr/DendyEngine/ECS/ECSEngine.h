#pragma once

#include <DendyEngine/ECS/GameComponents.h>
#include <DendyEngine/ECS/GameObject.h>

#include <vector>
#include <memory>
#include <unordered_map>

namespace DendyEngine::ECS
{

class CECSEngine
{
private:
    //inline static constexpr uint32_t c_MaxGameObjectCount{2048};
    inline static constexpr uint32_t c_MaxGameComponentCount{4096};
    std::vector<std::unique_ptr<CGameObject>> m_OwnedGameObjectVec;
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
        m_OwnedGameObjectVec.push_back( std::move(pGameObject) );
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
        for (uint32_t iGameObject=0; iGameObject<m_GameObjectCount; iGameObject++)
        {
            bool IsSuitable = true;
            for (auto RequiredComponent : RequiredComponentTypesVec)
            {
                auto ComponentsMapByType = m_OwnedGameObjectVec[iGameObject]->m_GameComponentIdMapByGameComponentType;
                if ( ComponentsMapByType.find(RequiredComponent) == ComponentsMapByType.end() )
                {
                    IsSuitable = false;
                    break;
                }
            }
            if (IsSuitable)
            {
                Result.push_back( m_OwnedGameObjectVec[iGameObject].get() );
            }
        }
        return Result;
    }

    std::vector<CGameObject*> GetAllGameObjectsVec() const
    {
        std::vector<CGameObject*> GameObjectReferencesVec;
        for (auto& pGameObject : m_OwnedGameObjectVec)
        {
            GameObjectReferencesVec.push_back(pGameObject.get());
        }
        return GameObjectReferencesVec;
    }
};

}