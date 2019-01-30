#pragma once

#include "../../types/pos.hh"
#include "../../types/size.hh"
#include "../limits.hh"
#include "../scalars/scalar_math.hh"

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
    return detail::limits<T>::min();
}
template <class T>
constexpr T max()
{
    return detail::limits<T>::max();
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
TG_IMPL_DEFINE_COMPWISE_FUNC_BINARY(pos, min);
TG_IMPL_DEFINE_COMPWISE_FUNC_BINARY(vec, min);
TG_IMPL_DEFINE_COMPWISE_FUNC_BINARY(size, min);

// Component-wise maximum
TG_IMPL_DEFINE_COMPWISE_FUNC_BINARY(pos, max);
TG_IMPL_DEFINE_COMPWISE_FUNC_BINARY(vec, max);
TG_IMPL_DEFINE_COMPWISE_FUNC_BINARY(size, max);

} // namespace tg
