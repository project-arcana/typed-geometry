#pragma once

#include <typed-geometry/detail/utility.hh>
#include <typed-geometry/functions/basic/scalar_math.hh>
#include <typed-geometry/types/scalars/fwd_diff.hh>

namespace tg
{
// ================================= Operators =================================

// auto-diff
template <class T>
[[nodiscard]] constexpr fwd_diff<T> operator+(fwd_diff<T> const& a, fwd_diff<T> const& b)
{
    return {a.value + b.value, a.derivative + b.derivative};
}
template <class T>
[[nodiscard]] constexpr fwd_diff<T> operator-(fwd_diff<T> const& a, fwd_diff<T> const& b)
{
    return {a.value - b.value, a.derivative - b.derivative};
}
template <class T>
[[nodiscard]] constexpr fwd_diff<T> operator*(fwd_diff<T> const& a, fwd_diff<T> const& b)
{
    return {a.value * b.value, a.derivative * b.value + a.value * b.derivative};
}
template <class T>
[[nodiscard]] constexpr fwd_diff<T> operator/(fwd_diff<T> const& a, fwd_diff<T> const& b)
{
    return {a.value / b.value, (a.derivative * b.value - a.value * b.derivative) / (b.value * b.value)};
}
template <class T>
[[nodiscard]] constexpr fwd_diff<T> operator+(fwd_diff<T> const& a)
{
    return {a.value, a.derivative};
}
template <class T>
[[nodiscard]] constexpr fwd_diff<T> operator-(fwd_diff<T> const& a)
{
    return {-a.value, -a.derivative};
}

// scalars
template <class T>
[[nodiscard]] constexpr fwd_diff<T> operator+(fwd_diff<T> const& a, dont_deduce<T> const& b)
{
    return {a.value + b, a.derivative};
}
template <class T>
[[nodiscard]] constexpr fwd_diff<T> operator+(dont_deduce<T> const& a, fwd_diff<T> const& b)
{
    return {a + b.value, b.derivative};
}
template <class T>
[[nodiscard]] constexpr fwd_diff<T> operator-(fwd_diff<T> const& a, dont_deduce<T> const& b)
{
    return {a.value - b, a.derivative};
}
template <class T>
[[nodiscard]] constexpr fwd_diff<T> operator-(dont_deduce<T> const& a, fwd_diff<T> const& b)
{
    return {a - b.value, -b.derivative};
}
template <class T>
[[nodiscard]] constexpr fwd_diff<T> operator*(fwd_diff<T> const& a, dont_deduce<T> const& b)
{
    return {a.value * b, a.derivative * b};
}
template <class T>
[[nodiscard]] constexpr fwd_diff<T> operator*(dont_deduce<T> const& a, fwd_diff<T> const& b)
{
    return {a * b.value, a * b.derivative};
}
template <class T>
[[nodiscard]] constexpr fwd_diff<T> operator/(fwd_diff<T> const& a, dont_deduce<T> const& b)
{
    return {a.value / b, a.derivative / b};
}
template <class T>
[[nodiscard]] constexpr fwd_diff<T> operator/(dont_deduce<T> const& a, fwd_diff<T> const& b)
{
    return {a / b.value, -a * b.derivative / (b.value * b.value)};
}

// comparisons
template <class T>
[[nodiscard]] constexpr bool operator<(fwd_diff<T> const& a, fwd_diff<T> const& b)
{
    return a.value < b.value;
}
template <class T>
[[nodiscard]] constexpr bool operator<(fwd_diff<T> const& a, dont_deduce<T> const& b)
{
    return a.value < b;
}
template <class T>
[[nodiscard]] constexpr bool operator<(dont_deduce<T> const& a, fwd_diff<T> const& b)
{
    return a < b.value;
}

template <class T>
[[nodiscard]] constexpr bool operator<=(fwd_diff<T> const& a, fwd_diff<T> const& b)
{
    return a.value <= b.value;
}
template <class T>
[[nodiscard]] constexpr bool operator<=(fwd_diff<T> const& a, dont_deduce<T> const& b)
{
    return a.value <= b;
}
template <class T>
[[nodiscard]] constexpr bool operator<=(dont_deduce<T> const& a, fwd_diff<T> const& b)
{
    return a <= b.value;
}

template <class T>
[[nodiscard]] constexpr bool operator>(fwd_diff<T> const& a, fwd_diff<T> const& b)
{
    return a.value > b.value;
}
template <class T>
[[nodiscard]] constexpr bool operator>(fwd_diff<T> const& a, dont_deduce<T> const& b)
{
    return a.value > b;
}
template <class T>
[[nodiscard]] constexpr bool operator>(dont_deduce<T> const& a, fwd_diff<T> const& b)
{
    return a > b.value;
}

template <class T>
[[nodiscard]] constexpr bool operator>=(fwd_diff<T> const& a, fwd_diff<T> const& b)
{
    return a.value >= b.value;
}
template <class T>
[[nodiscard]] constexpr bool operator>=(fwd_diff<T> const& a, dont_deduce<T> const& b)
{
    return a.value >= b;
}
template <class T>
[[nodiscard]] constexpr bool operator>=(dont_deduce<T> const& a, fwd_diff<T> const& b)
{
    return a >= b.value;
}

// ================================= Math =================================

template <class T>
[[nodiscard]] constexpr fwd_diff<T> abs(fwd_diff<T> const& v)
{
    return {abs(v.value), v.derivative * sign(v.value)};
}

template <class T>
[[nodiscard]] constexpr fwd_diff<T> sqrt(fwd_diff<T> const& v)
{
    auto const s = sqrt(v.value);
    return {s, v.derivative / (s + s)};
}

template <class T>
[[nodiscard]] constexpr fwd_diff<T> exp(fwd_diff<T> const& v)
{
    auto const e = exp(v.value);
    return {e, v.derivative * e};
}

template <class T>
[[nodiscard]] constexpr fwd_diff<T> log(fwd_diff<T> const& v)
{
    return {log(v.value), v.derivative / v.value};
}

}
