#pragma once

#include "../../types/angle.hh"
#include "../../types/mat.hh"
#include "../../types/vec.hh"
#include "translation.hh"

// TODO: rotation(axis, angle) or angle/axis?

namespace tg
{
template <class T>
constexpr mat<4, 4, T> rotation_x(angle<T> a)
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
constexpr mat<4, 4, T> rotation_y(angle<T> a)
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
constexpr mat<4, 4, T> rotation_z(angle<T> a)
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
constexpr mat<3, 3, ScalarT> rotation_around(pos<2, ScalarT> p, angle<ScalarT> a)
{
    auto origin_to_p = p - pos<2, ScalarT>::zero;

    auto ca = cos(a);
    auto sa = sin(a);

    auto r = mat<3, 3, ScalarT>::diag(ScalarT(1));
    r[2][2] = 1.0f;

    r[0][0] = ca;
    r[1][0] = -sa;
    r[0][1] = sa;
    r[1][1] = ca;

    return translation(origin_to_p) * r * translation(-origin_to_p);
}
} // namespace tg
