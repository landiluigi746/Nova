#pragma once

#include <mutex>
#include <cstdint>
#include <fmt/core.h>
#include <fmt/color.h>

namespace Nova
{
    class Logger
    {
    public:
        enum class Level : uint8_t
        {
            Trace = 0,
            Debug,
            Info,
            Warning,
            Error,
            Fatal
        };

        static constexpr const char* LevelToString(Level level)
        {
            // clang-format off
            switch (level)
            {
                case Level::Trace:      return "TRACE";
                case Level::Debug:      return "DEBUG";
                case Level::Info:       return "INFO";
                case Level::Warning:    return "WARNING";
                case Level::Error:      return "ERROR";
                case Level::Fatal:      return "FATAL";
            }
            // clang-format on

            return "UNKNOWN";
        }

        static void SetMinimumLogLevel(Level level)
        {
            s_LogLevel = level;
        }

        template<typename... Args>
        static void Log(Level level, fmt::terminal_color color, fmt::format_string<Args...> fmt, Args&&... args)
        {
            std::lock_guard<std::mutex> lock(s_Mutex);

            if (level < s_LogLevel)
                return;

            fmt::print(fmt::fg(color), "[{}]: {}\n", LevelToString(level),
                       fmt::format(fmt, std::forward<Args>(args)...));
        }

        template<typename... Args>
        static void Trace(fmt::format_string<Args...> fmt, Args&&... args)
        {
            Log(Level::Trace, fmt::terminal_color::white, fmt, std::forward<Args>(args)...);
        }

        template<typename... Args>
        static void Debug(fmt::format_string<Args...> fmt, Args&&... args)
        {
            Log(Level::Debug, fmt::terminal_color::bright_white, fmt, std::forward<Args>(args)...);
        }

        template<typename... Args>
        static void Info(fmt::format_string<Args...> fmt, Args&&... args)
        {
            Log(Level::Info, fmt::terminal_color::green, fmt, std::forward<Args>(args)...);
        }

        template<typename... Args>
        static void Warning(fmt::format_string<Args...> fmt, Args&&... args)
        {
            Log(Level::Warning, fmt::terminal_color::yellow, fmt, std::forward<Args>(args)...);
        }

        template<typename... Args>
        static void Error(fmt::format_string<Args...> fmt, Args&&... args)
        {
            Log(Level::Error, fmt::terminal_color::red, fmt, std::forward<Args>(args)...);
        }

        template<typename... Args>
        static void Fatal(fmt::format_string<Args...> fmt, Args&&... args)
        {
            Log(Level::Fatal, fmt::terminal_color::bright_red, fmt, std::forward<Args>(args)...);
        }

    private:
        static inline Level s_LogLevel = Level::Info;
        static inline std::mutex s_Mutex;
    };
} // namespace Nova
