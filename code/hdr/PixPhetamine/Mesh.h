#pragma once

#include <string>
#include <vector>
#include <glad/glad.h>

namespace PixPhetamine
{

class IMesh
{
protected:
    std::string m_Name;
    GLuint m_VAOHandle{0};
    std::vector<GLfloat> m_PositionsVec;
    bool m_IsLoadedInGPU{false};

public:
    IMesh(std::string name):m_Name(name) {}
    virtual ~IMesh();

    virtual void LoadToGPU() = 0;
    virtual void UnloadFromGPU() = 0;
    inline bool const GetIsLoadedInGPUState() const {return m_IsLoadedInGPU;}

    GLuint GetVAO() const {return m_VAOHandle;}
    virtual GLuint GetTriangleCount() const = 0;
};

class CMesh: public IMesh
{
private:
    std::vector<GLfloat> m_NormalsVec;
    std::vector<GLfloat> m_TextureCoordinatesVec;
    std::vector<GLuint> m_FaceIndicesVec;

public:
    enum class BasicMeshes
    {
        Triangle,
        Quad,
        Cube,
        Pawn
    };

    CMesh(std::string name, std::string filePath);
    CMesh(std::string name, BasicMeshes basicMesh);
    ~CMesh();

    void LoadToGPU() override;
    void UnloadFromGPU() override;
    GLuint GetTriangleCount() const override {return m_FaceIndicesVec.size();}
};

}