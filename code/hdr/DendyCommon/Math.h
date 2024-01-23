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
    inline static float BarycentricCoordinates(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec2 pos)
    {
		float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
		float l1 = ((p2.z - p3.z) * (pos.x - p3.x) + (p3.x - p2.x) * (pos.y - p3.z)) / det;
		float l2 = ((p3.z - p1.z) * (pos.x - p3.x) + (p1.x - p3.x) * (pos.y - p3.z)) / det;
		float l3 = 1.0f - l1 - l2;
		return l1 * p1.y + l2 * p2.y + l3 * p3.y;
	}
}