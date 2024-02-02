#pragma once

#include <cstdint>

namespace DendyEngine::Definitions
{
    // Terrain
    constexpr float c_TerrainScale{1.0f};
    constexpr uint8_t c_TerrainSize{50};
    constexpr float c_TerrainMaxHeight{100.0f};
    constexpr uint16_t c_TerrainArraySize{(c_TerrainSize+1)*(c_TerrainSize+1)};

    // Chunk
    constexpr uint8_t c_ChunkSize{50};
}

