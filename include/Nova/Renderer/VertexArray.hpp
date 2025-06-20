#pragma once

#include <cstdint>
#include <initializer_list>

namespace Nova
{
    enum class ShaderDataType : uint8_t
    {
        Float,
        Float2,
        Float3,
        Float4,
        Mat3,
        Mat4,
        Int,
        Int2,
        Int3,
        Int4,
        Bool
    };

    struct VertexBufferElement
    {
        ShaderDataType Type;
        bool Normalized;
    };

    class VertexArray
    {
    public:
        VertexArray() = default;
        virtual ~VertexArray();

        void Init();
        void Shutdown();

        VertexArray(const VertexArray&) = delete;
        VertexArray& operator=(const VertexArray&) = delete;

        void InitVertexBuffer(const void* vertices, uint32_t sizeBytes,
                              std::initializer_list<VertexBufferElement> layout = {});
        void InitIndexBuffer(const uint32_t* indices, uint32_t count);

        void SetVertexBufferData(const void* vertices, uint32_t sizeBytes);
        void SetIndexBufferData(const uint32_t* indices, uint32_t count);

        void Bind() const;
        void Unbind() const;

    private:
        uint32_t m_ID = 0;
        uint32_t m_VertexBufferID = 0;
        uint32_t m_IndexBufferID = 0;
        bool m_VertexBufferDynamic = false;
        bool m_IndexBufferDynamic = false;
    };
} // namespace Nova