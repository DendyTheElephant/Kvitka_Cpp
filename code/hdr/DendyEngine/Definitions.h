#pragma once

#include <cstdint>

namespace DendyEngine::Definitions
{
    // Terrain
    constexpr float c_TerrainScale{0.1f};
    constexpr uint32_t c_TerrainSize{500};
    constexpr float c_TerrainMaxHeight{4.0f};
    constexpr uint32_t c_TerrainArraySize{(c_TerrainSize+1)*(c_TerrainSize+1)};

    // Chunk
    constexpr uint8_t c_ChunkSize{50};
}

