#pragma once

#include <typed-geometry/types/quat.hh>

#include <typed-geometry/functions/basic/scalar_math.hh>

namespace tg
{
template <class ScalarT>
[[nodiscard]] constexpr bool is_normalized(quaternion<ScalarT> const& q, ScalarT eps = ScalarT(0.001))
{
    ScalarT const length2 = q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w;
    return tg::abs(ScalarT(1) - length2) < eps;
}
}
