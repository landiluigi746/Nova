#include "Nova/Renderer/Shader.hpp"
#include "Nova/Misc/Assert.hpp"
#include "Nova/Misc/Logger.hpp"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>

namespace Nova
{
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

    void Shader::InitFromFiles(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath)
    {
        if (!std::filesystem::is_regular_file(vertexPath))
        {
            Logger::Warning("Vertex shader path {} does not exist!", vertexPath.string().c_str());
            return;
        }

        if (!std::filesystem::is_regular_file(fragmentPath))
        {
            Logger::Warning("Fragment shader path {} does not exist!", fragmentPath.string().c_str());
            return;
        }

        std::string vertexSource;
        std::string fragmentSource;

        std::ifstream vertexFile(vertexPath);
        std::ifstream fragmentFile(fragmentPath);

        if (!vertexFile.is_open())
        {
            Logger::Warning("Failed to open vertex shader file {}!", vertexPath.string().c_str());
            return;
        }

        if (!fragmentFile.is_open())
        {
            Logger::Warning("Failed to open fragment shader file {}!", fragmentPath.string().c_str());
            return;
        }

        vertexFile.seekg(0, std::ios::end);
        vertexSource.resize(vertexFile.tellg());
        vertexFile.seekg(0, std::ios::beg);
        vertexFile.read(vertexSource.data(), vertexSource.size());

        fragmentFile.seekg(0, std::ios::end);
        fragmentSource.resize(fragmentFile.tellg());
        fragmentFile.seekg(0, std::ios::beg);
        fragmentFile.read(fragmentSource.data(), fragmentSource.size());

        Init(vertexSource, fragmentSource);
    }

    void Shader::Init(const std::string_view& vertexSource, const std::string_view& fragmentSource)
    {
        uint32_t vertexShader = CompileShader(GL_VERTEX_SHADER, vertexSource);
        uint32_t fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);

        m_ID = glCreateProgram();
        glAttachShader(m_ID, vertexShader);
        glAttachShader(m_ID, fragmentShader);
        glLinkProgram(m_ID);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
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
        if (m_UniformLocationCache.contains(name))
            return m_UniformLocationCache[name];

        int32_t location = glGetUniformLocation(m_ID, name.data());

        NOVA_ASSERT(location != -1, "Failed to get uniform location for {}", name);

        m_UniformLocationCache[name] = location;
        return location;
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
