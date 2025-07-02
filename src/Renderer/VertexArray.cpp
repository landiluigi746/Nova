#include "Nova/Renderer/VertexArray.hpp"
#include "Nova/Misc/Assert.hpp"

#include <glad/glad.h>

namespace Nova
{
    static inline uint32_t GetComponentCountOfType(ShaderDataType type)
    {
        // clang-format off
		switch (type)
		{
		case ShaderDataType::Float:     return 1;
		case ShaderDataType::Float2:    return 2;
		case ShaderDataType::Float3:    return 3;
		case ShaderDataType::Float4:    return 4;
		case ShaderDataType::Mat3:      return 3;
		case ShaderDataType::Mat4:      return 4;
		case ShaderDataType::Int:       return 1;
		case ShaderDataType::Int2:      return 2;
		case ShaderDataType::Int3:      return 3;
		case ShaderDataType::Int4:      return 4;
		case ShaderDataType::Bool:      return 1;
		}
        // clang-format on

        NOVA_ASSERT(false, "Unknown ShaderDataType");
    }

    static inline uint32_t GetSizeOfType(ShaderDataType type)
    {
        // clang-format off
        switch (type)
        {
        case ShaderDataType::Float:     return 4;
        case ShaderDataType::Float2:    return 4 * 2;
        case ShaderDataType::Float3:    return 4 * 3;
        case ShaderDataType::Float4:    return 4 * 4;
        case ShaderDataType::Mat3:      return 4 * 3 * 3;
        case ShaderDataType::Mat4:      return 4 * 4 * 4;
        case ShaderDataType::Int:       return 4;
        case ShaderDataType::Int2:      return 4 * 2;
        case ShaderDataType::Int3:      return 4 * 3;
        case ShaderDataType::Int4:      return 4 * 4;
        case ShaderDataType::Bool:      return 1;
        }
        // clang-format on

        NOVA_ASSERT(false, "Unknown ShaderDataType");
    }

    static inline GLenum GetGLType(ShaderDataType type)
    {
        // clang-format off
		switch (type)
		{
		case ShaderDataType::Float:     return GL_FLOAT;
		case ShaderDataType::Float2:    return GL_FLOAT;
		case ShaderDataType::Float3:    return GL_FLOAT;
		case ShaderDataType::Float4:    return GL_FLOAT;
		case ShaderDataType::Mat3:      return GL_FLOAT;
		case ShaderDataType::Mat4:      return GL_FLOAT;
		case ShaderDataType::Int:       return GL_INT;
		case ShaderDataType::Int2:      return GL_INT;
		case ShaderDataType::Int3:      return GL_INT;
		case ShaderDataType::Int4:      return GL_INT;
		case ShaderDataType::Bool:      return GL_BOOL;
		}
        // clang-format on

        NOVA_ASSERT(false, "Unknown ShaderDataType");
    }

    VertexArray::~VertexArray()
    {
        Shutdown();
    }

    void VertexArray::Init()
    {
        glGenVertexArrays(1, &m_ID);
    }

    void VertexArray::Shutdown()
    {
        if (m_ID)
        {
            glDeleteVertexArrays(1, &m_ID);
            m_ID = 0;
        }

        if (m_VertexBufferID)
        {
            glDeleteBuffers(1, &m_VertexBufferID);
            m_VertexBufferID = 0;
        }

        if (m_IndexBufferID)
        {
            glDeleteBuffers(1, &m_IndexBufferID);
            m_IndexBufferID = 0;
        }
    }

    void VertexArray::Bind() const
    {
        glBindVertexArray(m_ID);
    }

    void VertexArray::Unbind() const
    {
        glBindVertexArray(0);
    }

