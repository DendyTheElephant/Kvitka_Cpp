#pragma once

#include <DendyEngine/Components/GameComponents.h>

#include <glm/glm.hpp>


namespace DendyEngine
{

class CTerrainSystem
{
protected:
    //inline float const _GetHeight(size_t const& x, size_t const& y) const {return static_cast<float>(m_HeightsArray.at(y*(c_TerrainSize+1)+x))/65535.0f;} /// Between 0..1

public:
    CTerrainSystem() {};
    ~CTerrainSystem() {};

    void InitialiseTerrainChunkFromHeighmap(Components::STerrainChunk* pTerrainChunk, std::string const& heightmapFileName) const;

    glm::vec3 GetWorldPositionFromScenePosition(Components::STerrainChunk* pTerrainChunk, glm::vec2 const& scenePosition) const;

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