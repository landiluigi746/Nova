#pragma once

#include <cstdint>
#include <string_view>
#include <filesystem>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>

namespace Nova
{
	class Shader
	{
    public:
		Shader() = default;
        virtual ~Shader();

		void InitFromFiles(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath);
        void Init(const std::string_view& vertexSource, const std::string_view& fragmentSource);
		void Shutdown();

		Shader(const Shader&) = delete;
		Shader& operator=(const Shader&) = delete;

		void SetUniformInt(const std::string_view& name, const int32_t value) const;
		void SetUniformFloat(const std::string_view& name, const float value) const;
		void SetUniformFloat2(const std::string_view& name, const glm::vec2& value) const;
		void SetUniformFloat3(const std::string_view& name, const glm::vec3& value) const;
		void SetUniformFloat4(const std::string_view& name, const glm::vec4& value) const;
		void SetUniformMat3(const std::string_view& name, const glm::mat3& value) const;
		void SetUniformMat4(const std::string_view& name, const glm::mat4& value) const;

		void Bind() const;
		void Unbind() const;
    private:
		uint32_t m_ID = 0;
	};
}