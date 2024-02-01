#pragma once

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace DendyCommon::Math
{
    struct SEdge
    {
        glm::vec2 Start{0};
        glm::vec2 End{0};

        SEdge() {}
        SEdge(glm::vec2 const& start, glm::vec2 const& end): Start(start), End(end) {}

        SEdge& operator+=(glm::vec2 const& position)
        {
            // actual addition of rhs to *this
            Start += position;
            End += position;
            return *this;
        }
        // SEdge operator+(glm::vec2 const& position)
        // {
        //     SEdge Result;
        //     Result.Start = Start + position;
        //     Result.End = End + position;
        //     return Result;
        // }
        // operator std::string() const
        // {
        //     return std::string("[("+Start.x"]");
        // }
    };
    inline SEdge operator+(SEdge const& edge, glm::vec2 const& position)
    {
        SEdge Res = edge;
        Res += position;
        return Res;
    }

    struct SShape
    {
        std::vector<SEdge> EdgesVec;
        
        SShape() {}
        SShape(std::vector<SEdge> const& absoluteEdgesVec): EdgesVec(absoluteEdgesVec) {}
        SShape(glm::vec2 const& position, std::vector<SEdge> const& relativeEdgesVec)
        {
            for (auto& Edge : relativeEdgesVec)
            {
                EdgesVec.push_back(Edge + position);
            }
        }
    };

    inline bool IsLeft(SEdge const& edge, glm::vec2 const& point)
    {
        glm::vec2 Relative1 = edge.End - edge.Start;
        glm::vec2 Relative2 = point - edge.End;
        return glm::sign(glm::cross(Relative1, Relative2)) > 0;
    }

    inline bool IsColliding(SEdge const& edge1, SEdge const& edge2)
    {
        if ( IsLeft(edge1, edge2.Start) != IsLeft(edge1, edge2.End) )
        {
            if ( glm::dot(edge1.End - edge1.Start, edge2.End - edge1.End) > 0.0f && glm::dot(edge1.End - edge1.Start, edge2.Start - edge1.End) > 0.0f
            && glm::dot(edge1.Start - edge1.End, edge2.End - edge1.Start) > 0.0f && glm::dot(edge1.Start - edge1.End, edge2.Start - edge1.Start) > 0.0f )
                return true;
        }
        return false;
    }

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
        return glm::sign( glm::dot(pointB-pointA, pointB-pointA) - distance*distance );
    }
}