#pragma once

#include <typed-geometry/types/vec.hh>

#include <typed-geometry/common/constants.hh>
#include <typed-geometry/common/scalar_math.hh>

#include <typed-geometry/functions/cross.hh>
#include <typed-geometry/functions/dot.hh>

namespace tg
{
template <int D, class ScalarT>
TG_NODISCARD constexpr bool is_normalized(vec<D, ScalarT> const& v, ScalarT eps = 2 * D * tg::epsilon<ScalarT>)
{
    return abs(ScalarT(1) - length_sqr(v)) < eps;
}

template <int D, class ScalarT, class = enable_if<is_floating_point<ScalarT>>>
TG_NODISCARD constexpr bool is_zero_vector(vec<D, ScalarT> const& v, ScalarT eps = 2 * tg::epsilon<ScalarT>)
{
    return length_sqr(v) < eps * eps;
}

template <int D, class ScalarT>
TG_NODISCARD constexpr bool are_orthogonal(vec<D, ScalarT> const& a, vec<D, ScalarT> const& b, ScalarT eps = 50 * D * tg::epsilon<ScalarT>)
{
    return abs(dot(a, b)) < eps;
}

template <int D, class ScalarT>
TG_NODISCARD constexpr bool are_orthonormal(vec<D, ScalarT> const& a, vec<D, ScalarT> const& b, ScalarT eps = 50 * D * tg::epsilon<ScalarT>)
{
    return is_normalized(a, eps) && is_normalized(b, eps) && are_orthogonal(a, b, eps);
}

template <int D, class ScalarT>
TG_NODISCARD constexpr bool are_collinear(vec<D, ScalarT> const& a, vec<D, ScalarT> const& b, ScalarT eps = 50 * D * tg::epsilon<ScalarT>)
{
    return is_zero_vector(cross(a, b), eps);
}
}
