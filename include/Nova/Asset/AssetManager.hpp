#pragma once

#include "Nova/Asset/Assets.hpp"

#include <cstdint>
#include <string>
#include <memory>
#include <string_view>
#include <filesystem>
#include <unordered_map>

namespace Nova
{

    class AssetManager
    {
    public:
        AssetManager(const AssetManager&) = delete;
        AssetManager(AssetManager&&) = delete;
        AssetManager& operator=(const AssetManager&) = delete;
        AssetManager& operator=(AssetManager&&) = delete;

        static void Init();
        static void Shutdown();

        static void LoadFromDirectory(const std::filesystem::path& path);

        static void LoadTexture(const std::string& name, const std::filesystem::path& path);
        static void LoadShader(const std::string& name, const std::filesystem::path& fragmentPath);

        static TextureAsset GetTexture(const std::string& name);
        static ShaderAsset GetShader(const std::string& name);

    private:
        AssetManager() = default;
        ~AssetManager() = default;

        void LoadFromDirectoryImpl(const std::filesystem::path& path);

        void LoadTextureImpl(const std::string& name, const std::filesystem::path& path);
        void LoadShaderImpl(const std::string& name, const std::filesystem::path& fragmentPath);

        TextureAsset GetTextureImpl(const std::string& name);
        ShaderAsset GetShaderImpl(const std::string& name);

    private:
        std::unordered_map<std::string, TextureAsset> m_Textures;
        std::unordered_map<std::string, ShaderAsset> m_Shaders;
    };
} // namespace Nova
