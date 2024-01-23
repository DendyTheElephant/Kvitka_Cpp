#include <PixPhetamine/Mesh.h>
#include <DendyCommon/Logger.h>

PixPhetamine::CMesh::CMesh(std::string name, bool hasNormals, bool hasTextureCoordinates):
m_Name(name),
m_HasNormals(hasNormals),
m_HasTextureCoordinates(hasTextureCoordinates)
{

}

PixPhetamine::CMesh::CMesh(std::string name, std::string filePath, bool hasNormals, bool hasTextureCoordinates):
m_Name(name),
m_HasNormals(hasNormals),
m_HasTextureCoordinates(hasTextureCoordinates)
{
    LOG_CALLSTACK_PUSH(__FILE__,__LINE__,__PRETTY_FUNCTION__);

    // /* Fills the RAM with the mesh */
    // std::string Line;

    // std::ifstream objetFile(filePath, std::ios::in);
    // if (!objetFile.is_open())
    // {
    //     LOG_CRITICAL_ERROR("Loading Mesh from file, but File ["+std::string(filePath)+"] not found!");
    // }

    // float PositionX, PositionY, PositionZ, NormalX, NormalY, NormalZ, TextureCoordinateU, TextureCoordinateV;
    // unsigned int VertexIndex1, VertexIndex2, VertexIndex3;
    // while (getline(objetFile, Line))
    // {
    //     if (Line.substr(0, 2) == "v ")
    //     {
    //         std::istringstream InputStream(Line.substr(2));
    //         InputStream >> PositionX >> PositionY >> PositionZ >> NormalX >> NormalY >> NormalZ >> TextureCoordinateU >> TextureCoordinateV;
    //         m_vertices.push_back(posX); m_vertices.push_back(posY); m_vertices.push_back(posZ);
    //         m_normals.push_back(normalX); m_normals.push_back(normalY); m_normals.push_back(normalZ);
    //         m_colors.push_back(colorR); m_colors.push_back(colorG); m_colors.push_back(colorB);
    //     }
    //     else if (Line.substr(0, 2) == "f ")
    //     {
    //         std::istringstream s(Line.substr(2));
    //         s >> vertexNdx1 >> vertexNdx2 >> vertexNdx3;
    //         m_faces.push_back(vertexNdx1); m_faces.push_back(vertexNdx2); m_faces.push_back(vertexNdx3);
    //     }
    // }

    LOG_CALLSTACK_POP();
}

