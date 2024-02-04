#pragma once

#include <DendyEngine/Components/GameComponents.h>

#include <glm/glm.hpp>


namespace DendyEngine
{

class CTerrainSystem
{
protected:

public:
    CTerrainSystem() {};
    ~CTerrainSystem() {};

    void InitialiseTerrainChunkFromHeighmap(Components::STerrainChunk* pTerrainChunk, std::string const& heightmapFileName) const;
    glm::vec3 GetWorldPositionFromScenePosition(Components::STerrainChunk* pTerrainChunk, glm::vec2 const& scenePosition) const;
};

}