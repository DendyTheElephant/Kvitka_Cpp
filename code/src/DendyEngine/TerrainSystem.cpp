#include <DendyEngine/TerrainSystem.h>
#include <DendyCommon/Logger.h>
//#include <DendyCommon/Math.h>
#include <DendyEngine/Definitions.h>

#include <lodepng.h>



void DendyEngine::CTerrainSystem::InitialiseTerrainChunkFromHeighmap(Components::STerrainChunk* pTerrainChunk, std::string const& heightmapFileName) const
{
    LOG_CALLSTACK_PUSH(__FILE__,__LINE__,__PRETTY_FUNCTION__);

    // Load file and decode image.
    std::vector<unsigned char> Data;
    unsigned Width, Height;
    unsigned Error = lodepng::decode(Data, Width, Height, heightmapFileName);
    if (Error != 0)
    {
        LOG_CRITICAL_ERROR(lodepng_error_text(Error));
    }
    if (Width != Definitions::c_TerrainSize+1 || Height != Definitions::c_TerrainSize+1)
        LOG_CRITICAL_ERROR("Terrain size is ["+std::to_string(Definitions::c_TerrainSize)+"] but heightmap is ["+std::to_string(Width)+","+std::to_string(Height)+"], heightmap should be of size (Terrain size +1)");

    for (int y=0; y<Height; y++)
    {
        for (int x=0; x<Width; x++)
        {
            size_t Coordinate = y*(Definitions::c_TerrainSize+1)*4 + x*4;
            float MinValue255 = 0.0f;
            float MaxValue255 = 255.0f;
            float Height = (float)(Data.at(Coordinate)-MinValue255) / MaxValue255; // between 0..1
            pTerrainChunk->HeightsArray.at(y*(Definitions::c_TerrainSize+1)+x) = static_cast<uint16_t>(Height*65535.0f);
        }
    }

    LOG_CALLSTACK_POP();
}

glm::vec3 DendyEngine::CTerrainSystem::GetWorldPositionFromScenePosition(Components::STerrainChunk* pTerrainChunk, glm::vec2 const& scenePosition) const
{
    LOG_CALLSTACK_PUSH(__FILE__,__LINE__,__PRETTY_FUNCTION__);
//#if defined(_DEBUG)
    // if (std::abs(scenePosition.x) > static_cast<float>(Definitions::c_TerrainSize)/2.0*Definitions::c_TerrainScale)
    //     LOG_CRITICAL_ERROR("position.x value ["+std::to_string(scenePosition.x)+"] out of terrain!");
    // if (std::abs(scenePosition.y) > static_cast<float>(Definitions::c_TerrainSize)/2.0*Definitions::c_TerrainScale)
    //     LOG_CRITICAL_ERROR("position.y value ["+std::to_string(scenePosition.y)+"] out of terrain!");
//#endif // _DEBUG

    glm::vec2 PositionInTerrainSpace;
    PositionInTerrainSpace.x = (scenePosition.x - pTerrainChunk->Translation.x) / Definitions::c_TerrainScale;
    PositionInTerrainSpace.y = (scenePosition.y - pTerrainChunk->Translation.y) / Definitions::c_TerrainScale;

    int FloorX = static_cast<int>(std::floor(PositionInTerrainSpace.x));
    int FloorY = static_cast<int>(std::floor(PositionInTerrainSpace.y));

    float HeightTopLeft = static_cast<float>(pTerrainChunk->HeightsArray.at(FloorY*(Definitions::c_TerrainSize+1)+FloorX))/65535.0f;
    float HeightTopRight = static_cast<float>(pTerrainChunk->HeightsArray.at(FloorY*(Definitions::c_TerrainSize+1)+(FloorX+1)))/65535.0f;
    float HeightBotLeft = static_cast<float>(pTerrainChunk->HeightsArray.at((FloorY+1)*(Definitions::c_TerrainSize+1)+FloorX))/65535.0f;
    float HeightBotRight = static_cast<float>(pTerrainChunk->HeightsArray.at((FloorY+1)*(Definitions::c_TerrainSize+1)+(FloorX+1)))/65535.0f;

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
    return glm::vec3(scenePosition.x, Height*Definitions::c_TerrainMaxHeight, scenePosition.y);
}