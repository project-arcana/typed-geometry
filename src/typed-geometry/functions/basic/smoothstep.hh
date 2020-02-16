#pragma once

#include <typed-geometry/functions/basic/scalar_math.hh>
#include <typed-geometry/types/comp.hh>

namespace tg
{
// TODO: which component-wise smoothstep should be added?

// template <class ScalarT, class = enable_if<is_scalar<ScalarT>>>
// [[nodiscard]] constexpr ScalarT smoothstep(ScalarT edge0, ScalarT edge1, ScalarT x)
// {
//     auto t = clamp((x - edge0) / (edge1 - edge0), ScalarT(0), ScalarT(1));
//     return t * t * (ScalarT(3) - ScalarT(2) * t);
// }
}
