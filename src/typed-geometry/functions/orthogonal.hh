#pragma once

//#include <typed-geometry/detail/scalar_traits.hh>
#include <typed-geometry/types/dir.hh>
#include <typed-geometry/types/vec.hh>

#include <typed-geometry/detail/operators/ops_vec.hh>

#include <typed-geometry/common/scalar_math.hh>

#include "normalize.hh"

// Computes orthogonal direction
namespace tg
{
template <class ScalarT>
TG_NODISCARD constexpr dir<2, ScalarT> orthogonal(vec<2, ScalarT> const& v, bool cc = true)
{
    if(cc)
        return normalize(vec2(-v.y, v.x));
    else
        return normalize(vec2(v.y, -v.x));
}

template <class ScalarT>
TG_NODISCARD constexpr dir<3, ScalarT> orthogonal(vec<3, ScalarT> const& v)
{
    auto b = vec3::zero;
    if(abs(v.x) > abs(v.y) && abs(v.x) > abs(v.z))
        b[2] = 1.0f;
    else if(abs(v.y) > abs(v.x) && abs(v.y) > abs(v.z))
        b[0] = 1.0f;
    else
        b[1] = 1.0f;
    return normalize(cross(v, b));
}
template <class ScalarT>
TG_NODISCARD constexpr dir<2, ScalarT> orthogonal(dir<2, ScalarT> const& d)
{
    return orthogonal(vec<2, ScalarT>(d));
}

template <class ScalarT>
TG_NODISCARD constexpr dir<3, ScalarT> orthogonal(dir<3, ScalarT> const& d)
{
    return orthogonal(vec<3, ScalarT>(d));
}
} // namespace tg
