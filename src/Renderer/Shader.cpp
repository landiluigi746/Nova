#include "Nova/Renderer/Shader.hpp"
#include "Nova/Misc/Assert.hpp"
#include "Nova/Misc/Logger.hpp"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

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

    void Shader::SetUniformInt(const std::string_view& name, const int32_t value) const
    {
		glUniform1i(glGetUniformLocation(m_ID, name.data()), value);
    }

    void Shader::SetUniformFloat(const std::string_view& name, const float value) const
    {
		glUniform1f(glGetUniformLocation(m_ID, name.data()), value);
    }

    void Shader::SetUniformFloat2(const std::string_view& name, const glm::vec2& value) const
    {
		glUniform2f(glGetUniformLocation(m_ID, name.data()), value.x, value.y);
    }

    void Shader::SetUniformFloat3(const std::string_view& name, const glm::vec3& value) const
    {
		glUniform3f(glGetUniformLocation(m_ID, name.data()), value.x, value.y, value.z);
    }

    void Shader::SetUniformFloat4(const std::string_view& name, const glm::vec4& value) const
    {
        glUniform4f(glGetUniformLocation(m_ID, name.data()), value.x, value.y, value.z, value.w);
    }

    void Shader::SetUniformMat3(const std::string_view& name, const glm::mat3& value) const
    {
        glUniformMatrix3fv(glGetUniformLocation(m_ID, name.data()), 1, GL_FALSE, glm::value_ptr(value));
    }

    void Shader::SetUniformMat4(const std::string_view& name, const glm::mat4& value) const
    {
		glUniformMatrix4fv(glGetUniformLocation(m_ID, name.data()), 1, GL_FALSE, glm::value_ptr(value));
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