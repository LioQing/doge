#pragma once

#include <iostream>
#include <cmath>
#include <doge/utils/Vec2.hpp>

namespace doge
{
    template <typename T>
    struct Rect
    {
        T left, top, width, height;

        Rect(T left = 0.0, T top = 0.0, T width = 0.0, T height = 0.0) : left(left), top(top), width(width), height(height) {}
        Rect(const Vec2<T>& position, const Vec2<T>& size) : left(position.x), top(position.y), width(size.x), height(size.y) {}

        template <typename U>
        operator Rect<U>() const
        {
            return Rect<U>(left, top, width, height);
        }
        template <typename U>
        Rect<U> Cast() const
        {
            return Rect<U>(left, top, width, height);
        }

        Vec2<T> GetPosition() const
        {
            return Vec2<T>(left, top);
        }

        Rect& SetPosition(T left, T top) &
        {
            this->left = left;
            this->top = top;

            return *this;
        }

        Rect& SetPosition(const Vec2<T>& position) &
        {
            return SetPosition(position.x, position.y);
        }

        Vec2<T> GetSize() const
        {
            return Vec2<T>(width, height);
        }

        Rect& SetSize(T width, T height) &
        {
            this->width = width;
            this->height = height;

            return *this;
        }

        Rect& SetSize(const Vec2<T>& size) &
        {
            return SetSize(size.x, size.y);
        }

        std::pair<Vec2<T>, Vec2<T>> GetVec2() const
        {
            return std::make_pair(GetPosition(), GetSize());
        }

        Rect& Set(T left, T top, T width, T height) &
        {
            SetPosition(left, top);
            SetSize(width, height);

            return *this;
        }

        Rect& Set(const Vec2<T>& position, const Vec2<T>& size) &
        {
            SetPosition(position);
            SetSize(size);

            return *this;
        }
        
        template <typename U = T>
        static bool Overlap(const Rect& r1, const Rect<U>& r2)
        {
            return r1.Overlap(r2);
        }
        template <typename U>
        bool Overlap(const Rect<U>& r) const
        {
            return 
                this->left  < r.left + r.width &&
                r.left      < this->left + this->width &&
                this->top   < r.top + r.height &&
                r.top       < this->top + this->height;
        }

        static Rect FromCoordinates(T left, T top, T right, T bottom)
        {
            return Rect(left, top, right - left, bottom - top);
        }
    };

    template <typename T, typename U>
    Rect<T> operator+(const Rect<T>& r1, const Rect<U>& r2)
    {
        return Rect<T>(r1.GetPosition() + r2.GetPosition(), r1.GetSize() + r2.GetSize());
    }
    template <typename T, typename U>
    Rect<T> operator-(const Rect<T>& r1, const Rect<U>& r2)
    {
        return Rect<T>(r1.GetPosition() - r2.GetPosition(), r1.GetSize() - r2.GetSize());
    }
    template <typename T, typename U>
    Rect<T> operator*(const Rect<T>& r1, const Rect<U>& r2)
    {
        return Rect<T>(r1.GetPosition() * r2.GetPosition(), r1.GetSize() * r2.GetSize());
    }
    template <typename T, typename U>
    Rect<T> operator/(const Rect<T>& r1, const Rect<U>& r2)
    {
        return Rect<T>(r1.GetPosition() / r2.GetPosition(), r1.GetSize() / r2.GetSize());
    }

    template <typename T, typename U>
    Rect<T>& operator+=(Rect<T>& r1, const Rect<U>& r2)
    {
        r1 = r1 + r2;
        return r1;
    }
    template <typename T, typename U>
    Rect<T>& operator-=(Rect<T>& r1, const Rect<U>& r2)
    {
        r1 = r1 - r2;
        return r1;
    }
    template <typename T, typename U>
    Rect<T>& operator*=(Rect<T>& r1, const Rect<U>& r2)
    {
        r1 = r1 * r2;
        return r1;
    }
    template <typename T, typename U>
    Rect<T>& operator/=(Rect<T>& r1, const Rect<U>& r2)
    {
        r1 = r1 / r2;
        return r1;
    }

    template <typename T, typename U>
    Rect<T> operator+(const Rect<T>& r, const Vec2<U>& v)
    {
        return Rect<T>(r.GetPosition() + v, r.GetSize());
    }
    template <typename T, typename U>
    Rect<T> operator-(const Rect<T>& r, const Vec2<U>& v)
    {
        return Rect<T>(r.GetPosition() - v, r.GetSize());
    }
    template <typename T, typename U>
    Rect<T> operator*(const Rect<T>& r, const Vec2<U>& v)
    {
        return Rect<T>(r.GetPosition(), r.GetSize() * v);
    }
    template <typename T, typename U>
    Rect<T> operator/(const Rect<T>& r, const Vec2<U>& v)
    {
        return Rect<T>(r.GetPosition(), r.GetSize() / v);
    }

    template <typename T, typename U>
    Rect<T>& operator+=(Rect<T>& r, const Vec2<U>& v)
    {
        return r.SetPosition(r.GetPosition() + v);
    }
    template <typename T, typename U>
    Rect<T>& operator-=(Rect<T>& r, const Vec2<U>& v)
    {
        return r.SetPosition(r.GetPosition() - v);
    }
    template <typename T, typename U>
    Rect<T>& operator*=(Rect<T>& r, const Vec2<U>& v)
    {
        return r.SetSize(r.GetSize() * v);
    }
    template <typename T, typename U>
    Rect<T>& operator/=(Rect<T>& r, const Vec2<U>& v)
    {
        return r.SetSize(r.GetSize() / v);
    }

    template <typename T, typename U>
    bool operator==(const Rect<T>& r1, const Rect<U>& r2)
    {
        return r1.left == r2.left && r1.top == r2.top && r1.width == r2.width && r1.height == r2.height;
    }

    template <typename T>
    std::ostream& operator<<(std::ostream& os, const Rect<T>& r)
    {
        os << "(" << r.left << ", " << r.top << ", " << r.width << ", " << r.height << ")";
        return os;
    }

    using Rectd = Rect<double>;
    using Rectf = Rect<float>;
    using Recti = Rect<int>;
    using Rectu = Rect<unsigned int>;
}