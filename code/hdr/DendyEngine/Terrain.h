#pragma once

#include <glm/glm.hpp>

#include <array>
#include <memory>

namespace DendyEngine
{

class CTerrain
{
protected:
    static constexpr float c_Scale{1.0f};
    static constexpr size_t c_TerrainSize{4096};
    static constexpr float c_TerrainMaxHeight{40};
    std::array<float,c_TerrainSize*c_TerrainSize> m_HeightsArray;

protected:
    inline float const _GetHeight(size_t const& index) const {return m_HeightsArray.at(index);}
    inline float const _GetHeight(size_t const& x, size_t const& y) const {return m_HeightsArray.at(y*c_TerrainSize+x);}
    inline void _SetHeight(size_t const& x, size_t const& y, float value) { m_HeightsArray.at(y*c_TerrainSize+x) = value; }

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

    constexpr const float* GetData() const {return m_HeightsArray.data();}
    constexpr const float GetScale() const {return c_Scale;}
    constexpr const size_t GetTerrainSize() const {return c_TerrainSize;}
    constexpr const float GetTerrainMaxHeight() const {return c_TerrainMaxHeight;}

};

}