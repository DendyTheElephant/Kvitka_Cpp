#pragma once

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace DendyCommon::Math
{
    // template<typename T>
    // inline unsigned int Access1DArrayAs2DArray(std::array<T> theArray, unsigned int x, unsigned int y) 
    // {
    //     return theArray.at(x*theArray + y
    // }

    inline static size_t Access2DTo1D(size_t x, size_t y, size_t width)
    {
        return y*width+x;
    }

    // Compute barycentric coordinates (u, v, w) for
    // point p with respect to triangle (a, b, c)
    inline static float BarycentricCoordinates(glm::vec3 const& p1, glm::vec3 const& p2, glm::vec3 const& p3, glm::vec2 const& pos)
    {
        float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
        float l1 = ((p2.z - p3.z) * (pos.x - p3.x) + (p3.x - p2.x) * (pos.y - p3.z)) / det;
        float l2 = ((p3.z - p1.z) * (pos.x - p3.x) + (p1.x - p3.x) * (pos.y - p3.z)) / det;
        float l3 = 1.0f - l1 - l2;
        return l1 * p1.y + l2 * p2.y + l3 * p3.y;
    }

    inline static glm::quat GetQuaternionFromOrientation(glm::vec2 const& orientation)
    {
        float OrientationAngle = atan2f( -orientation.x, -orientation.y );
        return { cosf( OrientationAngle/2.0f ), 0, sinf( OrientationAngle/2.0f ), 0 };
    }

    inline static glm::mat4 GetRotationMatrixFromOrientation(glm::vec2 const& orientation)
    {
        return glm::mat4_cast(GetQuaternionFromOrientation(orientation));
    }

    inline static glm::vec3 GetWorldPositionFromScenePosition(glm::vec2 const& scenePosition)
    {
        return {scenePosition.x, 0, scenePosition.y};
    }

    inline static glm::vec2 GetScenePositionFromWorldPosition(glm::vec3 const& worldPosition)
    {
        return {worldPosition.x, worldPosition.z};
    }

    /// @brief Performs a check on distance (without sqrt length)
    /// @param pointA 2D position representing the origin
    /// @param pointB 2D position representing the target
    /// @param distance Desired distance for comparaison
    /// @return [-1: Point's distance lower than distance, 0: Point's distance equals distance, 1 Point's distance greater than distance]
    inline static int_fast8_t FastCompareDistance(glm::vec2 const& pointA, glm::vec2 const& pointB, float const& distance)
    {
        return glm::sign( glm::dot(pointB-pointA, pointB-pointA) - distance );
    }
}