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
        void LoadSound(const std::string_view& name, const std::filesystem::path& path);
        void LoadMusic(const std::string_view& name, const std::filesystem::path& path);

        TextureAsset GetTexture(const std::string_view& name);
        ShaderAsset GetShader(const std::string_view& name);
        SoundAsset GetSound(const std::string_view& name);
        MusicAsset GetMusic(const std::string_view& name);

    private:
        template<typename T>
        using AssetContainer = std::unordered_map<std::string, T, StringHash, std::equal_to<>>;

        AssetContainer<TextureAsset> m_Textures;
        AssetContainer<ShaderAsset> m_Shaders;
        AssetContainer<SoundAsset> m_Sounds;
        AssetContainer<MusicAsset> m_Musics;
    };
} // namespace Nova
