#pragma once

#include <functional>

namespace Nova
{
    enum class EasingType
    {
        LinearNone = 0,
        LinearIn,
        LinearOut,
        LinearInOut,
        SineIn,
        SineOut,
        SineInOut,
        CircIn,
        CircOut,
        CircInOut,
        CubicIn,
        CubicOut,
        CubicInOut,
        QuadIn,
        QuadOut,
        QuadInOut,
        ExpoIn,
        ExpoOut,
        ExpoInOut,
        BackIn,
        BackOut,
        BackInOut,
        BounceOut,
        BounceIn,
        BounceInOut,
        ElasticIn,
        ElasticOut,
        ElasticInOut,
        Last = ElasticInOut
    };

    struct EasingConfig
    {
        EasingType Type;
        float* Target;
        float To;
        float DurationMs;
        std::function<void()> OnComplete = nullptr;
    };

    class Easing
    {
    public:
        Easing(const EasingConfig& config);

        void Update(float milliseconds);
        bool IsCompleted() const noexcept
        {
            return m_ElapsedTime >= m_Config.DurationMs;
        }

    private:
        float m_StartValue = 0.0f;
        float m_ElapsedTime = 0.0f;
        EasingConfig m_Config;

        friend class Scene;
    };
} // namespace Nova
