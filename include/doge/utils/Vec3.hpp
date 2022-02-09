#pragma once

#include <iostream>
#include <cmath>

namespace doge
{
    template <typename T>
    struct Vec3
    {
        T x = 0.0;
        T y = 0.0;
        T z = 0.0;

        Vec3() = default;
        Vec3(const Vec3&) = default;
        Vec3(T x, T y, T z) : x(x), y(y), z(z) {}

        template <typename U>
        operator Vec3<U>() const
        {
            return Vec3<U>(x, y, z);
        }
        template <typename U>
        Vec3<U> Cast() const
        {
            return Vec3<U>(x, y, z);
        }

        Vec3& Set(T x, T y, T z) &
        {
            this->x = x;
            this->y = y;
            this->z = z;
            return *this;
        }

        static Vec3 Normalized(const Vec3& v) 
        {
            return v.Normalized();
        }
        Vec3 Normalized() const
        {
            if (Magnitude() == 0) return *this;
            return Vec3(*this / Magnitude());
        }
        Vec3& Normalize() &
        {
            if (Magnitude() == 0) return *this;
            *this *= (1.0 / Magnitude());
            return *this;
        }

        template <typename U = T>
        static auto Distance(const Vec3& v1, const Vec3<U>& v2)
        {
            return v1.Distance(v2);
        }
        template <typename U = T>
        auto Distance(const Vec3<U>& v) const
        {
            Vec3<decltype(std::declval<U&>() - std::declval<T&>())> d(v.x - x, v.y - y, v.z - z);
            return d.Magnitude();
        }

        static auto SqrMagnitude(const Vec3& v)
        {
            return v.SqrMagnitude();
        }
        auto SqrMagnitude() const
        {
            return x * x + y * y + z * z;
        }
        
        static auto Magnitude(const Vec3& v)
        {
            return v.Magnitude();
        }
        auto Magnitude() const
        {
            return std::hypot(x, y, z);
        }

        static Vec3 Abs(const Vec3& v)
        {
            return v.Abs();
        }
        Vec3 Abs() const
        {
            return Vec3(x < 0 ? x * -1 : x, y < 0 ? y * -1 : y, z < 0 ? z * -1 : z);
        }

        template <typename U = T>
        static auto Dot(const Vec3& v1, const Vec3<U>& v2)
        {
            return v1.Dot(v2);
        } 
        template <typename U = T>
        auto Dot(const Vec3<U>& v) const
        {
            return x * v.x + y * v.y + z * v.z;
        }
        template <typename U = T>
        static auto Cross(const Vec3& v1, const Vec3<U>& v2)
        {
            return v1.Cross(v2);
        } 
        template <typename U = T>
        auto Cross(const Vec3<U>& v) const
        {
            return Vec3<decltype(std::declval<T&>() * std::declval<U&>())>
            (
                (y * v.z) - (z * v.y),
                (z * v.x) - (x * v.z),
                (x * v.y) - (y * v.x)
            );
        }

        template <typename U = T>
        static auto HadamardMultiplication(const Vec3& v1, const Vec3<U>& v2)
        {
            return v1.HadamardMultiplication(v2);
        }
        template <typename U = T>
        auto HadamardMultiplication(const Vec3<U>& v) const
        {
            return Vec3<decltype(std::declval<T&>() * std::declval<U&>())>(x * v.x, y * v.y, z * v.z);
        }

        template <typename U = T>
        static auto HadamardDivision(const Vec3& v1, const Vec3<U>& v2)
        {
            return v1.HadamardDivision(v2);
        }
        template <typename U = T>
        auto HadamardDivision(const Vec3<U>& v) const
        {
            return Vec3<decltype(std::declval<T&>() / std::declval<U&>())>(x / v.x, y / v.y, z / v.z);
        }

        template <typename U = T>
        static auto HadamardModulo(const Vec3& v1, const Vec3<U>& v2)
        {
            return v1.HadamardModulo(v2);
        }
        template <typename U = T>
        auto HadamardModulo(const Vec3<U>& v) const
        {
            return Vec3<decltype(std::fmod(std::declval<T&>(), std::declval<U&>()))>(std::fmod(x, v.x), std::fmod(y, v.y), std::fmod(z, v.z));
        }

        template <typename U = T, typename S = T>
        static auto CollinearEx(const Vec3& v1, const Vec3<U>& v2, const Vec3<S>& v3)
        {
            return v1.CollinearEx(v2, v3);
        }
        template <typename U = T, typename S = T>
        auto CollinearEx(const Vec3<U>& v2, const Vec3<S>& v3) const
        {
            return v3.Cross(v2 - *this);
        }
        template <typename U = T, typename S = T>
        static bool Collinear(const Vec3& v1, const Vec3<U>& v2, const Vec3<S>& v3)
        {
            return v1.Collinear(v2, v3);
        }
        template <typename U = T, typename S = T>
        bool Collinear(const Vec3<U>& v2, const Vec3<S>& v3) const
        {
            return v3.Cross(v2 - *this).SqrMagnitude() == 0;
        }

        static const Vec3 One;
        static const Vec3 Zero;
        static const Vec3 Forward;
        static const Vec3 Back;
        static const Vec3 Up;
        static const Vec3 Down;
        static const Vec3 Right;
        static const Vec3 Left;
        static const Vec3 i;
        static const Vec3 j;
        static const Vec3 k;
    };

