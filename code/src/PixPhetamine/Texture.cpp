#include <PixPhetamine/Texture.h>
#include <DendyCommon/Logger.h>
#include <PixPhetamine/Definitions.h>
#include <PixPhetamine/RenderingSystem.h>

void PixPhetamine::CTexture::InitializeTexture(uint16_t const& width, uint16_t const& height, ETextureType const& textureType, bool const& isMultisampled, const void* pData)
{
    LOG_CALLSTACK_PUSH(__FILE__,__LINE__,__PRETTY_FUNCTION__);

    m_TextureType = textureType;
    m_IsMultisampled = isMultisampled;
    m_Width = width;
    m_Height = height;

    GLenum Target, Format, DataType;
    GLint InternalFormat, Filtering;

    (isMultisampled) ? Target = GL_TEXTURE_2D_MULTISAMPLE : Target = GL_TEXTURE_2D;

    switch (textureType) {
    case ETextureType::NORMAL:
        InternalFormat = GL_RGBA;
        Format = GL_RGBA;
        Filtering = GL_LINEAR;
        DataType = GL_FLOAT;
        break;
    case ETextureType::TRANSPARENT:
        InternalFormat = GL_RGBA;
        Format = GL_RGBA;
        Filtering = GL_LINEAR;
        DataType = GL_FLOAT;
        break;
    case ETextureType::HDR:
        InternalFormat = GL_RGB16F;
        Format = GL_RGB;
        Filtering = GL_LINEAR;
        DataType = GL_FLOAT;
        break;
    case ETextureType::HDR_TRANSPARENT:
        InternalFormat = GL_RGBA16F;
        Format = GL_RGBA;
        Filtering = GL_LINEAR;
        DataType = GL_FLOAT;
        break;
    case ETextureType::DEPTH:
        InternalFormat = GL_DEPTH_COMPONENT24;
        Format = GL_DEPTH_COMPONENT;
        Filtering = GL_NEAREST;
        DataType = GL_FLOAT;
        break;
    }

    glGenTextures(1, &m_Id);
    glBindTexture(Target, m_Id);

    if (isMultisampled)
    {
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, PixPhetamine::Definitions::c_MSAA, InternalFormat, width, height, GL_TRUE);
    }
    else
    {
        glTexParameteri(Target, GL_TEXTURE_MAG_FILTER, Filtering);
        glTexParameteri(Target, GL_TEXTURE_MIN_FILTER, Filtering);
        glTexParameteri(Target, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(Target, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexImage2D(GL_TEXTURE_2D, 0, InternalFormat, width, height, 0, Format, DataType, pData);
    }

    glBindTexture(Target, 0);

    PixPhetamine::CRenderingSystem::AssertOpenGLErrors();

    LOG_CALLSTACK_POP();
}

PixPhetamine::CTexture::CTexture(std::string const& texturePath, ETextureType const& textureType, bool const& isMultisampled)
{
    LOG_CALLSTACK_PUSH(__FILE__,__LINE__,__PRETTY_FUNCTION__);

    // Load file and decode image.
    std::vector<unsigned char> Data;
    unsigned Width, Height;
    unsigned Error = lodepng::decode(Data, Width, Height, texturePath);
    if (Error != 0)
    {
        LOG_CRITICAL_ERROR(lodepng_error_text(Error));
    }
    InitializeTexture(Width, Height, textureType, isMultisampled, Data.data());

    LOG_CALLSTACK_POP();
}

PixPhetamine::CTexture::CTexture(uint16_t const& width, uint16_t const& height, ETextureType const& textureType, bool const& isMultisampled)
{
    LOG_CALLSTACK_PUSH(__FILE__,__LINE__,__PRETTY_FUNCTION__);
    InitializeTexture(width, height, textureType, isMultisampled, nullptr);
    LOG_CALLSTACK_POP();
}

PixPhetamine::CTexture::~CTexture()
{
    glDeleteTextures(1, &m_Id);
}