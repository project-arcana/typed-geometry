#pragma once

#include <typed-geometry/common/scalar_math.hh>
#include <typed-geometry/functions/normalize.hh>
#include <typed-geometry/types/dir.hh>
#include <typed-geometry/types/quat.hh>
#include <typed-geometry/types/mat.hh>
#include <typed-geometry/types/vec.hh>

namespace tg
{
// ================================= Name constructors =================================

template <class ScalarT>
TG_NODISCARD constexpr quat<ScalarT> quat<ScalarT>::from_axis_angle(dir<3, ScalarT> const& axis, angle_t<ScalarT> angle)
{
    auto a_half = angle * ScalarT(0.5);
    auto [sa, ca] = sin_cos(a_half);
    return {axis * sa, ca};
}


// ================================= Operators =================================

template <class ScalarT>
TG_NODISCARD constexpr quat<ScalarT> operator+(quat<ScalarT> const& q0, quat<ScalarT> const& q1)
{
    return {q0.x + q1.x, q0.y + q1.y, q0.z + q1.z, q0.w + q1.w};
}

template <class ScalarT>
TG_NODISCARD constexpr quat<ScalarT> operator-(quat<ScalarT> const& q0, quat<ScalarT> const& q1)
{
    return {q0.x - q1.x, q0.y - q1.y, q0.z - q1.z, q0.w - q1.w};
}

template <class ScalarT>
TG_NODISCARD constexpr quat<ScalarT> operator*(quat<ScalarT> const& q0, quat<ScalarT> const& q1)
{
    return {
        q0.w * q1.x + q0.x * q1.w + q0.y * q1.z - q0.z * q1.y, //
        q0.w * q1.y - q0.x * q1.z + q0.y * q1.w + q0.z * q1.x, //
        q0.w * q1.z + q0.x * q1.y - q0.y * q1.x + q0.z * q1.w, //
        q0.x * q1.x + q0.y * q1.y + q0.z * q1.z + q0.w * q1.w  //
    };
}

template <class ScalarT>
TG_NODISCARD constexpr quat<ScalarT> operator*(quat<ScalarT> const& q0, dont_deduce<ScalarT> q1)
{
    return {q0.x * q1, q0.y * q1, q0.z * q1, q0.w * q1};
}

template <class ScalarT>
TG_NODISCARD constexpr quat<ScalarT> operator*(dont_deduce<ScalarT> q0, quat<ScalarT> const& q1)
{
    return {q0 * q1.x, q0 * q1.y, q0 * q1.z, q0 * q1.w};
}

template <class ScalarT>
TG_NODISCARD constexpr quat<ScalarT> operator/(quat<ScalarT> const& q0, dont_deduce<ScalarT> q1)
{
    return q0 * (ScalarT(1) / q1);
}

template <class ScalarT>
TG_NODISCARD constexpr quat<ScalarT> operator/(dont_deduce<ScalarT> q0, quat<ScalarT> const& q1)
{
    return {q0 / q1.x, q0 / q1.y, q0 / q1.z, q0 / q1.w};
}

// ================================= Functions =================================


template <class ScalarT>
TG_NODISCARD constexpr ScalarT real(quat<ScalarT> const& q)
{
    return q.w;
}

template <class ScalarT>
TG_NODISCARD constexpr vec<3, ScalarT> imaginary(quat<ScalarT> const& q)
{
    return {q.x, q.y, q.z};
}

// CAUTION: only works with normalized quats for now
template <class ScalarT>
TG_NODISCARD constexpr angle_t<ScalarT> angle_of(quat<ScalarT> const& q)
{
    return acos(q.w) * ScalarT(2);
}

// CAUTION: only works with normalized quats for now
template <class ScalarT>
TG_NODISCARD constexpr dir<3, ScalarT> axis_of(quat<ScalarT> const& q)
{
    return q.w * q.w >= ScalarT(1) ? dir<3, ScalarT>::pos_x : normalize(imaginary(q));
}

// CAUTION: only works with normalized quats for now
template <class ScalarT>
TG_NODISCARD constexpr angle_t<ScalarT> angle_between(quat<ScalarT> const& q0, quat<ScalarT> const& q1)
{
    return acos(saturate(dot(q0, q1))) * ScalarT(2);
}

template <class ScalarT>
TG_NODISCARD constexpr ScalarT length_sqr(quat<ScalarT> const& q)
{
    return q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w;
}

template <class ScalarT>
TG_NODISCARD constexpr ScalarT length(quat<ScalarT> const& q)
{
    return sqrt(length_sqr(q));
}

template <class ScalarT>
TG_NODISCARD constexpr quat<ScalarT> normalize(quat<ScalarT> const& q)
{
    return q / length(q);
}

template <class ScalarT>
TG_NODISCARD constexpr quat<ScalarT> conjugate(quat<ScalarT> const& q)
{
    return {-q.x, -q.y, -q.z, q.w};
}

template <class ScalarT>
TG_NODISCARD constexpr quat<ScalarT> inverse(quat<ScalarT> const& q)
{
    return conjugate(q) * (ScalarT(1) / length_sqr(q));
}

template <class ScalarT>
TG_NODISCARD constexpr quat<ScalarT> normalize_safe(quat<ScalarT> const& q, dont_deduce<ScalarT> eps = ScalarT(0))
{
    auto const l = length(q);
    return l <= eps ? quat<ScalarT>::zero : q / l;
}

template <class ScalarT>
TG_NODISCARD constexpr quat<ScalarT> dot(quat<ScalarT> const& q0, quat<ScalarT> const& q1)
{
    return {q0.x * q1.x, q0.y * q1.y, q0.z * q1.z, q0.w * q1.w};
}

template <class ScalarT>
TG_NODISCARD constexpr quat<ScalarT> mix(quat<ScalarT> const& q0, quat<ScalarT> const& q1, dont_deduce<ScalarT> t)
{
    return {mix(q0.x, q1.x, t), mix(q0.y, q1.y, t), mix(q0.z, q1.z, t), mix(q0.w, q1.w, t)};
}

template <class ScalarT>
TG_NODISCARD constexpr quat<ScalarT> lerp(quat<ScalarT> const& q0, quat<ScalarT> const& q1, dont_deduce<ScalarT> t)
{
    return mix(q0, q1, t);
}

template <class ScalarT>
TG_NODISCARD constexpr quat<ScalarT> nlerp(quat<ScalarT> const& q0, quat<ScalarT> const& q1, dont_deduce<ScalarT> t)
{
    return normalize(mix(q0, dot(q0, q1) >= ScalarT(0) ? q1 : -q1, t));
}

template <class ScalarT>
TG_NODISCARD constexpr quat<ScalarT> nlerp_unaligned(quat<ScalarT> const& q0, quat<ScalarT> const& q1, dont_deduce<ScalarT> t)
{
    return normalize(mix(q0, q1, t));
}

// CAUTION: only works with normalized quats for now
template <class ScalarT>
TG_NODISCARD constexpr quat<ScalarT> slerp(quat<ScalarT> const& q0, quat<ScalarT> q1, dont_deduce<ScalarT> t)
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
TG_NODISCARD constexpr quat<ScalarT> slerp_unaligned(quat<ScalarT> const& q0, quat<ScalarT> q1, dont_deduce<ScalarT> t)
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
