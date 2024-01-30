#pragma once

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
        x = static_cast<int32_t>(position.x) / N;
        y = static_cast<int32_t>(position.y) / N;
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
    struct hash<DendyEngine::SPosition2DHash<50>>
    {
        auto operator()(DendyEngine::SPosition2DHash<50> const& key) const -> size_t {return hash<size_t>()(key.x*10000000 + key.y);}
    };
} 