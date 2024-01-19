#include <PixPhetamine/Mesh.h>
#include <DendyCommon/Logger.h>

PixPhetamine::IMesh::~IMesh()
{
    LOG_CALLSTACK_PUSH(__FILE__,__LINE__,__PRETTY_FUNCTION__);
    if (m_IsLoadedInGPU)
        // UnloadFromGPU();
        LOG_CRITICAL_ERROR("Trying to delete Mesh ["+m_Name+"] but it is still loaded on the GPU side, make sure to unload first!");
        //std::cout << "Should unload from GPU first!" << std::endl;
    m_VAOHandle = 0;
    LOG_CALLSTACK_POP();
}

PixPhetamine::CMesh::CMesh(std::string name, std::string filePath):
IMesh(name)
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

PixPhetamine::CMesh::CMesh(std::string name, BasicMeshes basicMesh):
IMesh(name)
{
    LOG_CALLSTACK_PUSH(__FILE__,__LINE__,__PRETTY_FUNCTION__);

    switch (basicMesh)
    {
        case BasicMeshes::Triangle:
        {
            m_PositionsVec = {
                -0.5f, -0.5f, 0.0f, // left
                0.5f, -0.5f, 0.0f, // right
                0.0f,  0.5f, 0.0f  // top
            };
            m_NormalsVec = {
                 0.0f,  1.0f,  0.0f,
                 0.0f,  1.0f,  0.0f,
                 0.0f,  1.0f,  0.0f
            };
            m_TextureCoordinatesVec = {
                -1.0f,  1.0f,
                 1.0f,  1.0f,
                -1.0f, -1.0f
            };
            m_FaceIndicesVec = {
                0, 1, 2
            };
            break;
        }
        case BasicMeshes::Quad:
        {
            m_PositionsVec = {
                -1.0f,  0.0f,  1.0f,
                 1.0f,  0.0f,  1.0f,
                -1.0f,  0.0f, -1.0f,
                 1.0f,  0.0f, -1.0f
            };
            m_NormalsVec = {
                 0.0f,  1.0f,  0.0f,
                 0.0f,  1.0f,  0.0f,
                 0.0f,  1.0f,  0.0f,
                 0.0f,  1.0f,  0.0f
            };
            m_TextureCoordinatesVec = {
                -1.0f,  1.0f,
                 1.0f,  1.0f,
                -1.0f, -1.0f,
                 1.0f, -1.0f
            };
            m_FaceIndicesVec = {
                0, 1, 2, 3, 2, 1
            };
            break;
        }
        case BasicMeshes::Cube:
        {
            m_PositionsVec = {
                -1.0f, -1.0f,  1.0f,
                 1.0f, -1.0f,  1.0f,
                 1.0f, -1.0f, -1.0f,
                -1.0f, -1.0f, -1.0f,
                -1.0f,  1.0f,  1.0f,
                 1.0f,  1.0f,  1.0f,
                 1.0f,  1.0f, -1.0f,
                -1.0f,  1.0f, -1.0f,
            };
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
    }

    LOG_CALLSTACK_POP();
}

PixPhetamine::CMesh::~CMesh()
{
    
}

#include <array>
void PixPhetamine::CMesh::LoadToGPU()
{
    LOG_CALLSTACK_PUSH(__FILE__,__LINE__,__PRETTY_FUNCTION__);
    
    std::array<GLuint,3> VBOHandlesVec = {0,0,0};
    glGenBuffers(3, VBOHandlesVec.data());

    GLuint PositionsBufferHandle = VBOHandlesVec[0];
    GLuint NormalsBufferHandle = VBOHandlesVec[1];
    GLuint TextureCoordinatesBufferHandle = VBOHandlesVec[2];

    // Populate the position buffer
    glBindBuffer(GL_ARRAY_BUFFER, PositionsBufferHandle);
    glBufferData(GL_ARRAY_BUFFER, m_PositionsVec.size()*sizeof(GLfloat), m_PositionsVec.data(), GL_STATIC_DRAW);
    // Populate the normal buffer
    glBindBuffer(GL_ARRAY_BUFFER, NormalsBufferHandle);
    glBufferData(GL_ARRAY_BUFFER, m_NormalsVec.size()*sizeof(GLfloat), m_NormalsVec.data(), GL_STATIC_DRAW);
    // Populate the texture coordinate buffer
    glBindBuffer(GL_ARRAY_BUFFER, TextureCoordinatesBufferHandle);
    glBufferData(GL_ARRAY_BUFFER, m_TextureCoordinatesVec.size()*sizeof(GLfloat), m_TextureCoordinatesVec.data(), GL_STATIC_DRAW);

    // Create and set the VAO (Vertex Array Object)
    glGenVertexArrays(1, &m_VAOHandle);
    glBindVertexArray(m_VAOHandle);

    // Enable Vertex attributes arrays
    glEnableVertexAttribArray(0); // Vertex position
    glEnableVertexAttribArray(1); // Vertex normal
    glEnableVertexAttribArray(2); // Vertex texture coordinate

    // Tell the formats of each buffer
    glBindVertexBuffer(0, PositionsBufferHandle, 0, sizeof(GLfloat) * 3);
    glBindVertexBuffer(1, NormalsBufferHandle, 0, sizeof(GLfloat) * 3);
    glBindVertexBuffer(2, TextureCoordinatesBufferHandle, 0, sizeof(GLfloat) * 2);

    
    glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexAttribBinding(0, 0); // Map positions to shader pos0

    glVertexAttribFormat(1, 3, GL_FLOAT, GL_TRUE, 0);
    glVertexAttribBinding(1, 1); // Map normals to shader pos1

    glVertexAttribFormat(2, 2, GL_FLOAT, GL_FALSE, 0);
    glVertexAttribBinding(2, 2); // Map texture coords to shader pos2


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