    template <typename T> const Vec3<T> Vec3<T>::One        = Vec3<T>(1, 1, 1);
    template <typename T> const Vec3<T> Vec3<T>::Zero       = Vec3<T>(0, 0, 0);
    template <typename T> const Vec3<T> Vec3<T>::Forward    = Vec3<T>(0, 0, 1);
    template <typename T> const Vec3<T> Vec3<T>::Back       = Vec3<T>(0, 0, -1);
    template <typename T> const Vec3<T> Vec3<T>::Up         = Vec3<T>(0, 1, 0);
    template <typename T> const Vec3<T> Vec3<T>::Down       = Vec3<T>(0, -1, 0);
    template <typename T> const Vec3<T> Vec3<T>::Right      = Vec3<T>(1, 0, 0);
    template <typename T> const Vec3<T> Vec3<T>::Left       = Vec3<T>(-1, 0, 0);
    template <typename T> const Vec3<T> Vec3<T>::i          = Vec3<T>(1, 0, 0);
    template <typename T> const Vec3<T> Vec3<T>::j          = Vec3<T>(0, 1, 0);
    template <typename T> const Vec3<T> Vec3<T>::k          = Vec3<T>(0, 0, 1);

    template <typename T, typename U>
    auto operator+(const Vec3<T>& v1, const Vec3<U>& v2)
    {
        return Vec3<decltype(std::declval<T&>() + std::declval<U&>())>(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
    }
    template <typename T, typename U>
    auto operator-(const Vec3<T>& v1, const Vec3<U>& v2)
    {
        return Vec3<decltype(std::declval<T&>() - std::declval<U&>())>(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
    }
    template <typename T, typename U>
    auto operator*(const Vec3<T>& v1, const Vec3<U>& v2)
    {
        return Vec2<decltype(std::declval<T&>() * std::declval<U&>())>(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
    }
    template <typename T, typename U>
    auto operator/(const Vec3<T>& v1, const Vec3<U>& v2)
    {
        return Vec2<decltype(std::declval<T&>() / std::declval<U&>())>(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z);
    }
    template <typename T, typename U>
    auto operator%(const Vec3<T>& v1, const Vec3<U>& v2)
    {
        return Vec2<decltype(std::fmod(std::declval<T&>(), std::declval<U&>()))>(std::fmod(v1.x, v2.x), std::fmod(v1.y, v2.y), std::fmod(v1.z, v2.z));
    }

    template <typename T, typename U>
    Vec3<T>& operator+=(Vec3<T>& v1, const Vec3<U>& v2)
    {
        return v1 = v1 + v2;
    }
    template <typename T, typename U>
    Vec3<T>& operator-=(Vec3<T>& v1, const Vec3<U>& v2)
    {
        return v1 = v1 - v2;
    }
    template <typename T, typename U>
    Vec3<T>& operator*=(Vec3<T>& v1, const Vec3<U>& v2)
    {
        return v1 = v1 * v2;
    }
    template <typename T, typename U>
    Vec3<T>& operator/=(Vec3<T>& v1, const Vec3<U>& v2)
    {
        return v1 = v1 / v2;
    }
    template <typename T, typename U>
    Vec3<T>& operator%=(Vec3<T>& v1, const Vec3<U>& v2)
    {
        return v1 = v1 % v2;
    }

    template <typename T, typename U>
    auto operator*(U s, const Vec3<T>& v)
    {
        return Vec3<decltype(s * std::declval<T&>())>(v.x * s, v.y * s, v.z * s);
    }
    template <typename T, typename U>
    auto operator/(U s, const Vec3<T>& v)
    {
        return Vec3<decltype(s / std::declval<T&>())>(s / v.x, s / v.y, s / v.z);
    }
    template <typename T, typename U>
    auto operator%(U s, const Vec3<T>& v)
    {
        return Vec3<decltype(std::fmod(std::declval<T&>(), s))>(std::fmod(s, v.x), std::fmod(s, v.y), std::fmod(s, v.z));
    }

    template <typename T, typename U>
    auto operator*(const Vec3<T>& v, U s)
    {
        return Vec3<decltype(s * std::declval<T&>())>(v.x * s, v.y * s, v.z * s);
    }
    template <typename T, typename U>
    auto operator/(const Vec3<T>& v, U s)
    {
        return Vec3<decltype(s / std::declval<T&>())>(v.x / s, v.y / s, v.z / s);
    }
    template <typename T, typename U>
    auto operator%(const Vec3<T>& v, U s)
    {
        return Vec3<decltype(std::fmod(s, std::declval<T&>()))>(std::fmod(v.x, s), std::fmod(v.y, s), std::fmod(v.z, s));
    }

    template <typename T, typename U>
    Vec3<T>& operator*=(Vec3<T>& v, U s)
    {
        return v = v * s;
    }
    template <typename T, typename U>
    Vec3<T>& operator/=(Vec3<T>& v, U s)
    {
        return v = v / s;
    }
    template <typename T, typename U>
    Vec3<T>& operator%=(Vec3<T>& v, U s)
    {
        return v = v % s;
    }

    template <typename T>
    Vec3<T> operator-(const Vec3<T>& v)
    {
        return Vec3<T>(-v.x, -v.y, -v.z);
    }
    template <typename T>
    Vec3<T> operator+(const Vec3<T>& v)
    {
        return Vec3<T>(v.x, v.y, v.z);
    }

    template <typename T, typename U>
    bool operator==(const Vec3<T>& v1, const Vec3<U>& v2)
    {
        return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z;
    }
    template <typename T, typename U>
    bool operator!=(const Vec3<T>& v1, const Vec3<U>& v2)
    {
        return !(v1.x == v2.x && v1.y == v2.y && v1.z == v2.z);
    }

    template <typename T>
    std::ostream& operator<<(std::ostream& os, const Vec3<T>& v)
    {
        os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
        return os;
    }

    using Vec3d = Vec3<double>;
    using Vec3f = Vec3<float>;
    using Vec3i = Vec3<int>;
    using Vec3u = Vec3<unsigned int>;
}