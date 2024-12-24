#include "GLTexture.h"
#define STB_IMAGE_IMPLEMENTATION
#include <Utils/stb_image.h>
#include <Core/Core.h>
#include <glad/glad.h>

PIX3D::GL::GLTexture::~GLTexture()
{
}

void PIX3D::GL::GLTexture::LoadFromFile(const std::string& path)
{
    int w, h, bpp;
    unsigned char* data = stbi_load(path.c_str(), &w, &h, &bpp, STBI_rgb_alpha);

    PIX_ASSERT_MSG(data, "Failed To Load Texture!");


    glCreateTextures(GL_TEXTURE_2D, 1, &m_Handle);

    glTextureParameteri(m_Handle, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(m_Handle, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTextureParameteri(m_Handle, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_Handle, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTextureStorage2D(m_Handle, 1, GL_RGBA8, w, h);
    glTextureSubImage2D(m_Handle, 0, 0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, data);

    m_TextureID = glGetTextureHandleARB(m_Handle); // Generate ID For Texture In Vram
    glMakeTextureHandleResidentARB(m_TextureID); // Make It Visible To Shaders

    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(data);
}

void PIX3D::GL::GLTexture::Bind()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_Handle);
}

void PIX3D::GL::GLTexture::Destroy()
{
    glMakeTextureHandleNonResidentARB(m_TextureID);
    glDeleteTextures(1, &m_Handle);
}
