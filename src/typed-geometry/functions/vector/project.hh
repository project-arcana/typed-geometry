#pragma once

#include <typed-geometry/types/pos.hh>
#include <typed-geometry/types/vec.hh>

#include <typed-geometry/detail/operators/ops_vec.hh>

namespace tg
{
template <int D, class ScalarT>
[[nodiscard]] constexpr vec<D, ScalarT> project(vec<D, ScalarT> const& a, vec<D, ScalarT> const& b)
{
    return b * dot(a, b) / dot(b, b);
}

template <int D, class ScalarT>
[[nodiscard]] constexpr vec<D, ScalarT> project(vec<D, ScalarT> const& a, dir<D, ScalarT> const& b)
{
    return b * dot(a, b);
}
}
