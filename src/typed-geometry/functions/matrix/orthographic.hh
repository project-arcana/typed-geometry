#pragma once

#include <typed-geometry/types/mat.hh>

namespace tg
{
// right-handed, -1..1 (OpenGL default)
template <typename T>
mat<4, 4, T> orthographic(T left, T right, T bottom, T top, T zNear, T zFar)
{
    mat<4, 4, T> m;
    m[0][0] = T(2) / (right - left);
    m[1][1] = T(2) / (top - bottom);
    m[2][2] = -T(2) / (zFar - zNear);
    m[3][0] = -(right + left) / (right - left);
    m[3][1] = -(top + bottom) / (top - bottom);
    m[3][2] = -(zFar + zNear) / (zFar - zNear);
    m[3][3] = T(1);
    return m;
}

// right-handed, xy: -1..1, z: 1..0
template <typename T>
mat<4, 4, T> orthographic_reverse_z(T left, T right, T bottom, T top, T zNear, T zFar)
{
    mat<4, 4, T> m;
    m[0][0] = T(2) / (right - left);
    m[1][1] = T(2) / (top - bottom);
    m[2][2] = T(1) / (zFar - zNear);
    m[3][0] = -(right + left) / (right - left);
    m[3][1] = -(top + bottom) / (top - bottom);
    m[3][2] = zFar / (zFar - zNear);
    m[3][3] = T(1);
    return m;
}

}
