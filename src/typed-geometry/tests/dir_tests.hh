#pragma once

#include <typed-geometry/types/dir.hh>

#include <typed-geometry/common/constants.hh>
#include <typed-geometry/common/scalar_math.hh>

#include <typed-geometry/functions/dot.hh>

namespace tg
{
template <int D, class ScalarT>
TG_NODISCARD constexpr bool are_orthogonal(dir<D, ScalarT> const& a, dir<D, ScalarT> const& b, ScalarT eps = 50 * D * tg::epsilon<ScalarT>)
{
    return abs(dot(a, b)) < eps;
}
}
