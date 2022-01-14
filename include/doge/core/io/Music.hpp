#pragma once

#include <SFML/Audio.hpp>
#include <doge/utils/Time.hpp>
#include <doge/utils/Vec3.hpp>

namespace doge::io
{
    struct Music
    {
        enum Status
        {
            Stopped, Paused, Playing
        };

        sf::Music music;

        bool FromFile(const std::string& filename);
        bool FromMemory(void* data, std::size_t size);

        std::uint32_t GetSampleRate() const;
        std::uint32_t GetChannelCount() const;

        Time GetDuration() const;

        void Play();
        void Pause();
        void Stop();

        void SetStatus(Status status);
        Status GetStatus() const;

        void SetLoop(bool loop);
        bool GetLoop() const;

        void SetLoopPoints(Time start, Time length);
        std::pair<Time, Time> GetLoopPoints() const;

        void SetPlayingOffset(Time timestamp);
        Time GetPlayingOffset() const;

        void SetPitch(float pitch);
        float GetPitch() const;

        void SetVolume(float volume);
        float GetVolume() const;

        void SetRelativeToListener(bool relative);
        bool IsRelativeToListener() const;

        void SetPosition(const Vec3f& position);
        Vec3f GetPosition() const;

        void SetMinDistance(float distance);
        float GetMinDistance() const;

        void SetAttenuation(float attenuation);
        float GetAttenuation() const;
    };
}