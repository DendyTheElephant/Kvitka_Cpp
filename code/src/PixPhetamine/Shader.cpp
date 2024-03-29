#include <PixPhetamine/Shader.h>
#include <DendyCommon/Logger.h>
#include <PixPhetamine/RenderingSystem.h>

#include <iostream>
#include <vector>
#include <fstream>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

PixPhetamine::CShader::CShader(const char* vertexPath, const char* fragmentPath):
m_IsRealodable(true),
m_VertexFilePath(vertexPath),
m_FragmentFilePath(fragmentPath)
{
    LOG_CALLSTACK_PUSH(__FILE__,__LINE__,__PRETTY_FUNCTION__);
    _Load(_GetCode(vertexPath), _GetCode(fragmentPath));
    LOG_CALLSTACK_POP();
}

PixPhetamine::CShader::CShader(std::string vertexShaderName, std::string vertexCode, std::string fragmentShaderName, std::string fragmentCode):
m_IsRealodable(true),
m_VertexFilePath(vertexShaderName),
m_FragmentFilePath(fragmentShaderName)
{
    LOG_CALLSTACK_PUSH(__FILE__,__LINE__,__PRETTY_FUNCTION__);
    _Load(vertexCode, fragmentCode);
    LOG_CALLSTACK_POP();
}


PixPhetamine::CShader::~CShader()
{
    if (glIsProgram(m_Id))
    {
        glDeleteProgram(m_Id);
    }
}

void PixPhetamine::CShader::_CheckContext() const
{
    // Verify that the current binded shader is this one! (To optimise performance, prefer not to switch between shaders)
    // Active the shader before sending textures...
    GLint currentShader;
    glGetIntegerv(GL_CURRENT_PROGRAM, &currentShader);
    if (currentShader != m_Id)
    {
        glUseProgram(m_Id);
    }
}

void PixPhetamine::CShader::_Load(std::string vertexCode, std::string fragmentCode)
{
    LOG_CALLSTACK_PUSH(__FILE__,__LINE__,__PRETTY_FUNCTION__);

    unsigned int vertexId = glCreateShader(GL_VERTEX_SHADER);
    if (vertexId == 0)
    {
        LOG_CRITICAL_ERROR("Creation of Vertex shader failed!");
    }
    unsigned int fragmentId = glCreateShader(GL_FRAGMENT_SHADER);
    if (fragmentId == 0)
    {
        LOG_CRITICAL_ERROR("Creation of Fragment shader failed!");
    }

    // Vertex compilation
    const char * VertexCode = vertexCode.c_str();
    int VertexCodeLength = vertexCode.length();
    glShaderSource(vertexId, 1, &VertexCode, &VertexCodeLength);
    std::cerr << "Compilation of Vertex shader " << m_VertexFilePath.c_str() << ":";
    glCompileShader(vertexId);
    _CheckCompilation(vertexId);

    // Fragment compilation
    const char * FragmentCode = fragmentCode.c_str();
    int FragmentCodeLength = fragmentCode.length();
    glShaderSource(fragmentId, 1, &FragmentCode, &FragmentCodeLength);
    std::cerr << "Compilation of Fragment shader " << m_FragmentFilePath.c_str() << ":";
    glCompileShader(fragmentId);
    _CheckCompilation(fragmentId);


    // Create, Attach and Link program
    m_Id = glCreateProgram();
    glAttachShader(m_Id, fragmentId);
    glAttachShader(m_Id, vertexId);
    glLinkProgram(m_Id);
    _CheckLinks(m_Id);

    glDeleteShader(vertexId);
    glDeleteShader(fragmentId);

    PixPhetamine::CRenderingSystem::AssertOpenGLErrors();
    LOG_CALLSTACK_POP();
}


void PixPhetamine::CShader::Reload()
{
    LOG_CALLSTACK_PUSH(__FILE__,__LINE__,__PRETTY_FUNCTION__);
    // check if the program already contains a shader
    if (glIsProgram(m_Id))
    {
        // delete it...
        glDeleteProgram(m_Id);
    }

    // ... and reload it
    if (m_IsRealodable)
    {
        _Load(_GetCode(m_VertexFilePath.c_str()), _GetCode(m_FragmentFilePath.c_str()));
    }
    LOG_CALLSTACK_POP();
}


void PixPhetamine::CShader::_CheckCompilation(unsigned int shaderId) const
{
    // check if the compilation was successfull (and display syntax errors)
    // call it after each shader compilation
    int Result = GL_FALSE;
    int InfoLogLength;

    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &InfoLogLength);

    if (InfoLogLength > 1) 
    {
        std::cerr << std::endl;
        std::vector<char> message(InfoLogLength + 1);
        glGetShaderInfoLog(shaderId, InfoLogLength, NULL, &message[0]);
        std::cerr << &message[0] << std::endl;
        glDeleteShader(shaderId); // Don't leak the shader.
    }
    else {
        std::cerr << " OK!" << std::endl;
    }
}


void PixPhetamine::CShader::_CheckLinks(unsigned int programId) const
{
    // check if links were successfull (and display errors)
    // call it after linking the program
    int Result = GL_FALSE;
    int InfoLogLength;

    glGetProgramiv(programId, GL_LINK_STATUS, &Result);
    glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &InfoLogLength);

    if (InfoLogLength > 0)
    {
        std::vector<char> message(InfoLogLength + 1);
        glGetProgramInfoLog(programId, InfoLogLength, NULL, &message[0]);
        std::cerr << &message[0] << std::endl;
    }
}


