#pragma once

#include <typed-geometry/detail/optional.hh>
#include <typed-geometry/detail/scalar_traits.hh>
#include <typed-geometry/types/dir.hh>
#include <typed-geometry/types/vec.hh>

#include "length.hh"

namespace tg
{
template <int D, class ScalarT>
[[nodiscard]] constexpr dir<D, fractional_result<ScalarT>> normalize(vec<D, ScalarT> const& v)
{
    return dir<D, fractional_result<ScalarT>>(v / length(v));
}

/// returns vec::zero if too small
template <int D, class ScalarT>
[[nodiscard]] constexpr vec<D, fractional_result<ScalarT>> normalize_safe(vec<D, ScalarT> const& v, ScalarT eps = ScalarT(0))
{
    auto l = length(v);
    return l <= eps ? vec<D, ScalarT>::zero : v / l;
}
} // namespace tg
