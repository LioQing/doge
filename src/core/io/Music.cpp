#include <doge/core/io/Music.hpp>

#include <doge/utils.hpp>

namespace doge::io
{
    bool Music::FromFile(const std::string& filename)
    {
        return music.openFromFile(filename);
    }

    bool Music::FromMemory(void* data, std::size_t size)
    {
        return music.openFromMemory(data, size);
    }

    std::uint32_t Music::GetSampleRate() const
    {
        return music.getSampleRate();
    }

    std::uint32_t Music::GetChannelCount() const
    {
        return music.getChannelCount();
    }

    Time Music::GetDuration() const
    {
        return Time(music.getDuration().asMicroseconds());
    }

    void Music::Play()
    {
        music.play();
    }

    void Music::Pause()
    {
        music.pause();
    }

    void Music::Stop()
    {
        music.stop();
    }

    void Music::SetStatus(Status status)
    {
        switch (status)
        {
            case Status::Stopped: Stop(); return;
            case Status::Paused: Pause(); return;
            case Status::Playing: Play(); return;
            default: throw std::invalid_argument("Invalid Status");
        }
    }

    Music::Status Music::GetStatus() const
    {
        switch (music.getStatus())
        {
            case sf::Music::Stopped: return Status::Stopped;
            case sf::Music::Paused: return Status::Paused;
            case sf::Music::Playing: return Status::Playing;
            default: return Status::Stopped;
        }
    }

    void Music::SetLoop(bool loop)
    {
        music.setLoop(loop);
    }

    bool Music::GetLoop() const
    {
        return music.getLoop();
    }

    void Music::SetLoopPoints(Time start, Time length)
    {
        music.setLoopPoints(sf::Music::TimeSpan(sf::microseconds(start.microsecond), sf::microseconds(length.microsecond)));
    }

    std::pair<Time, Time> Music::GetLoopPoints() const
    {
        auto timespan = music.getLoopPoints();
        return std::make_pair(Time(timespan.offset.asMicroseconds()), Time(timespan.length.asMicroseconds()));
    }

    void Music::SetPlayingOffset(Time timestamp)
    {
        music.setPlayingOffset(sf::microseconds(timestamp.microsecond));
    }

    Time Music::GetPlayingOffset() const
    {
        return Time(music.getPlayingOffset().asMicroseconds());
    }

    void Music::SetPitch(float pitch)
    {
        music.setPitch(pitch);
    }

    float Music::GetPitch() const
    {
        return music.getPitch();
    }

    void Music::SetVolume(float volume)
    {
        music.setVolume(volume);
    }

    float Music::GetVolume() const
    {
        return music.getVolume();
    }

    void Music::SetRelativeToListener(bool relative)
    {
        music.setRelativeToListener(relative);
    }

    bool Music::IsRelativeToListener() const
    {
        return music.isRelativeToListener();
    }

    void Music::SetPosition(const Vec3f& position)
    {
        music.setPosition(cast::ToSfVec3(position));
    }

    Vec3f Music::GetPosition() const
    {
        return cast::FromSfVec3(music.getPosition());
    }

    void Music::SetMinDistance(float distance)
    {
        music.setMinDistance(distance);
    }

    float Music::GetMinDistance() const
    {
        return music.getMinDistance();
    }

    void Music::SetAttenuation(float attenuation)
    {
        music.setAttenuation(attenuation);
    }

    float Music::GetAttenuation() const
    {
        return music.getAttenuation();
    }
}
