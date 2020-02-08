#pragma once

#include <typed-geometry/types/dir.hh>

#include <typed-geometry/functions/basic/constants.hh>
#include <typed-geometry/functions/basic/scalar_math.hh>
#include <typed-geometry/functions/vector/dot.hh>

namespace tg
{
template <int D, class ScalarT>
[[nodiscard]] constexpr bool are_orthogonal(dir<D, ScalarT> const& a, dir<D, ScalarT> const& b, ScalarT eps = 50 * D * tg::epsilon<ScalarT>)
{
    return abs(dot(a, b)) < eps;
}
}
