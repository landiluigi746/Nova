#pragma once

#include "Nova/Renderer/Texture.hpp"
#include "Nova/Renderer/Shader.hpp"

#include <memory>

namespace Nova
{
    template<typename T>
    class AssetHandle
    {
    public:
        AssetHandle() = default;
        AssetHandle(std::shared_ptr<T> asset) : Asset(asset) {}

        operator bool() const
        {
            return (bool)Asset;
        }

        operator std::shared_ptr<T>() const
        {
            return Asset;
        }

    private:
        std::shared_ptr<T> Asset;

        friend class AssetManager;
    };

    using TextureAsset = AssetHandle<Texture>;
	using ShaderAsset = AssetHandle<Shader>;
} // namespace Nova
