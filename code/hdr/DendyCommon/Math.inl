
inline int_fast8_t FastCompareDistance(glm::vec2 const& position1, glm::vec2 const& position2, float const& distance)
{
    return glm::sign( glm::dot(position2-position1, position2-position1) - distance*distance );
}

inline int_fast8_t FastCompareMagnitude(glm::vec2 const& vector, float const& distance)
{
    return glm::sign( glm::dot(vector, vector) - distance*distance );
}

inline glm::vec2 ComputeClosestPositionOnEdge(glm::vec2 const& position, glm::vec2 const& edgeStart, glm::vec2 const& edgeEnd)
{
    const glm::vec2 Tangent = edgeEnd - edgeStart;

    if (glm::dot((position - edgeStart), Tangent) <= 0)
        return edgeStart; // 'P' is before 'a' on the line (ab)

    if (glm::dot((position - edgeEnd), Tangent) >= 0)
        return edgeEnd; // 'P' is after 'b' on the line (ab)

    auto const NormalizedTangent = glm::normalize(Tangent);
    auto const RelativePos = position - edgeStart;
    return edgeStart + NormalizedTangent * (NormalizedTangent * RelativePos);
}

inline bool IsPositionOnRightOfEdge(glm::vec2 const& position, glm::vec2 const& edgeStart, glm::vec2 const& edgeEnd)
{
    glm::vec2 Edge = edgeEnd - edgeStart;
    glm::vec2 EdgeStartToPosition = position - edgeStart;

    return (Edge.x*EdgeStartToPosition.y - Edge.y*EdgeStartToPosition.x) < 0.0f;
}

inline bool IsPositionOnLeftOfEdge(glm::vec2 const& position, glm::vec2 const& edgeStart, glm::vec2 const& edgeEnd)
{
    glm::vec2 Edge = edgeEnd - edgeStart;
    glm::vec2 EdgeStartToPosition = position - edgeStart;

    return (Edge.x*EdgeStartToPosition.y - Edge.y*EdgeStartToPosition.x) > 0.0f;
}

inline bool IsPositionInsideConvexShape(glm::vec2 const& position, std::vector<glm::vec2> const& convexShape)
{
    for (auto iShapePosition=0; iShapePosition<convexShape.size(); iShapePosition++)
    {
        glm::vec2 EdgeStart = convexShape.at(iShapePosition);
        glm::vec2 EdgeEnd;
        if (iShapePosition == iShapePosition<convexShape.size()-1)
            EdgeEnd = convexShape.at(0);
        else
            EdgeEnd = convexShape.at(iShapePosition+1);
        
        if (IsPositionOnRightOfEdge(position, EdgeStart, EdgeEnd))
            return false; // Position is on right of edge : outside
    }
    return true;
}

inline bool IsCollisionEdgeWithEdge(glm::vec2 const& edge1Start, glm::vec2 const& edge1End, glm::vec2 const& edge2Start, glm::vec2 const& edge2End)
{
    glm::vec2 Edge1 = edge1End - edge1Start;
    glm::vec2 Edge2 = edge2End - edge2Start;
    double Denominator = Edge1.x*Edge2.y - Edge1.y*Edge2.x;
    if (Denominator == 0)
        return false;
    float T = - (edge1Start.x*Edge2.y-edge2Start.x*Edge2.y-Edge2.x*edge1Start.y+Edge2.x*edge2Start.y) / Denominator;
    if (T<0 || T>=1)
        return false;
    float U = - (-Edge1.x*edge1Start.y+Edge1.x*edge2Start.y+Edge1.y*edge1Start.x-Edge1.y*edge2Start.x) / Denominator;
    if (U<0 || U>=1)
        return false;
    return true;
}

inline bool IsCollisionEdgeWithConvexShape(glm::vec2 const& edgeStart, glm::vec2 const& edgeEnd, std::vector<glm::vec2> const& convexShape)
{
    for (auto iShapePosition=0; iShapePosition<convexShape.size(); iShapePosition++)
    {
        glm::vec2 EdgeShapeStart = convexShape.at(iShapePosition);
        glm::vec2 EdgeShapeEnd;
        if (iShapePosition == convexShape.size()-1)
            EdgeShapeEnd = convexShape.at(0);
        else
            EdgeShapeEnd = convexShape.at(iShapePosition+1);
        
        if (IsCollisionEdgeWithEdge(edgeStart, edgeEnd, EdgeShapeStart, EdgeShapeEnd))
            return true;
    }
    return false;
}

inline bool IsCollisionDiskWithEdge(glm::vec2 const& diskPosition, float const& diskRadius, glm::vec2 const& edgeStart, glm::vec2 const& edgeEnd)
{
    const glm::vec2 DiskToEdge = diskPosition - ComputeClosestPositionOnEdge(diskPosition, edgeStart, edgeEnd);

    if (FastCompareMagnitude(DiskToEdge, diskRadius) > 0.0f)
        return false;
    return true;
}

inline DendyCommon::Math::SCollision CollisionDiskWithEdge(glm::vec2 const& diskPosition, float const& diskRadius, glm::vec2 const& edgeStart, glm::vec2 const& edgeEnd)
{
    const glm::vec2 DiskToEdge = diskPosition - ComputeClosestPositionOnEdge(diskPosition, edgeStart, edgeEnd);

    if (FastCompareMagnitude(DiskToEdge, diskRadius) > 0.0f)
        return DendyCommon::Math::SCollision();

    return DendyCommon::Math::SCollision(diskRadius - glm::length(DiskToEdge), glm::normalize(DiskToEdge));
}



inline float BarycentricCoordinates(glm::vec3 const& p1, glm::vec3 const& p2, glm::vec3 const& p3, glm::vec2 const& pos)
{
    float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
    float l1 = ((p2.z - p3.z) * (pos.x - p3.x) + (p3.x - p2.x) * (pos.y - p3.z)) / det;
    float l2 = ((p3.z - p1.z) * (pos.x - p3.x) + (p1.x - p3.x) * (pos.y - p3.z)) / det;
    float l3 = 1.0f - l1 - l2;
    return l1 * p1.y + l2 * p2.y + l3 * p3.y;
}

inline glm::quat GetQuaternionFromOrientation(glm::vec2 const& orientation)
{
    float OrientationAngle = atan2f( -orientation.x, -orientation.y );
    return { cosf( OrientationAngle/2.0f ), 0, sinf( OrientationAngle/2.0f ), 0 };
}

inline glm::mat4 GetRotationMatrixFromOrientation(glm::vec2 const& orientation)
{
    return glm::mat4_cast(GetQuaternionFromOrientation(orientation));
}

inline glm::vec3 GetWorldPositionFromScenePosition(glm::vec2 const& scenePosition)
{
    return {scenePosition.x, 0, scenePosition.y};
}

inline glm::vec2 GetScenePositionFromWorldPosition(glm::vec3 const& worldPosition)
{
    return {worldPosition.x, worldPosition.z};
}

