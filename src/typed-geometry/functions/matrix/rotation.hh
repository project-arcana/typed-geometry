#pragma once

#include <typed-geometry/feature/assert.hh>

#include <typed-geometry/types/angle.hh>
#include <typed-geometry/types/dir.hh>
#include <typed-geometry/types/mat.hh>

#include <typed-geometry/detail/operators/ops_mat.hh>

#include <typed-geometry/functions/vector/normalize.hh>

#include "translation.hh"

namespace tg
{
template <class T>
[[nodiscard]] constexpr mat<4, 4, T> rotation_around(dir<3, T> const& axis, angle_t<T> angle)
{
    auto ca = cos(angle);
    auto sa = sin(angle);
    auto one_minus_ca = T(1) - ca;

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
[[nodiscard]] constexpr mat<4, 4, T> rotation_around(angle_t<T> angle, dir<3, T> const& axis)
{
    return rotation_around(axis, angle);
}

template <class T>
[[nodiscard]] constexpr mat<4, 4, T> rotation_x(angle_t<T> a)
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
[[nodiscard]] constexpr mat<4, 4, T> rotation_y(angle_t<T> a)
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
[[nodiscard]] constexpr mat<4, 4, T> rotation_z(angle_t<T> a)
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

// TODO: more performant with no matrix mul
template <class T>
[[nodiscard]] constexpr vec<3, T> rotate_x(vec<3, T> const& v, angle_t<T> a)
{
    return rotation_x(a) * v;
}
template <class T>
[[nodiscard]] constexpr dir<3, T> rotate_x(dir<3, T> const& v, angle_t<T> a)
{
    return dir<3, T>(rotation_x(a) * v);
}
template <class T>
[[nodiscard]] constexpr pos<3, T> rotate_x(pos<3, T> const& v, angle_t<T> a)
{
    return rotation_x(a) * v;
}
template <class T>
[[nodiscard]] constexpr vec<3, T> rotate_y(vec<3, T> const& v, angle_t<T> a)
{
    return rotation_y(a) * v;
}
template <class T>
[[nodiscard]] constexpr dir<3, T> rotate_y(dir<3, T> const& v, angle_t<T> a)
{
    return dir<3, T>(rotation_y(a) * v);
}
template <class T>
[[nodiscard]] constexpr pos<3, T> rotate_y(pos<3, T> const& v, angle_t<T> a)
{
    return rotation_y(a) * v;
}
template <class T>
[[nodiscard]] constexpr vec<3, T> rotate_z(vec<3, T> const& v, angle_t<T> a)
{
    return rotation_z(a) * v;
}
template <class T>
[[nodiscard]] constexpr dir<3, T> rotate_z(dir<3, T> const& v, angle_t<T> a)
{
    return dir<3, T>(rotation_z(a) * v);
}
template <class T>
[[nodiscard]] constexpr pos<3, T> rotate_z(pos<3, T> const& v, angle_t<T> a)
{
    return rotation_z(a) * v;
}

template <class ScalarT>
[[nodiscard]] constexpr mat<3, 3, ScalarT> rotation_around(pos<2, ScalarT> p, angle_t<ScalarT> a)
{
    auto origin_to_p = p - pos<2, ScalarT>::zero;

    auto ca = cos(a);
    auto sa = sin(a);

    auto r = mat<3, 3, ScalarT>::identity;

    r[0][0] = ca;
    r[1][0] = -sa;
    r[0][1] = sa;
    r[1][1] = ca;

    return translation(origin_to_p) * r * translation(-origin_to_p);
}

template <class ScalarT>
[[nodiscard]] constexpr vec<2, ScalarT> rotate(vec<2, ScalarT> v, angle_t<ScalarT> a)
{
    auto [sin, cos] = tg::sin_cos(a);
    return {cos * v.x - sin * v.y, sin * v.x + cos * v.y};
}
template <class ScalarT>
[[nodiscard]] constexpr dir<2, ScalarT> rotate(dir<2, ScalarT> v, angle_t<ScalarT> a)
{
    auto [sin, cos] = tg::sin_cos(a);
    return {cos * v.x - sin * v.y, sin * v.x + cos * v.y};
}
template <class ScalarT>
[[nodiscard]] constexpr pos<2, ScalarT> rotate(pos<2, ScalarT> v, angle_t<ScalarT> a)
{
    auto [sin, cos] = tg::sin_cos(a);
    return {cos * v.x - sin * v.y, sin * v.x + cos * v.y};
}
} // namespace tg
