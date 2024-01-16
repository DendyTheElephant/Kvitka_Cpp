#include "DendyEngine/Terrain.h"
#include "DendyCommon/Logger.h"

#include <algorithm>

DendyEngine::CTerrain::CTerrain()
{
    m_HeightsArray.fill(0.0f);
    m_HeightsArray.at(2000*1000+1000) = 500.0f;
}

DendyEngine::CTerrain::~CTerrain()
{
    
}

float DendyEngine::CTerrain::GetHeightAtPosition(glm::vec2 const& position) const
{
    LOG_CALLSTACK_PUSH(__FILE__,__LINE__,__PRETTY_FUNCTION__);
//#if defined(_DEBUG)
    if (std::abs(position.x) > static_cast<float>(c_TerrainSize)/2.0*m_Scale)
        LOG_CRITICAL_ERROR("position.x value ["+std::to_string(position.x)+"] out of terrain!");
    if (std::abs(position.y) > static_cast<float>(c_TerrainSize)/2.0*m_Scale)
        LOG_CRITICAL_ERROR("position.y value ["+std::to_string(position.y)+"] out of terrain!");
//#endif // _DEBUG

    int FloorX = static_cast<int>(std::floor((position.x+static_cast<float>(c_TerrainSize)/2.0)/m_Scale));
    int FloorY = static_cast<int>(std::floor((position.y+static_cast<float>(c_TerrainSize)/2.0)/m_Scale));
    float Height00 = m_HeightsArray.at(FloorY*c_TerrainSize+FloorX);
    float Height10 = m_HeightsArray.at(FloorY*c_TerrainSize+(FloorX+1));
    float Height01 = m_HeightsArray.at((FloorY+1)*c_TerrainSize+(FloorX));
    float Height11 = m_HeightsArray.at((FloorY+1)*c_TerrainSize+(FloorX+1));

    LOG_CALLSTACK_POP();
    return Height00;
}

float DendyEngine::CTerrain::GetHeightAtPosition(glm::vec3 const& position) const
{
    return GetHeightAtPosition(glm::vec2(position.x, position.z));
}
