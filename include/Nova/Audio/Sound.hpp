#pragma once

#include <raudio.h>
#include <filesystem>

namespace Nova
{
	class Sound
	{
    public:
        Sound() = default;
		virtual ~Sound();

		Sound(const Sound& sound) = delete;
		Sound(Sound&& sound) = delete;
		Sound& operator=(const Sound& sound) = delete;
		Sound& operator=(Sound&& sound) = delete;

		bool Init(const std::filesystem::path& path);
		void Shutdown() const;

		void Play() const;
        void Stop() const;
        void Pause() const;
		void Resume() const;

		void SetVolume(float volume) const;
		void SetPitch(float pitch) const;
		void SetPan(float pan) const;

		bool IsPlaying() const;

	private:
        ::Sound m_Sound = {};
	};
}