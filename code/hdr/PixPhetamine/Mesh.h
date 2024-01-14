#include "PixPhetamine/VertexArrayObject.h"
#include <glad/glad.h>

namespace PixPhetamine
{

class CMesh
{
private:
    std::string m_Name;
    GLuint m_VAOHandle{0};
    std::vector<GLfloat> m_PositionsVec;
    std::vector<GLfloat> m_NormalsVec;
    std::vector<GLfloat> m_TextureCoordinatesVec;
    std::vector<GLuint> m_FaceIndicesVec;

public:
    enum class BasicMeshes
    {
        Triangle,
        Quad,
        Cube
    };

    CMesh(std::string name, std::string filePath);
    CMesh(std::string name, BasicMeshes basicMesh);
    ~CMesh();

    void LoadToGPU();
    void UnloadFromGPU();

    GLuint GetVAO() const {return m_VAOHandle;}
    GLuint GetTriangleCount() const {return m_FaceIndicesVec.size();}
};

}