#pragma once

#include <typed-geometry/detail/scalar_traits.hh>
#include <typed-geometry/types/vec.hh>

#include "length.hh"

namespace tg
{
template <int D, class ScalarT>
TG_NODISCARD constexpr bool is_normalized(vec<D, ScalarT> const& v)
{
    return tg::abs(ScalarT(1) - length2(v)) < 2 * D * tg::epsilon<ScalarT>;
}

template <int D, class ScalarT>
TG_NODISCARD constexpr vec<D, fractional_result<ScalarT>> normalize(vec<D, ScalarT> const& v)
{
    return v / length(v);
}

template <int D, class ScalarT>
TG_NODISCARD constexpr vec<D, fractional_result<ScalarT>> normalize_safe(vec<D, ScalarT> const& v, ScalarT eps = ScalarT(0))
{
    auto l = length(v);
    return l <= eps ? vec<D, ScalarT>::zero : v / l;
}
} // namespace tg
