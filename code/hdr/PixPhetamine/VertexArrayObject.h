/// \brief		My VertexArrayObject structure to handle 3D models efficiently in VRAM
/// \details	Allocate VBOs (buffers) to store vertices, normals and colors of the mesh \n
///				Then load mesh attributes to the GPU (VRAM) \n
///				Use the ID of the VBO to display from the VRAM \n
///				Then free ressources on the GPU!

#include <glad/glad.h>
#include <vector>
#include <string>

namespace PixPhetamine
{

struct SVertexArrayObject
{
    GLuint Id{0}; ///< ID of the VAO, used to reference the active VBO in the rendering pipeline
    GLuint VBO_Vertices{0}; ///< Buffer containing the vertices of the 3D model
    GLuint VBO_Normals{0}; ///< Buffer containing the normals of the 3D model
    GLuint VBO_Colors{0}; ///< Buffer containing the colors of the 3D model
    GLuint VBO_Indices{0}; ///< Buffer containing the indices of the vertices of the 3D model

    /// \brief		Default constructor, to make static declaration then use with loadToGPU
    SVertexArrayObject() {}
    SVertexArrayObject(std::string filePath);
    
    void LoadToGPU(std::vector<float> &verticesVec, std::vector<float> &normalsVec, std::vector<float> &colorsVec, std::vector<float> &facesVec, GLenum mode);
    void Free();
};

}