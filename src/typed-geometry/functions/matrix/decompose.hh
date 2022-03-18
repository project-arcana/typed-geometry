#pragma once

#include <typed-geometry/detail/operators.hh>
#include <typed-geometry/functions/basic/scalar_math.hh>
#include <typed-geometry/types/mat.hh>
#include <typed-geometry/types/quat.hh>
#include <typed-geometry/types/size.hh>
#include <typed-geometry/types/vec.hh>

#include <typed-geometry/feature/quat.hh>

namespace tg
{
/// decomposes a transformation matrix into a translation t, rotation matrix R, and uniform scale s
///
/// Satisfies:
///   M = tg::translation(t) * tg::rotation(R) * tg::scaling<3>(s)
///   or:
///   M * p = R * s * p + t
///
/// Usage:
///
///   tg::mat4 M = ...;
///   tg::vec3 t;
///   tg::mat3 R;
///   float s;
///   decompose_transformation(M, t, R, s);
///   .. use t,R,s
///
/// NOTE: assumes that M is actually properly decomposable
///       (e.g. rotation may not be orthonormal if M has non-uniform scale)
template <class T>
constexpr void decompose_transformation(mat<4, 4, T> const& M, vec<3, T>& translation, mat<3, 3, T>& rotation, float& scale)
{
    // last column is translation
    translation[0] = M[3][0];
    translation[1] = M[3][1];
    translation[2] = M[3][2];

    // uniform scale is length of first col
    auto s = length(vec<3, T>(M[0]));
    scale = s;

    // rot is 3x3 part
    auto inv_s = T(1) / s;
    rotation[0] = vec<3, T>(M[0]) * inv_s;
    rotation[1] = vec<3, T>(M[1]) * inv_s;
    rotation[2] = vec<3, T>(M[2]) * inv_s;
}
template <class T>
constexpr void decompose_transformation(mat<4, 4, T> const& M, vec<3, T>& translation, mat<3, 3, T>& rotation, size<3, T>& scale)
{
    // last column is translation
    translation[0] = M[3][0];
    translation[1] = M[3][1];
    translation[2] = M[3][2];

    // scale is length of cols
    auto s0 = length(vec<3, T>(M[0]));
    auto s1 = length(vec<3, T>(M[1]));
    auto s2 = length(vec<3, T>(M[2]));
    scale = {s0, s1, s2};

    // rot is 3x3 part
    rotation[0] = vec<3, T>(M[0]) / s0;
    rotation[1] = vec<3, T>(M[1]) / s1;
    rotation[2] = vec<3, T>(M[2]) / s2;
}

/// returns the translation encoded in the transformation matrix as a vector
template <class T, int D>
constexpr vec<D - 1, T> translation_of(mat<D, D, T> const& M)
{
    return vec<D - 1, T>(M[3]);
}

/// returns the scale encoded in the transformation matrix as a vector
template <class T, int D>
constexpr size<D - 1, T> scale_of(mat<D, D, T> const& M)
{
    size<D - 1, T> s;
    for (auto i = 0; i < D - 1; ++i)
        s[i] = length(vec<D - 1, T>(M[i]));
    return s;
}
/// returns the scale encoded in the transformation matrix as a scalar
/// NOTE: assumes that the matrix only has uniform scale
template <class T, int D>
constexpr T scale_uniform_of(mat<D, D, T> const& M)
{
    return length(vec<D - 1, T>(M[0]));
}

/// returns the rotation part of the transformation matrix as a mat3
/// NOTE: assumes that the matrix has no shear
template <class T>
constexpr mat<3, 3, T> rotation_mat3_of(mat<4, 4, T> const& M)
{
    auto const s = scale_of(M);

    mat<3, 3, T> rot;
    rot[0] = vec<3, T>(M[0]) / s[0];
    rot[1] = vec<3, T>(M[1]) / s[1];
    rot[2] = vec<3, T>(M[2]) / s[2];
    return rot;
}
/// returns the rotation part of the transformation matrix as a mat4 (i.e. a valid mat4 rotation matrix)
/// NOTE: assumes that the matrix has no shear
template <class T>
constexpr mat<4, 4, T> rotation_mat4_of(mat<4, 4, T> const& M)
{
    auto const s = scale_of(M);

    mat<4, 4, T> rot;
    rot[0] = vec<4, T>(vec<3, T>(M[0]) / s[0], T(0));
    rot[1] = vec<4, T>(vec<3, T>(M[1]) / s[1], T(0));
    rot[2] = vec<4, T>(vec<3, T>(M[2]) / s[2], T(0));
    rot[3][3] = T(1);
    return rot;
}
/// returns the rotation part of the transformation matrix as a quaternion
/// NOTE: assumes that the matrix has no shear
template <class T>
constexpr quaternion<T> rotation_quat_of(mat<4, 4, T> const& M)
{
    return quaternion<T>::from_rotation_matrix(rotation_mat3_of(M));
}
}
