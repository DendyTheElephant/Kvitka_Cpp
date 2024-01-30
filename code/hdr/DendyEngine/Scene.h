#pragma once

#include <DendyEngine/GameObject.h>
#include <DendyEngine/Components/GameComponentBase.h>
#include <DendyCommon/Logger.h>
#include <DendyEngine/Position2DHash.h>

#include <glm/glm.hpp>

#include <array>
#include <memory>
#include <vector>
#include <unordered_set>

namespace DendyEngine
{

class CScene
{
public:
    inline static constexpr uint16_t c_ChunkSize{50};
protected:
    inline static uint32_t m_GameComponentCount = 0;
    std::vector<std::unique_ptr<CGameObject>> m_pOwnedGameObjectsVec;
    std::unordered_map<SPosition2DHash<c_ChunkSize>,std::unordered_set<CGameObject*>> m_pGameObjectReferencesSetByChunk;


public:
    CScene() {}
    ~CScene() {}

    CGameObject* AddGameObject(std::string name, glm::vec2 scenePosition, glm::vec2 sceneOrientation = glm::vec2(0,1))
    {
        LOG_CALLSTACK_PUSH(__FILE__,__LINE__,__PRETTY_FUNCTION__);

        m_pOwnedGameObjectsVec.push_back( std::make_unique<CGameObject>(m_pOwnedGameObjectsVec.size(), name, scenePosition, sceneOrientation) );
        SPosition2DHash<c_ChunkSize> ChunkHash = SPosition2DHash<c_ChunkSize>(m_pOwnedGameObjectsVec.back()->GetScenePose()->Position);
        if (m_pGameObjectReferencesSetByChunk.count(ChunkHash) == 0)
        {
            std::unordered_set<CGameObject*> GameObjectReferencesSet;
            GameObjectReferencesSet.insert(m_pOwnedGameObjectsVec.back().get());
            m_pGameObjectReferencesSetByChunk.insert( {ChunkHash, GameObjectReferencesSet} );
        }
        else
        {
            std::unordered_set<CGameObject*>* pGameObjectReferencesSet = &m_pGameObjectReferencesSetByChunk.at(ChunkHash);
            pGameObjectReferencesSet->insert(m_pOwnedGameObjectsVec.back().get());
        }


        LOG_CALLSTACK_POP();
        return m_pOwnedGameObjectsVec.back().get();
    }

    void RemoveGameObject(CGameObject* pGameObject)
    {
        LOG_CALLSTACK_PUSH(__FILE__,__LINE__,__PRETTY_FUNCTION__);

        auto ChunkHash = SPosition2DHash<c_ChunkSize>(pGameObject->GetScenePose()->Position);
        std::unordered_set<CGameObject*>* pGameObjectReferencesSet = &m_pGameObjectReferencesSetByChunk.at(ChunkHash);
        pGameObjectReferencesSet->erase(pGameObject);

        auto IteratorForErase = std::find_if(std::begin(m_pOwnedGameObjectsVec), std::end(m_pOwnedGameObjectsVec), [pGameObject](std::unique_ptr<CGameObject>& p) { return p.get() == pGameObject; });
        m_pOwnedGameObjectsVec.erase( IteratorForErase );

        LOG_CALLSTACK_POP();
    }

    std::unordered_set<CGameObject*> GetGameObjectsSetNearScenePosition(glm::vec2 const& scenePosition)
    {
        LOG_CALLSTACK_PUSH(__FILE__,__LINE__,__PRETTY_FUNCTION__);

        SPosition2DHash<c_ChunkSize> ChunkHash = SPosition2DHash<c_ChunkSize>(scenePosition);

        std::unordered_set<CGameObject*> Result;
        for (int x=-1; x<2; x++)
        {
            for (int y=-1; y<2; y++)
            {
                SPosition2DHash<c_ChunkSize> ChunkHashNeighboors = SPosition2DHash<c_ChunkSize>(ChunkHash);
                ChunkHashNeighboors.x + x;
                ChunkHashNeighboors.y + y;
                auto GameObjectSetInChunk = m_pGameObjectReferencesSetByChunk.at(ChunkHashNeighboors);
                for (auto pGameObjectReferences : GameObjectSetInChunk)
                {
                    Result.insert(pGameObjectReferences);
                }
            }
        }

        LOG_CALLSTACK_POP();
        return Result;
    }

    template<class... TGameComponents>
    std::unordered_set<CGameObject*> GetGameObjectsSetNearScenePositionWithComponents(glm::vec2 const& scenePosition)
    {
        std::unordered_set<CGameObject*> Result;
        std::vector<Components::EGameComponentType> RequiredComponentTypesVec;
        ((RequiredComponentTypesVec.push_back(TGameComponents::Type)),...);
        auto GameObjectsSetFromChunk = GetGameObjectsSetNearScenePosition(scenePosition);
        for (auto pGameObject : GameObjectsSetFromChunk)
        {
            bool IsSuitable = true;
            for (auto RequiredComponent : RequiredComponentTypesVec)
            {
                if ( pGameObject->HasComponent(RequiredComponent) == false )
                {
                    IsSuitable = false;
                    break;
                }
            }
            if (IsSuitable)
            {
                Result.insert( pGameObject );
            }
        }
        return Result;
    }


    // void LoadFromFiles(std::string fileNameHeightmap);

    // inline constexpr float GetMaxHeight() const {return c_TerrainMaxHeight;}
    // float GetHeightAtPosition(glm::vec2 const& position) const;
    // float GetHeightAtPosition(glm::vec3 const& position) const;
    // glm::vec3 GetNormalAtPosition(glm::vec2 const& position) const;
    // glm::vec3 GetNormalAtPosition(glm::vec3 const& position) const;
    // float ComputeDistance(glm::vec3 positionStart, glm::vec3 positionEnd) const {return 0.0f;}

    // constexpr const uint16_t* GetData() const {return m_HeightsArray.data();}
    // std::vector<glm::vec3> GetWorldPositionsOfChunk(glm::vec2 const& min, glm::vec2 const& max);
    // constexpr const float GetScale() const {return c_Scale;}
    // constexpr const size_t GetTerrainSize() const {return c_TerrainSize;}
    // constexpr const float GetTerrainMaxHeight() const {return c_TerrainMaxHeight;}

};

}