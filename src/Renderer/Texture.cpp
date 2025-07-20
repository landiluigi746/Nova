#include "Nova/Renderer/Texture.hpp"
#include "Nova/Renderer/GLError.hpp"
#include "Nova/Misc/Assert.hpp"
#include "Nova/Misc/Logger.hpp"

#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Nova
{
    Texture::~Texture()
    {
        Shutdown();
    }

    bool Texture::Init(const std::filesystem::path& path)
    {
        if (m_ID)
        {
            Logger::Warning("Can't set texture twice on the same texture!");
            return false;
        }

        if (!std::filesystem::is_regular_file(path))
        {
            Logger::Warning("Path {} either does not exist or is not a regular file!", path.string());
            return false;
        }

        std::string pathString = path.string();

        uint8_t* data = stbi_load(pathString.c_str(), &m_Width, &m_Height, &m_Channels, 0);

        if (!data)
        {
            Logger::Warning("Failed to load texture: {}", pathString);
            return false;
        }

        GLenum format;

        switch (m_Channels)
        {
        case 4:
            format = GL_RGBA;
            break;
        case 3:
            format = GL_RGB;
            break;
        default:
            Logger::Warning("Texture {} is in an unsupported format!", pathString);
            m_Width = m_Height = m_Channels = -1;
            stbi_image_free(data);
            return false;
        }

        Init(format, data);
        stbi_image_free(data);

        return true;
    }

    bool Texture::Init(uint32_t width, uint32_t height, const Color* data)
    {
        NOVA_ASSERT(data, "Cannot initialize texture with null data!");

        m_Width = width;
        m_Height = height;

        return Init(GL_RGBA, data);
    }

    void Texture::SetFilter(TextureFilter filter)
    {
        Bind(0);

        switch (filter)
        {
        case TextureFilter::Nearest:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            m_Filter = filter;
            break;
        case TextureFilter::Linear:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            m_Filter = filter;
            break;
        default:
            Logger::Warning("Unrecognized texture filter!");
        }

        Unbind();
    }

    void Texture::Bind(uint32_t slot) const
    {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, m_ID);
    }

    void Texture::Unbind() const
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void Texture::Shutdown()
    {
        if (m_ID)
        {
            glDeleteTextures(1, &m_ID);
            m_ID = 0;
        }
    }

    bool Texture::Init(int format, const void* data)
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        glGenTextures(1, &m_ID);
        glBindTexture(GL_TEXTURE_2D, m_ID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        GLenum internalFormat = (format == GL_RGBA) ? GL_RGBA8 : GL_RGB8;
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, data);

        glBindTexture(GL_TEXTURE_2D, 0);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

        CheckOpenGLErrors();

        return true;
    }
} // namespace Nova
