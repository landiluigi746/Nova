#pragma once

#include <cstdint>

namespace Nova
{
    struct Color
    {
        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a = 255;
    };

    // clang-format off
    static constexpr Color Blank        = {0,     0,   0,   0};
    static constexpr Color White        = {255, 255, 255};

    static constexpr Color LightGray    = {200, 200, 200};
    static constexpr Color Gray         = {130, 130, 130};
    static constexpr Color DarkGray     = {80,   80,  80};
        
    static constexpr Color Black        = {0,     0,   0};
        
    static constexpr Color Yellow       = {253, 249,   0};
    static constexpr Color Gold         = {255, 203,   0};
        
    static constexpr Color Orange       = {255, 161,   0};
        
    static constexpr Color Red          = {230,  41,  55};
    static constexpr Color Maroon       = {190,  33,  55};
        
    static constexpr Color Green        = {0,   228,  48};
    static constexpr Color Lime         = {0,   158,  47};
    static constexpr Color DarkGreen    = {0,   117,  44};
        
    static constexpr Color SkyBlue      = {102, 191, 255};
    static constexpr Color Blue         = {0,   121, 241};
    static constexpr Color DarkBlue     = {0,    82, 172};
        
    static constexpr Color Pink         = {255, 109, 194};
    static constexpr Color Purple       = {200, 122, 255};
    static constexpr Color DarkPurple   = {112, 31,  126};
    static constexpr Color Magenta      = {255,   0, 255};
    static constexpr Color Violet       = {135, 60,  190};

    static constexpr Color Beige        = {211, 176, 131};
    static constexpr Color Brown        = {127, 106,  79};
    static constexpr Color DarkBrown    = {76,   63,  47};
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
