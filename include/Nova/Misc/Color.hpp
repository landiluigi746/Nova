#pragma once

#include <cstdint>

namespace Nova
{
    struct Color
    {
        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a;
    };

    // clang-format off
    inline constexpr Color Blank        = {0,     0,   0,   0};
    inline constexpr Color White        = {255, 255, 255, 255};

    inline constexpr Color LightGray    = {200, 200, 200, 255};
    inline constexpr Color Gray         = {130, 130, 130, 255};
    inline constexpr Color DarkGray     = {80,   80,  80, 255};
        
    inline constexpr Color Black        = {0,     0,   0, 255};
        
    inline constexpr Color Yellow       = {253, 249,   0, 255};
    inline constexpr Color Gold         = {255, 203,   0, 255};
        
    inline constexpr Color Orange       = {255, 161,   0, 255};
        
    inline constexpr Color Red          = {230,  41,  55, 255};
    inline constexpr Color Maroon       = {190,  33,  55, 255};
        
    inline constexpr Color Green        = {0,   228,  48, 255};
    inline constexpr Color Lime         = {0,   158,  47, 255};
    inline constexpr Color DarkGreen    = {0,   117,  44, 255};
        
    inline constexpr Color SkyBlue      = {102, 191, 255, 255};
    inline constexpr Color Blue         = {0,   121, 241, 255};
    inline constexpr Color DarkBlue     = {0,    82, 172, 255};
        
    inline constexpr Color Pink         = {255, 109, 194, 255};
    inline constexpr Color Purple       = {200, 122, 255, 255};
    inline constexpr Color DarkPurple   = {112, 31,  126, 255};
    inline constexpr Color Magenta      = {255,   0, 255, 255};
    inline constexpr Color Violet       = {135, 60,  190, 255};

    inline constexpr Color Beige        = {211, 176, 131, 255};
    inline constexpr Color Brown        = {127, 106,  79, 255};
    inline constexpr Color DarkBrown    = {76,   63,  47, 255};
    // clang-format on

    inline constexpr bool operator==(const Color& lhs, const Color& rhs) noexcept
    {
        return ((lhs.r == rhs.r) && (lhs.g == rhs.g) && (lhs.b == rhs.b) && (lhs.a == rhs.a));
    }

    inline constexpr bool operator!=(const Color& lhs, const Color& rhs) noexcept
    {
        return !(lhs == rhs);
    }
} // namespace Nova
