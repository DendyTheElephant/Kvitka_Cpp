#pragma once

#include <DendyEngine/Definitions.h>

#include <glm/glm.hpp>

#include <functional>
#include <cstdint>

namespace DendyEngine
{

template<uint16_t N>
struct SPosition2DHash
{
    //friend class std::hash<DendyEngine::SPosition2DHash>;
    int32_t x;
    int32_t y;

    SPosition2DHash(glm::vec2 const& position)
    {
        glm::vec2 ScaledPosition = position / static_cast<float>(N);

        // When On bounds :
        // if (ceilf(ScaledPosition.x) == ScaledPosition.x)
        //     ScaledPosition.x *= 1.000001f;
        // if (ceilf(ScaledPosition.y) == ScaledPosition.y)
        //     ScaledPosition.y *= 1.000001f;
        
        if (ScaledPosition.x >= 0)
            x = static_cast<int32_t>(ScaledPosition.x);
        else
            x = static_cast<int32_t>(std::floor(ScaledPosition.x));
        if (ScaledPosition.y >= 0)
            y = static_cast<int32_t>(ScaledPosition.y);
        else
            y = static_cast<int32_t>(std::floor(ScaledPosition.y));
    }

    SPosition2DHash(SPosition2DHash const& original) {x=original.x; y=original.y;}
    SPosition2DHash& operator=(SPosition2DHash const& original) {x=original.x; y=original.y;}

    // For Hashable functions
    bool operator==(SPosition2DHash const& other) const
    { 
        return (x == other.x && y == other.y);
    }
};

}

// For Hashable functions
namespace std
{
    template <>
    struct hash<DendyEngine::SPosition2DHash<DendyEngine::Definitions::c_ChunkSize>>
    {
        auto operator()(DendyEngine::SPosition2DHash<DendyEngine::Definitions::c_ChunkSize> const& key) const -> size_t {return hash<size_t>()(key.x*10000000 + key.y);}
    };
} 