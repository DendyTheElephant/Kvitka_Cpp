#pragma once

#include <lodepng.h>

#include <glad/glad.h>
#include <iostream>

namespace PixPhetamine
{

class CTexture
{
public:
    enum class ETextureType {
        DEPTH, ///< Used to save the depth component of a render pass (depth intensity (black-white))
        NORMAL, ///< Used to save the colors of a render pass will be clamped between [0,1] as floats in RGB space
        TRANSPARENT, ///< Used to save the colors of a render pass with transparency, will  be clamped between [0,1] as floats in RGBA space
        HDR, ///< Used to save the colors of a render pass will not be clamped, as floats in RGB space
        HDR_TRANSPARENT ///< Used to save the colors of a render pass with transparency, will not be clamped between, as floats in RGBA space
    };

protected:
    uint16_t m_Width{0}; ///< Texture width
    uint16_t m_Height{0}; ///< Texture height
    GLuint m_Id{0}; ///< ID of the Texture, used to reference the active Tetxure in the rendering pipeline
    bool m_IsMultisampled{ false };	///< If is multisampled, AA filters can be applied using interpolation
    ETextureType m_TextureType{ ETextureType::NORMAL };	///< Texture type, see ETextureType to know more about textures

/* Methods */
protected:
    void InitializeTexture(uint16_t const& width, uint16_t const& height, ETextureType const& textureType, bool const& willBeMultisampled, const void* data);

public:
    CTexture(std::string const& texturePath, ETextureType const& textureType, bool const& willBeMultisampled);
    CTexture(uint16_t const& width, uint16_t const& height, ETextureType const& textureType, bool const& willBeMultisampled);
    ~CTexture();
    uint16_t GetWidth() const { return m_Width; }
    uint16_t GetHeight() const { return m_Height; }
    GLuint GetId() const { return m_Id; }
    ETextureType GetType() const { return m_TextureType; }
    bool IsMultisampled() const { return m_IsMultisampled; }
};

}

