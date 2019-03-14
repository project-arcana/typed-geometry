#pragma once

#include "../../types/angle.hh"
#include "../../types/mat.hh"
#include "../../types/vec.hh"

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
} // namespace tg
