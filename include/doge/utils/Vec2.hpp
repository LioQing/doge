#pragma once

#include <iostream>
#include <cmath>

namespace doge
{
    template <typename T>
    struct Vec2
    {
        T x, y;

        Vec2(T x = 0.0, T y = 0.0) : x(x), y(y) {}

        template <typename U>
        operator Vec2<U>() const
        {
            return Vec2<U>(x, y);
        }
        template <typename U>
        Vec2<U> Cast() const
        {
            return Vec2<U>(x, y);
        }

        Vec2& Set(T x, T y)
        {
            this->x = x;
            this->y = y;
            return *this;
        }

        template <typename U>
        static Vec2 Rotated(Vec2 v, U theta)
        {
            return v.Rotated(theta);
        }
        template <typename U>
        Vec2<T> Rotated(U theta) const
        {
            U c = std::cos(theta);
            U s = std::sin(theta);
            T tx = c * x - s * y;
            T ty = s * x + c * y;

            return Vec2<T>(tx, ty);
        }
        template <typename U>
        Vec2& Rotate(U theta)
        {
            *this = Rotated(theta);

            return *this;
        }

        static Vec2 Normalized(const Vec2& v) 
        {
            return v.Normalized();
        }
        Vec2 Normalized() const
        {
            if (Magnitude() == 0) return *this;
            return Vec2(*this / Magnitude());
        }
        Vec2& Normalize()
        {
            if (Magnitude() == 0) return *this;
            *this *= (1.0 / Magnitude());
            return *this;
        }

        static Vec2 Ortho(const Vec2& v)
        {
            return v.Ortho();
        }
        Vec2 Ortho() const
        {
            return Vec2(y, -x);
        }

        template <typename U = T>
        static auto Distance(const Vec2& v1, const Vec2<U>& v2)
        {
            return v1.Distance(v2);
        }
        template <typename U = T>
        auto Distance(const Vec2<U>& v) const
        {
            Vec2<decltype(std::declval<U&>() - std::declval<T&>())> d(v.x - x, v.y - y);
            return d.Magnitude();
        }

        static auto SqrMagnitude(const Vec2& v)
        {
            return v.SqrMagnitude();
        }
        auto SqrMagnitude() const
        {
            return x * x + y * y;
        }

        static auto Magnitude(const Vec2& v)
        {
            return v.Magnitude();
        }
        auto Magnitude() const
        {
            return std::hypot(x, y);
        }

        static Vec2 Abs(const Vec2& v)
        {
            return v.Abs();
        }
        Vec2 Abs() const
        {
            return Vec2(x < 0 ? x * -1 : x, y < 0 ? y * -1 : y);
        }

        template <typename U = T>
        static auto Dot(const Vec2& v1, const Vec2<U>& v2)
        {
            return v1.Dot(v2);
        } 
        template <typename U = T>
        auto Dot(const Vec2<U>& v) const
        {
            return x * v.x + y * v.y;
        }
        template <typename U = T>
        static auto Cross(const Vec2& v1, const Vec2<U>& v2)
        {
            return v1.Cross(v2);
        } 
        template <typename U = T>
        auto Cross(const Vec2<U>& v) const
        {
            return (x * v.y) - (y * v.x);
        }

        template <typename U = T>
        static auto HadamardMultiplication(const Vec2& v1, const Vec2<U>& v2)
        {
            return v1.HadamardMultiplication(v2);
        }
        template <typename U = T>
        auto HadamardMultiplication(const Vec2<U>& v) const
        {
            return Vec2<decltype(std::declval<T&>() * std::declval<U&>())>(x * v.x, y * v.y);
        }

        template <typename U = T>
        static auto HadamardDivision(const Vec2& v1, const Vec2<U>& v2)
        {
            return v1.HadamardDivision(v2);
        }
        template <typename U = T>
        auto HadamardDivision(const Vec2<U>& v) const
        {
            return Vec2<decltype(std::declval<T&>() / std::declval<U&>())>(x / v.x, y / v.y);
        }

        template <typename U = T>
        static auto HadamardModulo(const Vec2& v1, const Vec2<U>& v2)
        {
            return v1.HadamardModulo(v2);
        }
        template <typename U = T>
        auto HadamardModulo(const Vec2<U>& v) const
        {
            return Vec2<decltype(std::fmod(std::declval<T&>(), std::declval<U&>()))>(std::fmod(x, v.x), std::fmod(y, v.y));
        }

        // 0 --> collinear
        // >0 -> v3 to rotate clockwise to become collinear
        // <0 -> v3 to rotate anticlockwise to become collinear
        template <typename U = T, typename S = T>
        static auto OrientationEx(const Vec2& v1, const Vec2<U>& v2, const Vec2<S>& v3)
        {
            return v1.OrientationEx(v2, v3);
        }
        template <typename U = T, typename S = T>
        auto OrientationEx(const Vec2<U>& v2, const Vec2<S>& v3) const
        {
            return v3.Cross(v2 - *this);
        }

