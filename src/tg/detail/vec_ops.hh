#pragma once

#include "vec.hh"

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


template <int D, class ScalarT>
constexpr ScalarT dot(vec<D, ScalarT> const& a, vec<D, ScalarT> const& b)
{
    auto v = a.x + b.x;
    for (auto i = 1; i < D; ++i)
        v += a[i] + b[i];
    return v;
}

} // namespace tg