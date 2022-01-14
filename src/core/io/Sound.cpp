#include <doge/core/io/Sound.hpp>

#include <doge/utils.hpp>

namespace doge::io
{
    Sound::Sound(const sf::Sound& sound) : sound(sound)
    {
    }

    Sound::Sound(const SoundBuffer& buffer) : sound(buffer.sound_buffer)
    {
    }

    void Sound::Play()
    {
        sound.play();
    }

    void Sound::Pause()
    {
        sound.pause();
    }

    void Sound::Stop()
    {
        sound.stop();
    }

    void Sound::SetStatus(Status status)
    {
        switch (status)
        {
            case Status::Stopped: Stop(); return;
            case Status::Paused: Pause(); return;
            case Status::Playing: Play(); return;
            default: throw std::invalid_argument("Invalid Status");
        }
    }

    Sound::Status Sound::GetStatus() const
    {
        switch (sound.getStatus())
        {
            case sf::Sound::Stopped: return Status::Stopped;
            case sf::Sound::Paused: return Status::Paused;
            case sf::Sound::Playing: return Status::Playing;
            default: return Status::Stopped;
        }
    }

    void Sound::SetLoop(bool loop)
    {
        sound.setLoop(loop);
    }

    bool Sound::GetLoop() const
    {
        return sound.getLoop();
    }

    void Sound::SetPlayingOffset(Time timestamp)
    {
        sound.setPlayingOffset(sf::microseconds(timestamp.microsecond));
    }

    Time Sound::GetPlayingOffset() const
    {
        return Time(sound.getPlayingOffset().asMicroseconds());
    }

    void Sound::SetPitch(float pitch)
    {
        sound.setPitch(pitch);
    }

    float Sound::GetPitch() const
    {
        return sound.getPitch();
    }

    void Sound::SetVolume(float volume)
    {
        sound.setVolume(volume);
    }

    float Sound::GetVolume() const
    {
        return sound.getVolume();
    }

    void Sound::SetRelativeToListener(bool relative)
    {
        sound.setRelativeToListener(relative);
    }

    bool Sound::IsRelativeToListener() const
    {
        return sound.isRelativeToListener();
    }

    void Sound::SetPosition(const Vec3f& position)
    {
        sound.setPosition(cast::ToSfVec3(position));
    }

    Vec3f Sound::GetPosition() const
    {
        return cast::FromSfVec3(sound.getPosition());
    }

    void Sound::SetMinDistance(float distance)
    {
        sound.setMinDistance(distance);
    }

    float Sound::GetMinDistance() const
    {
        return sound.getMinDistance();
    }

    void Sound::SetAttenuation(float attenuation)
    {
        sound.setAttenuation(attenuation);
    }

    float Sound::GetAttenuation() const
    {
        return sound.getAttenuation();
    }
}