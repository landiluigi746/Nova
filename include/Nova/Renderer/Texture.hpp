#pragma once

#include "Nova/Misc/Color.hpp"

#include <cstdint>
#include <filesystem>

namespace Nova
{
    enum class TextureFilter : uint8_t
    {
        Nearest,
        Linear
    };

    class Texture
    {
    public:
        Texture() = default;
        virtual ~Texture();

        Texture(const Texture&) = delete;
        Texture(Texture&&) noexcept = delete;
        Texture& operator=(const Texture&) = delete;
        Texture& operator=(Texture&&) noexcept = delete;

        bool Init(const std::filesystem::path& path);
        bool Init(uint32_t width, uint32_t height, const Color* data);
        void Shutdown();

        void SetFilter(TextureFilter filter);
        void Bind(uint32_t slot) const;
        void Unbind() const;

        int GetID() const
        {
            return m_ID;
        }

        int GetWidth() const
        {
            return m_Width;
        }

        int GetHeight() const
        {
            return m_Height;
        }

        TextureFilter GetFilter() const
        {
            return m_Filter;
        }

    private:
        bool Init(int format, const void* data);

    private:
        uint32_t m_ID = 0;
        int m_Width = 0;
        int m_Height = 0;
        int m_Channels = 0;
        TextureFilter m_Filter = TextureFilter::Linear;
    };
} // namespace Nova
