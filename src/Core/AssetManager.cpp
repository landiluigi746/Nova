#include "Nova/Core/AssetManager.hpp"
#include "Nova/Misc/Assert.hpp"
#include "Nova/Misc/Logger.hpp"

namespace Nova
{
    static AssetManager* s_Instance = nullptr;

    void AssetManager::Init()
    {
        if (s_Instance)
            return;

        Logger::Info("Initializing AssetManager...");
        s_Instance = new AssetManager();
        Logger::Info("AssetManager initialized successfully!");
    }

    void AssetManager::LoadFromDirectory(const std::filesystem::path& path)
    {
        s_Instance->LoadFromDirectoryImpl(path);
    }

    void AssetManager::LoadTexture(const std::string& name, const std::filesystem::path& path)
    {
        s_Instance->LoadTextureImpl(name, path);
    }

    void AssetManager::LoadShader(const std::string& name, const std::filesystem::path& fragmentPath)
    {
        s_Instance->LoadShaderImpl(name, fragmentPath);
    }

    TextureAsset AssetManager::GetTexture(const std::string& name)
    {
        return s_Instance->GetTextureImpl(name);
    }

    ShaderAsset AssetManager::GetShader(const std::string& name)
    {
        return s_Instance->GetShaderImpl(name);
    }

    void AssetManager::Shutdown()
    {
        if (!s_Instance)
            return;

        Logger::Info("Shutting down AssetManager...");
        delete s_Instance;
        s_Instance = nullptr;
        Logger::Info("AssetManager shut down successfully!");
    }

    void AssetManager::LoadFromDirectoryImpl(const std::filesystem::path& path)
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
                LoadTextureImpl(path.stem().string(), path);
            }
        }

        if (std::filesystem::is_directory(path / "shaders"))
        {
            for (const auto& entry : std::filesystem::directory_iterator(path / "shaders"))
            {
                auto path = entry.path();
                LoadShaderImpl(path.stem().string(), path);
            }
        }

        for (const auto& [name, texture] : m_Textures)
            Logger::Info("{}, id {}", name, texture.Asset->GetID());
    }

    void AssetManager::LoadTextureImpl(const std::string& name, const std::filesystem::path& path)
    {
        if (m_Textures.contains(name))
        {
            Logger::Warning("Texture with name {} already exists! Skipping...", name);
            return;
        }

        Logger::Info("Loading texture {}...", name);

        TextureAsset texture(std::make_shared<Texture>());
        texture.Asset->Init(path);

        m_Textures[name] = texture;
    }

    void AssetManager::LoadShaderImpl(const std::string& name, const std::filesystem::path& fragmentPath)
    {
        if (m_Shaders.contains(name))
        {
            Logger::Warning("Shader with name {} already exists! Skipping...", name);
            return;
        }

        Logger::Info("Loading shader {}...", name);

        ShaderAsset shader(std::make_shared<Shader>());
        shader.Asset->InitFromFile(fragmentPath);

        m_Shaders[name] = shader;
    }

    TextureAsset AssetManager::GetTextureImpl(const std::string& name)
    {
        if (m_Textures.contains(name))
            return m_Textures[name];

        Logger::Warning("Texture with name {} does not exist!", name);
        return TextureAsset();
    }

    ShaderAsset AssetManager::GetShaderImpl(const std::string& name)
    {
        if (m_Shaders.contains(name))
            return m_Shaders[name];

        Logger::Warning("Shader with name {} does not exist!", name);
        return ShaderAsset();
    }
} // namespace Nova
