#pragma once

#include <typed-geometry/detail/operators.hh>
#include <typed-geometry/functions/basic/scalar_math.hh>
#include <typed-geometry/types/mat.hh>
#include <typed-geometry/types/quat.hh>
#include <typed-geometry/types/size.hh>
#include <typed-geometry/types/vec.hh>

namespace tg
{
template <class T>
constexpr mat<4, 4, T> compose_transformation(vec<3, T> const& translation, mat<3, 3, T> const& rotation, float scale)
{
    mat<4, 4, T> M;
    M[0] = tg::vec4(rotation[0] * scale, T(0));
    M[1] = tg::vec4(rotation[1] * scale, T(0));
    M[2] = tg::vec4(rotation[2] * scale, T(0));
    M[3] = tg::vec4(translation, T(1));
    return M;
}
template <class T>
constexpr mat<4, 4, T> compose_transformation(vec<3, T> const& translation, mat<3, 3, T> const& rotation, size<3, T> const& scale)
{
    mat<4, 4, T> M;
    M[0] = tg::vec4(rotation[0] * scale[0], T(0));
    M[1] = tg::vec4(rotation[1] * scale[1], T(0));
    M[2] = tg::vec4(rotation[2] * scale[2], T(0));
    M[3] = tg::vec4(translation, T(1));
    return M;
}
}
