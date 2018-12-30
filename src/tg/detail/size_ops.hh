#pragma once

#include "op_helper.hh"

#include "../types/size.hh"

namespace tg
{
template <int D, class ScalarT>
constexpr ScalarT const* data_ptr(size<D, ScalarT> const& v)
{
    return &v.width;
}
template <int D, class ScalarT>
constexpr ScalarT* data_ptr(size<D, ScalarT>& v)
{
    return &v.width;
}

// -- operator@ --

template <int D, class ScalarT>
constexpr size<D, ScalarT> operator+(size<D, ScalarT> const& a, size<D, ScalarT> const& b)
{
    return detail::compwise<ScalarT>(a, b, detail::add<ScalarT>);
}
template <int D, class ScalarT>
constexpr size<D, ScalarT> operator+(size<D, ScalarT> const& a, ScalarT b)
{
    return detail::compwise<ScalarT>(a, b, detail::add<ScalarT>);
}

template <int D, class ScalarT>
constexpr size<D, ScalarT> operator-(size<D, ScalarT> const& a, size<D, ScalarT> const& b)
{
    return detail::compwise<ScalarT>(a, b, detail::sub<ScalarT>);
}
template <int D, class ScalarT>
constexpr size<D, ScalarT> operator-(size<D, ScalarT> const& a, ScalarT b)
{
    return detail::compwise<ScalarT>(a, b, detail::sub<ScalarT>);
}
template <int D, class ScalarT>
constexpr size<D, ScalarT> operator-(size<D, ScalarT> const& a)
{
    return detail::compwise<ScalarT>(a, detail::neg<ScalarT>);
}

template <int D, class ScalarT>
constexpr size<D, ScalarT> operator*(size<D, ScalarT> const& a, ScalarT b)
{
    return detail::compwise<ScalarT>(a, b, detail::mul<ScalarT>);
}
template <int D, class ScalarT>
constexpr size<D, fractional_result<ScalarT>> operator/(size<D, ScalarT> const& a, ScalarT b)
{
    return detail::compwise<ScalarT>(a, 1 / fractional_result<ScalarT>(b), detail::mul<ScalarT>);
}

template <int D, class ScalarT>
constexpr bool operator==(size<D, ScalarT> const& a, size<D, ScalarT> const& b)
{
    return all(equals(a, b));
}
template <int D, class ScalarT>
constexpr bool operator!=(size<D, ScalarT> const& a, size<D, ScalarT> const& b)
{
    return !(a == b);
}

template <int D>
constexpr size<D, bool> operator&&(size<D, bool> const& a, size<D, bool> const& b)
{
    return detail::compwise<bool>(a, b, detail::logic_and);
}
template <int D>
constexpr size<D, bool> operator||(size<D, bool> const& a, size<D, bool> const& b)
{
    return detail::compwise<bool>(a, b, detail::logic_or);
}
template <int D>
constexpr size<D, bool> operator!(size<D, bool> const& a)
{
    return detail::compwise<bool>(a, detail::logic_not);
}

// -- functions --

template <int D, class ScalarT>
constexpr size<D, bool> equals(size<D, ScalarT> const& a, size<D, ScalarT> const& b)
{
    return detail::compwise<bool>(a, b, detail::eq<ScalarT>);
}
template <int D>
constexpr bool any(size<D, bool> const& v)
{
    return detail::reduce(v, detail::logic_or);
}
template <int D>
constexpr bool all(size<D, bool> const& v)
{
    return detail::reduce(v, detail::logic_and);
}
} // namespace tg