#pragma once

#include <typed-geometry/types/quadric.hh>

namespace tg
{
template <int D, class ScalarT>
[[nodiscard]] constexpr quadric<D, ScalarT> operator+(quadric<D, ScalarT> const& a, quadric<D, ScalarT> const& b)
{
    quadric<D, ScalarT> r = a; // copy
    r.add(b);
    return r;
}

template <class ScalarT>
[[nodiscard]] constexpr quadric<3, ScalarT> operator*(quadric<3, ScalarT> q, dont_deduce<ScalarT> s)
{
    // q is already a copy
    q.A00 *= s;
    q.A01 *= s;
    q.A02 *= s;
    q.A11 *= s;
    q.A12 *= s;
    q.A22 *= s;
    q.b0 *= s;
    q.b1 *= s;
    q.b2 *= s;
    q.c *= s;
    return q;
}

template <class ScalarT>
[[nodiscard]] constexpr quadric<3, ScalarT> operator*(dont_deduce<ScalarT> s, quadric<3, ScalarT> q)
{
    return q * s;
}
template <class ScalarT>
[[nodiscard]] constexpr quadric<3, ScalarT> operator/(quadric<3, ScalarT> q, dont_deduce<ScalarT> s)
{
    return q * (ScalarT(1) / s);
}
}
