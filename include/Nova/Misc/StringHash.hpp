#pragma once

#include <cstdint>
#include <string_view>

namespace Nova
{
    struct StringHash
    {
        using is_transparent = void;

        std::size_t operator()(const std::string_view& str) const noexcept
        {
            return std::hash<std::string_view>{}(str);
        }
    };
} // namespace Nova