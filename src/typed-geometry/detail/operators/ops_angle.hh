#pragma once

#include <typed-geometry/detail/scalar_traits.hh>
#include <typed-geometry/detail/utility.hh>
#include <typed-geometry/types/angle.hh>

namespace tg
{
template <class T>
[[nodiscard]] constexpr angle_t<T> operator-(angle_t<T> const& a)
{
    return radians(-a.radians());
}
template <class T>
[[nodiscard]] constexpr angle_t<T> operator+(angle_t<T> const& a)
{
    return a;
}

template <class T>
[[nodiscard]] constexpr angle_t<T> operator+(angle_t<T> a, angle_t<T> b)
{
    return radians(a.radians() + b.radians());
}
template <class T>
[[nodiscard]] constexpr angle_t<T> operator-(angle_t<T> a, angle_t<T> b)
{
    return radians(a.radians() - b.radians());
}

template <class T>
[[nodiscard]] constexpr angle_t<T> operator*(angle_t<T> a, dont_deduce<T> b)
{
    return radians(a.radians() * b);
}
template <class T>
[[nodiscard]] constexpr angle_t<T> operator/(angle_t<T> a, dont_deduce<T> b)
{
    return radians(a.radians() / b);
}

template <class T>
[[nodiscard]] constexpr angle_t<T> operator*(dont_deduce<T> a, angle_t<T> b)
{
    return radians(a * b.radians());
}

template <class T>
[[nodiscard]] constexpr T operator/(angle_t<T> a, angle_t<T> b)
{
    return a.radians() / b.radians();
}
} // namespace tg
