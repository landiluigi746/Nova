#include "Nova/Asset/AssetManager.hpp"
#include "Nova/Misc/Assert.hpp"
#include "Nova/Misc/Logger.hpp"

namespace Nova
{
    static AssetManager* s_Instance = nullptr;

    AssetManager::AssetManager()
    {
        NOVA_ASSERT(!s_Instance, "AssetManager already created!");
        s_Instance = this;
    }

    void AssetManager::Shutdown()
    {
        m_Shaders.clear();
        m_Textures.clear();
        m_Sounds.clear();
        m_Musics.clear();
    }

    void AssetManager::LoadFromDirectory(const std::filesystem::path& path)
    {
        if (!std::filesystem::is_directory(path))
        {
            Logger::Warning("{} is not a directory! Skipping...", path.string());
            return;
        }

        if (std::filesystem::is_directory(path / "textures"))
        {
            for (const auto& entry : std::filesystem::directory_iterator(path / "textures"))
            {
                const auto& entryPath = entry.path();
                LoadTexture(entryPath.stem().string(), entryPath);
            }
        }

        if (std::filesystem::is_directory(path / "shaders"))
        {
            for (const auto& entry : std::filesystem::directory_iterator(path / "shaders"))
            {
                const auto& entryPath = entry.path();
                LoadShader(entryPath.stem().string(), entryPath);
            }
        }
    }

    void AssetManager::LoadTexture(const std::string_view& name, const std::filesystem::path& path)
    {
        if (m_Textures.contains(name))
        {
            Logger::Warning("Texture with name {} already exists! Skipping...", name);
            return;
        }

        Logger::Info("Loading texture {} ({})...", name, path.string());

        TextureAsset texture(std::make_shared<Texture>());

        if (!texture->Init(path))
            return;

        m_Textures.emplace(name, texture);
    }

    void AssetManager::LoadShader(const std::string_view& name, const std::filesystem::path& fragmentPath)
    {
        if (m_Shaders.contains(name))
        {
            Logger::Warning("Shader with name {} already exists! Skipping...", name);
            return;
        }

        Logger::Info("Loading fragment shader {} ({})...", name, fragmentPath.string());

        ShaderAsset shader(std::make_shared<Shader>());

        if (!shader->InitFromFile(fragmentPath))
            return;

        m_Shaders.emplace(name, shader);
    }

    void AssetManager::LoadSound(const std::string_view& name, const std::filesystem::path& path)
    {
        if (m_Sounds.contains(name))
        {
            Logger::Warning("Sound with name {} already exists! Skipping...", name);
            return;
        }

        Logger::Info("Loading sound {} ({})", name, path.string());

        SoundAsset sound(std::make_shared<Sound>());

        if (!sound->Init(path))
            return;

        m_Sounds.emplace(name, sound);
    }

    void AssetManager::LoadMusic(const std::string_view& name, const std::filesystem::path& path)
    {
        if (m_Musics.contains(name))
        {
            Logger::Warning("Music with name {} already exists! Skipping...", name);
            return;
        }

        Logger::Info("Loading music {} ({})", name, path.string());

        MusicAsset music(std::make_shared<Music>());

        if (!music->Init(path))
            return;

        m_Musics.emplace(name, music);
    }

    TextureAsset AssetManager::GetTexture(const std::string_view& name)
    {
        if (auto it = m_Textures.find(name); it != std::end(m_Textures))
            return it->second;

        Logger::Warning("Texture with name {} does not exist!", name);
        return TextureAsset();
    }

    ShaderAsset AssetManager::GetShader(const std::string_view& name)
    {
        if (auto it = m_Shaders.find(name); it != std::end(m_Shaders))
            return it->second;

        Logger::Warning("Shader with name {} does not exist!", name);
        return ShaderAsset();
    }

    SoundAsset AssetManager::GetSound(const std::string_view& name)
    {
        if (auto it = m_Sounds.find(name); it != std::end(m_Sounds))
            return it->second;

        Logger::Warning("Sound with name {} does not exist!", name);
        return SoundAsset();
    }

    MusicAsset AssetManager::GetMusic(const std::string_view& name)
    {
        if (auto it = m_Musics.find(name); it != std::end(m_Musics))
            return it->second;

        Logger::Warning("Music with name {} does not exist!", name);
        return MusicAsset();
    }
} // namespace Nova
