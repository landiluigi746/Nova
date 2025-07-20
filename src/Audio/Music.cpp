#include "Nova/Audio/Music.hpp"

#include "Nova/Misc/Logger.hpp"

namespace Nova
{
    Music::~Music()
    {
        Shutdown();
    }

    bool Music::Init(const std::filesystem::path& path)
    {
        std::string pathString = path.string();
        m_Music = LoadMusicStream(pathString.c_str());

        if (!IsMusicReady(m_Music))
        {
            Logger::Warning("Failed to load sound: {}", pathString);
            return false;
        }

        return true;
    }

    void Music::Shutdown() const
    {
        UnloadMusicStream(m_Music);
    }

    void Music::Play() const
    {
        PlayMusicStream(m_Music);
    }

    void Music::Update() const
    {
        UpdateMusicStream(m_Music);
    }

    void Music::Stop() const
    {
        StopMusicStream(m_Music);
    }

    void Music::Pause() const
    {
        PauseMusicStream(m_Music);
    }

    void Music::Resume() const
    {
        ResumeMusicStream(m_Music);
    }

    void Music::Seek(float seconds) const
    {
        SeekMusicStream(m_Music, seconds);
    }

    void Music::SetVolume(float volume) const
    {
        SetMusicVolume(m_Music, volume);
    }

    void Music::SetPitch(float pitch) const
    {
        SetMusicPitch(m_Music, pitch);
    }

    void Music::SetPan(float pan) const
    {
        SetMusicPan(m_Music, pan);
    }

    float Music::GetSecondsLength() const
    {
        return GetMusicTimeLength(m_Music);
    }

    float Music::GetSecondsPlayed() const
    {
        return GetMusicTimePlayed(m_Music);
    }

    bool Music::IsPlaying() const
    {
        return IsMusicStreamPlaying(m_Music);
    }
} // namespace Nova