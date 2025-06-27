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
                auto path = entry.path();
                LoadTexture(path.stem().string(), path);
            }
        }

        if (std::filesystem::is_directory(path / "shaders"))
        {
            for (const auto& entry : std::filesystem::directory_iterator(path / "shaders"))
            {
                auto path = entry.path();
                LoadShader(path.stem().string(), path);
            }
        }
    }

    void AssetManager::LoadTexture(const std::string& name, const std::filesystem::path& path)
    {
        if (m_Textures.contains(name))
        {
            Logger::Warning("Texture with name {} already exists! Skipping...", name);
            return;
        }

        Logger::Info("Loading texture {} ({})...", name, path.string());

        TextureAsset texture(std::make_shared<Texture>());

        if (!texture.Asset->Init(path))
            return;

        m_Textures[name] = texture;
    }

    void AssetManager::LoadShader(const std::string& name, const std::filesystem::path& fragmentPath)
    {
        if (m_Shaders.contains(name))
        {
            Logger::Warning("Shader with name {} already exists! Skipping...", name);
            return;
        }

        Logger::Info("Loading fragment shader {} ()...", name, fragmentPath.string());

        ShaderAsset shader(std::make_shared<Shader>());

        if (!shader.Asset->InitFromFile(fragmentPath))
            return;

        m_Shaders[name] = shader;
    }

    TextureAsset AssetManager::GetTexture(const std::string& name)
    {
        if (m_Textures.contains(name))
            return m_Textures[name];

        Logger::Warning("Texture with name {} does not exist!", name);
        return TextureAsset();
    }

    ShaderAsset AssetManager::GetShader(const std::string& name)
    {
        if (m_Shaders.contains(name))
            return m_Shaders[name];

        Logger::Warning("Shader with name {} does not exist!", name);
        return ShaderAsset();
    }
} // namespace Nova
