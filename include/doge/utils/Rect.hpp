#pragma once

#include <iostream>
#include <cmath>

namespace doge
{
    template <typename T>
    struct Rect
    {
        T left, top, width, height;

        Rect(T left = 0.0, T top = 0.0, T width = 0.0, T height = 0.0) : left(left), top(top), width(width), height(height) {}

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

        Rect& Set(T left, T top, T width, T height)
        {
            
            this->left = left;
            this->top = top;
            this->width = width;
            this->height = height;

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

        bool operator==(const Rect&) const = default;
    };

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