#pragma once

#include <typed-geometry/common/limits.hh>

#include <typed-geometry/common/scalar_math.hh>

#include <typed-geometry/types/pos.hh>
#include <typed-geometry/types/size.hh>
#include <typed-geometry/types/vec.hh>

#include "math.hh"

// Contained functions:
// - min
// - max
// - min_element
// - max_element

// TODO:
// - minmax
// - minmax_element

namespace tg
{
template <class T>
[[nodiscard]] constexpr T min()
{
    return detail::limits<T>::min();
}
template <class T>
[[nodiscard]] constexpr T max()
{
    return detail::limits<T>::max();
}

template <class T>
[[nodiscard]] constexpr T min(T const& a)
{
    return a;
}
template <class T, class... Ts>
[[nodiscard]] constexpr T min(T const& a, T const& b, T const& c, Ts const&... rest)
{
    return min(min(a, b), c, rest...);
}

template <class T>
[[nodiscard]] constexpr T max(T const& a)
{
    return a;
}
template <class T, class... Ts>
[[nodiscard]] constexpr T max(T const& a, T const& b, T const& c, Ts const&... rest)
{
    return max(max(a, b), c, rest...);
}

template <class ScalarT>
[[nodiscard]] constexpr ScalarT min_element(pos<1, ScalarT> const& p)
{
    return p.x;
}
template <class ScalarT>
[[nodiscard]] constexpr ScalarT min_element(pos<2, ScalarT> const& p)
{
    return min(p.x, p.y);
}
template <class ScalarT>
[[nodiscard]] constexpr ScalarT min_element(pos<3, ScalarT> const& p)
{
    return min(p.x, p.y, p.z);
}
template <class ScalarT>
[[nodiscard]] constexpr ScalarT min_element(pos<4, ScalarT> const& p)
{
    return min(min(p.x, p.y), min(p.z, p.w));
}

template <class ScalarT>
[[nodiscard]] constexpr ScalarT min_element(vec<1, ScalarT> const& p)
{
    return p.x;
}
template <class ScalarT>
[[nodiscard]] constexpr ScalarT min_element(vec<2, ScalarT> const& p)
{
    return min(p.x, p.y);
}
template <class ScalarT>
[[nodiscard]] constexpr ScalarT min_element(vec<3, ScalarT> const& p)
{
    return min(p.x, p.y, p.z);
}
template <class ScalarT>
[[nodiscard]] constexpr ScalarT min_element(vec<4, ScalarT> const& p)
{
    return min(min(p.x, p.y), min(p.z, p.w));
}

template <class ScalarT>
[[nodiscard]] constexpr ScalarT min_element(size<1, ScalarT> const& p)
{
    return p.width;
}
template <class ScalarT>
[[nodiscard]] constexpr ScalarT min_element(size<2, ScalarT> const& p)
{
    return min(p.width, p.height);
}
template <class ScalarT>
[[nodiscard]] constexpr ScalarT min_element(size<3, ScalarT> const& p)
{
    return min(p.width, p.height, p.depth);
}
template <class ScalarT>
[[nodiscard]] constexpr ScalarT min_element(size<4, ScalarT> const& p)
{
    return min(min(p.width, p.height), min(p.depth, p.w));
}

template <class ScalarT>
[[nodiscard]] constexpr ScalarT max_element(pos<1, ScalarT> const& p)
{
    return p.x;
}
template <class ScalarT>
[[nodiscard]] constexpr ScalarT max_element(pos<2, ScalarT> const& p)
{
    return max(p.x, p.y);
}
template <class ScalarT>
[[nodiscard]] constexpr ScalarT max_element(pos<3, ScalarT> const& p)
{
    return max(p.x, p.y, p.z);
}
template <class ScalarT>
[[nodiscard]] constexpr ScalarT max_element(pos<4, ScalarT> const& p)
{
    return max(max(p.x, p.y), max(p.z, p.w));
}

template <class ScalarT>
[[nodiscard]] constexpr ScalarT max_element(vec<1, ScalarT> const& p)
{
    return p.x;
}
template <class ScalarT>
[[nodiscard]] constexpr ScalarT max_element(vec<2, ScalarT> const& p)
{
    return max(p.x, p.y);
}
template <class ScalarT>
[[nodiscard]] constexpr ScalarT max_element(vec<3, ScalarT> const& p)
{
    return max(p.x, p.y, p.z);
}
template <class ScalarT>
[[nodiscard]] constexpr ScalarT max_element(vec<4, ScalarT> const& p)
{
    return max(max(p.x, p.y), max(p.z, p.w));
}

template <class ScalarT>
[[nodiscard]] constexpr ScalarT max_element(size<1, ScalarT> const& p)
{
    return p.width;
}
template <class ScalarT>
[[nodiscard]] constexpr ScalarT max_element(size<2, ScalarT> const& p)
{
    return max(p.width, p.height);
}
template <class ScalarT>
[[nodiscard]] constexpr ScalarT max_element(size<3, ScalarT> const& p)
{
    return max(p.width, p.height, p.depth);
}
template <class ScalarT>
[[nodiscard]] constexpr ScalarT max_element(size<4, ScalarT> const& p)
{
    return max(max(p.width, p.height), max(p.depth, p.w));
}

} // namespace tg
