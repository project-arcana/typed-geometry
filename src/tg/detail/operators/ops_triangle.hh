#pragma once

#include "../../types/triangle.hh"
#include "../../types/size.hh"

namespace tg
{
// -- operator@ --

template <int D, class ScalarT>
constexpr triangle<D, ScalarT> operator+(triangle<D, ScalarT> const& a, vec<D, ScalarT> const& b)
{
    triangle<D, ScalarT> r;
    r.v0 = a.v0 + b;
    r.v1 = a.v1 + b;
    r.v2 = a.v2 + b;
    return r;
}

template <int D, class ScalarT>
constexpr triangle<D, ScalarT> operator-(triangle<D, ScalarT> const& a, vec<D, ScalarT> const& b)
{
    triangle<D, ScalarT> r;
    r.v0 = a.v0 - b;
    r.v1 = a.v1 - b;
    r.v2 = a.v2 - b;
    return r;
}

template <int D, class ScalarT>
constexpr triangle<D, ScalarT> operator*(triangle<D, ScalarT> const& a, ScalarT b)
{
    triangle<D, ScalarT> r;
    r.v0 = a.v0 * b;
    r.v1 = a.v1 * b;
    r.v2 = a.v2 * b;
    return r;
}

template <int D, class ScalarT>
constexpr triangle<D, ScalarT> operator*(triangle<D, ScalarT> const& a, size<D, ScalarT> const& b)
{
    triangle<D, ScalarT> r;
    r.v0 = a.v0 * b;
    r.v1 = a.v1 * b;
    r.v2 = a.v2 * b;
    return r;
}

template <int D, class ScalarT>
constexpr triangle<D, ScalarT> operator/(triangle<D, ScalarT> const& a, ScalarT b)
{
    triangle<D, ScalarT> r;
    r.v0 = a.v0 / b;
    r.v1 = a.v1 / b;
    r.v2 = a.v2 / b;
    return r;
}

template <int D, class ScalarT>
constexpr triangle<D, ScalarT> operator/(triangle<D, ScalarT> const& a, size<D, ScalarT> const& b)
{
    triangle<D, ScalarT> r;
    r.v0 = a.v0 / b;
    r.v1 = a.v1 / b;
    r.v2 = a.v2 / b;
    return r;
}

} // namespace tg
