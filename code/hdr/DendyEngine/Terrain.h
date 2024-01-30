#pragma once

#include <glm/glm.hpp>

#include <array>
#include <memory>
#include <vector>

namespace DendyEngine
{

class CTerrain
{
protected:
    static constexpr float c_Scale{50.0f};
    static constexpr size_t c_TerrainSize{3};
    static constexpr float c_TerrainMaxHeight{100.0f};
    std::array<uint16_t,(c_TerrainSize+1)*(c_TerrainSize+1)> m_HeightsArray;

protected:
    inline float const _GetHeight(size_t const& x, size_t const& y) const {return static_cast<float>(m_HeightsArray.at(y*(c_TerrainSize+1)+x))/65535.0f;} /// Between 0..1
    inline void _SetHeight(size_t const& x, size_t const& y, float value) { m_HeightsArray.at(y*(c_TerrainSize+1)+x) = static_cast<uint16_t>(value*65535.0f); } /// Value must be between 0..1

public:
    CTerrain();
    ~CTerrain();

    void LoadFromFiles(std::string fileNameHeightmap);

    inline constexpr float GetMaxHeight() const {return c_TerrainMaxHeight;}
    float GetHeightAtPosition(glm::vec2 const& position) const;
    float GetHeightAtPosition(glm::vec3 const& position) const;
    glm::vec3 GetNormalAtPosition(glm::vec2 const& position) const;
    glm::vec3 GetNormalAtPosition(glm::vec3 const& position) const;
    float ComputeDistance(glm::vec3 positionStart, glm::vec3 positionEnd) const {return 0.0f;}

    constexpr const uint16_t* GetData() const {return m_HeightsArray.data();}
    std::vector<glm::vec3> GetWorldPositionsOfChunk(glm::vec2 const& min, glm::vec2 const& max);
    constexpr const float GetScale() const {return c_Scale;}
    constexpr const size_t GetTerrainSize() const {return c_TerrainSize;}
    constexpr const float GetTerrainMaxHeight() const {return c_TerrainMaxHeight;}

};

}