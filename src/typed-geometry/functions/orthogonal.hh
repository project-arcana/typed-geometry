#pragma once

//#include <typed-geometry/detail/scalar_traits.hh>
#include <typed-geometry/types/dir.hh>
#include <typed-geometry/types/vec.hh>

//#include <typed-geometry/detail/operators/ops_vec.hh>

#include <typed-geometry/common/scalar_math.hh>

// Computes orthogonal direction
namespace tg
{
template <class ScalarT>
TG_NODISCARD constexpr dir<2, ScalarT> orthogonal(vec<2, ScalarT> const& v)
{
    // TODO check if input is 0 and reject/warn?
    // TODO alternative: x' = -y, y' = x ?
    if(abs(v.x) > abs(v.y))
        return normalize(cross(v, vec2(0.0f, 1.0f)));
    else
        return normalize(cross(v, vec2(1.0f, 0.0f)));
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
    return normalize(orthogonal(vec<2, ScalarT>(d)));
}

template <class ScalarT>
TG_NODISCARD constexpr dir<3, ScalarT> orthogonal(dir<3, ScalarT> const& d)
{
    return normalize(orthogonal(vec<3, ScalarT>(d)));
}
} // namespace tg
