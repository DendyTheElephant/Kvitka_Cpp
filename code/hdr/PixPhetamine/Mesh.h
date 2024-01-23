#pragma once

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>
#include <string>
#include <vector>

namespace PixPhetamine
{

class CMesh
{
public:
    enum class EBasicMeshes
    {
        Triangle,
        Quad,
        Cube,
        Pawn
    };

private:
    std::string m_Name;
    GLuint m_VAOHandle{0};
    std::vector<GLfloat> m_PositionsVec;
    std::vector<GLfloat> m_NormalsVec;
    std::vector<GLfloat> m_TextureCoordinatesVec;
    std::vector<GLuint> m_FaceIndicesVec;
    bool m_IsLoadedInGPU;
    const bool m_HasNormals;
    const bool m_HasTextureCoordinates;


public:
    CMesh(std::string name, bool hasNormals, bool hasTextureCoordinates);
    CMesh(std::string name, std::string filePath, bool hasNormals, bool hasTextureCoordinates);
    CMesh(std::string name, EBasicMeshes basicMesh, bool hasNormals, bool hasTextureCoordinates);
    ~CMesh();

    void AddPosition(glm::vec3 const& position);
    void AddNormal(glm::vec3 const& normal);
    void AddTextureCoordinate(glm::vec2 const& textureCoordinate);
    void AddTriangleIndices(uint32_t v1, uint32_t v2, uint32_t v3);

    void LoadToGPU();
    void UnloadFromGPU();
    GLuint GetVAO() const {return m_VAOHandle;}
    GLuint GetTriangleCount() const {return m_FaceIndicesVec.size();}

    inline bool const GetIsLoadedInGPUState() const {return m_IsLoadedInGPU;}
};

}