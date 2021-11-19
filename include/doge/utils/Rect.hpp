#pragma once

#include <iostream>
#include <cmath>

namespace doge
{
    template <typename T>
    struct Rect
    {
        T left, top, right, bottom;

        Rect(T left = 0.0, T top = 0.0, T right = 0.0, T bottom = 0.0) : left(left), top(top), right(right), bottom(bottom) {}

        template <typename U>
        operator Rect<U>() const
        {
            return Rect<U>(left, top, right, bottom);
        }
        template <typename U>
        Rect<U> Cast() const
        {
            return Rect<U>(left, top, right, bottom);
        }

        Rect& Set(T left, T top, T right, T bottom)
        {
            
            this->left = left;
            this->top = top;
            this->right = right;
            this->bottom = bottom;

            return *this;
        }

        bool operator==(const Rect&) const = default;
    };

    template <typename T>
    std::ostream& operator<<(std::ostream& os, const Rect<T>& r)
    {
        os << "(" << r.left << ", " << r.top << ", " << r.right << ", " << r.bottom << ")";
        return os;
    }

    using Rectd = Rect<double>;
    using Rectf = Rect<float>;
    using Recti = Rect<int>;
    using Rectu = Rect<unsigned int>;
}