#pragma once

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <array>


namespace DendyEngine
{

class CTerrain
{
protected:
    float m_Scale{1.0f};
    static constexpr size_t c_TerrainSize{2000};
    std::array<float,c_TerrainSize*c_TerrainSize> m_HeightsArray;

protected:

public:
    CTerrain();
    ~CTerrain();

    float GetHeightAtPosition(glm::vec2 const& position) const;
    float GetHeightAtPosition(glm::vec3 const& position) const;
    float ComputeDistance(glm::vec3 positionStart, glm::vec3 positionEnd) const {return 0.0f;}


};

}