#include <algorithm>
PixPhetamine::CMesh::CMesh(std::string name, EBasicMeshes basicMesh, bool hasNormals, bool hasTextureCoordinates):
m_Name(name),
m_HasNormals(hasNormals),
m_HasTextureCoordinates(hasTextureCoordinates)
{
    LOG_CALLSTACK_PUSH(__FILE__,__LINE__,__PRETTY_FUNCTION__);

    switch (basicMesh)
    {
        case EBasicMeshes::Triangle:
        {
            m_PositionsVec = {
                -0.5f, -0.5f, 0.0f, // left
                0.5f, -0.5f, 0.0f, // right
                0.0f,  0.5f, 0.0f  // top
            };
            if (m_HasNormals)
            {
                m_NormalsVec = {
                    0.0f,  1.0f,  0.0f,
                    0.0f,  1.0f,  0.0f,
                    0.0f,  1.0f,  0.0f
                };
            }
            if (m_HasTextureCoordinates)
            {
                m_TextureCoordinatesVec = {
                    -1.0f,  1.0f,
                    1.0f,  1.0f,
                    -1.0f, -1.0f
                };
            }
            m_FaceIndicesVec = {
                0, 1, 2
            };
            break;
        }
        case EBasicMeshes::Quad:
        {
            m_PositionsVec = {
                -0.5f,  0.0f,  0.5f,
                 0.5f,  0.0f,  0.5f,
                -0.5f,  0.0f, -0.5f,
                 0.5f,  0.0f, -0.5f
            };
            if (m_HasNormals)
            {
                m_NormalsVec = {
                    0.0f,  1.0f,  0.0f,
                    0.0f,  1.0f,  0.0f,
                    0.0f,  1.0f,  0.0f,
                    0.0f,  1.0f,  0.0f
                };
            }
            if (m_HasTextureCoordinates)
            {
                m_TextureCoordinatesVec = {
                    -1.0f,  1.0f,
                    1.0f,  1.0f,
                    -1.0f, -1.0f,
                    1.0f, -1.0f
                };
            }
            m_FaceIndicesVec = {
                0, 1, 2, 3, 2, 1
            };
            break;
        }
        case EBasicMeshes::Cube:
        {
            m_PositionsVec = {
                -0.5f, -0.5f,  0.5f,
                 0.5f, -0.5f,  0.5f,
                 0.5f, -0.5f, -0.5f,
                -0.5f, -0.5f, -0.5f,
                -0.5f,  0.5f,  0.5f,
                 0.5f,  0.5f,  0.5f,
                 0.5f,  0.5f, -0.5f,
                -0.5f,  0.5f, -0.5f,
            };
            if (m_HasNormals)
            {
                m_NormalsVec = {
                    -1.0f, -1.0f, -1.0f,
                    -1.0f, -1.0f, -1.0f,
                    -1.0f, -1.0f, -1.0f,
                    -1.0f, -1.0f, -1.0f,
                    -1.0f, -1.0f, -1.0f,
                    -1.0f, -1.0f, -1.0f,
                    -1.0f, -1.0f, -1.0f,
                    -1.0f, -1.0f, -1.0f
                };
            }
            if (m_HasTextureCoordinates)
            {
                m_TextureCoordinatesVec = {
                    -1.0f, -1.0f,
                    -1.0f, -1.0f,
                    -1.0f, -1.0f,
                    -1.0f, -1.0f,
                    -1.0f, -1.0f,
                    -1.0f, -1.0f,
                    -1.0f, -1.0f,
                    -1.0f, -1.0f
                };
            }
            m_FaceIndicesVec = {
                0, 1, 2, 2, 3, 0,
                0, 1, 5, 5, 4, 0,
                1, 2, 6, 6, 5, 1,
                2, 3, 7, 7, 6, 2,
                3, 0, 4, 4, 7, 3,
                4, 5, 6, 6, 7, 4
            };
            break;
        }
        case EBasicMeshes::Pawn:
        {
            m_PositionsVec = {
                0.5f, 1.75f, 0.0f, // right
                -0.5f, 1.75f, 0.0f, // left
                0.0f,  0.0f, 0.0f  // down
            };
            if (m_HasNormals)
            {
                m_NormalsVec = {
                    1.0f,  0.0f,  0.0f,
                   -1.0f,  0.0f,  0.0f,
                    0.0f, -1.0f,  0.0f
                };
            }
            if (m_HasTextureCoordinates)
            {
                m_TextureCoordinatesVec = {
                    -1.0f,  1.0f,
                    1.0f,  1.0f,
                    -1.0f, -1.0f
                };
            }
            m_FaceIndicesVec = {
                0, 1, 2
            };
            break;
        }
    }

    LOG_CALLSTACK_POP();
}

PixPhetamine::CMesh::~CMesh()
{
    LOG_CALLSTACK_PUSH(__FILE__,__LINE__,__PRETTY_FUNCTION__);
    if (m_IsLoadedInGPU)
        LOG_CRITICAL_ERROR("Trying to delete Mesh ["+m_Name+"] but it is still loaded on the GPU side, make sure to unload first!");
    m_VAOHandle = 0;
    LOG_CALLSTACK_POP();
}

void PixPhetamine::CMesh::AddPosition(glm::vec3 const& position)
{
    m_PositionsVec.push_back(position.x);
    m_PositionsVec.push_back(position.y);
    m_PositionsVec.push_back(position.z);
}

void PixPhetamine::CMesh::AddNormal(glm::vec3 const& normal)
{
    m_NormalsVec.push_back(normal.x);
    m_NormalsVec.push_back(normal.y);
    m_NormalsVec.push_back(normal.z);
}

void PixPhetamine::CMesh::AddTextureCoordinate(glm::vec2 const& textureCoordinate)
{
    m_TextureCoordinatesVec.push_back(textureCoordinate.x);
    m_TextureCoordinatesVec.push_back(textureCoordinate.y);
}

