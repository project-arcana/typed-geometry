#pragma once

#include <typed-geometry/detail/operators/ops_mat.hh>
#include <typed-geometry/functions/basic/mix.hh>
#include <typed-geometry/functions/basic/scalar_math.hh>
#include <typed-geometry/functions/vector/normalize.hh>
#include <typed-geometry/types/dir.hh>
#include <typed-geometry/types/mat.hh>
#include <typed-geometry/types/quat.hh>
#include <typed-geometry/types/vec.hh>

namespace tg
{
// ================================= constructors =================================

template <class ScalarT>
[[nodiscard]] constexpr quaternion<ScalarT> quaternion<ScalarT>::from_axis_angle(dir<3, ScalarT> const& axis, angle_t<ScalarT> angle)
{
    auto const a_half = angle * ScalarT(0.5);
    auto const [sa, ca] = sin_cos(a_half);
    return {axis * sa, ca};
}

template <class ScalarT>
[[nodiscard]] constexpr quaternion<ScalarT> quaternion<ScalarT>::from_rotation_matrix(mat<3, 3, ScalarT> const& m)
{
    // see http://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/

    quaternion<ScalarT> q;
    auto const trace = m[0][0] + m[1][1] + m[2][2];
    if (trace > ScalarT(0))
    {
        auto const sq = sqrt(trace + ScalarT(1));
        auto const s = ScalarT(0.5) / sq;
        q.w = sq * ScalarT(0.5);
        q.x = (m[1][2] - m[2][1]) * s;
        q.y = (m[2][0] - m[0][2]) * s;
        q.z = (m[0][1] - m[1][0]) * s;
    }
    else
    {
        if (m[0][0] > m[1][1] && m[0][0] > m[2][2])
        {
            auto const sq = sqrt(ScalarT(1) + m[0][0] - m[1][1] - m[2][2]);
            auto const s = ScalarT(0.5) / sq;
            q.x = ScalarT(0.5) * sq;
            q.y = (m[0][1] + m[1][0]) * s;
            q.z = (m[0][2] + m[2][0]) * s;
            q.w = (m[1][2] - m[2][1]) * s;
        }
        else if (m[1][1] > m[2][2])
        {
            auto const sq = sqrt(ScalarT(1) + m[1][1] - m[0][0] - m[2][2]);
            auto const s = ScalarT(0.5) / sq;
            q.x = (m[0][1] + m[1][0]) * s;
            q.y = ScalarT(0.5) * sq;
            q.z = (m[1][2] + m[2][1]) * s;
            q.w = (m[2][0] - m[0][2]) * s;
        }
        else
        {
            auto const sq = sqrt(ScalarT(1) + m[2][2] - m[0][0] - m[1][1]);
            auto const s = ScalarT(0.5) / sq;
            q.x = (m[0][2] + m[2][0]) * s;
            q.y = (m[1][2] + m[2][1]) * s;
            q.z = ScalarT(0.5) * sq;
            q.w = (m[0][1] - m[1][0]) * s;
        }
    }
    return q;
}

template <class ScalarT>
[[nodiscard]] constexpr quaternion<ScalarT> quaternion<ScalarT>::from_rotation_matrix(mat<4, 4, ScalarT> const& m)
{
    return from_rotation_matrix(mat<3, 3, ScalarT>(m));
}

template <class ScalarT>
[[nodiscard]] constexpr quaternion<ScalarT>::operator mat<3, 3, ScalarT>() const
{
    // see https://en.wikipedia.org/wiki/Quaternions_and_spatial_rotation

    // TODO: versor version
    auto const s2 = ScalarT(2) / length_sqr(*this);

    auto const qii = x * x;
    auto const qij = x * y;
    auto const qik = x * z;
    auto const qir = x * w;
    auto const qjj = y * y;
    auto const qjk = y * z;
    auto const qjr = y * w;
    auto const qkk = z * z;
    auto const qkr = z * w;

    mat<3, 3, ScalarT> m;
    m[0][0] = 1 - s2 * (qjj + qkk);
    m[1][1] = 1 - s2 * (qii + qkk);
    m[2][2] = 1 - s2 * (qii + qjj);
    m[1][0] = s2 * (qij - qkr);
    m[2][0] = s2 * (qik + qjr);
    m[0][1] = s2 * (qij + qkr);
    m[2][1] = s2 * (qjk - qir);
    m[0][2] = s2 * (qik - qjr);
    m[1][2] = s2 * (qjk + qir);
    return m;
}
template <class ScalarT>
[[nodiscard]] constexpr quaternion<ScalarT>::operator mat<4, 4, ScalarT>() const
{
    // see https://en.wikipedia.org/wiki/Quaternions_and_spatial_rotation

    // TODO: versor version
    auto const s2 = ScalarT(2) / length_sqr(*this);

    auto const qii = x * x;
    auto const qij = x * y;
    auto const qik = x * z;
    auto const qir = x * w;
    auto const qjj = y * y;
    auto const qjk = y * z;
    auto const qjr = y * w;
    auto const qkk = z * z;
    auto const qkr = z * w;

    mat<4, 4, ScalarT> m;
    m[0][0] = 1 - s2 * (qjj + qkk);
    m[1][1] = 1 - s2 * (qii + qkk);
    m[2][2] = 1 - s2 * (qii + qjj);
    m[1][0] = s2 * (qij - qkr);
    m[2][0] = s2 * (qik + qjr);
    m[0][1] = s2 * (qij + qkr);
    m[2][1] = s2 * (qjk - qir);
    m[0][2] = s2 * (qik - qjr);
    m[1][2] = s2 * (qjk + qir);
    m[3][3] = ScalarT(1);
    return m;
}

// ================================= Operators =================================

template <class ScalarT>
[[nodiscard]] constexpr quaternion<ScalarT> operator+(quaternion<ScalarT> const& q0, quaternion<ScalarT> const& q1)
{
    return {q0.x + q1.x, q0.y + q1.y, q0.z + q1.z, q0.w + q1.w};
}

template <class ScalarT>
[[nodiscard]] constexpr quaternion<ScalarT> operator-(quaternion<ScalarT> const& q0, quaternion<ScalarT> const& q1)
{
    return {q0.x - q1.x, q0.y - q1.y, q0.z - q1.z, q0.w - q1.w};
}

template <class ScalarT>
[[nodiscard]] constexpr quaternion<ScalarT> operator-(quaternion<ScalarT> const& q)
{
    return {-q.x, -q.y, -q.z, -q.w};
}

template <class ScalarT>
[[nodiscard]] constexpr quaternion<ScalarT> operator+(quaternion<ScalarT> const& q)
{
    return q;
}

template <class ScalarT>
[[nodiscard]] constexpr quaternion<ScalarT> operator*(quaternion<ScalarT> const& q0, quaternion<ScalarT> const& q1)
{
    return {
        q0.w * q1.x + q0.x * q1.w + q0.y * q1.z - q0.z * q1.y, //
        q0.w * q1.y - q0.x * q1.z + q0.y * q1.w + q0.z * q1.x, //
        q0.w * q1.z + q0.x * q1.y - q0.y * q1.x + q0.z * q1.w, //
        q0.w * q1.w - q0.x * q1.x - q0.y * q1.y - q0.z * q1.z  //
    };
}

template <class ScalarT>
[[nodiscard]] constexpr quaternion<ScalarT> operator*(quaternion<ScalarT> const& q0, dont_deduce<ScalarT> q1)
{
    return {q0.x * q1, q0.y * q1, q0.z * q1, q0.w * q1};
}

template <class ScalarT>
[[nodiscard]] constexpr quaternion<ScalarT> operator*(dont_deduce<ScalarT> q0, quaternion<ScalarT> const& q1)
{
    return {q0 * q1.x, q0 * q1.y, q0 * q1.z, q0 * q1.w};
}

template <class ScalarT>
[[nodiscard]] constexpr quaternion<ScalarT> operator/(quaternion<ScalarT> const& q0, dont_deduce<ScalarT> q1)
{
    return q0 * (ScalarT(1) / q1);
}

template <class ScalarT>
[[nodiscard]] constexpr quaternion<ScalarT> operator/(dont_deduce<ScalarT> q0, quaternion<ScalarT> const& q1)
{
    return {q0 / q1.x, q0 / q1.y, q0 / q1.z, q0 / q1.w};
}

template <class ScalarT>
[[nodiscard]] constexpr pos<3, ScalarT> operator*(quaternion<ScalarT> const& q, pos<3, ScalarT> const& p)
{
    return pos<3, ScalarT>(mat<3, 3, ScalarT>(q) * vec<3, ScalarT>(p));
}

template <class ScalarT>
[[nodiscard]] constexpr vec<3, ScalarT> operator*(quaternion<ScalarT> const& q, vec<3, ScalarT> const& v)
{
    return mat<3, 3, ScalarT>(q) * v;
}

template <class ScalarT>
[[nodiscard]] constexpr dir<3, ScalarT> operator*(quaternion<ScalarT> const& q, dir<3, ScalarT> const& d)
{
    return dir<3, ScalarT>(mat<3, 3, ScalarT>(q) * d);
}


// ================================= Functions =================================

template <class ScalarT>
[[nodiscard]] constexpr ScalarT real(quaternion<ScalarT> const& q)
{
    return q.w;
}

template <class ScalarT>
[[nodiscard]] constexpr vec<3, ScalarT> imaginary(quaternion<ScalarT> const& q)
{
    return {q.x, q.y, q.z};
}

template <class ScalarT>
[[nodiscard]] constexpr mat<3, 3, ScalarT> to_mat3(quaternion<ScalarT> const& q)
{
    return mat<3, 3, ScalarT>(q);
}

template <class ScalarT>
[[nodiscard]] constexpr mat<4, 4, ScalarT> to_mat4(quaternion<ScalarT> const& q)
{
    return mat<4, 4, ScalarT>(q);
}

// CAUTION: only works with normalized quats for now
template <class ScalarT>
[[nodiscard]] constexpr angle_t<ScalarT> angle_of(quaternion<ScalarT> const& q)
{
    return acos(q.w) * ScalarT(2);
}

// CAUTION: only works with normalized quats for now
template <class ScalarT>
[[nodiscard]] constexpr dir<3, ScalarT> axis_of(quaternion<ScalarT> const& q)
{
    return q.w * q.w >= ScalarT(1) ? dir<3, ScalarT>::pos_x : normalize(imaginary(q));
}

// CAUTION: only works with normalized quats for now
template <class ScalarT>
[[nodiscard]] constexpr angle_t<ScalarT> angle_between(quaternion<ScalarT> const& q0, quaternion<ScalarT> const& q1)
{
    return acos(saturate(dot(q0, q1))) * ScalarT(2);
}

template <class ScalarT>
[[nodiscard]] constexpr ScalarT length_sqr(quaternion<ScalarT> const& q)
{
    return q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w;
}

template <class ScalarT>
[[nodiscard]] constexpr ScalarT length(quaternion<ScalarT> const& q)
{
    return sqrt(length_sqr(q));
}

template <class ScalarT>
[[nodiscard]] constexpr quaternion<ScalarT> normalize(quaternion<ScalarT> const& q)
{
    return q / length(q);
}

template <class ScalarT>
[[nodiscard]] constexpr quaternion<ScalarT> conjugate(quaternion<ScalarT> const& q)
{
    return {-q.x, -q.y, -q.z, q.w};
}

template <class ScalarT>
[[nodiscard]] constexpr quaternion<ScalarT> inverse(quaternion<ScalarT> const& q)
{
    return conjugate(q) * (ScalarT(1) / length_sqr(q));
}

template <class ScalarT>
[[nodiscard]] constexpr quaternion<ScalarT> normalize_safe(quaternion<ScalarT> const& q, dont_deduce<ScalarT> eps = ScalarT(0))
{
    auto const l = length(q);
    return l <= eps ? quaternion<ScalarT>::zero : q / l;
}

template <class ScalarT>
[[nodiscard]] constexpr ScalarT dot(quaternion<ScalarT> const& q0, quaternion<ScalarT> const& q1)
{
    return q0.x * q1.x + q0.y * q1.y + q0.z * q1.z + q0.w * q1.w;
}

template <class ScalarT>
[[nodiscard]] constexpr quaternion<ScalarT> mix(quaternion<ScalarT> const& q0, quaternion<ScalarT> const& q1, dont_deduce<ScalarT> t)
{
    return {mix(q0.x, q1.x, t), mix(q0.y, q1.y, t), mix(q0.z, q1.z, t), mix(q0.w, q1.w, t)};
}

template <class ScalarT>
[[nodiscard]] constexpr quaternion<ScalarT> lerp(quaternion<ScalarT> const& q0, quaternion<ScalarT> const& q1, dont_deduce<ScalarT> t)
{
    return mix(q0, q1, t);
}

template <class ScalarT>
[[nodiscard]] constexpr quaternion<ScalarT> nlerp(quaternion<ScalarT> const& q0, quaternion<ScalarT> const& q1, dont_deduce<ScalarT> t)
{
    return normalize(mix(q0, dot(q0, q1) >= ScalarT(0) ? q1 : -q1, t));
}

template <class ScalarT>
[[nodiscard]] constexpr quaternion<ScalarT> nlerp_unaligned(quaternion<ScalarT> const& q0, quaternion<ScalarT> const& q1, dont_deduce<ScalarT> t)
{
    return normalize(mix(q0, q1, t));
}

// CAUTION: only works with normalized quats for now
template <class ScalarT>
[[nodiscard]] constexpr quaternion<ScalarT> slerp(quaternion<ScalarT> const& q0, quaternion<ScalarT> q1, dont_deduce<ScalarT> t)
{
    auto dotAB = dot(q0, q1);

    // ensure alignment
    if (dotAB < ScalarT(0))
    {
        q1 = -q1;
        dotAB = -dotAB;
    }

    // for almost-same quaternions this prevents div-by-zero
    if (dotAB >= ScalarT(1) - tg::epsilon<ScalarT>)
        return lerp(q0, q1, t);

    auto const alpha = acos(dotAB);
    auto const alpha0 = (ScalarT(1) - t) * alpha;
    auto const alpha1 = t * alpha;
    return (q0 * sin(alpha0) + q1 * sin(alpha1)) / sin(alpha);
}

// CAUTION: only works with normalized quats for now
template <class ScalarT>
[[nodiscard]] constexpr quaternion<ScalarT> slerp_unaligned(quaternion<ScalarT> const& q0, quaternion<ScalarT> q1, dont_deduce<ScalarT> t)
{
    auto dotAB = dot(q0, q1);

    // for almost-same quaternions this prevents div-by-zero
    if (dotAB >= ScalarT(1) - tg::epsilon<ScalarT>)
        return lerp(q0, q1, t);

    auto const alpha = acos(dotAB);
    auto const alpha0 = (ScalarT(1) - t) * alpha;
    auto const alpha1 = t * alpha;
    return (q0 * sin(alpha0) + q1 * sin(alpha1)) / sin(alpha);
}
}
