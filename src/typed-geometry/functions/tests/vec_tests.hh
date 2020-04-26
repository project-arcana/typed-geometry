#pragma once

#include <typed-geometry/types/vec.hh>

#include <typed-geometry/functions/basic/constants.hh>
#include <typed-geometry/functions/basic/scalar_math.hh>

#include <typed-geometry/functions/basic/minmax.hh>
#include <typed-geometry/functions/vector/cross.hh>
#include <typed-geometry/functions/vector/dot.hh>

namespace tg
{
template <int D, class ScalarT>
[[nodiscard]] constexpr bool is_normalized(vec<D, ScalarT> const& v, ScalarT eps = 2 * D * tg::epsilon<ScalarT>)
{
    return abs(ScalarT(1) - length_sqr(v)) < eps;
}

template <int D, class ScalarT, class = enable_if<is_integer<ScalarT>>>
[[nodiscard]] constexpr bool is_zero_vector(vec<D, ScalarT> const& v)
{
    return v == vec<D, ScalarT>::zero;
}

template <int D, class ScalarT, class = enable_if<is_floating_point<ScalarT>>>
[[nodiscard]] constexpr bool is_zero_vector(vec<D, ScalarT> const& v, ScalarT eps = 2 * tg::epsilon<ScalarT>)
{
    return length_sqr(v) < eps * eps;
}

template <int D, class ScalarT>
[[nodiscard]] constexpr bool are_orthogonal(vec<D, ScalarT> const& a, vec<D, ScalarT> const& b, ScalarT eps = 100 * D * tg::epsilon<ScalarT>)
{
    return abs(dot(a, b)) < max(max_element(abs(a)), max_element(abs(b))) * eps;
}

template <int D, class ScalarT>
[[nodiscard]] constexpr bool are_orthonormal(vec<D, ScalarT> const& a, vec<D, ScalarT> const& b, ScalarT eps = 100 * D * tg::epsilon<ScalarT>)
{
    return is_normalized(a, eps) && is_normalized(b, eps) && are_orthogonal(a, b, eps);
}

template <int D, class ScalarT>
[[nodiscard]] constexpr bool are_collinear(vec<D, ScalarT> const& a, vec<D, ScalarT> const& b, ScalarT eps = 50 * D * tg::epsilon<ScalarT>)
{
    return is_zero_vector(cross(a, b), eps);
}
}
