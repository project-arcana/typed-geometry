#pragma once

#include <typed-geometry/common/assert.hh>

#include <typed-geometry/types/angle.hh>
#include <typed-geometry/types/mat.hh>
#include <typed-geometry/types/vec.hh>

#include "normalize.hh"
#include "translation.hh"

namespace tg
{
template <class T>
TG_NODISCARD constexpr mat<4, 4, T> rotation(vec<3, T> const& axis, angle<T> angle)
{
    TG_ASSERT(is_normalized(axis));

    auto ca = cos(angle);
    auto sa = sin(angle);
    auto one_minus_ca = 1 - ca;

    auto ux = axis.x;
    auto uy = axis.y;
    auto uz = axis.z;

    // see https://en.wikipedia.org/wiki/Rotation_matrix
    auto m = mat<4, 4, T>::identity;

    m[0][0] = ca + ux * ux * one_minus_ca;
    m[1][1] = ca + uy * uy * one_minus_ca;
    m[2][2] = ca + uz * uz * one_minus_ca;

    m[0][1] = uy * ux * one_minus_ca + uz * sa;
    m[0][2] = uz * ux * one_minus_ca - uy * sa;
    m[1][0] = ux * uy * one_minus_ca - uz * sa;
    m[1][2] = uz * uy * one_minus_ca + ux * sa;
    m[2][0] = ux * uz * one_minus_ca + uy * sa;
    m[2][1] = uy * uz * one_minus_ca - ux * sa;

    return m;
}
template <class T>
TG_NODISCARD constexpr mat<4, 4, T> rotation(angle<T> angle, vec<3, T> const& axis)
{
    return rotation(axis, angle);
}

template <class T>
TG_NODISCARD constexpr mat<4, 4, T> rotation_x(angle<T> a)
{
    auto ca = cos(a);
    auto sa = sin(a);

    auto m = mat<4, 4, T>::identity;
    m[1][1] = ca;
    m[2][1] = -sa;
    m[1][2] = sa;
    m[2][2] = ca;
    return m;
}
template <class T>
TG_NODISCARD constexpr mat<4, 4, T> rotation_y(angle<T> a)
{
    auto ca = cos(a);
    auto sa = sin(a);

    auto m = mat<4, 4, T>::identity;
    m[0][0] = ca;
    m[2][0] = sa;
    m[0][2] = -sa;
    m[2][2] = ca;
    return m;
}
template <class T>
TG_NODISCARD constexpr mat<4, 4, T> rotation_z(angle<T> a)
{
    auto ca = cos(a);
    auto sa = sin(a);

    auto m = mat<4, 4, T>::identity;
    m[0][0] = ca;
    m[1][0] = -sa;
    m[0][1] = sa;
    m[1][1] = ca;
    return m;
}

template <class ScalarT>
TG_NODISCARD constexpr mat<3, 3, ScalarT> rotation_around(pos<2, ScalarT> p, angle<ScalarT> a)
{
    auto origin_to_p = p - pos<2, ScalarT>::zero;

    auto ca = cos(a);
    auto sa = sin(a);

    auto r = mat<3, 3, ScalarT>::identity;
    r[2][2] = 1.0f;

    r[0][0] = ca;
    r[1][0] = -sa;
    r[0][1] = sa;
    r[1][1] = ca;

    return translation(origin_to_p) * r * translation(-origin_to_p);
}
} // namespace tg
