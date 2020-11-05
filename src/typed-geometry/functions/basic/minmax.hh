#pragma once

#include <typed-geometry/functions/basic/limits.hh>
#include <typed-geometry/functions/basic/scalar_math.hh>

#include <typed-geometry/types/comp.hh>
#include <typed-geometry/types/pos.hh>
#include <typed-geometry/types/size.hh>
#include <typed-geometry/types/span.hh>
#include <typed-geometry/types/vec.hh>

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
// no-arg min/max is in special_values.hh

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
[[nodiscard]] constexpr ScalarT min_element(comp<1, ScalarT> const& p)
{
    return p.comp0;
}
template <class ScalarT>
[[nodiscard]] constexpr ScalarT min_element(comp<2, ScalarT> const& p)
{
    return min(p.comp0, p.comp1);
}
template <class ScalarT>
[[nodiscard]] constexpr ScalarT min_element(comp<3, ScalarT> const& p)
{
    return min(p.comp0, p.comp1, p.comp2);
}
template <class ScalarT>
[[nodiscard]] constexpr ScalarT min_element(comp<4, ScalarT> const& p)
{
    return min(min(p.comp0, p.comp1), min(p.comp2, p.comp3));
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
[[nodiscard]] constexpr ScalarT max_element(comp<1, ScalarT> const& p)
{
    return p.comp0;
}
template <class ScalarT>
[[nodiscard]] constexpr ScalarT max_element(comp<2, ScalarT> const& p)
{
    return max(p.comp0, p.comp1);
}
template <class ScalarT>
[[nodiscard]] constexpr ScalarT max_element(comp<3, ScalarT> const& p)
{
    return max(p.comp0, p.comp1, p.comp2);
}
template <class ScalarT>
[[nodiscard]] constexpr ScalarT max_element(comp<4, ScalarT> const& p)
{
    return max(max(p.comp0, p.comp1), max(p.comp2, p.comp3));
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
