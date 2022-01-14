#pragma once

#include <doge/core/io/SoundBuffer.hpp>
#include <SFML/Audio.hpp>
#include <doge/utils/Vec3.hpp>
#include <doge/utils/Time.hpp>

namespace doge::io
{
    struct SoundBuffer;

    struct Sound
    {
        enum Status
        {
            Stopped, Paused, Playing
        };

        sf::Sound sound;

        Sound() = default;
        Sound(const Sound& sound) = default;
        Sound(const sf::Sound& sound);
        Sound(const SoundBuffer& buffer);

        void Play();
        void Pause();
        void Stop();

        void SetStatus(Status status);
        Status GetStatus() const;

        void SetLoop(bool loop);
        bool GetLoop() const;

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