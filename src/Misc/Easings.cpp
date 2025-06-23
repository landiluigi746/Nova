#include "Nova/Misc/Easings.hpp"

#include <reasings.h>
#include <array>

namespace Nova
{
    using EaseFunc = float (*)(float t, float b, float c, float d);

    static std::array<EaseFunc, (size_t) EasingType::Last + 1> s_EasingFuncs = {
        &EaseLinearNone,  &EaseLinearIn,  &EaseLinearOut,  &EaseLinearInOut,  &EaseSineIn,    &EaseSineOut,
        &EaseSineInOut,   &EaseCircIn,    &EaseCircOut,    &EaseCircInOut,    &EaseCubicIn,   &EaseCubicOut,
        &EaseCubicInOut,  &EaseQuadIn,    &EaseQuadOut,    &EaseQuadInOut,    &EaseExpoIn,    &EaseExpoOut,
        &EaseExpoInOut,   &EaseBackIn,    &EaseBackOut,    &EaseBackInOut,    &EaseBounceOut, &EaseBounceIn,
        &EaseBounceInOut, &EaseElasticIn, &EaseElasticOut, &EaseElasticInOut,
    };

    Easing::Easing(const EasingConfig& config)
        : m_StartValue(config.Target ? *config.Target : 0.0f), m_ElapsedTime(0.0f), m_Config(config)
    {
    }

    void Easing::Update(float deltaTime)
    {
        size_t type = (size_t) m_Config.Type;

        if (type > (size_t) EasingType::Last)
            return;

        if (m_Config.Target == nullptr)
            return;

        *(m_Config.Target) =
            s_EasingFuncs[type](m_ElapsedTime, m_StartValue, m_Config.To - m_StartValue, m_Config.DurationMs);

        m_ElapsedTime += deltaTime;

        if (IsCompleted())
            *(m_Config.Target) = m_Config.To;
    }
} // namespace Nova
