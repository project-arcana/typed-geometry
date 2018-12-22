#pragma once

#include "op_helper.hh"

#include "../types/vec.hh"

namespace tg
{
template <int D, class ScalarT>
constexpr ScalarT const* data_ptr(vec<D, ScalarT> const& v)
{
    return &v.x;
}
template <int D, class ScalarT>
constexpr ScalarT* data_ptr(vec<D, ScalarT>& v)
{
    return &v.x;
}

// -- operator@ --

template <int D, class ScalarT>
constexpr vec<D, ScalarT> operator+(vec<D, ScalarT> const& a, vec<D, ScalarT> const& b)
{
    return detail::compwise<ScalarT>(a, b, detail::add<ScalarT>);
}
template <int D, class ScalarT>
constexpr vec<D, ScalarT> operator+(vec<D, ScalarT> const& a, ScalarT b)
{
    return detail::compwise<ScalarT>(a, b, detail::add<ScalarT>);
}

template <int D, class ScalarT>
constexpr vec<D, ScalarT> operator-(vec<D, ScalarT> const& a, vec<D, ScalarT> const& b)
{
    return detail::compwise<ScalarT>(a, b, detail::sub<ScalarT>);
}
template <int D, class ScalarT>
constexpr vec<D, ScalarT> operator-(vec<D, ScalarT> const& a, ScalarT b)
{
    return detail::compwise<ScalarT>(a, b, detail::sub<ScalarT>);
}
template <int D, class ScalarT>
constexpr vec<D, ScalarT> operator-(vec<D, ScalarT> const& a)
{
    return detail::compwise<ScalarT>(a, detail::neg<ScalarT>);
}

template <int D, class ScalarT>
constexpr vec<D, ScalarT> operator*(vec<D, ScalarT> const& a, ScalarT b)
{
    return detail::compwise<ScalarT>(a, b, detail::mul<ScalarT>);
}
template <int D, class ScalarT>
constexpr vec<D, fractional_result<ScalarT>> operator/(vec<D, ScalarT> const& a, ScalarT b)
{
    return detail::compwise<ScalarT>(a, 1 / fractional_result<ScalarT>(b), detail::mul<ScalarT>);
}

template <int D, class ScalarT>
constexpr bool operator==(vec<D, ScalarT> const& a, vec<D, ScalarT> const& b)
{
    return all(equals(a, b));
}
template <int D, class ScalarT>
constexpr bool operator!=(vec<D, ScalarT> const& a, vec<D, ScalarT> const& b)
{
    return !(a == b);
}

template <int D>
constexpr vec<D, bool> operator&&(vec<D, bool> const& a, vec<D, bool> const& b)
{
    return detail::compwise<bool>(a, b, detail::logic_and);
}
template <int D>
constexpr vec<D, bool> operator||(vec<D, bool> const& a, vec<D, bool> const& b)
{
    return detail::compwise<bool>(a, b, detail::logic_or);
}
template <int D>
constexpr vec<D, bool> operator!(vec<D, bool> const& a)
{
    return detail::compwise<bool>(a, detail::logic_not);
}

// -- functions --

template <int D, class ScalarT>
constexpr vec<D, bool> equals(vec<D, ScalarT> const& a, vec<D, ScalarT> const& b)
{
    return detail::compwise<bool>(a, b, detail::eq<ScalarT>);
}
template <int D>
constexpr bool any(vec<D, bool> const& v)
{
    return detail::reduce(v, detail::logic_or);
}
template <int D>
constexpr bool all(vec<D, bool> const& v)
{
    return detail::reduce(v, detail::logic_and);
}

template <int D, class ScalarT>
constexpr ScalarT dot(vec<D, ScalarT> const& a, vec<D, ScalarT> const& b)
{
    vec<D, ScalarT> r;
    detail::apply(r, a, b, detail::mul<ScalarT>);
    return detail::reduce(r, detail::add<ScalarT>);
}

template <class ScalarT>
constexpr vec<3, ScalarT> cross(vec<3, ScalarT> const& a, vec<3, ScalarT> const& b)
{
    vec<3, ScalarT> r;
    r.x = a.y * b.z - a.z * b.y;
    r.y = a.z * b.x - a.x * b.z;
    r.z = a.x * b.y - a.y * b.x;
    return r;
}

template <class ScalarT>
constexpr ScalarT cross(vec<2, ScalarT> const& a, vec<2, ScalarT> const& b)
{
    return a.y * b.z - a.z * b.y;
}

template <int D, class ScalarT>
constexpr squared_result<ScalarT> length2(vec<D, ScalarT> const& v)
{
    auto a = vec<D, squared_result<ScalarT>>(v);
    return dot(a, a);
}
template <int D, class ScalarT>
constexpr fractional_result<ScalarT> length(vec<D, ScalarT> const& v)
{
    return sqrt(length2(vec<D, fractional_result<ScalarT>>(v)));
}

template <int D, class ScalarT>
constexpr vec<D, fractional_result<ScalarT>> normalize(vec<D, ScalarT> const& v)
{
    return v / length(v);
}
template <int D, class ScalarT>
constexpr vec<D, fractional_result<ScalarT>> normalize_safe(vec<D, ScalarT> const& v, ScalarT eps = ScalarT(0))
{
    auto l = length(v);
    return l <= eps ? vec<D, ScalarT>::zero : v / l;
}

} // namespace tg