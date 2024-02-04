#pragma once

#include <DendyEngine/GameObject.h>
#include <DendyEngine/Components/GameComponentBase.h>
#include <DendyCommon/Logger.h>
#include <DendyEngine/Position2DHash.h>
#include <DendyEngine/Definitions.h>

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

protected:
    inline static uint32_t m_GameComponentCount = 0;
    std::vector<std::unique_ptr<CGameObject>> m_pOwnedGameObjectsVec;
    std::unordered_map<SPosition2DHash<Definitions::c_ChunkSize>,std::unordered_set<CGameObject*>> m_pGameObjectReferencesSetByChunk;
    std::unordered_map<SPosition2DHash<Definitions::c_ChunkSize>,Components::STerrainChunk*> m_pTerrainChunkReferencesByChunk;


public:
    CScene() {}
    ~CScene() {}

    Components::STerrainChunk* AddTerrainChunk(std::string const& name, glm::vec2 const& scenePosition)
    {
        LOG_CALLSTACK_PUSH(__FILE__,__LINE__,__PRETTY_FUNCTION__);

        m_pOwnedGameObjectsVec.push_back( std::make_unique<CGameObject>(m_pOwnedGameObjectsVec.size(), name, scenePosition, glm::vec2(0,1)) );
        Components::STerrainChunk* pTerrainChunk = m_pOwnedGameObjectsVec.back()->AddComponent<Components::STerrainChunk>();
        pTerrainChunk->TerrainId = m_pOwnedGameObjectsVec.back().get()->GetId();
        auto ChunkHash = SPosition2DHash<Definitions::c_ChunkSize>(m_pOwnedGameObjectsVec.back()->GetScenePose()->Position);
        if (m_pTerrainChunkReferencesByChunk.count(ChunkHash) == 0)
        {
            m_pTerrainChunkReferencesByChunk.insert( {ChunkHash, pTerrainChunk} );
        }
        else
        {
            LOG_CRITICAL_ERROR("Trying to add Terrain Chunk at position ["+std::to_string(scenePosition.x)+","+std::to_string(scenePosition.y)+"] but "+ *(m_pOwnedGameObjectsVec.back().get())+" already there");
        }

        LOG_CALLSTACK_POP();
        return pTerrainChunk;
    }

    Components::STerrainChunk* GetTerrainChunkAtScenePosition(glm::vec2 const& scenePosition)
    {
        LOG_CALLSTACK_PUSH(__FILE__,__LINE__,__PRETTY_FUNCTION__);

        SPosition2DHash<Definitions::c_ChunkSize> ChunkHash = SPosition2DHash<Definitions::c_ChunkSize>(scenePosition);

        if (m_pTerrainChunkReferencesByChunk.count(ChunkHash) == 0)
        {
            LOG_CRITICAL_ERROR("No Terrain Chunk at position ["+std::to_string(scenePosition.x)+","+std::to_string(scenePosition.y)+"]");
        }

        LOG_CALLSTACK_POP();
        return m_pTerrainChunkReferencesByChunk.at(ChunkHash);
    }

    std::unordered_set<Components::STerrainChunk*> GetTerrainChunksSetNearScenePosition(glm::vec2 const& scenePosition)
    {
        LOG_CALLSTACK_PUSH(__FILE__,__LINE__,__PRETTY_FUNCTION__);

        SPosition2DHash<Definitions::c_ChunkSize> ChunkHash = SPosition2DHash<Definitions::c_ChunkSize>(scenePosition);

        std::unordered_set<Components::STerrainChunk*> Result;
        for (int x=-1; x<2; x++)
        {
            for (int y=-1; y<2; y++)
            {
                SPosition2DHash<Definitions::c_ChunkSize> ChunkHashNeighboors = SPosition2DHash<Definitions::c_ChunkSize>(ChunkHash);
                ChunkHashNeighboors.x += x;
                ChunkHashNeighboors.y += y;
                if (m_pTerrainChunkReferencesByChunk.count(ChunkHashNeighboors) > 0)
                {
                    auto pTerrainChunk = m_pTerrainChunkReferencesByChunk.at(ChunkHashNeighboors);
                    Result.insert(pTerrainChunk);
                }
            }
        }

        LOG_CALLSTACK_POP();
        return Result;
    }

    CGameObject* AddGameObject(std::string const& name, glm::vec2 const& scenePosition, glm::vec2 const& sceneOrientation = glm::vec2(0,1))
    {
        LOG_CALLSTACK_PUSH(__FILE__,__LINE__,__PRETTY_FUNCTION__);

        m_pOwnedGameObjectsVec.push_back( std::make_unique<CGameObject>(m_pOwnedGameObjectsVec.size(), name, scenePosition, sceneOrientation) );
        SPosition2DHash<Definitions::c_ChunkSize> ChunkHash = SPosition2DHash<Definitions::c_ChunkSize>(m_pOwnedGameObjectsVec.back()->GetScenePose()->Position);
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

    void UpdateGameObjectScenePosition(CGameObject* pGameObject, glm::vec2 const& scenePosition)
    {
        LOG_CALLSTACK_PUSH(__FILE__,__LINE__,__PRETTY_FUNCTION__);

        auto OldChunkHash = SPosition2DHash<Definitions::c_ChunkSize>(pGameObject->GetScenePose()->Position);
        auto ChunkHash = SPosition2DHash<Definitions::c_ChunkSize>(scenePosition);

        if (OldChunkHash != ChunkHash)
        {
            m_pGameObjectReferencesSetByChunk.at(OldChunkHash).erase(pGameObject);
            if (m_pGameObjectReferencesSetByChunk.count(ChunkHash) == 0)
            {
                std::unordered_set<CGameObject*> GameObjectReferencesSet;
                GameObjectReferencesSet.insert(pGameObject);
                m_pGameObjectReferencesSetByChunk.insert( {ChunkHash, GameObjectReferencesSet} );
            }
            else
            {
                std::unordered_set<CGameObject*>* pGameObjectReferencesSet = &m_pGameObjectReferencesSetByChunk.at(ChunkHash);
                pGameObjectReferencesSet->insert(pGameObject);
            }
        }

        pGameObject->GetScenePose()->Position = scenePosition;
        LOG_CALLSTACK_POP();
    }

    void RemoveGameObject(CGameObject* pGameObject)
    {
        LOG_CALLSTACK_PUSH(__FILE__,__LINE__,__PRETTY_FUNCTION__);

        auto ChunkHash = SPosition2DHash<Definitions::c_ChunkSize>(pGameObject->GetScenePose()->Position);
        std::unordered_set<CGameObject*>* pGameObjectReferencesSet = &m_pGameObjectReferencesSetByChunk.at(ChunkHash);
        pGameObjectReferencesSet->erase(pGameObject);

        auto IteratorForErase = std::find_if(std::begin(m_pOwnedGameObjectsVec), std::end(m_pOwnedGameObjectsVec), [pGameObject](std::unique_ptr<CGameObject>& p) { return p.get() == pGameObject; });
        m_pOwnedGameObjectsVec.erase( IteratorForErase );

        LOG_CALLSTACK_POP();
    }

    std::unordered_set<CGameObject*> GetGameObjectsSetNearScenePosition(glm::vec2 const& scenePosition)
    {
        LOG_CALLSTACK_PUSH(__FILE__,__LINE__,__PRETTY_FUNCTION__);

        SPosition2DHash<Definitions::c_ChunkSize> ChunkHash = SPosition2DHash<Definitions::c_ChunkSize>(scenePosition);

        std::unordered_set<CGameObject*> Result;
        for (int x=-1; x<2; x++)
        {
            for (int y=-1; y<2; y++)
            {
                SPosition2DHash<Definitions::c_ChunkSize> ChunkHashNeighboors = SPosition2DHash<Definitions::c_ChunkSize>(ChunkHash);
                ChunkHashNeighboors.x += x;
                ChunkHashNeighboors.y += y;
                if (m_pGameObjectReferencesSetByChunk.count(ChunkHashNeighboors) > 0)
                {
                    auto GameObjectSetInChunk = m_pGameObjectReferencesSetByChunk.at(ChunkHashNeighboors);
                    for (auto pGameObjectReferences : GameObjectSetInChunk)
                    {
                        Result.insert(pGameObjectReferences);
                    }
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

    template<class... TGameComponents>
    std::unordered_set<CGameObject*> GetGameObjectsSetWithComponents()
    {
        std::unordered_set<CGameObject*> Result;
        std::vector<Components::EGameComponentType> RequiredComponentTypesVec;
        ((RequiredComponentTypesVec.push_back(TGameComponents::Type)),...);
        for (size_t iGameObject=0; iGameObject<m_pOwnedGameObjectsVec.size(); iGameObject++)
        {
            bool IsSuitable = true;
            for (auto RequiredComponent : RequiredComponentTypesVec)
            {
                if ( m_pOwnedGameObjectsVec.at(iGameObject)->HasComponent(RequiredComponent) == false )
                {
                    IsSuitable = false;
                    break;
                }
            }
            if (IsSuitable)
            {
                Result.insert( m_pOwnedGameObjectsVec.at(iGameObject).get() );
            }
        }
        return Result;
    }
};

}