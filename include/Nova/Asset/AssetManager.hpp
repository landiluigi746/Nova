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
        AssetManager();

        AssetManager(const AssetManager&) = delete;
        AssetManager(AssetManager&&) = delete;
        AssetManager& operator=(const AssetManager&) = delete;
        AssetManager& operator=(AssetManager&&) = delete;

        void Shutdown();
        void LoadFromDirectory(const std::filesystem::path& path);

        void LoadTexture(const std::string& name, const std::filesystem::path& path);
        void LoadShader(const std::string& name, const std::filesystem::path& fragmentPath);

        TextureAsset GetTexture(const std::string& name);
        ShaderAsset GetShader(const std::string& name);

    private:
        std::unordered_map<std::string, TextureAsset> m_Textures;
        std::unordered_map<std::string, ShaderAsset> m_Shaders;
    };
} // namespace Nova