        // 0 --> collinear
        // 1 --> v3 to rotate clockwise to become collinear
        // -1 -> v3 to rotate anticlockwise to become collinear 
        template <typename U = T, typename S = T>
        static int Orientation(const Vec2& v1, const Vec2<U>& v2, const Vec2<S>& v3)
        {
            return v1.Orientation(v2, v3);
        }
        template <typename U = T, typename S = T>
        int Orientation(const Vec2<U>& v2, const Vec2<S>& v3) const
        {
            auto val = OrientationEx(v2, v3);

            if (val == 0) return 0;
            return (val > 0) ? 1 : -1;
        }

        static constexpr Vec2 One()  { return Vec2(1.0, 1.0); }
        static constexpr Vec2 Zero() { return Vec2(0.0, 0.0); }
        static constexpr Vec2 Up()   { return Vec2(0.0, 1.0); }
        static constexpr Vec2 Down() { return Vec2(0.0, -1.0); }
        static constexpr Vec2 Left() { return Vec2(-1.0, 0.0); }
        static constexpr Vec2 Right(){ return Vec2(1.0, 0.0); }
        static constexpr Vec2 i()    { return Vec2(1.0, 0.0); }
        static constexpr Vec2 j()    { return Vec2(0.0, 1.0); }
    };

    template <typename T, typename U>
    auto operator+(const Vec2<T>& v1, const Vec2<U>& v2)
    {
        return Vec2<decltype(std::declval<T&>() + std::declval<U&>())>(v1.x + v2.x, v1.y + v2.y);
    }
    template <typename T, typename U>
    auto operator-(const Vec2<T>& v1, const Vec2<U>& v2)
    {
        return Vec2<decltype(std::declval<T&>() - std::declval<U&>())>(v1.x - v2.x, v1.y - v2.y);
    }

    template <typename T, typename U>
    Vec2<T>& operator+=(Vec2<T>& v1, const Vec2<U>& v2)
    {
        return v1 = v1 + v2;
    }
    template <typename T, typename U>
    Vec2<T>& operator-=(Vec2<T>& v1, const Vec2<U>& v2)
    {
        return v1 = v1 - v2;
    }

    template <typename T, typename U>
    auto operator*(U s, const Vec2<T>& v)
    {
        return Vec2<decltype(s * std::declval<T&>())>(v.x * s, v.y * s);
    }
    template <typename T, typename U>
    auto operator/(U s, const Vec2<T>& v)
    {
        return Vec2<decltype(s / std::declval<T&>())>(s / v.x, s / v.y);
    }
    template <typename T, typename U>
    auto operator%(U s, const Vec2<T>& v)
    {
        return Vec2<decltype(std::fmod(std::declval<T&>(), s))>(std::fmod(s, v.x), std::fmod(s, v.y));
    }

    template <typename T, typename U>
    auto operator*(const Vec2<T>& v, U s)
    {
        return Vec2<decltype(s * std::declval<T&>())>(v.x * s, v.y * s);
    }
    template <typename T, typename U>
    auto operator/(const Vec2<T>& v, U s)
    {
        return Vec2<decltype(s / std::declval<T&>())>(v.x / s, v.y / s);
    }
    template <typename T, typename U>
    auto operator%(const Vec2<T>& v, U s)
    {
        return Vec2<decltype(std::fmod(s, std::declval<T&>()))>(std::fmod(v.x, s), std::fmod(v.y, s));
    }

    template <typename T, typename U>
    Vec2<T>& operator*=(Vec2<T>& v, U s)
    {
        return v = v * s;
    }
    template <typename T, typename U>
    Vec2<T>& operator/=(Vec2<T>& v, U s)
    {
        return v = v / s;
    }
    template <typename T, typename U>
    Vec2<T>& operator%=(Vec2<T>& v, U s)
    {
        return v = v % s;
    }

    template <typename T>
    Vec2<T> operator-(const Vec2<T>& v)
    {
        return Vec2<T>(-v.x, -v.y);
    }
    template <typename T>
    Vec2<T> operator+(const Vec2<T>& v)
    {
        return Vec2<T>(v.x, v.y);
    }

    template <typename T, typename U>
    bool operator==(const Vec2<T>& v1, const Vec2<U>& v2)
    {
        return v1.x == v2.x && v1.y == v2.y;
    }
    template <typename T, typename U>
    bool operator!=(const Vec2<T>& v1, const Vec2<U>& v2)
    {
        return !(v1.x == v2.x && v1.y == v2.y);
    }

    template <typename T>
    std::ostream& operator<<(std::ostream& os, const Vec2<T>& v)
    {
        os << "(" << v.x << ", " << v.y << ")";
        return os;
    }

    using Vec2d = Vec2<double>;
    using Vec2f = Vec2<float>;
    using Vec2i = Vec2<int>;
    using Vec2u = Vec2<unsigned int>;
}