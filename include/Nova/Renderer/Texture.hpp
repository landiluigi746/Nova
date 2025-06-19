#pragma once

#include "Nova/Misc/Color.hpp"

#include <cstdint>
#include <filesystem>

namespace Nova
{
    class Texture
    {
    public:
        Texture() = default;
        virtual ~Texture();

        void Init(const std::filesystem::path& path);
        void Init(uint32_t width, uint32_t height, const Color* data);
        void Shutdown();

        void Bind() const;
        void Unbind() const;

        int GetWidth() const
        {
            return m_Width;
        }

        int GetHeight() const
        {
            return m_Height;
        }

    private:
        void Init(int format, const void* data);

    private:
        uint32_t m_ID = 0;
        int m_Width = 0;
        int m_Height = 0;
        int m_Channels = 0;
    };
} // namespace Nova
