#include <DendyEngine/Terrain.h>
#include <DendyCommon/Logger.h>
#include <DendyCommon/Math.h>

//#include <CImg.h>
#include <lodepng.h>

#include <algorithm>
#include <random>
#include <iostream>

DendyEngine::CTerrain::CTerrain()
{
    LoadFromFiles("ressources/images/ruggedTerrainHeightmap4.png");
}

DendyEngine::CTerrain::~CTerrain()
{
    
}

void DendyEngine::CTerrain::LoadFromFiles(std::string fileNameHeightmap)
{
    LOG_CALLSTACK_PUSH(__FILE__,__LINE__,__PRETTY_FUNCTION__);

    // Load file and decode image.
    std::vector<unsigned char> Data;
    unsigned Width, Height;
    unsigned Error = lodepng::decode(Data, Width, Height, fileNameHeightmap);
    if (Error != 0)
    {
        LOG_CRITICAL_ERROR(lodepng_error_text(Error));
    }

    for (int y=0; y<Height; y++)
    {
        for (int x=0; x<Width; x++)
        {
            size_t Coordinate = y*(c_TerrainSize+1)*4 + x*4;
            float MinValue255 = 0.0f;
            float MaxValue255 = 255.0f;
            float Height = (float)(Data.at(Coordinate)-MinValue255) / MaxValue255; // between 0..1
            _SetHeight(x,y,Height);
        }
    }

    LOG_CALLSTACK_POP();
}

float DendyEngine::CTerrain::GetHeightAtPosition(glm::vec2 const& position) const
{
    LOG_CALLSTACK_PUSH(__FILE__,__LINE__,__PRETTY_FUNCTION__);
//#if defined(_DEBUG)
    if (std::abs(position.x) > static_cast<float>(c_TerrainSize)/2.0*c_Scale)
        LOG_CRITICAL_ERROR("position.x value ["+std::to_string(position.x)+"] out of terrain!");
    if (std::abs(position.y) > static_cast<float>(c_TerrainSize)/2.0*c_Scale)
        LOG_CRITICAL_ERROR("position.y value ["+std::to_string(position.y)+"] out of terrain!");
//#endif // _DEBUG

    glm::vec2 PositionInTerrainSpace;
    PositionInTerrainSpace.x = (position.x / c_Scale + static_cast<float>(c_TerrainSize)/2.0);
    PositionInTerrainSpace.y = (position.y / c_Scale + static_cast<float>(c_TerrainSize)/2.0);

    int FloorX = static_cast<int>(std::floor(PositionInTerrainSpace.x));
    int FloorY = static_cast<int>(std::floor(PositionInTerrainSpace.y));
    float HeightTopLeft = _GetHeight(FloorX,FloorY);
    float HeightTopRight = _GetHeight(FloorX+1,FloorY);
    float HeightBotLeft = _GetHeight(FloorX,FloorY+1);
    float HeightBotRight = _GetHeight(FloorX+1,FloorY+1);

    float FractionX = PositionInTerrainSpace.x - std::floor(PositionInTerrainSpace.x);
    float FractionY = PositionInTerrainSpace.y - std::floor(PositionInTerrainSpace.y);

    // Bilinear
    // float HeightTop = std::lerp(HeightTopLeft, HeightTopRight, FractionX);
    // float HeightBot = std::lerp(HeightBotLeft, HeightBotRight, FractionX);

    // float Height = std::lerp(HeightTop, HeightBot, FractionY);
    
    // Triangle Barycentric
    float Height;
    if (FractionX < 1.0f - FractionY)
    { // Triangle 00
        
        Height = DendyCommon::Math::BarycentricCoordinates(glm::vec3 (0, HeightTopLeft, 0), glm::vec3(1,HeightTopRight, 0), glm::vec3(0,HeightBotLeft, 1), glm::vec2(FractionX, FractionY));
    }
    else
    { // Triangle 11
        Height = DendyCommon::Math::BarycentricCoordinates(glm::vec3 (1, HeightBotRight, 1), glm::vec3(1,HeightTopRight, 0), glm::vec3(0,HeightBotLeft, 1), glm::vec2(FractionX, FractionY));
    }

    LOG_CALLSTACK_POP();
    return Height*c_TerrainMaxHeight;
}

glm::vec3 DendyEngine::CTerrain::GetNormalAtPosition(glm::vec2 const& position) const
{
    return glm::vec3(0.0f);
}

glm::vec3 DendyEngine::CTerrain::GetNormalAtPosition(glm::vec3 const& position) const
{
    return glm::vec3(0.0f);
}

float DendyEngine::CTerrain::GetHeightAtPosition(glm::vec3 const& position) const
{
    return GetHeightAtPosition(glm::vec2(position.x, position.z));
}


std::vector<glm::vec3> DendyEngine::CTerrain::GetWorldPositionsOfChunk(glm::vec2 const& min, glm::vec2 const& max)
{
    return std::vector<glm::vec3>();
}