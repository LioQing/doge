#pragma once

#include <SFML/Audio.hpp>
#include <doge/utils/Time.hpp>

namespace doge::io
{
    struct SoundBuffer
    {
        sf::SoundBuffer sound_buffer;

        bool FromFile(const std::string& filename);
        bool FromMemory(void* data, std::size_t size);
        bool FromSamples(const std::int16_t* samples, std::uint64_t sample_count, std::uint32_t channel_count, std::uint32_t sample_rate);

        bool ToFile(const std::string& filename) const;

        std::uint64_t GetSampleCount() const;
        std::uint32_t GetSampleRate() const;
        std::uint32_t GetChannelCount() const;
        const std::int16_t* GetSamplesPtr() const;

        Time GetDuration() const;
    };
}