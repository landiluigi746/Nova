#include "Nova/Renderer/Shader.hpp"
#include "Nova/Renderer/GLError.hpp"
#include "Nova/Misc/Assert.hpp"
#include "Nova/Misc/Logger.hpp"

#include <glad/glad.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>

namespace Nova
{
    static std::string ReadFile(const std::filesystem::path& path)
    {
        std::ifstream in(path);
        std::string buffer;

        if (!in.is_open())
        {
            Logger::Warning("Failed to open shader file {}!", path.string().c_str());
            return buffer;
        }

        in.seekg(0, std::ios::end);
        buffer.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(buffer.data(), buffer.size());
        in.close();

        return buffer;
    }

    static uint32_t CompileShader(GLenum type, const std::string_view& source)
    {
        uint32_t shader = glCreateShader(type);

        const char* src = source.data();
        glShaderSource(shader, 1, &src, nullptr);
        glCompileShader(shader);

        int success = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

        if (success)
            return shader;

        int len;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);

        std::string infoLog(len, '\0');

        glGetShaderInfoLog(shader, len, nullptr, infoLog.data());
        NOVA_ASSERT(false, "Failed to compile shader: {}", infoLog);
    }

    Shader::~Shader()
    {
        Shutdown();
    }

    bool Shader::InitFromFile(const std::filesystem::path& fragmentPath)
    {
        if (!std::filesystem::is_regular_file(fragmentPath))
        {
            Logger::Warning("Fragment shader path {} does not exist!", fragmentPath.string().c_str());
            return false;
        }

        return Init("", ReadFile(fragmentPath));
    }

    bool Shader::InitFromFiles(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath)
    {
        if (!std::filesystem::is_regular_file(vertexPath))
        {
            Logger::Warning("Vertex shader path {} does not exist!", vertexPath.string().c_str());
            return false;
        }

        if (!std::filesystem::is_regular_file(fragmentPath))
        {
            Logger::Warning("Fragment shader path {} does not exist!", fragmentPath.string().c_str());
            return false;
        }

        std::string vertexSource = ReadFile(vertexPath);
        std::string fragmentSource = ReadFile(fragmentPath);

        return Init(vertexSource, fragmentSource);
    }

    bool Shader::Init(const std::string_view& vertexSource, const std::string_view& fragmentSource)
    {
        if (fragmentSource.empty())
        {
            Logger::Warning("To create a shader, a fragment source must be provided!");
            return false;
        }

        uint32_t vertexShader, fragmentShader;

        m_ID = glCreateProgram();

        if (!vertexSource.empty())
        {
            vertexShader = CompileShader(GL_VERTEX_SHADER, vertexSource);
            glAttachShader(m_ID, vertexShader);
        }

        fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);
        glAttachShader(m_ID, fragmentShader);

        glLinkProgram(m_ID);
        glValidateProgram(m_ID);

        if (!vertexSource.empty())
            glDeleteShader(vertexShader);

        glDeleteShader(fragmentShader);

        CheckOpenGLErrors();
        return true;
    }

    void Shader::Shutdown()
    {
        if (m_ID)
        {
            glDeleteProgram(m_ID);
            m_ID = 0;
        }
    }

    int32_t Shader::GetUniformLocation(const std::string_view& name)
    {
        if (auto it = m_UniformLocationCache.find(name); it != m_UniformLocationCache.end())
            return it->second;

        int32_t location = glGetUniformLocation(m_ID, name.data());

        NOVA_ASSERT(location != -1, "Failed to get uniform location for {}", name);

        m_UniformLocationCache.emplace(name, location);
        return location;
    }

    void Shader::SetUniformIntV(const std::string_view& name, const int32_t* values, uint32_t count)
    {
        glUniform1iv(GetUniformLocation(name), count, values);
    }

    void Shader::SetUniformInt(const std::string_view& name, const int32_t value)
    {
        glUniform1i(GetUniformLocation(name), value);
    }

    void Shader::SetUniformFloat(const std::string_view& name, const float value)
    {
        glUniform1f(GetUniformLocation(name), value);
    }

    void Shader::SetUniformFloat2(const std::string_view& name, const glm::vec2& value)
    {
        glUniform2f(GetUniformLocation(name), value.x, value.y);
    }

    void Shader::SetUniformFloat3(const std::string_view& name, const glm::vec3& value)
    {
        glUniform3f(GetUniformLocation(name), value.x, value.y, value.z);
    }

    void Shader::SetUniformFloat4(const std::string_view& name, const glm::vec4& value)
    {
        glUniform4f(GetUniformLocation(name), value.x, value.y, value.z, value.w);
    }

    void Shader::SetUniformMat3(const std::string_view& name, const glm::mat3& value)
    {
        glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
    }

    void Shader::SetUniformMat4(const std::string_view& name, const glm::mat4& value)
    {
        glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
    }

    void Shader::Bind() const
    {
        glUseProgram(m_ID);
    }

    void Shader::Unbind() const
    {
        glUseProgram(0);
    }
} // namespace Nova
