#pragma once

#include <cstdint>
#include <string_view>
#include <filesystem>
#include <unordered_map>
#include <glm/fwd.hpp>

namespace Nova
{
    class Shader
    {
    public:
        Shader() = default;
        virtual ~Shader();

        bool InitFromFile(const std::filesystem::path& fragmentPath);
        bool InitFromFiles(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath);
        bool Init(const std::string_view& vertexSource, const std::string_view& fragmentSource);
        void Shutdown();

        Shader(const Shader&) = delete;
        Shader& operator=(const Shader&) = delete;

        void SetUniformIntV(const std::string_view& name, const int32_t* values, uint32_t count);
        void SetUniformInt(const std::string_view& name, const int32_t value);
        void SetUniformFloat(const std::string_view& name, const float value);
        void SetUniformFloat2(const std::string_view& name, const glm::vec2& value);
        void SetUniformFloat3(const std::string_view& name, const glm::vec3& value);
        void SetUniformFloat4(const std::string_view& name, const glm::vec4& value);
        void SetUniformMat3(const std::string_view& name, const glm::mat3& value);
        void SetUniformMat4(const std::string_view& name, const glm::mat4& value);

        void Bind() const;
        void Unbind() const;

    private:
        int32_t GetUniformLocation(const std::string_view& name);

    private:
        uint32_t m_ID = 0;

        std::unordered_map<std::string_view, int32_t> m_UniformLocationCache;
    };
} // namespace Nova
