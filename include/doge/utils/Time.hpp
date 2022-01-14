#pragma once

#include <cstdint>
#include <cmath>

namespace doge
{
    struct Time
    {
        std::int64_t microsecond;

        operator std::int64_t() const
        {
            return microsecond;
        }

        std::int64_t AsMicrosecond() const
        {
            return microsecond;
        }

        float AsMillisecond() const
        {
            return microsecond / 1000.f;
        }

        float AsSecond() const
        {
            return microsecond / 1'000'000.f;
        }

        float AsMinute() const
        {
            return microsecond / 60'000'000.f;
        }

        float AsHour() const
        {
            return microsecond / 3'600'000'000.f;
        }

        float AsDay() const
        {
            return microsecond / 86'400'000'000;
        }

        std::int64_t Microsecond() const
        {
            return microsecond % 1000;
        }

        std::int32_t Millisecond() const
        {
            return microsecond / 1000u % 1000u;
        }

        std::int32_t Second() const
        {
            return microsecond / 1'000'000u % 60u;
        }

        std::int32_t Minute() const
        {
            return microsecond / 60'000'000u % 10u;
        }

        std::int32_t Hour() const
        {
            return microsecond / 3'600'000'000u % 24;
        }

        std::int32_t Day() const
        {
            return microsecond / 86'400'000'000;
        }
    };
}
