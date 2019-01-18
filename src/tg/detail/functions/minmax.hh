#pragma once

#include <limits>
#include "../../detail/traits.hh"
#include "../../types/pos.hh"
#include "../../types/size.hh"

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
constexpr T min()
{
    return std::numeric_limits<T>::lowest();
}
template <class T>
constexpr T max()
{
    return std::numeric_limits<T>::max();
}

template <class ScalarT>
constexpr ScalarT min_element(pos<1, ScalarT> const& p)
{
    return p.x;
}
template <class ScalarT>
constexpr ScalarT min_element(pos<2, ScalarT> const& p)
{
    return min(p.x, p.y);
}
template <class ScalarT>
constexpr ScalarT min_element(pos<3, ScalarT> const& p)
{
    return min(p.x, p.y, p.z);
}
template <class ScalarT>
constexpr ScalarT min_element(pos<4, ScalarT> const& p)
{
    return min(min(p.x, p.y), min(p.z, p.w));
}

template <class ScalarT>
constexpr ScalarT max_element(pos<1, ScalarT> const& p)
{
    return p.x;
}
template <class ScalarT>
constexpr ScalarT max_element(pos<2, ScalarT> const& p)
{
    return max(p.x, p.y);
}
template <class ScalarT>
constexpr ScalarT max_element(pos<3, ScalarT> const& p)
{
    return max(p.x, p.y, p.z);
}
template <class ScalarT>
constexpr ScalarT max_element(pos<4, ScalarT> const& p)
{
    return max(max(p.x, p.y), max(p.z, p.w));
}

template <class T>
constexpr T min(T const& a)
{
    return a;
}
template <class T, class... Ts>
constexpr T min(T const& a, T const& b, T const& c, Ts const&... rest)
{
    return min(min(a, b), c, rest...);
}

template <class T>
constexpr T max(T const& a)
{
    return a;
}
template <class T, class... Ts>
constexpr T max(T const& a, T const& b, T const& c, Ts const&... rest)
{
    return max(max(a, b), c, rest...);
}


// Component-wise minimum
template <class ScalarT>
constexpr pos<1, ScalarT> min(pos<1, ScalarT> const& a, pos<1, ScalarT> const& b)
{
    return min(a, b);
}
template <class ScalarT>
constexpr pos<2, ScalarT> min(pos<2, ScalarT> const& a, pos<2, ScalarT> const& b)
{
    pos<2, ScalarT> r;
    r.x = min(a.x, b.x);
    r.y = min(a.y, b.y);
    return r;
}
template <class ScalarT>
constexpr pos<3, ScalarT> min(pos<3, ScalarT> const& a, pos<3, ScalarT> const& b)
{
    pos<3, ScalarT> r;
    r.x = min(a.x, b.x);
    r.y = min(a.y, b.y);
    r.z = min(a.z, b.z);
    return r;
}
template <class ScalarT>
constexpr pos<4, ScalarT> min(pos<4, ScalarT> const& a, pos<4, ScalarT> const& b)
{
    pos<4, ScalarT> r;
    r.x = min(a.x, b.x);
    r.y = min(a.y, b.y);
    r.z = min(a.z, b.z);
    r.w = min(a.w, b.w);
    return r;
}
template <class ScalarT>
constexpr size<1, ScalarT> min(size<1, ScalarT> const& a, size<1, ScalarT> const& b)
{
    return min(a, b);
}
template <class ScalarT>
constexpr size<2, ScalarT> min(size<2, ScalarT> const& a, size<2, ScalarT> const& b)
{
    size<2, ScalarT> r;
    r.x = min(a.x, b.x);
    r.y = min(a.y, b.y);
    return r;
}
template <class ScalarT>
constexpr size<3, ScalarT> min(size<3, ScalarT> const& a, size<3, ScalarT> const& b)
{
    size<3, ScalarT> r;
    r.x = min(a.x, b.x);
    r.y = min(a.y, b.y);
    r.z = min(a.z, b.z);
    return r;
}
template <class ScalarT>
constexpr size<4, ScalarT> min(size<4, ScalarT> const& a, size<4, ScalarT> const& b)
{
    size<4, ScalarT> r;
    r.x = min(a.x, b.x);
    r.y = min(a.y, b.y);
    r.z = min(a.z, b.z);
    r.w = min(a.w, b.w);
    return r;
}

// Component-wise maximum
template <class ScalarT>
constexpr pos<1, ScalarT> max(pos<1, ScalarT> const& a, pos<1, ScalarT> const& b)
{
    return max(a, b);
}
template <class ScalarT>
constexpr pos<2, ScalarT> max(pos<2, ScalarT> const& a, pos<2, ScalarT> const& b)
{
    pos<2, ScalarT> r;
    r.x = max(a.x, b.x);
    r.y = max(a.y, b.y);
    return r;
}
template <class ScalarT>
constexpr pos<3, ScalarT> max(pos<3, ScalarT> const& a, pos<3, ScalarT> const& b)
{
    pos<3, ScalarT> r;
    r.x = max(a.x, b.x);
    r.y = max(a.y, b.y);
    r.z = max(a.z, b.z);
    return r;
}
template <class ScalarT>
constexpr pos<4, ScalarT> max(pos<4, ScalarT> const& a, pos<4, ScalarT> const& b)
{
    pos<4, ScalarT> r;
    r.x = max(a.x, b.x);
    r.y = max(a.y, b.y);
    r.z = max(a.z, b.z);
    r.w = max(a.w, b.w);
    return r;
}
template <class ScalarT>
constexpr size<1, ScalarT> max(size<1, ScalarT> const& a, size<1, ScalarT> const& b)
{
    return max(a, b);
}
template <class ScalarT>
constexpr size<2, ScalarT> max(size<2, ScalarT> const& a, size<2, ScalarT> const& b)
{
    size<2, ScalarT> r;
    r.x = max(a.x, b.x);
    r.y = max(a.y, b.y);
    return r;
}
template <class ScalarT>
constexpr size<3, ScalarT> max(size<3, ScalarT> const& a, size<3, ScalarT> const& b)
{
    size<3, ScalarT> r;
    r.x = max(a.x, b.x);
    r.y = max(a.y, b.y);
    r.z = max(a.z, b.z);
    return r;
}
template <class ScalarT>
constexpr size<4, ScalarT> max(size<4, ScalarT> const& a, size<4, ScalarT> const& b)
{
    size<4, ScalarT> r;
    r.x = max(a.x, b.x);
    r.y = max(a.y, b.y);
    r.z = max(a.z, b.z);
    r.w = max(a.w, b.w);
    return r;
}

} // namespace tg
