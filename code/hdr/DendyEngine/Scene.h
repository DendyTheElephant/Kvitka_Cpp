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

    Components::STerrainChunk* AddTerrainChunk(std::string const& name, glm::vec2 const& scenePosition);
    Components::STerrainChunk* GetTerrainChunkAtScenePosition(glm::vec2 const& scenePosition);
    std::unordered_set<Components::STerrainChunk*> GetTerrainChunksSetNearScenePosition(glm::vec2 const& scenePosition);

    CGameObject* AddGameObject(std::string const& name, glm::vec2 const& scenePosition, glm::vec2 const& sceneOrientation = glm::vec2(0,1));
    void UpdateGameObjectScenePosition(CGameObject* pGameObject, glm::vec2 const& scenePosition);
    void RemoveGameObject(CGameObject* pGameObject);

    std::unordered_set<CGameObject*> GetGameObjectsSetNearScenePosition(glm::vec2 const& scenePosition);
    template<class... TGameComponents>
    std::unordered_set<CGameObject*> GetGameObjectsSetNearScenePositionWithComponents(glm::vec2 const& scenePosition);
    template<class... TGameComponents>
    std::unordered_set<CGameObject*> GetGameObjectsSetWithComponents();
};



#include <DendyEngine/Scene.inl>
}