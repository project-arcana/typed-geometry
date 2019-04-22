#pragma once

#include <typed-geometry/types/dir.hh>

#include <typed-geometry/common/constants.hh>
#include <typed-geometry/common/scalar_math.hh>

#include <typed-geometry/functions/cross.hh>
#include <typed-geometry/functions/dot.hh>

namespace tg
{
template <int D, class ScalarT>
TG_NODISCARD constexpr bool is_normalized(dir<D, ScalarT> const& v, ScalarT eps = 2 * D * tg::epsilon<ScalarT>)
{
    return tg::abs(ScalarT(1) - dot(v, v)) < eps;
}

template <int D, class ScalarT>
TG_NODISCARD constexpr bool is_zero(dir<D, ScalarT> const& v)
{
    return v == dir<D, ScalarT>();
}

template <int D, class ScalarT>
TG_NODISCARD constexpr bool are_orthogonal(dir<D, ScalarT> const& a, dir<D, ScalarT> const& b, ScalarT eps = 50 * D * tg::epsilon<ScalarT>)
{
    return dot(a, b) < eps;
}

template <int D, class ScalarT>
TG_NODISCARD constexpr bool are_orthonormal(dir<D, ScalarT> const& a, dir<D, ScalarT> const& b, ScalarT eps = 50 * D * tg::epsilon<ScalarT>)
{
    return !is_zero(a) && !is_zero(b) && are_orthogonal(a, b, eps);
}
}
