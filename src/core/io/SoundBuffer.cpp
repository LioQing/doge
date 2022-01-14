#include <doge/core/io/SoundBuffer.hpp>

namespace doge::io
{
    bool SoundBuffer::FromFile(const std::string& filename)
    {
        return sound_buffer.loadFromFile(filename);
    }

    bool SoundBuffer::FromMemory(void* data, std::size_t size)
    {
        return sound_buffer.loadFromMemory(data, size);
    }

    bool SoundBuffer::FromSamples(const std::int16_t* samples, std::uint64_t sample_count, std::uint32_t channel_count, std::uint32_t sample_rate)
    {
        return sound_buffer.loadFromSamples(samples, sample_count, channel_count, sample_rate);
    }

    bool SoundBuffer::ToFile(const std::string& filename) const
    {
        return sound_buffer.saveToFile(filename);
    }

    std::uint64_t SoundBuffer::GetSampleCount() const
    {
        return sound_buffer.getSampleCount();
    }

    std::uint32_t SoundBuffer::GetSampleRate() const
    {
        return sound_buffer.getSampleRate();
    }

    std::uint32_t SoundBuffer::GetChannelCount() const
    {
        return sound_buffer.getChannelCount();
    }

    const std::int16_t* SoundBuffer::GetSamplesPtr() const
    {
        return sound_buffer.getSamples();
    }

    Time SoundBuffer::GetDuration() const
    {
        return Time(sound_buffer.getDuration().asMicroseconds());
    }
}