std::string PixPhetamine::CShader::_GetCode(const char * filePath) const
{
    LOG_CALLSTACK_PUSH(__FILE__,__LINE__,__PRETTY_FUNCTION__);
    // Return a string containing the source code of the input file
    std::string ShaderCode;
    std::ifstream ShaderStream(filePath, std::ios::in);

    if (!ShaderStream.is_open())
    {
        LOG_CRITICAL_ERROR("File ["+std::string(filePath) +"] not found!");
    }

    std::string Line = "";
    while (getline(ShaderStream, Line))
        ShaderCode += "\n" + Line;
    ShaderStream.close();

    LOG_CALLSTACK_POP();
    return ShaderCode;
}


void PixPhetamine::CShader::DeclareUniformVariableName(const char * correspondingVariableNameInShader)
{
    LOG_CALLSTACK_PUSH(__FILE__,__LINE__,__PRETTY_FUNCTION__);
    _CheckContext();
    m_VariableNamesMap[correspondingVariableNameInShader] = glGetUniformLocation(m_Id, correspondingVariableNameInShader);
    PixPhetamine::CRenderingSystem::AssertOpenGLErrors();
    LOG_CALLSTACK_POP();
}


// Here goes the specifications of the SendVariable():

// Template specification <pxFloat> to send
template <>
void PixPhetamine::CShader::SendUniformVariable(const char * correspondingVariableNameInShader, float const& a_variable)
{
    LOG_CALLSTACK_PUSH(__FILE__,__LINE__,__PRETTY_FUNCTION__);
    _CheckContext();
    if (m_VariableNamesMap.count(correspondingVariableNameInShader) != 0)
    {
        glUniform1f(m_VariableNamesMap[correspondingVariableNameInShader], a_variable);
        PixPhetamine::CRenderingSystem::AssertOpenGLErrors();
    }
    else
    {
        LOG_CRITICAL_ERROR("["+std::string(correspondingVariableNameInShader) + "] haven't been binded to the shader!");
    }
    LOG_CALLSTACK_POP();
}

// Template specification <int> to send
template <>
void PixPhetamine::CShader::SendUniformVariable(const char * correspondingVariableNameInShader, int const& a_variable)
{
    LOG_CALLSTACK_PUSH(__FILE__,__LINE__,__PRETTY_FUNCTION__);
    _CheckContext();
    if (m_VariableNamesMap.count(correspondingVariableNameInShader) != 0)
    {
        glUniform1i(m_VariableNamesMap[correspondingVariableNameInShader], a_variable);
        PixPhetamine::CRenderingSystem::AssertOpenGLErrors();
    }
    else 
    {
        LOG_CRITICAL_ERROR("["+std::string(correspondingVariableNameInShader) + "] haven't been binded to the shader!");
    }
    LOG_CALLSTACK_POP();
}

// Template specification <glm::vec3> to send
template <>
void PixPhetamine::CShader::SendUniformVariable(const char * correspondingVariableNameInShader, glm::vec3 const& a_variable)
{
    LOG_CALLSTACK_PUSH(__FILE__,__LINE__,__PRETTY_FUNCTION__);
    _CheckContext();
    if (m_VariableNamesMap.count(correspondingVariableNameInShader) != 0)
    {
        glUniform3f(m_VariableNamesMap[correspondingVariableNameInShader], a_variable[0], a_variable[1], a_variable[2]);
        PixPhetamine::CRenderingSystem::AssertOpenGLErrors();
    }
    else
    {
        LOG_CRITICAL_ERROR("["+std::string(correspondingVariableNameInShader) + "] haven't been binded to the shader!");
    }
    LOG_CALLSTACK_POP();
}

// Template specification <glm::vec4> to send
template <>
void PixPhetamine::CShader::SendUniformVariable(const char * correspondingVariableNameInShader, glm::vec4 const& a_variable)
{
    LOG_CALLSTACK_PUSH(__FILE__,__LINE__,__PRETTY_FUNCTION__);
    _CheckContext();
    if (m_VariableNamesMap.count(correspondingVariableNameInShader) != 0)
    {
        glUniform4f(m_VariableNamesMap[correspondingVariableNameInShader], a_variable[0], a_variable[1], a_variable[2], a_variable[3]);
        PixPhetamine::CRenderingSystem::AssertOpenGLErrors();
    }
    else
    {
        LOG_CRITICAL_ERROR("["+std::string(correspondingVariableNameInShader) + "] haven't been binded to the shader!");
    }
    LOG_CALLSTACK_POP();
}

// Template specification <glm::mat4> to send
template <>
void PixPhetamine::CShader::SendUniformVariable(const char * correspondingVariableNameInShader, glm::mat4 const& a_variable)
{
    LOG_CALLSTACK_PUSH(__FILE__,__LINE__,__PRETTY_FUNCTION__);
    _CheckContext();
    if (m_VariableNamesMap.count(correspondingVariableNameInShader) != 0)
    {
        glUniformMatrix4fv(m_VariableNamesMap[correspondingVariableNameInShader], 1, GL_FALSE, glm::value_ptr(a_variable));
        PixPhetamine::CRenderingSystem::AssertOpenGLErrors();
    }
    else
    {
        LOG_CRITICAL_ERROR("["+std::string(correspondingVariableNameInShader) + "] haven't been binded to the shader!");
    }
    LOG_CALLSTACK_POP();
}