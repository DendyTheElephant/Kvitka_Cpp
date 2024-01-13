#pragma once

#include <glad/glad.h>
#include <string>
#include <map>

namespace PixPhetamine
{

class CShader
{
private:
    unsigned int m_Id{0};
    std::map<std::string, GLuint> m_VariableNamesMap;
    std::string m_VertexFilePath;
    std::string m_FragmentFilePath;
    bool m_IsRealodable{false};

private:
    // string containing the source code of the input file
    std::string _GetCode(const char * filePath) const;

    void _Load(std::string vertexCode, std::string fragmentCode);
    
    // call it after each shader compilation
    void _CheckCompilation(unsigned int shaderId) const;
    
    // call it after linking the program
    void _CheckLinks(unsigned int programId) const;
    
    // call before sending or binding var/name to make shure that the current program is this shader!
    void _CheckContext() const;

public:
    CShader(const char* vertexPath, const char* fragmentPath);
    CShader(std::string vertexShaderName, std::string vertexCode, std::string fragmentShaderName, std::string fragmentCode);
    ~CShader();

    void Reload();
    inline unsigned int GetId() { return m_Id; }

    void BindVariableName(const char * correspondingVariableNameInShader);
    GLuint GetVariableAttachment(const char * correspondingVariableNameInShader) { return m_VariableNamesMap[correspondingVariableNameInShader]; }

    template<typename SHADER_FRIENDLY_TYPE>
    void SendVariable(const char * correspondingVariableNameInShader, SHADER_FRIENDLY_TYPE const& variable);
};

}