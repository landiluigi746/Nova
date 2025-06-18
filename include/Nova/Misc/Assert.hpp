#pragma once

#include "Nova/Misc/Logger.hpp"

#include <functional>
#include <string_view>

namespace Nova
{
    using AssertCleanupFunc = std::function<void()>;

#ifdef NOVA_DEBUG
    template<typename... Args>
    void Assert(bool test, std::string_view testStr, const AssertCleanupFunc& cleanupFunc, std::string_view file,
                unsigned line, fmt::format_string<Args...> fmt, Args&&... args)
    {
        if (test)
            return;

        Logger::Fatal("Assertion Failed: {}\nError Message: {}\nFile:Line: {}:{}", testStr,
                      fmt::format(fmt, std::forward<Args>(args)...), file, line);

        if (cleanupFunc)
            cleanupFunc();

        std::abort();
    }
#else
    template<typename... Args>
    void Assert(bool test, std::string_view testStr, const AssertCleanupFunc& cleanupFunc,
                fmt::format_string<Args...> fmt, Args&&... args)
    {
        if (test)
            return;

        Logger::Fatal("Assertion Failed: {}\nError Message: {}", testStr,
                      fmt::format(fmt, std::forward<Args>(args)...));

        if (cleanupFunc)
            cleanupFunc();

        std::abort();
    }
#endif

} // namespace Nova

#ifdef NOVA_DEBUG
#define NOVA_ASSERT(test, msg, ...)                                                                                    \
    Nova::Assert((test), (#test), nullptr, __FILE__, __LINE__, (msg) __VA_OPT__(, ) __VA_ARGS__)
#define NOVA_ASSERT_CLEANUP_FUNC(test, msg, func, ...)                                                                 \
    Nova::Assert((test), (#test), (func), __FILE__, __LINE__, (msg) __VA_OPT__(, ) __VA_ARGS__)
#else
#define NOVA_ASSERT(test, msg, ...) Nova::Assert((test), (#test), nullptr, (msg) __VA_OPT__(, ) __VA_ARGS__)
#define NOVA_ASSERT_CLEANUP_FUNC(test, msg, func, ...)                                                                 \
    Nova::Assert((test), (#test), (func), (msg) __VA_OPT__(, ) __VA_ARGS__)
#endif