    void VertexArray::InitVertexBuffer(const void* data, uint32_t sizeBytes,
                                       std::initializer_list<VertexBufferElement> layout)
    {
        if (m_VertexBufferID)
        {
            Logger::Warning("Cannot set vertex buffer twice on the same vertex array!");
            return;
        }

        uint32_t stride = 0;
        for (const auto& element : layout)
            stride += GetSizeOfType(element.Type);

        GLenum usage = (data) ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW;
        m_VertexBufferDynamic = (usage == GL_DYNAMIC_DRAW);

        glGenBuffers(1, &m_VertexBufferID);
        glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID);
        glBufferData(GL_ARRAY_BUFFER, sizeBytes, data, usage);

        uint32_t offset = 0;
        uint32_t index = 0;

        // moved here because of compiler errors
        GLenum type;
        uint32_t rows;
        uint32_t cols;

        for (const auto& element : layout)
        {
            uint32_t count = GetComponentCountOfType(element.Type);
            uint32_t typeSize = GetSizeOfType(element.Type);

            switch (element.Type)
            {
            case ShaderDataType::Float:
            case ShaderDataType::Float2:
            case ShaderDataType::Float3:
            case ShaderDataType::Float4:
                glEnableVertexAttribArray(index);
                glVertexAttribPointer(index, count, GetGLType(element.Type), (element.Normalized ? GL_TRUE : GL_FALSE),
                                      stride, (const void*) offset);
                offset += typeSize;
                break;
            case ShaderDataType::Int:
            case ShaderDataType::Int2:
            case ShaderDataType::Int3:
            case ShaderDataType::Int4:
            case ShaderDataType::Bool:
                type = (element.Type == ShaderDataType::Bool) ? GL_INT : GetGLType(element.Type);

                glEnableVertexAttribArray(index);
                glVertexAttribIPointer(index, count, type, stride, (const void*) offset);
                offset += typeSize;
                break;
            case ShaderDataType::Mat3:
            case ShaderDataType::Mat4:
                cols = count;
                rows = (element.Type == ShaderDataType::Mat3) ? 3 : 4;
                for (uint32_t i = 0; i < cols; ++i)
                {
                    glEnableVertexAttribArray(index + i);
                    glVertexAttribPointer(index + i, rows, GetGLType(element.Type), GL_FALSE, stride,
                                          (const void*) (offset + sizeof(float) * rows * i));
                }
                offset += typeSize;
                index += cols - 1;
                break;
            default:
                NOVA_ASSERT(false, "Unknown ShaderDataType");
            }

            ++index;
        }
    }

    void VertexArray::InitIndexBuffer(const uint32_t* indices, uint32_t count)
    {
        if (m_IndexBufferID)
        {
            Logger::Warning("Cannot set index buffer twice on the same vertex array!");
            return;
        }

        GLenum usage = (indices) ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW;
        m_IndexBufferDynamic = (usage == GL_DYNAMIC_DRAW);

        glGenBuffers(1, &m_IndexBufferID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, usage);
    }

    void VertexArray::SetVertexBufferData(const void* vertices, uint32_t sizeBytes)
    {
        if (!m_VertexBufferID)
        {
            Logger::Warning("Cannot set vertex buffer on a vertex array with no vertex buffer!");
            return;
        }

        if (!m_VertexBufferDynamic)
        {
            Logger::Warning("Cannot set vertex buffer on a vertex array with a static vertex buffer!");
            return;
        }

        if (!vertices)
        {
            Logger::Warning("Cannot set vertex buffer to nullptr!");
            return;
        }

        glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeBytes, vertices);
    }

    void VertexArray::SetIndexBufferData(const uint32_t* indices, uint32_t count)
    {
        if (!m_IndexBufferID)
        {
            Logger::Warning("Cannot set index buffer on a vertex array with no index buffer!");
            return;
        }

        if (!m_IndexBufferDynamic)
        {
            Logger::Warning("Cannot set index buffer on a vertex array with a static index buffer!");
            return;
        }

        if (!indices)
        {
            Logger::Warning("Cannot set index buffer to nullptr!");
            return;
        }

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferID);
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, count * sizeof(uint32_t), indices);
    }
} // namespace Nova
