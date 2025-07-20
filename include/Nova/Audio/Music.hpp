#pragma once

#include <raudio.h>
#include <filesystem>

namespace Nova
{
	class Music
	{
	public:
        Music() = default;
		virtual ~Music();

		Music(const Music& music) = delete;
		Music(Music&& music) = delete;
		Music& operator=(const Music& music) = delete;
		Music& operator=(Music&& music) = delete;

		bool Init(const std::filesystem::path& path);
		void Shutdown() const;
		
		void Play() const;
        void Update() const;
		void Stop() const;
		void Pause() const;
		void Resume() const;
		
		void Seek(float seconds) const;
		void SetVolume(float volume) const;
		void SetPitch(float pitch) const;
		void SetPan(float pan) const;

		float GetSecondsLength() const;
		float GetSecondsPlayed() const;
		
		bool IsPlaying() const;
	private:
		::Music m_Music = {};
	};
}