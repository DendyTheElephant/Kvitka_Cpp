#include "PixPhetamine/VertexArrayObject.h"

void PixPhetamine::SVertexArrayObject::LoadToGPU(std::vector<float> &verticesVec, std::vector<float> &normalsVec, std::vector<float> &colorsVec, std::vector<float> &facesVec, GLenum mode)
{
    // Create some buffers inside the GPU memory
    glGenVertexArrays(1, &Id);
    glGenBuffers(1, &VBO_Vertices);
    glGenBuffers(1, &VBO_Normals);
    glGenBuffers(1, &VBO_Colors);
    glGenBuffers(1, &VBO_Indices);

    // Activate VAO
    glBindVertexArray(Id);

    // Store mesh positions into buffer inside the GPU memory
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Vertices);
    glBufferData(GL_ARRAY_BUFFER, verticesVec.size() * sizeof(float), verticesVec.data(), mode);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glEnableVertexAttribArray(0);

    // Store mesh normals into buffer inside the GPU memory
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Normals);
    glBufferData(GL_ARRAY_BUFFER, normalsVec.size() * sizeof(float), normalsVec.data(), mode);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 0, (void *)0);
    glEnableVertexAttribArray(1);

    // Store mesh colors into buffer inside the GPU memory
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Colors);
    glBufferData(GL_ARRAY_BUFFER, colorsVec.size() * sizeof(float), colorsVec.data(), mode);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glEnableVertexAttribArray(2);

    // Store mesh indices into buffer inside the GPU memory
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO_Indices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, facesVec.size() * sizeof(GLushort), facesVec.data(), mode);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


void PixPhetamine::SVertexArrayObject::Free()
{
    glDeleteBuffers(1, &VBO_Vertices);
    glDeleteBuffers(1, &VBO_Normals);
    glDeleteBuffers(1, &VBO_Colors);
    glDeleteBuffers(1, &VBO_Indices);
    glDeleteVertexArrays(1, &Id);
}