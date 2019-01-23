#pragma once

#include "../../types/vec.hh"
#include "../traits.hh"
#include "length.hh"

namespace tg
{
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
