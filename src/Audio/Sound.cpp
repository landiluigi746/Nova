#include "Nova/Audio/Sound.hpp"

#include "Nova/Misc/Logger.hpp"

namespace Nova
{
    Sound::~Sound()
    {
        Shutdown();
    }

    bool Sound::Init(const std::filesystem::path& path)
    {
        std::string pathString = path.string();
        m_Sound = LoadSound(pathString.c_str());

        if (!IsSoundReady(m_Sound))
        {
            Logger::Warning("Failed to load sound: {}", pathString);
            return false;
        }

        return true;
    }

    void Sound::Shutdown() const
    {
        UnloadSound(m_Sound);
    }

    void Sound::Play() const
    {
        PlaySound(m_Sound);
    }

    void Sound::Stop() const
    {
        StopSound(m_Sound);
    }

    void Sound::Pause() const
    {
        PauseSound(m_Sound);
    }

    void Sound::Resume() const
    {
        ResumeSound(m_Sound);
    }

    void Sound::SetVolume(float volume) const
    {
        SetSoundVolume(m_Sound, volume);
    }

    void Sound::SetPitch(float pitch) const
    {
        SetSoundPitch(m_Sound, pitch);
    }

    void Sound::SetPan(float pan) const
    {
        SetSoundPan(m_Sound, pan);
    }

    bool Sound::IsPlaying() const
    {
        return IsSoundPlaying(m_Sound);
    }
} // namespace Nova