void PixPhetamine::CMesh::AddTriangleIndices(uint32_t v1, uint32_t v2, uint32_t v3)
{
    m_FaceIndicesVec.push_back(v1);
    m_FaceIndicesVec.push_back(v2);
    m_FaceIndicesVec.push_back(v3);
}

#include <array>
void PixPhetamine::CMesh::LoadToGPU()
{
    LOG_CALLSTACK_PUSH(__FILE__,__LINE__,__PRETTY_FUNCTION__);

    std::array<GLuint,3> VBOHandlesVec = {0,0,0};

    uint8_t BufferIndex = 1;
    if (m_HasNormals)
        BufferIndex++;
    if (m_HasTextureCoordinates)
        BufferIndex++;
    glGenBuffers(BufferIndex, VBOHandlesVec.data());

    // Create and set the VAO (Vertex Array Object)
    glGenVertexArrays(1, &m_VAOHandle);
    glBindVertexArray(m_VAOHandle);

    BufferIndex = 0;
    GLuint PositionsBufferHandle = VBOHandlesVec[BufferIndex];
    // Populate the position buffer
    glBindBuffer(GL_ARRAY_BUFFER, PositionsBufferHandle);
    glBufferData(GL_ARRAY_BUFFER, m_PositionsVec.size()*sizeof(GLfloat), m_PositionsVec.data(), GL_STATIC_DRAW);
    // Enable Vertex attributes arrays
    glEnableVertexAttribArray(BufferIndex); // Vertex position
    // Tell the formats of each buffer
    glBindVertexBuffer(BufferIndex, PositionsBufferHandle, 0, sizeof(GLfloat) * 3);
    
    glVertexAttribFormat(BufferIndex, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexAttribBinding(BufferIndex, 0); // Map positions to shader pos0

    if (m_HasNormals)
    {
        BufferIndex++;
        GLuint NormalsBufferHandle = VBOHandlesVec[BufferIndex];
        // Populate the normal buffer
        glBindBuffer(GL_ARRAY_BUFFER, NormalsBufferHandle);
        glBufferData(GL_ARRAY_BUFFER, m_NormalsVec.size()*sizeof(GLfloat), m_NormalsVec.data(), GL_STATIC_DRAW);
        // Enable Vertex attributes arrays
        glEnableVertexAttribArray(BufferIndex); // Vertex normal
        // Tell the formats of each buffer
        glBindVertexBuffer(BufferIndex, NormalsBufferHandle, 0, sizeof(GLfloat) * 3);

        glVertexAttribFormat(BufferIndex, 3, GL_FLOAT, GL_FALSE, 0);
        glVertexAttribBinding(BufferIndex, 1); // Map positions to shader pos1
    }

    if (m_HasTextureCoordinates)
    {
        BufferIndex++;
        GLuint TextureCoordinatesBufferHandle = VBOHandlesVec[BufferIndex];
        // Populate the texture coordinate buffer
        glBindBuffer(GL_ARRAY_BUFFER, TextureCoordinatesBufferHandle);
        glBufferData(GL_ARRAY_BUFFER, m_TextureCoordinatesVec.size()*sizeof(GLfloat), m_TextureCoordinatesVec.data(), GL_STATIC_DRAW);
        // Enable Vertex attributes arrays
        glEnableVertexAttribArray(BufferIndex); // Vertex texture coordinate
        // Tell the formats of each buffer
        glBindVertexBuffer(BufferIndex, TextureCoordinatesBufferHandle, 0, sizeof(GLfloat) * 2);

        glVertexAttribFormat(BufferIndex, 2, GL_FLOAT, GL_FALSE, 0);
        glVertexAttribBinding(BufferIndex, 2); // Map positions to shader pos2
    }

    // Indices
    GLuint IndicesHandle;
    glGenBuffers(1, &IndicesHandle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndicesHandle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_FaceIndicesVec.size()*sizeof(GLuint), m_FaceIndicesVec.data(), GL_STATIC_DRAW);

    LOG_CALLSTACK_POP();
}

void PixPhetamine::CMesh::UnloadFromGPU()
{
    glDeleteBuffers(1, &m_VAOHandle);
}

