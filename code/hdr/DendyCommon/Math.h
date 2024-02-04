#pragma once

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

namespace DendyCommon::Math
{
    struct SCollision
    {
        float Magnitude{0};
        glm::vec2 Normal{0};
        SCollision() {}
        SCollision(float const& magnitude, glm::vec2 const& normal): Magnitude(magnitude), Normal(normal) {}
    };

    /// @brief Performs a check on distance (without sqrt length)
    /// @param pointA 2D position representing the origin
    /// @param pointB 2D position representing the target
    /// @param distance Desired distance for comparaison
    /// @return [-1: Point's distance lower than distance, 0: Point's distance equals distance, 1 Point's distance greater than distance]
    int_fast8_t FastCompareDistance(glm::vec2 const& pointA, glm::vec2 const& pointB, float const& distance);

    /// @brief Performs a check on distance (without sqrt length)
    /// @param vector 2D vector to check (from 0,0)
    /// @param distance Desired distance for comparaison
    /// @return [-1: Point's distance lower than distance, 0: Point's distance equals distance, 1 Point's distance greater than distance]
    int_fast8_t FastCompareMagnitude(glm::vec2 const& vector, float const& distance);

    inline glm::vec2 ComputeOrthogonalLeft(glm::vec2 const& vector) { return {vector.y, -vector.x}; }
    inline glm::vec2 ComputeOrthogonalRight(glm::vec2 const& vector) { return {-vector.y, vector.x}; }

    /// 2D vector cross product analog.
    /// The cross product of 2D vectors results in a 3D vector with only a z component.
    /// This function returns the magnitude of the z value.
    bool IsPositionOnRightOfEdge(glm::vec2 const& position, glm::vec2 const& edgeStart, glm::vec2 const& edgeEnd);

    /// 2D vector cross product analog.
    /// The cross product of 2D vectors results in a 3D vector with only a z component.
    /// This function returns the magnitude of the z value.
    bool IsPositionOnLeftOfEdge(glm::vec2 const& position, glm::vec2 const& edgeStart, glm::vec2 const& edgeEnd);

    // returns the point from the segment [edgeStart,edgeEnd] which is the closest to 'position'
    glm::vec2 ComputeClosestPositionOnEdge(glm::vec2 const& position, glm::vec2 const& edgeStart, glm::vec2 const& edgeEnd);

    bool IsPositionInsideConvexShape(glm::vec2 const& position, std::vector<glm::vec2> const& convexShape);

    bool IsCollisionDiskWithEdge(glm::vec2 const& diskPosition, float const& diskRadius, glm::vec2 const& edgeStart, glm::vec2 const& edgeEnd);
    bool IsCollisionEdgeWithEdge(glm::vec2 const& edge1Start, glm::vec2 const& edge1End, glm::vec2 const& edge2Start, glm::vec2 const& edge2End);
    bool IsCollisionEdgeWithConvexShape(glm::vec2 const& edgeStart, glm::vec2 const& edgeEnd, std::vector<glm::vec2> const& convexShape);

    SCollision CollisionDiskWithEdge(glm::vec2 const& diskPosition, float const& diskRadius, glm::vec2 const& edgeStart, glm::vec2 const& edgeEnd);


    inline size_t Access2DTo1D(size_t x, size_t y, size_t width) { return y*width+x; }

    // Compute barycentric coordinates (u, v, w) for
    // point p with respect to triangle (a, b, c)
    float BarycentricCoordinates(glm::vec3 const& p1, glm::vec3 const& p2, glm::vec3 const& p3, glm::vec2 const& pos);

    glm::quat GetQuaternionFromOrientation(glm::vec2 const& orientation);

    glm::mat4 GetRotationMatrixFromOrientation(glm::vec2 const& orientation);

    glm::vec3 GetWorldPositionFromScenePosition(glm::vec2 const& scenePosition);

    glm::vec2 GetScenePositionFromWorldPosition(glm::vec3 const& worldPosition);


#include <DendyCommon/Math.inl>
}