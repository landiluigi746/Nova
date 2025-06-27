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

        // clang-format off
        operator bool() const { return (bool) Asset; }
        operator std::shared_ptr<T>() const { return Asset; }

        T* operator->() const { return Asset.get(); }

        T& operator*() const { return *Asset; }

        bool operator==(const AssetHandle& other) const noexcept = default;
        bool operator==(std::nullptr_t) const noexcept { return Asset == nullptr; }
        // clang-format on

    private:
        std::shared_ptr<T> Asset;

        friend class AssetManager;
    };

    using TextureAsset = AssetHandle<Texture>;
    using ShaderAsset = AssetHandle<Shader>;
} // namespace Nova
