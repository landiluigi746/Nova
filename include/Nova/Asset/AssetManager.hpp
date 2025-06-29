#pragma once

#include "Nova/Asset/Assets.hpp"
#include "Nova/Misc/StringHash.hpp"

#include <cstdint>
#include <string>
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

        void LoadTexture(const std::string_view& name, const std::filesystem::path& path);
        void LoadShader(const std::string_view& name, const std::filesystem::path& fragmentPath);

        TextureAsset GetTexture(const std::string_view& name);
        ShaderAsset GetShader(const std::string_view& name);

    private:
        std::unordered_map<std::string, TextureAsset, StringHash, std::equal_to<>> m_Textures;
        std::unordered_map<std::string, ShaderAsset, StringHash, std::equal_to<>> m_Shaders;
    };
} // namespace Nova
