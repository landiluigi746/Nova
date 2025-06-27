#pragma once

#include <string>

namespace Nova
{
    // clang-format off
    enum WindowFlags_
    {
        WindowFlags_None            = 0,
        WindowFlags_Vsync           = 1 << 0,
        WindowFlags_Fullscreen      = 1 << 1,
        WindowFlags_Resizable       = 1 << 2,
        WindowFlags_Undecorated     = 1 << 3,
        WindowFlags_EnableMSAAx4    = 1 << 4
    };
    // clang-format on

    using WindowFlags = int;

    struct WindowConfig
    {
        WindowFlags Flags;
        int Width;
        int Height;
        std::string Title;
    };
} // namespace Nova